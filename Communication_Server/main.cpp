#include <Common/database.hpp>
#include <Common/shared_memory.hpp>
#include <Common/runtime.hpp>
#include <Common/configuration.hpp>
#include <Common/log.hpp>
#include "backend.hpp"
#include <future>

int main() {
    // Initialize logger
    Logger::init("/Small-Home-Hub/logs/Communication_Server.log", "Communication_Server");
    Logger::info("Starting");

    // Register handler signals 
    if (std::signal(SIGINT, Runtime::SignalHandler) == SIG_ERR || std::signal(SIGTERM, Runtime::SignalHandler) == SIG_ERR) {
        Logger::error("Could not register handlers for system signals.");
        return 1;
    }

    // Connect to shared memory
    try {
        SharedMemory::Manager::Connect();
    }
    catch (const std::exception& e) {
        Logger::critical("Could not connect to shared memory - quitting program.");
        return 1;
    }
    Logger::info("Connected to shared memory");

    // Init configuration database
    try {
        ConfigDB::Init("/Small-Home-Hub/Database/config.db");
    }
    catch (const std::runtime_error& e) {
        Logger::critical("Error connecting to database, SQLite error: {}.", e.what());
        return 1;
    }

    // Load configuration
    try {
        Configuration::Load();
    }
    catch (const std::runtime_error& e) {
        Logger::critical("Error loading configuration from database, SQLite error: {}.", e.what());
        return 1;
    }

    std::future<void> backend_worker = std::async(std::launch::async, backend_function);
    while (Runtime::Run()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    backend_worker.wait();
    Logger::info("Turning off");
    return 0;
}