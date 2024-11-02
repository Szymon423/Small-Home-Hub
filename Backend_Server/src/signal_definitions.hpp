#pragma once

#include <iostream>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

/// @brief Namespace for functions interacting with configuration database with SignalDefinitions table
namespace SignalDefinitions {
    /// @brief Retrieves all SignalDefinitions
    /// @param request HTTP request
    /// @param response HTTP response
    void get_all(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept;

    /// @brief Adds mew record in SignalDefinitions table in database
    /// @param request HTTP request
    /// @param response HTTP response
    void add_signal(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept;

    /// @brief Updates record in SignalDefinitions table in database
    /// @param request HTTP request
    /// @param response HTTP response
    void update_signal(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept;

    /// @brief Deletes record in SignalDefinitions table in database
    /// @param request HTTP request
    /// @param response HTTP response
    void delete_signal(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept;
}

/// @brief Namespace for functions interacting with configuration database with Groups table
namespace GroupDefinitions {
    /// @brief Retrieves all Groups
    /// @param request HTTP request
    /// @param response HTTP response
    void get_all(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept;

    /// @brief Adds mew record in Groups table in database
    /// @param request HTTP request
    /// @param response HTTP response
    void add_group(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept;

    /// @brief Updates record in Groups table in database
    /// @param request HTTP request
    /// @param response HTTP response
    void update_group(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept;

    /// @brief Deletes record in Binary_Signals_Definitions table in database
    /// @param request HTTP Groups
    /// @param response HTTP response
    void delete_group(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept;
}