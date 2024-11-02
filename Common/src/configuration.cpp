#include <Common/configuration.hpp>
#include <Common/database.hpp>
#include <Common/log.hpp>
#include <SQLiteCpp/SQLiteCpp.h>
#include <regex>

Configuration::Config Configuration::config{
    .deviceIp{ "127.0.0.1" },
    .deviceSubnetMask{ "255.255.255.0" },
    .deviceName{ "Test home" },
    .dataRetentionPeriod{ 30 },
    .backendServerPort{ 9990 },
    .communicationServerPort{ 9999 }
};

void Configuration::Load() {
    try {
        Logger::trace("Reading configuration from DB.");
        
        std::lock_guard<std::mutex> lock(ConfigDB::GetMutex());
        SQLite::Statement query(*ConfigDB::Get(), "SELECT * FROM Configuration");
        
        while (query.executeStep()) {
            std::string key = query.getColumn(0).getString();
            std::string value = query.getColumn(1).getString();
            if (key == "deviceIp") {
                config.deviceIp = value;
            } 
            else if (key == "deviceSubnetMask") {
                config.deviceSubnetMask = value;
            } 
            else if (key == "deviceName") {
                config.deviceName = value;
            } 
            else if (key == "dataRetentionPeriod") {
                if (std::regex_match(value, std::regex("^[0-9]+$"))) {
                    config.dataRetentionPeriod = static_cast<std::uint16_t>(std::stoi(value));
                } 
                else {
                    Logger::warn("Wrong value '{}' for key '{}' in configuration.", value, key);
                }
            }
            else if (key == "backendServerPort") {
                if (std::regex_match(value, std::regex("^[0-9]+$"))) {
                    config.backendServerPort = static_cast<std::uint16_t>(std::stoi(value));
                } 
                else {
                    Logger::warn("Wrong value '{}' for key '{}' in configuration.", value, key);
                }
            } 
            else if (key == "communicationServerPort") {
                if (std::regex_match(value, std::regex("^[0-9]+$"))) {
                    config.communicationServerPort = static_cast<std::uint16_t>(std::stoi(value));
                } 
                else {
                    Logger::warn("Wrong value '{}' for key '{}' in configuration.", value, key);
                }
            } 
            else {
                Logger::warn("Unhandled key '{}' with value '{}' in configuration.", key, value);
            }
        }
    }
    catch (const SQLite::Exception& e) {
        Logger::error("SQLite error: {}", e.what());
    }
    catch (const std::exception& e) {
        Logger::error("Error: {}", e.what());
    }
}

Configuration::Config Configuration::Get() {
    return config;
}