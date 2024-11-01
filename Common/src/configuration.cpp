#include <Common/configuration.hpp>
#include <Common/database.hpp>
#include <Common/log.hpp>
#include <SQLiteCpp/SQLiteCpp.h>
#include <regex>

Configuration::Config Configuration::config{
    .device_ip{ "127.0.0.1" },
    .device_name{ "Kontroler szafy RACK" },
    .data_retention_period{ 31 },
    .modbus_server_enabled{ false },
    .modbus_server_port{ 502 },
    .snmp_manager_enabled{ false },
    .snmp_agent_port{ 126 },
    .backend_server_port{ 9999 }
};

void Configuration::Load() {
    try {
        Logger::trace("Reading configuration from DB.");
        
        // Blokujemy mutex przed operacjami na bazie danych
        std::lock_guard<std::mutex> lock(ConfigDB::GetMutex());

        SQLite::Statement query(*ConfigDB::Get(), "SELECT * FROM Configuration");
        
        while (query.executeStep()) {
            std::string key = query.getColumn(0).getString();
            std::string value = query.getColumn(1).getString();
            if (key == "device_ip") {
                config.device_ip = value;
            } 
            else if (key == "device_name") {
                config.device_name = value;
            } 
            else if (key == "modbus_server_enabled") {
                config.modbus_server_enabled = (value == "true");
            } 
            else if (key == "modbus_server_port") {
                if (std::regex_match(value, std::regex("^[0-9]+$"))) {
                    config.modbus_server_port = static_cast<std::uint16_t>(std::stoi(value));
                } 
                else {
                    Logger::warn("Wrong value '{}' for key '{}' in configuration.", value, key);
                }
            } 
            else if (key == "data_retention_period") {
                if (std::regex_match(value, std::regex("^[0-9]+$"))) {
                    config.data_retention_period = static_cast<std::uint16_t>(std::stoi(value));
                } 
                else {
                    Logger::warn("Wrong value '{}' for key '{}' in configuration.", value, key);
                }
            } 
            else if (key == "snmp_manager_enabled") {
                config.snmp_manager_enabled = (value == "true");
            } 
            else if (key == "snmp_agent_port") {
                if (std::regex_match(value, std::regex("^[0-9]+$"))) {
                    config.snmp_agent_port = static_cast<std::uint16_t>(std::stoi(value));
                } 
                else {
                    Logger::warn("Wrong value '{}' for key '{}' in configuration.", value, key);
                }
            } 
            else if (key == "backend_server_port") {
                if (std::regex_match(value, std::regex("^[0-9]+$"))) {
                    config.backend_server_port = static_cast<std::uint16_t>(std::stoi(value));
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