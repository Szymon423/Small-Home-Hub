#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

/// @brief Namespace for devices related elements
namespace Devices {
    /// @brief Helper class representing short definition of signal associated with given device type
    class Signal {
    public:
        bool isAnalog;        // when set to true - this signal is analog type
        bool isSteerable;     // when set to true - this signal value can be changed from system
        std::string name;     // Template name of this signal
        std::string unit;     // Template unit of this signal

        /// @brief Function to convert Signal to JSON object
        /// @return JSON object with Signal related variables
        nlohmann::json ToJson() const;

        /// @brief Function to create Signal from JSON
        /// @param j JSON from which create Signal
        /// @return Signal object
        static Signal FromJson(const nlohmann::json& j);
    };

    /// @brief Class representing entry in DeviceTypes table
    class Type {
    public:
        std::uint16_t typeID;           // ID of type in DeviceTypes table
        std::string name;               // name of device type
        std::vector<Signal> signals;    // list of signals associated with device type

        /// @brief Function to convert Type to JSON object
        /// @return JSON object with Type related variables
        nlohmann::json ToJson();

        /// @brief Function to create Type from JSON
        /// @param j JSON from which create Type
        /// @return Type object
        static Type FromJson(const nlohmann::json& j);

        /// @brief Function to read all Type from Configuration Database
        /// @return Vector of Type 
        static std::vector<Type> GetFromDatabase();
    };

    /// @brief Class representing entry in Devices table
    class Device {
    public:
        std::uint32_t deviceID;         // ID of device in Users table
        std::uint32_t deviceTypeID;     // ID of type of this device
        std::string description;        // Description of device

        /// @brief Function to convert Device to JSON object
        /// @return JSON object with Device related variables
        nlohmann::json ToJson();

        /// @brief Function to create Device from JSON
        /// @param j JSON from which create Device
        /// @return Device object
        static Device FromJson(const nlohmann::json& j);

        /// @brief Function to read all Device from Configuration Database
        /// @return Vector of Device 
        static std::vector<Device> GetFromDatabase();
    };
}