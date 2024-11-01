#pragma once

#include <cstddef>
#include <chrono>
#include <string>
#include <vector>
#include <atomic>
#include <nlohmann/json.hpp>

/// @brief Namespace for Events related stuff
namespace Events {
    /// @brief Enum with supported event types
    enum class Type : std::uint8_t {
        Alarm,
        Warning,
        Action,
        Unknown
    };

    /// @brief Function to convert integer to associated event type 
    /// @param type integer holding event type
    /// @return parsed type of event
    Type TypeFromInt(const int type);

    /// @brief Class encapsulating Event 
    class Event {    
    public:
        std::uint64_t eventID;                                        // ID of Event
        Type type;                                                    // Type of Event
        std::string message;                                          // Message associated with event
        std::chrono::system_clock::time_point issueTimestamp;         // Timestamp of event issue
        std::chrono::system_clock::time_point acknowledgeTimestamp;   // Timestamp of event issue
        bool isAcknowledged;                                          // True when user acknowledged this event
        std::int32_t userID;                                          // ID of user who acknowledged this event
        std::uint16_t associatedSignalID;                             // Id of signal associated with event

        /// @brief Function to convert group to JSON object
        /// @return JSON object with group related variables
        nlohmann::json ToJson() const;

        /// @brief Function to read all Event from archive Database
        /// @return Vector of Event 
        static std::vector<Event> GetFromDatabase();

        /// @brief Function to read Event from archive Database
        /// @param acknowledged When true then gets all acknowledged Events
        /// @return Vector of Event 
        static std::vector<Event> GetFromDatabaseFiltered(const bool acknowledged = false);
    };
}