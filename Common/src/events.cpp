#include <Common/events.hpp>
#include <Common/log.hpp>
#include <Common/utilities.hpp>
#include <cpr/cpr.h>

namespace Events {
    Type TypeFromInt(const int type) {
        switch (type) {
            case 0: return Type::Alarm;
            case 1: return Type::Warning;
            case 2: return Type::Action;
            default: return Type::Unknown;
        }
    }

    nlohmann::json Event::ToJson() const {
        return {
            { "EventID", eventID },
            { "Type", static_cast<std::uint8_t>(type) },
            { "Message", message },
            { "IssueTimestamp", issueTimestamp.time_since_epoch().count() },
            { "AcknowledgeTimestamp", acknowledgeTimestamp.time_since_epoch().count() },
            { "IsAcknowledged", isAcknowledged },
            { "UserID", userID },
            { "AssociatedSignalID", associatedSignalID }
        };
    }

    std::vector<Event> Event:: GetFromDatabase() {
        std::vector<Event> events;

        std::string queryEvents = "SELECT * FROM Events;";
        cpr::Response r = cpr::Get(
            cpr::Url{"http://localhost:9000/exec"},
            cpr::Parameters{{"query", queryEvents}}
        );

        if (r.status_code != 200) {
            Logger::error("Error on database connection: {}", r.status_code);
            throw std::runtime_error("Error on database connection: " + std::to_string(r.status_code));
        }

        nlohmann::json dbResponse = nlohmann::json::parse(r.text);

        if (dbResponse.contains("dataset") && dbResponse["dataset"].is_array()) {
            for (const auto& set : dbResponse["dataset"]) {
                if (!set.is_array() || set.size() != 9) {
                    continue;
                }
                std::uint8_t i{ 0 };
                Event event;
                event.associatedSignalID = std::stoi(set[i++].get<std::string>());
                event.eventID = set[i++];
                std::string it = set[i++].get<std::string>();
                event.issueTimestamp = Utilities::Time::FromString(it.replace(10, 1, " ").substr(0, 19), Utilities::Time::Zone::UTC);
                std::string at = set[i++].get<std::string>();
                event.acknowledgeTimestamp = Utilities::Time::FromString(at.replace(10, 1, " ").substr(0, 19), Utilities::Time::Zone::UTC);
                event.isAcknowledged = set[i++].get<bool>();
                event.userID = set[i++].get<int>();
                event.message = set[i++].get<std::string>();
                event.type = TypeFromInt(set[i++].get<int>());
                events.push_back(event);
            }
        }
        return events;
    }

    std::vector<Event> Event::GetFromDatabaseFiltered(const bool acknowledged) {
        std::vector<Event> events;


        return events;
    }
}