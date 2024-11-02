#include "system_configuration.hpp"
#include <Common/database.hpp>
#include <Common/utilities.hpp>
#include <Common/log.hpp>
#include <SQLiteCpp/SQLiteCpp.h>
#include <nlohmann/json.hpp>
#include <filesystem>
#include <exception>
#include <fstream>
#include <string>
#include <utility>
#include <regex>

namespace SystemConfiguration {
    void get_all(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept {
        try {
            Logger::trace("Reading configuration from DB.");
            
            // Blokujemy mutex przed operacjami na bazie danych
            std::lock_guard<std::mutex> lock(ConfigDB::GetMutex());

            SQLite::Statement query(*ConfigDB::Get(), "SELECT * FROM Configuration");
            
            // Configuration has two columns: entry (TEXT), value (TEXT)
            std::vector<std::pair<std::string, std::string>> data;

            while (query.executeStep()) {
                std::string entry = query.getColumn(0).getString();
                std::string value = query.getColumn(1).getString();
                data.emplace_back(entry, value);
            }
            
            nlohmann::json j;
            for (const auto& [entry, value] : data) {
                j[entry] = value;
            }

            // Response
            response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            out << j.dump();
        }
        catch (const SQLite::Exception& e) {
            response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json errorJson = { {"status", "error"}, {"message", "Database error"} };
            out << errorJson.dump();
            Logger::error("SQLite error: {}", e.what());
        }
        catch (const std::exception& e) {
            response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json errorJson = { {"status", "error"}, {"message", "Internal server error"} };
            out << errorJson.dump();
            Logger::error("Internal Server Error: {}", e.what());
        }
        catch (...) {
            response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json errorJson = {{"status", "error"}, {"message", "An unexpected error occurred"}};
            out << errorJson.dump();
            Logger::error("Unexpected error occurred while reading configuration");
        }
    }

    /// @brief Helper enum with oprtation status code 
    enum class Code : std::uint8_t {
        Success,
        Error
    };

    /// @brief Function responsible for updating configuration in database
    /// @param key key of entry to update
    /// @param value value of entry to update
    /// @return pair of code and associated message
    std::pair<Code, std::string> update(const std::string& key, const std::string& value) noexcept {
        try {
            // Blokujemy mutex przed operacjami na bazie danych
            std::lock_guard<std::mutex> lock(ConfigDB::GetMutex());

            SQLite::Statement query(*ConfigDB::Get(), "UPDATE Configuration SET Value = ? WHERE Key = ?");
            query.bind(1, value);
            query.bind(2, key);

            try {
                query.exec();
            } 
            catch (const SQLite::Exception& e) {
                throw std::runtime_error("SQLite error: " + std::string(e.what()));
            }

            // Sprawdzamy, czy aktualizacja faktycznie miała miejsce
            if (query.getChanges() == 0) {
                Logger::warn("No rows updated. The '{}' key might not exist in the Configuration table.", key);
                throw std::runtime_error("Failed to update configuration. The key might not exist in the database.");
            }
            return { Code::Success, "Succes." };
        }
        catch (const std::exception& e) {
            Logger::error("Internal Server Error: {}", e.what());
            return { Code::Error, std::string{"Internal Server Error: "} + e.what() };
        }
        catch (...) {
            Logger::error("Unknown Internal Server Error");
            return { Code::Error, "Unknown Internal Server Error" };
        }
    }

    void update_network(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept {
        try {
            Logger::trace("Updating device ip and mask in configuration database.");

            // Parse JSON from request body
            nlohmann::json requestBody;
            try {
                requestBody = nlohmann::json::parse(request.stream());
            } 
            catch (const nlohmann::json::parse_error& e) {
                throw std::runtime_error("Invalid JSON: " + std::string(e.what()));
            }

            // Validate required fields
            static const std::vector<std::tuple<std::string, Utilities::JSON::FieldDataType, bool>> required_fields {
                { "deviceIp", Utilities::JSON::FieldDataType::String, true },
                { "deviceSubnetMask", Utilities::JSON::FieldDataType::String, true }
            };
            Utilities::JSON::Validate(required_fields, requestBody);

            std::string deviceIp = requestBody["deviceIp"].get<std::string>();
            std::string deviceSubnetMask = requestBody["deviceSubnetMask"].get<std::string>();

            // RegEx for IPv4 validation
            const std::regex ipPattern(R"((\b(25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9]?[0-9])\b\.){3}(\b(25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9]?[0-9])\b))");

            if (!std::regex_match(deviceIp, ipPattern)) {
                throw std::runtime_error("Provided ip is not valid.");
            }

            if (!std::regex_match(deviceSubnetMask, ipPattern)) {
                throw std::runtime_error("Provided subnet mask is not valid.");
            }

            {
                auto [code, message] = update("device_ip", deviceIp);
                if (code != Code::Success) {
                    throw std::runtime_error("Could not update device_ip: " + message);
                }
            }
            {
                auto [code, message] = update("subnet_mask", deviceSubnetMask);
                if (code != Code::Success) {
                    throw std::runtime_error("Could not update subnet_mask: " + message);
                }
            }

            // Response
            response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json responseJson = {{"status", "success"}, {"message", "Device network configuration updated successfully"}};
            out << responseJson.dump();
        }   
        catch (const std::exception& e) {
            response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json errorJson = { {"status", "error"}, {"message", e.what()} };
            out << errorJson.dump();
            Logger::error("Internal Server Error: {}", e.what());
        }
        catch (...) {
            response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json errorJson = {{"status", "error"}, {"message", "An unexpected error occurred"}};
            out << errorJson.dump();
            Logger::error("Unexpected error occurred while updating network configuration");
        }
    }

    void update_device_name(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept {
        try {
            Logger::trace("Updating device name in configuration database.");

            // Parse JSON from request body
            nlohmann::json requestBody;
            try {
                requestBody = nlohmann::json::parse(request.stream());
            } 
            catch (const nlohmann::json::parse_error& e) {
                throw std::runtime_error("Invalid JSON: " + std::string(e.what()));
            }

            // Validate required fields
            static const std::vector<std::tuple<std::string, Utilities::JSON::FieldDataType, bool>> required_fields {
                { "deviceName", Utilities::JSON::FieldDataType::String, true }
            };
            Utilities::JSON::Validate(required_fields, requestBody);

            std::string deviceNameString = requestBody["deviceName"].get<std::string>();

            auto [code, message] = update("deviceName", deviceNameString);
            if (code != Code::Success) {
                throw std::runtime_error("Could not update deviceName: " + message);
            }

            // Response
            response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json responseJson = {{"status", "success"}, {"message", "Device Name updated successfully"}};
            out << responseJson.dump();
        }   
        catch (const std::exception& e) {
            response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json errorJson = { {"status", "error"}, {"message", e.what()} };
            out << errorJson.dump();
            Logger::error("Internal Server Error: {}", e.what());
        }
        catch (...) {
            response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json errorJson = {{"status", "error"}, {"message", "An unexpected error occurred"}};
            out << errorJson.dump();
            Logger::error("Unexpected error occurred while updating device name");
        }
    }

    void update_data_retention_period(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept {
        try {
            Logger::trace("Updating data retention in configuration database.");

            // Parse JSON from request body
            nlohmann::json requestBody;
            try {
                requestBody = nlohmann::json::parse(request.stream());
            } 
            catch (const nlohmann::json::parse_error& e) {
                throw std::runtime_error("Invalid JSON: " + std::string(e.what()));
            }

            // Validate required fields
            static const std::vector<std::tuple<std::string, Utilities::JSON::FieldDataType, bool>> required_fields {
                { "dataRetentionPeriod", Utilities::JSON::FieldDataType::Integer, true }
            };
            Utilities::JSON::Validate(required_fields, requestBody);

            std::string dataRetentionPeriodString = std::to_string(requestBody["dataRetentionPeriod"].get<int>());

            auto [code, message] = update("dataRetentionPeriod", dataRetentionPeriodString);
            if (code != Code::Success) {
                throw std::runtime_error("Could not update dataRetentionPeriod: " + message);
            }

            // Response
            response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json responseJson = {{"status", "success"}, {"message", "Data retention period updated successfully"}};
            out << responseJson.dump();
        }   
        catch (const std::exception& e) {
            response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json errorJson = { {"status", "error"}, {"message", e.what()} };
            out << errorJson.dump();
            Logger::error("Internal Server Error: {}", e.what());
        }
        catch (...) {
            response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json errorJson = {{"status", "error"}, {"message", "An unexpected error occurred"}};
            out << errorJson.dump();
            Logger::error("Unexpected error occurred while updating data retention period");
        }
    }

    void import_db(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept {
        try {
            Logger::trace("Configuration database importing.");

            // Parse JSON from request body
            nlohmann::json requestBody;
            try {
                requestBody = nlohmann::json::parse(request.stream());
            } 
            catch (const nlohmann::json::parse_error& e) {
                throw std::runtime_error("Invalid JSON: " + std::string(e.what()));
            }

            // Response
            response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json responseJson = { {"status", "success"}, {"message", "Configuration updated successfully"} };
            out << responseJson.dump();
        }
        catch (const std::exception& e) {
            response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json errorJson = { {"status", "error"}, {"message", "Internal server error"} };
            out << errorJson.dump();
            Logger::error("Internal Server Error: {}", e.what());
        }
    }

    void export_db(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept {
        try {
            Logger::trace("Exporting configuration database");

            std::filesystem::path db_path{ "./DB/config.db" };

            if (std::filesystem::exists(db_path) && std::filesystem::is_regular_file(db_path)) {
                // Przygotowanie odpowiedzi
                response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                response.setContentType("application/octet-stream");
                response.set("Content-Disposition", "attachment; filename=\"config.db\"");

                // Otwieranie pliku i wysyłanie jego zawartości
                std::ifstream file(db_path, std::ios::binary);
                if (file) {
                    std::ostream& out = response.send();
                    out << file.rdbuf();
                    file.close();
                    Logger::info("Configuration database exported successfully");
                } 
                else {
                    throw std::runtime_error("Unable to open database file");
                }
            } 
            else {
                throw std::runtime_error("Database file not found or is not a regular file");
            }
        }
        catch (const std::exception& e) {
            response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json errorJson = { {"status", "error"}, {"message", e.what()} };
            out << errorJson.dump();
            Logger::error("Error exporting configuration database: {}", e.what());
        }
    }
} 