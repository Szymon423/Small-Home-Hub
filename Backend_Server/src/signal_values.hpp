#pragma once

#include <iostream>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

namespace SignalValues {
    /// @brief Retrieves all current analog values
    /// @param request HTTP request
    /// @param response HTTP response
    void get_all_analogs(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept;
    
    /// @brief Retrieves all current binary values
    /// @param request HTTP request
    /// @param response HTTP response
    void get_all_binaries(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept;
}