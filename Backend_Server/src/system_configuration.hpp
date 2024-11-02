#pragma once

#include <iostream>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

/// @brief Namespace for functions interacting with configuration database
namespace SystemConfiguration {
    /// @brief Retrieves all configurations
    /// @param request HTTP request
    /// @param response HTTP response
    void get_all(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept;

    /// @brief Updates the device IP address and mask
    /// @param request HTTP request containing the new IP address
    /// @param response HTTP response
    void update_network(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept;

    /// @brief Updates the device name
    /// @param request HTTP request containing the new device name
    /// @param response HTTP response
    void update_device_name(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept;

    /// @brief Updates the Modbus configuration
    /// @param request HTTP request containing the new Modbus configuration
    /// @param response HTTP response
    void update_modbus_config(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept;

    /// @brief Updates the data retention period
    /// @param request HTTP request containing the new data retention period
    /// @param response HTTP response
    void update_data_retention_period(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept;

    /// @brief Handles importing configuration database to system
    /// @param request HTTP request
    /// @param response HTTP response
    void import_db(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept;

    /// @brief Handles exporting configuration database from system
    /// @param request HTTP request
    /// @param response HTTP response
    void export_db(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept;
}
