#include "authentication.hpp"
#include <Common/database.hpp>
#include <Common/users.hpp>
#include <Poco/JWT/Token.h>
#include <Poco/JWT/Signer.h>
#include <nlohmann/json.hpp>
#include <vector>
#include <algorithm>
#include <sstream>
#include <chrono>

namespace Authentication {
    // Funkcja pomocnicza do tworzenia tokena JWT
    std::string createAccessToken(Users::User user) {
        Poco::JWT::Token token;
        token.setType("JWT");
        token.setSubject(std::to_string(user.userID));
        token.payload().set("privilege", static_cast<std::uint8_t>(user.privileges));
        token.payload().set("username", user.name);
        auto now = std::chrono::system_clock::now();
        auto nowInSeconds = duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
        token.payload().set("iat", nowInSeconds);
        auto expTime = now + std::chrono::minutes(30);
        auto expInSeconds = duration_cast<std::chrono::seconds>(expTime.time_since_epoch()).count();
        token.payload().set("exp", expInSeconds);
        
        // This will be my secret Key
        Poco::JWT::Signer signer("0123456789ABCDEF0123456789ABCDEF");
        return signer.sign(token, Poco::JWT::Signer::ALGO_HS256);
    }

    void login(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept {
      try {
            nlohmann::json requestJson = nlohmann::json::parse(request.stream());

            std::string username = requestJson["username"];
            std::string password = requestJson["password"];

            // Read users from db
            auto users = Users::User::GetFromDatabase();
            
            // Search for user
            auto it = std::find_if(users.begin(), users.end(), [&](const Users::User& u) {
                return u.login == username && u.password == password;
            });

            if (it != users.end()) {
                // User found
                bool is_admin = (it->privileges == Users::Privileges::Admin) | (it->privileges == Users::Privileges::Service);
                std::string access_token = createAccessToken(*it);

                nlohmann::json responseJson = {
                    {"token", access_token},
                    {"user", {
                        {"id", it->userID},
                        {"username", it->name},
                        {"is_admin", is_admin}
                    }}
                };

                response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                response.setContentType("application/json");
                std::ostream& out = response.send();
                out << responseJson.dump();
            } 
            else {
                // User not found
                nlohmann::json errorJson = {{"msg", "Invalid credentials"}};
                response.setStatus(Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED);
                response.setContentType("application/json");
                std::ostream& out = response.send();
                out << errorJson.dump();
            }
        } 
        catch (const nlohmann::json::parse_error& e) {
            nlohmann::json errorJson = { {"status", "error"}, {"message", "Internal JSON: " + std::string(e.what())} };
            response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            out << errorJson.dump();
        } 
        catch (const Poco::Exception& e) {
            nlohmann::json errorJson = { {"status", "error"}, {"message", "Internal server error: " + std::string(e.displayText())} };
            response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            out << errorJson.dump();
        } 
        catch (const std::exception& e) {
            nlohmann::json errorJson = { {"status", "error"}, {"message", "Internal server error: " + std::string(e.what())} };
            response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            out << errorJson.dump();
        }
    }
}