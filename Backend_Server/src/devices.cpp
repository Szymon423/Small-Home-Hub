#include "devices.hpp"
#include <Common/log.hpp>
#include <Common/database.hpp>
#include <Common/utilities.hpp>
#include <Common/devices.hpp>
#include <nlohmann/json.hpp>
#include <SQLiteCpp/SQLiteCpp.h>
#include <exception>

namespace Devices {
    void get_all_types(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept {
        try {
            Logger::trace("Reading device types from DB.");
            
            nlohmann::json j = nlohmann::json::array();
            auto types = Devices::Type::GetFromDatabase();
            for (auto& type : types) {
                j.push_back(type.ToJson());
            }

            // Response
            response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            out << j.dump();
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
            Logger::error("Unexpected error occurred while reaading device types");
        }
    }

    void get_all_devices(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept {
        try {
            Logger::trace("Reading devices from DB.");
            
            nlohmann::json j = nlohmann::json::array();
            auto devices = Devices::Device::GetFromDatabase();
            for (auto& device : devices) {
                j.push_back(device.ToJson());
            }

            // Response
            response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            out << j.dump();
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
            Logger::error("Unexpected error occurred while reaading devices");
        }
    }

    void add_device(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept {
        try {
            Logger::trace("Adding new device to DB.");
            
            // Parse JSON from request body
            nlohmann::json requestBody = nlohmann::json::parse(request.stream());

            // Validate required fields
            static const std::vector<std::tuple<std::string, Utilities::JSON::FieldDataType, bool>> required_fields {
                { "DeviceTypeID", Utilities::JSON::FieldDataType::Integer, true },
                { "Description", Utilities::JSON::FieldDataType::String, true }
            };
            Utilities::JSON::Validate(required_fields, requestBody);

            // Lock mutex before database operations
            std::lock_guard<std::mutex> lock(ConfigDB::GetMutex());
            SQLite::Statement query(*ConfigDB::Get(), "INSERT INTO Devices (DeviceTypeID, Description) VALUES (?, ?)");
            query.bind(1, requestBody["DeviceTypeID"].get<int>());
            query.bind(2, requestBody["Description"].get<std::string>());
            query.exec();

            // Response
            response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json responseJson = {{"status", "success"}, {"message", "Device added successfully"}};
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
        catch (...) {
            response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json errorJson = {{"status", "error"}, {"message", "An unexpected error occurred"}};
            out << errorJson.dump();
            Logger::error("Unexpected error occurred while adding devices");
        }
    }

    void delete_device(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept {
        try {
            Logger::trace("Deleting device from DB.");
            
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
                { "DeviceID", Utilities::JSON::FieldDataType::Integer, true }
            };
            Utilities::JSON::Validate(required_fields, requestBody);

            // Lock mutex before database operations
            std::lock_guard<std::mutex> lock(ConfigDB::GetMutex());

            SQLite::Statement query(*ConfigDB::Get(), "DELETE FROM Devices WHERE DeviceID = ?");
            
            query.bind(1, requestBody["DeviceID"].get<int>());

            try {
                query.exec();
            } 
            catch (const SQLite::Exception& e) {
                throw std::runtime_error("SQLite error: " + std::string(e.what()));
            }

            if (query.getChanges() == 0) {
                throw std::runtime_error("No record found with the given UserID");
            }

            // Response
            response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json responseJson = {{"status", "success"}, {"message", "Device deleted successfully"}};
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
        catch (...) {
            response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json errorJson = {{"status", "error"}, {"message", "An unexpected error occurred"}};
            out << errorJson.dump();
            Logger::error("Unexpected error occurred while deleting device");
        }
    }
}