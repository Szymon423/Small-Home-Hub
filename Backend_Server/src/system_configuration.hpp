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
    
    /// @brief Updates configuration
    /// @param request HTTP request
    /// @param response HTTP response
    void update(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept;

    /// @brief Handles importing configuration database to system
    /// @param request HTTP request
    /// @param response HTTP response
    void import_db(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept;

    /// @brief Handles exporting configuration database from system
    /// @param request HTTP request
    /// @param response HTTP response
    void export_db(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept;
}
