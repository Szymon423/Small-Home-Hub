#pragma once

#include <iostream>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

namespace SignalValues {
    /// @brief Retrieves all current signals values
    /// @param request HTTP request
    /// @param response HTTP response
    void get_all(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept;
}