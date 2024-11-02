#include "events.hpp"
#include "utilities.hpp"
#include <Common/log.hpp>
#include <Common/events.hpp>
#include <Common/utilities.hpp>
#include <nlohmann/json.hpp>
#include <exception>
#include <cpr/cpr.h>

namespace EventsHandling {
    void get_all(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept {
        try {
            Logger::trace("Reading all events from DB.");

            auto events = Events::Event::GetFromDatabase();
            nlohmann::json j = nlohmann::json::array();
            for (auto& event : events) {
                j.push_back(event.ToJson());
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
            Logger::error("Error retrieving events: {}", e.what());
        }
    }

    void acknowledge(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept {
        try {
            Logger::trace("Acknowledging event.");

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
                { "EventID", FieldDataType::Integer, true },
                { "UserID", FieldDataType::Integer, true }
            };
            validate_fields(required_fields, requestBody);

            std::uint16_t eventID = requestBody["EventID"];
            std::uint16_t userID = requestBody["UserID"];

            std::string query = "UPDATE Events SET UserID = " + std::to_string(userID) + 
                ", IsAcknowledged = true, AcknowledgeTimestamp = " + Utilities::Time::ToString(std::chrono::system_clock::now()) + 
                " WHERE EventID = " + std::to_string(eventID);

            cpr::Response r = cpr::Get(
                cpr::Url{"http://localhost:9000/exec"},
                cpr::Parameters{{"query", query}}
            );

            if (r.status_code != 200) {
                Logger::error("Error on database connection: {}", r.status_code);
                throw std::runtime_error("Error on database connection: " + std::to_string(r.status_code));
            }


            // Response
            response.setStatus(Poco::Net::HTTPResponse::HTTP_CREATED);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json responseJson = {{"status", "success"}, {"message", "Event acknowledged successfully"}};
            out << responseJson.dump();
        }
        catch (const std::exception& e) {
            response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json errorJson = {{"status", "error"}, {"message", e.what()}};
            out << errorJson.dump();
            Logger::error("Error acknowledging event: {}", e.what());
        }
        catch (...) {
            // Catch any other unexpected exceptions
            response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json errorJson = {{"status", "error"}, {"message", "An unexpected error occurred"}};
            out << errorJson.dump();
            Logger::error("Unexpected error occurred while acknowledging event");
        }
    }
}