#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

/// @brief Namespace for users related elements
namespace Users {
    /// @brief Enum with allowed user privileges
    enum class Privileges : std::uint8_t {
        Service,
        Admin,
        Operator,
        Unknown
    };

    /// @brief Function to convert Privileges to associated integer value
    /// @param p Privileges refference
    /// @return Associated integer value
    std::uint8_t PrivilegesToInt(const Privileges& p);

    /// @brief Function to convert integer to associated Privileges
    /// @param p Integer defining privileges
    /// @return Associated Privileges
    Privileges PrivilegesFromInt(const int p);

    /// @brief Class representing entry in Users table
    class User {
    public:
        std::uint32_t userID;       // ID of user in Users table
        std::string name;           // Name of user
        std::string surname;        // Surname of user
        std::string login;          // Login of user
        std::string password;       // Password of user
        Privileges privileges;      // Privileges of user

        /// @brief Function to convert user to JSON object
        /// @return JSON object with user related variables
        nlohmann::json ToJson();

        /// @brief Function to create User from JSON
        /// @param j JSON from which create User
        /// @return User object
        static User FromJson(const nlohmann::json& j);

        /// @brief Function to read all User from Configuration Database
        /// @return Vector of User 
        static std::vector<User> GetFromDatabase();
    };
}