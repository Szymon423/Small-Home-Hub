#include "signal_definitions.hpp"
#include "utilities.hpp"
#include <Common/log.hpp>
#include <Common/database.hpp>
#include <Common/groups.hpp>
#include <Common/signals.hpp>
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

    // TODO: this function must be adjusted to used member variables
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
            static const std::vector<std::tuple<std::string, FieldDataType, bool>> required_fields {
                { "IsArchived", FieldDataType::Boolean, true },
                { "GroupID", FieldDataType::Integer, true },
                { "SourceType", FieldDataType::Integer, true },
                { "SourceID", FieldDataType::Integer, true },
                { "AlarmLowEnabled", FieldDataType::Boolean, true },
                { "AlarmHighEnabled", FieldDataType::Boolean, true },
                { "AlarmLowThreshold", FieldDataType::Float, true },
                { "AlarmHighThreshold", FieldDataType::Float, true },
                { "WarningLowEnabled", FieldDataType::Boolean, true },
                { "WarningHighEnabled", FieldDataType::Boolean, true },
                { "WarningLowThreshold", FieldDataType::Float, true },
                { "WarningHighThreshold", FieldDataType::Float, true },
                { "Unit", FieldDataType::String, true },
                { "Name", FieldDataType::String, true },
                { "Description", FieldDataType::String, true }
            };
            validate_fields(required_fields, requestBody);

            nlohmann::json alarms = {
                {"AlarmLowEnabled", requestBody["AlarmLowEnabled"].get<bool>()},
                {"AlarmHighEnabled", requestBody["AlarmHighEnabled"].get<bool>()},
                {"AlarmLowThreshold", requestBody["AlarmLowThreshold"].get<double>()},
                {"AlarmHighThreshold", requestBody["AlarmHighThreshold"].get<double>()},
            };

            nlohmann::json warnings = {
                {"WarningLowEnabled", requestBody["WarningLowEnabled"].get<bool>()},
                {"WarningHighEnabled", requestBody["WarningHighEnabled"].get<bool>()},
                {"WarningLowThreshold", requestBody["WarningLowThreshold"].get<double>()},
                {"WarningHighThreshold", requestBody["WarningHighThreshold"].get<double>()},
            };

            nlohmann::json labels = {
                {"Unit", requestBody["Unit"].get<std::string>()}
            };

            // Lock mutex before database operations
            std::lock_guard<std::mutex> lock(ConfigDB::GetMutex());

            SQLite::Statement query(*ConfigDB::Get(), 
                "INSERT INTO Signals (Name, Description, DataType, IsArchived, GroupID, SourceType, SourceID, Alarms, Warnings, Labels) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
            
            query.bind(1, requestBody["Name"].get<std::string>());
            query.bind(2, requestBody["Description"].get<std::string>());
            query.bind(3, 0);
            query.bind(4, static_cast<int>(requestBody["IsArchived"].get<bool>()));
            query.bind(5, requestBody["GroupID"].get<int>());
            query.bind(6, requestBody["SourceType"].get<int>());
            query.bind(7, requestBody["SourceID"].get<int>());
            query.bind(8, alarms.dump());
            query.bind(9, warnings.dump());
            query.bind(10, labels.dump());

            try {
                query.exec();
            } 
            catch (const SQLite::Exception& e) {
                throw std::runtime_error("SQLite error: " + std::string(e.what()));
            }

            response.setStatus(Poco::Net::HTTPResponse::HTTP_CREATED);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json responseJson = {{"status", "success"}, {"message", "Analog signal added successfully"}};
            out << responseJson.dump();
        }
        catch (const std::exception& e) {
            response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json errorJson = {{"status", "error"}, {"message", e.what()}};
            out << errorJson.dump();
            Logger::error("Error adding analog signal: {}", e.what());
        }
        catch (...) {
            response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json errorJson = {{"status", "error"}, {"message", "An unexpected error occurred"}};
            out << errorJson.dump();
            Logger::error("Unexpected error occurred while adding analog signal definitions");
        }
    }

    // TODO: this function must be adjusted to used member variables
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
            static const std::vector<std::tuple<std::string, FieldDataType, bool>> required_fields {
                { "AnalogDefinitionID", FieldDataType::Integer, true },
                { "IsArchived", FieldDataType::Boolean, false },
                { "GroupID", FieldDataType::Integer, false },
                { "SourceType", FieldDataType::Integer, false },
                { "SourceID", FieldDataType::Integer, false },
                { "AlarmLowEnabled", FieldDataType::Boolean, false },
                { "AlarmHighEnabled", FieldDataType::Boolean, false },
                { "AlarmLowThreshold", FieldDataType::Float, false },
                { "AlarmHighThreshold", FieldDataType::Float, false },
                { "WarningLowEnabled", FieldDataType::Boolean, false },
                { "WarningHighEnabled", FieldDataType::Boolean, false },
                { "WarningLowThreshold", FieldDataType::Float, false },
                { "WarningHighThreshold", FieldDataType::Float, false },
                { "Unit", FieldDataType::String, false },
                { "Name", FieldDataType::String, false },
                { "Description", FieldDataType::String, false }
            };
            validate_fields(required_fields, requestBody);

            nlohmann::json alarms = {
                {"AlarmLowEnabled", requestBody.value("AlarmLowEnabled", false)},
                {"AlarmHighEnabled", requestBody.value("AlarmHighEnabled", false)},
                {"AlarmLowThreshold", requestBody.value("AlarmLowThreshold", 0.0)},
                {"AlarmHighThreshold", requestBody.value("AlarmHighThreshold", 0.0)}
            };

            nlohmann::json warnings = {
                {"WarningLowEnabled", requestBody.value("WarningLowEnabled", false)},
                {"WarningHighEnabled", requestBody.value("WarningHighEnabled", false)},
                {"WarningLowThreshold", requestBody.value("WarningLowThreshold", 0.0)},
                {"WarningHighThreshold", requestBody.value("WarningHighThreshold", 0.0)}
            };

            nlohmann::json labels = {
                {"Unit", requestBody.value("Unit", "")}
            };

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
            if (requestBody.contains("GroupID")) {
                updateFields.push_back("GroupID = ?");
                bindValues.push_back(std::to_string(requestBody["GroupID"].get<int>()));
            }
            if (requestBody.contains("SourceType")) {
                updateFields.push_back("SourceType = ?");
                bindValues.push_back(std::to_string(requestBody["SourceType"].get<int>()));
            }
            if (requestBody.contains("SourceID")) {
                updateFields.push_back("SourceID = ?");
                bindValues.push_back(std::to_string(requestBody["SourceID"].get<int>()));
            }
            updateFields.push_back("Alarms = ?");
            bindValues.push_back(alarms.dump());
            updateFields.push_back("Warnings = ?");
            bindValues.push_back(warnings.dump());
            updateFields.push_back("Labels = ?");
            bindValues.push_back(labels.dump());

            for (size_t i = 0; i < updateFields.size(); ++i) {
                updateQuery += updateFields[i];
                if (i < updateFields.size() - 1) {
                    updateQuery += ", ";
                }
            }

            updateQuery += " WHERE SignalID = ?";

            SQLite::Statement query(*ConfigDB::Get(), updateQuery);
            
            int bindIndex = 1;
            for (const auto& value : bindValues) {
                query.bind(bindIndex++, value);
            }
            query.bind(bindIndex, requestBody["AnalogDefinitionID"].get<int>());

            try {
                query.exec();
            } 
            catch (const SQLite::Exception& e) {
                throw std::runtime_error("SQLite error: " + std::string(e.what()));
            }

            if (query.getChanges() == 0) {
                throw std::runtime_error("No record found with the given AnalogDefinitionID");
            }

            response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json responseJson = {{"status", "success"}, {"message", "Analog signal definition updated successfully"}};
            out << responseJson.dump();
        }
        catch (const std::exception& e) {
            response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json errorJson = {{"status", "error"}, {"message", e.what()}};
            out << errorJson.dump();
            Logger::error("Error updating analog signal: {}", e.what());
        }
        catch (...) {
            response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json errorJson = {{"status", "error"}, {"message", "An unexpected error occurred"}};
            out << errorJson.dump();
            Logger::error("Unexpected error occurred while updating analog signal definitions");
        }
    }

    // TODO: this function must be adjusted to used member variables
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
            static const std::vector<std::tuple<std::string, FieldDataType, bool>> required_fields {
                { "AnalogDefinitionID", FieldDataType::Integer, true }
            };
            validate_fields(required_fields, requestBody);

            std::lock_guard<std::mutex> lock(ConfigDB::GetMutex());

            SQLite::Statement query(*ConfigDB::Get(), "DELETE FROM Signals WHERE SignalID = ?");
            
            query.bind(1, requestBody["AnalogDefinitionID"].get<int>());

            try {
                query.exec();
            } 
            catch (const SQLite::Exception& e) {
                throw std::runtime_error("SQLite error: " + std::string(e.what()));
            }

            if (query.getChanges() == 0) {
                throw std::runtime_error("No record found with the given AnalogDefinitionID");
            }

            response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json responseJson = {{"status", "success"}, {"message", "Analog signal definition deleted successfully"}};
            out << responseJson.dump();
        }
        catch (const std::exception& e) {
            response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json errorJson = {{"status", "error"}, {"message", e.what()}};
            out << errorJson.dump();
            Logger::error("Error deleting analog signal definition: {}", e.what());
        }
        catch (...) {
            response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json errorJson = {{"status", "error"}, {"message", "An unexpected error occurred"}};
            out << errorJson.dump();
            Logger::error("Unexpected error occurred while deleting analog signal definitions");
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
            static const std::vector<std::tuple<std::string, FieldDataType, bool>> required_fields {
                { "Name", FieldDataType::String, true },
                { "Description", FieldDataType::String, true }
            };
            validate_fields(required_fields, requestBody);

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
            static const std::vector<std::tuple<std::string, FieldDataType, bool>> required_fields {
                { "GroupID", FieldDataType::Integer, true },
                { "Name", FieldDataType::String, true },
                { "Description", FieldDataType::String, true }
            };
            validate_fields(required_fields, requestBody);

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
            static const std::vector<std::tuple<std::string, FieldDataType, bool>> required_fields {
                { "GroupID", FieldDataType::Integer, true }
            };
            validate_fields(required_fields, requestBody);

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