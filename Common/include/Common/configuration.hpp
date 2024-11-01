#pragma once 

#include <string>

/// @brief 
class Configuration {
public:
    /// @brief Structure with config itself
    struct Config {
        /// @brief String with device ip
        std::string deviceIp;

        /// @brief String with ip-mask
        std::string deviceNetworkMask;

        /// @brief String with device name
        std::string deviceName;
        
        /// @brief Number of days after which archived data will be deleted
        std::uint16_t dataRetentionPeriod;
        
        /// @brief Port number which is associated with backend server
        std::uint16_t backendServerPort;
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