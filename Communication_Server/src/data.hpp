#pragma once

#include <iostream>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

/// @brief Namespace for functions interacting with configuration database with Analog_Signals_Definitions table
namespace Data {
    /// @brief Collects the data from incomming post request
    /// @param request HTTP request
    /// @param response HTTP response
    void collect(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept;

    /// @brief Collects the data from incomming post request
    /// @param request HTTP request
    /// @param response HTTP response
    void get(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept;
}
