#pragma once 

#include <string>

/// @brief 
class Configuration {
public:
    /// @brief Structure with config itself
    struct Config {
        std::string deviceIp;                   // String with device ip
        std::string deviceSubnetMask;          // String with ip-mask
        std::string deviceName;                 // String with device name
        std::uint16_t dataRetentionPeriod;      // Number of days after which archived data will be deleted
        std::uint16_t backendServerPort;        // Port number which is associated with backend server
        std::uint16_t communicationServerPort;  // Port number which is associated with communication server
    };

    /// @brief Structure with configuration 
    static Config config;

    /// @brief Function loading configuration from config database
    static void Load();
    
    /// @brief Function to get config 
    static Config Get();

    /// @brief Signleton style
    Configuration() = delete;
    ~Configuration() = delete;
    Configuration(Configuration&) = delete;
    Configuration(Configuration&&) = delete;
    Configuration& operator=(const Configuration&) = delete;
    Configuration& operator=(Configuration&&) = delete;
};