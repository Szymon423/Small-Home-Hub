#include <Common/log.hpp>
#include <Common/runtime.hpp>
#include <Common/database.hpp>
#include <Common/shared_memory.hpp>
#include <Common/communication.hpp>
#include <memory>
#include <chrono>
#include <thread>
#include <fmt/chrono.h>
#include "SHH.hpp"

int main() {
    // Initialize logger
    Logger::init("/Small-Home-Hub/logs/SHH.log", "SHH");
    Logger::info("Starting");

    // Register handler signals 
    if (std::signal(SIGINT, Runtime::SignalHandler) == SIG_ERR || std::signal(SIGTERM, Runtime::SignalHandler) == SIG_ERR) {
        Logger::error("Could not register handlers for system signals.");
        return 1;
    }

    // Connect to configuration database
    try {
        ConfigDB::Init("/Small-Home-Hub/DB/config.db");
    }
    catch (const std::runtime_error& e) {
        Logger::critical("Error connecting to database, SQLite error: {}.", e.what());
        return 1;
    }

    // Connect to shared memory
    try {
        SharedMemory::Manager::Create();
    }
    catch (const std::exception& e) {
        Logger::critical("Could not connect to shared memory - quitting program.");
        return 1;
    }

    // Update shared memory
    if (!UpdateSignalDefinitionsInSharedMemory()) {
        Logger::error("Error on Update Signal Definitions in Shared Memory.");
        return 1;
    }
    Logger::info("Signals definitions in shared memory updated.");

    // Communication Initialization
    Communication::Manager::Initialize(3);

    // Run communication broker
    Communication::Broker broker;
    try {
        broker.Initialize();
        broker.Start();
    }
    catch (const std::exception& e) {
        Logger::critical("Could not start internal communication broker - quitting program.");
        return 1;
    }

    auto last_update = GetLastUpdateTime();
    while (Runtime::Run()) {
        if (last_update < GetLastUpdateTime()) {
            Logger::info("Database updated, restarting all programs.");
            Runtime::Stop();
            break;
        }
        std::this_thread::sleep_for(std::chrono::seconds{ 10 });
    }

    broker.Stop();
    Logger::info("Turning off");
    return 0;
}