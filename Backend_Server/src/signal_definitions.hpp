#pragma once

#include <iostream>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

/// @brief Namespace for functions interacting with configuration database with Analog_Signals_Definitions table
namespace AnalogSignalDefinitions {
    /// @brief Retrieves all Analog_Signals_Definitions
    /// @param request HTTP request
    /// @param response HTTP response
    void get_all(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept;

    /// @brief Adds mew record in Analog_Signals_Definitions table in database
    /// @param request HTTP request
    /// @param response HTTP response
    void add_signal(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept;

    /// @brief Updates record in Analog_Signals_Definitions table in database
    /// @param request HTTP request
    /// @param response HTTP response
    void update_signal(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept;

    /// @brief Deletes record in Analog_Signals_Definitions table in database
    /// @param request HTTP request
    /// @param response HTTP response
    void delete_signal(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept;
}

/// @brief Namespace for functions interacting with configuration database with Binary_Signals_Definitions table
namespace BinarySignalDefinitions {
    /// @brief Retrieves all Binary_Signals_Definitions
    /// @param request HTTP request
    /// @param response HTTP response
    void get_all(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept;

    /// @brief Adds mew record in Binary_Signals_Definitions table in database
    /// @param request HTTP request
    /// @param response HTTP response
    void add_signal(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept;

    /// @brief Updates record in Binary_Signals_Definitions table in database
    /// @param request HTTP request
    /// @param response HTTP response
    void update_signal(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept;

    /// @brief Deletes record in Binary_Signals_Definitions table in database
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