#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

/// @brief Namespace for groups related elements
namespace Groups {
    /// @brief Class representing entry in Groups table
    class Group {
    public:
        std::uint32_t groupID;      // ID of group in Groups table
        std::string name;           // Name of group
        std::string description;    // Description of group

        /// @brief Function to convert group to JSON object
        /// @return JSON object with group related variables
        nlohmann::json ToJson();

        /// @brief Function to create Group from JSON
        /// @param j JSON from which create Group
        /// @return Group object
        static Group FromJson(const nlohmann::json& j);
    
        /// @brief Function to read all Group from Configuration Database
        /// @return Vector of Group 
        static std::vector<Group> GetFromDatabase();
    };
}