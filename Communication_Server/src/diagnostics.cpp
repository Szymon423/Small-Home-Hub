#include "authentication.hpp"
#include <Common/database.hpp>
#include <Common/users.hpp>
#include <Poco/JWT/Token.h>
#include <Poco/JWT/Signer.h>
#include <nlohmann/json.hpp>
#include <vector>
#include <sstream>
#include <chrono>

namespace Diagnostics {
    void status(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept {
        try {
            nlohmann::json responseJson = {
                {"status", "OK"},
            };

            response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            out << responseJson.dump();
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
        catch (...) {
            nlohmann::json errorJson = { {"status", "error"}, {"message", "Internal server error"} };
            response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            out << errorJson.dump();
        }
    }
}