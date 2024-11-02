#include "SHH.hpp"
#include <Common/log.hpp>
#include <Common/shared_memory.hpp>
#include <Common/signals.hpp>
#include <Common/database.hpp>
#include <nlohmann/json.hpp>

bool UpdateSignalDefinitionsInSharedMemory() {
    try {
        Logger::trace("Updating Signal Definitions in Shared Memory.");

        auto shmem = SharedMemory::Manager::Get();
        
        // read signals from database
        auto signals = Signals::Definition::GetFromDatabase();

        // update header
        shmem->header.numberOfSignals = signals.size();

        // update shared memory
        for (std::size_t i{ 0 }; i < shmem->header.numberOfSignals; ++i) {
            auto& targetSignal = shmem->definitions[i];
            std::memcpy(&targetSignal, &signals[i], sizeof(signals[i]));
        }
        return true;
    }
    catch (const std::exception& e) {
        Logger::error("Error when updating signals definitions: {}", e.what());
        return false;
    }
}

std::chrono::system_clock::time_point GetLastUpdateTime() {
    try {
        std::lock_guard<std::mutex> lock(ConfigDB::GetMutex());
        SQLite::Statement query(*ConfigDB::Get(), "SELECT Value FROM \"Update\" WHERE Key = 'timestamp'");

        while (query.executeStep()) {
            return std::chrono::system_clock::from_time_t(query.getColumn(0).getInt());
        }
    }
    catch (const SQLite::Exception& e) {
        Logger::error("SQLite error: {}", e.what());
    }
    catch (const std::exception& e) {
        Logger::error("std::exception: {}", e.what());
    }
    return std::chrono::system_clock::now();
}