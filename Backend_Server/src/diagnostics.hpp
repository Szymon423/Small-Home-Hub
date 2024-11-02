#pragma once

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

namespace Diagnostics {
    /// @brief Function to check if server if server is up and running
    /// @param request HTTP request
    /// @param response HTTP response
    void status(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept;
}