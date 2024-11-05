#include "signal_definitions.hpp"
#include <Common/log.hpp>
#include <Common/database.hpp>
#include <Common/groups.hpp>
#include <Common/signals.hpp>
#include <Common/utilities.hpp>
#include <nlohmann/json.hpp>
#include <SQLiteCpp/SQLiteCpp.h>
#include <exception>
#include <tuple>

namespace SignalDefinitions {
    void get_all(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept {
        try {
            Logger::trace("Reading all signal definitions from DB.");

            auto definitions = Signals::Definition::GetFromDatabase();
            nlohmann::json j = nlohmann::json::array();
            for (auto& def : definitions) {
                j.push_back(def.ToJson());
            }

            response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            out << j.dump();
        }
        catch (const std::exception& e) {
            response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json errorJson = {{"status", "error"}, {"message", e.what()}};
            out << errorJson.dump();
            Logger::error("Error retrieving signal definitions: {}", e.what());
        }
    }

    void add_signal(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept {
        try {
            Logger::trace("Adding new signal definition to DB.");

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
                { "IsArchived", Utilities::JSON::FieldDataType::Boolean, true },
                { "IsSteerable", Utilities::JSON::FieldDataType::Boolean, true },
                { "GroupID", Utilities::JSON::FieldDataType::Integer, true },
                { "DeviceID", Utilities::JSON::FieldDataType::Integer, true },
                { "DataType", Utilities::JSON::FieldDataType::Integer, true },
                { "Alarms", Utilities::JSON::FieldDataType::String, true },
                { "Labels", Utilities::JSON::FieldDataType::String, true },
                { "Name", Utilities::JSON::FieldDataType::String, true },
                { "Description", Utilities::JSON::FieldDataType::String, true }
            };
            Utilities::JSON::Validate(required_fields, requestBody);

            // Lock mutex before database operations
            std::lock_guard<std::mutex> lock(ConfigDB::GetMutex());

            SQLite::Statement query(*ConfigDB::Get(), 
                "INSERT INTO Signals (Name, Description, DataType, IsArchived, IsSteerable, GroupID, DeviceID, Alarms, Labels) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)");
            
            query.bind(1, requestBody["Name"].get<std::string>());
            query.bind(2, requestBody["Description"].get<std::string>());
            query.bind(3, requestBody["Description"].get<int>());
            query.bind(4, static_cast<int>(requestBody["IsArchived"].get<bool>()));
            query.bind(5, static_cast<int>(requestBody["IsSteerable"].get<bool>()));
            query.bind(6, requestBody["GroupID"].get<int>());
            query.bind(7, requestBody["DeviceID"].get<int>());
            query.bind(8, requestBody["Alarms"].dump());
            query.bind(9, requestBody["Labels"].dump());

            try {
                query.exec();
            } 
            catch (const SQLite::Exception& e) {
                throw std::runtime_error("SQLite error: " + std::string(e.what()));
            }

            response.setStatus(Poco::Net::HTTPResponse::HTTP_CREATED);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json responseJson = {{"status", "success"}, {"message", "Signal added successfully"}};
            out << responseJson.dump();
        }
        catch (const std::exception& e) {
            response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json errorJson = {{"status", "error"}, {"message", e.what()}};
            out << errorJson.dump();
            Logger::error("Error adding signal: {}", e.what());
        }
        catch (...) {
            response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json errorJson = {{"status", "error"}, {"message", "An unexpected error occurred"}};
            out << errorJson.dump();
            Logger::error("Unexpected error occurred while adding signal definition");
        }
    }

    void update_signal(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept {
        try {
            Logger::trace("Updating analog signal definition in DB.");

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
                { "ID", Utilities::JSON::FieldDataType::Integer, true },
                { "IsArchived", Utilities::JSON::FieldDataType::Boolean, false },
                { "IsSteerable", Utilities::JSON::FieldDataType::Boolean, false },
                { "GroupID", Utilities::JSON::FieldDataType::Integer, false },
                { "DeviceID", Utilities::JSON::FieldDataType::Integer, false },
                { "DataType", Utilities::JSON::FieldDataType::Integer, false },
                { "Alarms", Utilities::JSON::FieldDataType::String, false },
                { "Labels", Utilities::JSON::FieldDataType::String, false },
                { "Name", Utilities::JSON::FieldDataType::String, false },
                { "Description", Utilities::JSON::FieldDataType::String, false }
            };
            Utilities::JSON::Validate(required_fields, requestBody);

            // Lock mutex before database operations
            std::lock_guard<std::mutex> lock(ConfigDB::GetMutex());

            std::string updateQuery = "UPDATE Signals SET ";
            std::vector<std::string> updateFields;
            std::vector<std::string> bindValues;

            if (requestBody.contains("Name")) {
                updateFields.push_back("Name = ?");
                bindValues.push_back(requestBody["Name"].get<std::string>());
            }
            if (requestBody.contains("Description")) {
                updateFields.push_back("Description = ?");
                bindValues.push_back(requestBody["Description"].get<std::string>());
            }
            if (requestBody.contains("IsArchived")) {
                updateFields.push_back("IsArchived = ?");
                bindValues.push_back(std::to_string(requestBody["IsArchived"].get<bool>() ? 1 : 0));
            }
            if (requestBody.contains("IsSteerable")) {
                updateFields.push_back("IsSteerable = ?");
                bindValues.push_back(std::to_string(requestBody["IsSteerable"].get<bool>() ? 1 : 0));
            }
            if (requestBody.contains("GroupID")) {
                updateFields.push_back("GroupID = ?");
                bindValues.push_back(std::to_string(requestBody["GroupID"].get<int>()));
            }
            if (requestBody.contains("DeviceID")) {
                updateFields.push_back("DeviceID = ?");
                bindValues.push_back(std::to_string(requestBody["DeviceID"].get<int>()));
            }
            updateFields.push_back("Alarms = ?");
            bindValues.push_back(requestBody["Alarms"].dump());
            updateFields.push_back("Labels = ?");
            bindValues.push_back(requestBody["Labels"].dump());

            for (size_t i = 0; i < updateFields.size(); ++i) {
                updateQuery += updateFields[i];
                if (i < updateFields.size() - 1) {
                    updateQuery += ", ";
                }
            }

            updateQuery += " WHERE ID = ?";

            SQLite::Statement query(*ConfigDB::Get(), updateQuery);
            
            int bindIndex = 1;
            for (const auto& value : bindValues) {
                query.bind(bindIndex++, value);
            }
            query.bind(bindIndex, requestBody["ID"].get<int>());

            try {
                query.exec();
            } 
            catch (const SQLite::Exception& e) {
                throw std::runtime_error("SQLite error: " + std::string(e.what()));
            }

            if (query.getChanges() == 0) {
                throw std::runtime_error("No record found with the given ID");
            }

            response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json responseJson = {{"status", "success"}, {"message", "Signal definition updated successfully"}};
            out << responseJson.dump();
        }
        catch (const std::exception& e) {
            response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json errorJson = {{"status", "error"}, {"message", e.what()}};
            out << errorJson.dump();
            Logger::error("Error updating signal: {}", e.what());
        }
        catch (...) {
            response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json errorJson = {{"status", "error"}, {"message", "An unexpected error occurred"}};
            out << errorJson.dump();
            Logger::error("Unexpected error occurred while updating signal definition");
        }
    }

    void delete_signal(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept {
        try {
            Logger::trace("Deleting analog signal definition from DB.");

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
                { "ID", Utilities::JSON::FieldDataType::Integer, true }
            };
            Utilities::JSON::Validate(required_fields, requestBody);

            std::lock_guard<std::mutex> lock(ConfigDB::GetMutex());

            SQLite::Statement query(*ConfigDB::Get(), "DELETE FROM Signals WHERE ID = ?");
            
            query.bind(1, requestBody["ID"].get<int>());

            try {
                query.exec();
            } 
            catch (const SQLite::Exception& e) {
                throw std::runtime_error("SQLite error: " + std::string(e.what()));
            }

            if (query.getChanges() == 0) {
                throw std::runtime_error("No record found with the given ID");
            }

            response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json responseJson = {{"status", "success"}, {"message", "Signal definition deleted successfully"}};
            out << responseJson.dump();
        }
        catch (const std::exception& e) {
            response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json errorJson = {{"status", "error"}, {"message", e.what()}};
            out << errorJson.dump();
            Logger::error("Error deleting signal definition: {}", e.what());
        }
        catch (...) {
            response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json errorJson = {{"status", "error"}, {"message", "An unexpected error occurred"}};
            out << errorJson.dump();
            Logger::error("Unexpected error occurred while deleting signal definitions");
        }
    }
}

namespace GroupDefinitions {
    void get_all(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept {
        try {
            Logger::trace("Reading all group definitions from DB.");

            auto groups = Groups::Group::GetFromDatabase();
            nlohmann::json j = nlohmann::json::array();
            for (auto& group : groups) {
                j.push_back(group.ToJson());
            }

            response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            out << j.dump();
        }
        catch (const std::exception& e) {
            response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json errorJson = {{"status", "error"}, {"message", e.what()}};
            out << errorJson.dump();
            Logger::error("Error retrieving group definitions: {}", e.what());
        }
    }

    void add_group(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept {
        try {
            Logger::trace("Adding new group definition to DB.");

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
                { "Name", Utilities::JSON::FieldDataType::String, true },
                { "Description", Utilities::JSON::FieldDataType::String, true }
            };
            Utilities::JSON::Validate(required_fields, requestBody);

            std::lock_guard<std::mutex> lock(ConfigDB::GetMutex());

            SQLite::Statement query(*ConfigDB::Get(), 
                "INSERT INTO Groups (Name, Description) VALUES (?, ?)");
            
            query.bind(1, requestBody["Name"].get<std::string>());
            query.bind(2, requestBody["Description"].get<std::string>());

            try {
                query.exec();
            } 
            catch (const SQLite::Exception& e) {
                throw std::runtime_error("SQLite error: " + std::string(e.what()));
            }

            response.setStatus(Poco::Net::HTTPResponse::HTTP_CREATED);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json responseJson = {{"status", "success"}, {"message", "Group added successfully"}};
            out << responseJson.dump();
        }
        catch (const std::exception& e) {
            response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json errorJson = {{"status", "error"}, {"message", e.what()}};
            out << errorJson.dump();
            Logger::error("Error adding group: {}", e.what());
        }
    }

    void update_group(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept {
        try {
            Logger::trace("Updating group definition in DB.");

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
                { "GroupID", Utilities::JSON::FieldDataType::Integer, true },
                { "Name", Utilities::JSON::FieldDataType::String, true },
                { "Description", Utilities::JSON::FieldDataType::String, true }
            };
            Utilities::JSON::Validate(required_fields, requestBody);

            std::lock_guard<std::mutex> lock(ConfigDB::GetMutex());

            std::string updateQuery = "UPDATE Groups SET ";
            std::vector<std::string> updateFields;
            std::vector<std::string> bindValues;

            if (requestBody.contains("Name")) {
                updateFields.push_back("Name = ?");
                bindValues.push_back(requestBody["Name"].get<std::string>());
            }
            if (requestBody.contains("Description")) {
                updateFields.push_back("Description = ?");
                bindValues.push_back(requestBody["Description"].get<std::string>());
            }

            if (updateFields.empty()) {
                throw std::runtime_error("No fields to update");
            }

            for (size_t i = 0; i < updateFields.size(); ++i) {
                updateQuery += updateFields[i];
                if (i < updateFields.size() - 1) {
                    updateQuery += ", ";
                }
            }

            updateQuery += " WHERE GroupID = ?";

            SQLite::Statement query(*ConfigDB::Get(), updateQuery);
            
            int bindIndex = 1;
            for (const auto& value : bindValues) {
                query.bind(bindIndex++, value);
            }
            query.bind(bindIndex, requestBody["GroupID"].get<int>());

            try {
                query.exec();
            } 
            catch (const SQLite::Exception& e) {
                throw std::runtime_error("SQLite error: " + std::string(e.what()));
            }

            if (query.getChanges() == 0) {
                throw std::runtime_error("No record found with the given GroupID");
            }

            response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json responseJson = {{"status", "success"}, {"message", "Group updated successfully"}};
            out << responseJson.dump();
        }
        catch (const std::exception& e) {
            response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json errorJson = {{"status", "error"}, {"message", e.what()}};
            out << errorJson.dump();
            Logger::error("Error updating group: {}", e.what());
        }
    }

    void delete_group(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept {
        try {
            Logger::trace("Deleting group definition from DB.");

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
                { "GroupID", Utilities::JSON::FieldDataType::Integer, true }
            };
            Utilities::JSON::Validate(required_fields, requestBody);

            // Lock mutex before database operations
            std::lock_guard<std::mutex> lock(ConfigDB::GetMutex());

            SQLite::Statement query(*ConfigDB::Get(), "DELETE FROM Groups WHERE GroupID = ?");
            
            query.bind(1, requestBody["GroupID"].get<int>());

            try {
                query.exec();
            } 
            catch (const SQLite::Exception& e) {
                throw std::runtime_error("SQLite error: " + std::string(e.what()));
            }

            if (query.getChanges() == 0) {
                throw std::runtime_error("No record found with the given GroupID");
            }

            response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json responseJson = {{"status", "success"}, {"message", "Group deleted successfully"}};
            out << responseJson.dump();
        }
        catch (const std::exception& e) {
            response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json errorJson = {{"status", "error"}, {"message", e.what()}};
            out << errorJson.dump();
            Logger::error("Error deleting group: {}", e.what());
        }
    }
}