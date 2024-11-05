#pragma once

#include <iostream>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

/// @brief Namespace for functions interacting with configuration database with SensorTypes table
namespace Devices {
    /// @brief Retrieves all Device Types
    /// @param request HTTP request
    /// @param response HTTP response
    void get_all_types(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept;
    
    /// @brief Retrieves all Devices
    /// @param request HTTP request
    /// @param response HTTP response
    void get_all_devices(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept;
}