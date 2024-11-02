#include "users.hpp"
#include <Common/utilities.hpp>
#include <nlohmann/json.hpp>
#include <Common/log.hpp>
#include <Common/users.hpp>
#include <Common/database.hpp>
#include <exception>
#include <tuple>
#include <string>
#include <vector>

namespace Users {
    void get_all(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept {
        try {
            Logger::trace("Reading users from DB.");
            
            nlohmann::json j = nlohmann::json::array();
            auto users = Users::User::GetFromDatabase();
            for (auto& user : users) {
                j.push_back(user.ToJson());
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
            // Catch any other unexpected exceptions
            response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json errorJson = {{"status", "error"}, {"message", "An unexpected error occurred"}};
            out << errorJson.dump();
            Logger::error("Unexpected error occurred while reaading users");
        }
    }

    void add_user(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept {
        try {
            Logger::trace("Adding new user to DB.");

            // Parse JSON from request body
            nlohmann::json requestBody = nlohmann::json::parse(request.stream());

            // Validate required fields
            static const std::vector<std::tuple<std::string, Utilities::JSON::FieldDataType, bool>> required_fields {
                { "Name", Utilities::JSON::FieldDataType::String, true },
                { "Surname", Utilities::JSON::FieldDataType::String, true },
                { "Login", Utilities::JSON::FieldDataType::String, true },
                { "Password", Utilities::JSON::FieldDataType::String, false },
                { "Privileges", Utilities::JSON::FieldDataType::Integer, false }
            };
            Utilities::JSON::Validate(required_fields, requestBody);

            // Lock mutex before database operations
            std::lock_guard<std::mutex> lock(ConfigDB::GetMutex());

            SQLite::Statement query(*ConfigDB::Get(), 
                "INSERT INTO Users (Name, Surname, Login, Password, Privileges) VALUES (?, ?, ?, ?, ?)");
            
            query.bind(1, requestBody["Name"].get<std::string>());
            query.bind(2, requestBody["Surname"].get<std::string>());
            query.bind(3, requestBody["Login"].get<std::string>());
            query.bind(4, requestBody["Password"].get<std::string>());
            query.bind(5, requestBody["Privileges"].get<int>());

            query.exec();

            // Response
            response.setStatus(Poco::Net::HTTPResponse::HTTP_CREATED);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json responseJson = {{"status", "success"}, {"message", "User added successfully"}};
            out << responseJson.dump();
        }
        catch (const std::exception& e) {
            response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json errorJson = {{"status", "error"}, {"message", e.what()}};
            out << errorJson.dump();
            Logger::error("Error adding user: {}", e.what());
        }
        catch (...) {
            // Catch any other unexpected exceptions
            response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json errorJson = {{"status", "error"}, {"message", "An unexpected error occurred"}};
            out << errorJson.dump();
            Logger::error("Unexpected error occurred while adding users");
        }
    }

    void update_user(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept {
        try {
            Logger::trace("Updating user in DB.");

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
                { "UserID", Utilities::JSON::FieldDataType::Integer, true },
                { "Name", Utilities::JSON::FieldDataType::String, false },
                { "Surname", Utilities::JSON::FieldDataType::String, false },
                { "Login", Utilities::JSON::FieldDataType::String, false },
                { "Password", Utilities::JSON::FieldDataType::String, false },
                { "Privileges", Utilities::JSON::FieldDataType::Integer, false }
            };
            Utilities::JSON::Validate(required_fields, requestBody);

            std::string updateQuery = "UPDATE Users SET ";
            std::vector<std::string> updateFields;
            std::vector<std::string> bindValues;

            if (requestBody.contains("Name")) {
                updateFields.push_back("Name = ?");
                bindValues.push_back(requestBody["Name"].get<std::string>());
            }
            if (requestBody.contains("Surname")) {
                updateFields.push_back("Surname = ?");
                bindValues.push_back(requestBody["Surname"].get<std::string>());
            }
            if (requestBody.contains("Login")) {
                updateFields.push_back("Login = ?");
                bindValues.push_back(requestBody["Login"].get<std::string>());
            }
            if (requestBody.contains("Password")) {
                updateFields.push_back("Password = ?");
                bindValues.push_back(requestBody["Password"].get<std::string>());
            }
            if (requestBody.contains("Privileges")) {
                updateFields.push_back("Privileges = ?");
                bindValues.push_back(std::to_string(requestBody["Privileges"].get<int>()));
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

            updateQuery += " WHERE UserID = ?";

            // Lock mutex before database operations
            std::lock_guard<std::mutex> lock(ConfigDB::GetMutex());

            SQLite::Statement query(*ConfigDB::Get(), updateQuery);
            
            int bindIndex = 1;
            for (const auto& value : bindValues) {
                query.bind(bindIndex++, value);
            }
            query.bind(bindIndex, requestBody["UserID"].get<int>());

            try {
                query.exec();
            } 
            catch (const SQLite::Exception& e) {
                throw std::runtime_error("SQLite error: " + std::string(e.what()));
            }

            if (query.getChanges() == 0) {
                throw std::runtime_error("No record found with the given id");
            }

            // Response
            response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json responseJson = {{"status", "success"}, {"message", "User updated successfully"}};
            out << responseJson.dump();
        }
        catch (const std::exception& e) {
            response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json errorJson = {{"status", "error"}, {"message", e.what()}};
            out << errorJson.dump();
            Logger::error("Error updating user: {}", e.what());
        }
        catch (...) {
            // Catch any other unexpected exceptions
            response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json errorJson = {{"status", "error"}, {"message", "An unexpected error occurred"}};
            out << errorJson.dump();
            Logger::error("Unexpected error occurred while updating users");
        }
    }

    void delete_user(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept {
        try {
            Logger::trace("Deleting user from DB.");

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
                { "UserID", Utilities::JSON::FieldDataType::Integer, true }
            };
            Utilities::JSON::Validate(required_fields, requestBody);

            // Lock mutex before database operations
            std::lock_guard<std::mutex> lock(ConfigDB::GetMutex());

            SQLite::Statement query(*ConfigDB::Get(), "DELETE FROM Users WHERE UserID = ?");
            
            query.bind(1, requestBody["UserID"].get<int>());

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
            nlohmann::json responseJson = {{"status", "success"}, {"message", "User deleted successfully"}};
            out << responseJson.dump();
        }
        catch (const std::exception& e) {
            response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json errorJson = {{"status", "error"}, {"message", e.what()}};
            out << errorJson.dump();
            Logger::error("Error deleting user: {}", e.what());
        }
        catch (...) {
            // Catch any other unexpected exceptions
            response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json errorJson = {{"status", "error"}, {"message", "An unexpected error occurred"}};
            out << errorJson.dump();
            Logger::error("Unexpected error occurred while deleting users");
        }
    }
}