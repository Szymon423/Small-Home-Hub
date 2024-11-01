#include <Common/shared_memory.hpp>
#include <Common/log.hpp>

namespace SharedMemory {
    bip::shared_memory_object Manager::shm;
    bip::mapped_region Manager::region;
    Data* Manager::data{ nullptr };
    std::mutex Manager::mutex;
    std::atomic<bool> Manager::initialized{ false };

    void Manager::NullOut() {
        std::memset(region.get_address(), '\0', region.get_size());
    }

    void Manager::Close() {
        std::lock_guard<std::mutex> lock(mutex);
        if (initialized) {
            bip::shared_memory_object::remove(SHMEM_NAME);
            initialized = false;
        }
    }

    void Manager::Create() {
        std::lock_guard<std::mutex> lock(mutex);
        if (initialized) return;

        for (int i = 0; i < 10; ++i) {
            try {
                shm = bip::shared_memory_object(bip::open_or_create, SHMEM_NAME, bip::read_write);
                shm.truncate(sizeof(Data));
                region = bip::mapped_region(shm, bip::read_write);
                data = static_cast<Data*>(region.get_address());
                NullOut();
                initialized = true;
                return;
            }
            catch(const bip::interprocess_exception& ex) {
                Logger::error("Could not create and connect to shared memory.");
                std::this_thread::sleep_for(std::chrono::seconds(2));
            }
        }
        throw std::runtime_error("Failed to create shared memory after multiple attempts.");
    }

    void Manager::Connect() {
        std::lock_guard<std::mutex> lock(mutex);
        if (initialized) return;

        for (int i = 0; i < 10; ++i) {
            try {
                shm = bip::shared_memory_object(bip::open_only, SHMEM_NAME, bip::read_write);
                region = bip::mapped_region(shm, bip::read_write);
                data = static_cast<Data*>(region.get_address());
                initialized = true;
                return;
            }
            catch (const bip::interprocess_exception& ex) {
                Logger::error("Could not connect to shared memory.");
                std::this_thread::sleep_for(std::chrono::seconds(2));
            }
        }
        throw std::runtime_error("Failed to connect to shared memory after multiple attempts");
    }

    Data* Manager::Get() {
        if (!initialized) {
            throw std::runtime_error("Shared memory not initialized");
        }
        return data;
    }
}