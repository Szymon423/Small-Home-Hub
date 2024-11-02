#pragma once

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

namespace Authentication {
    /// @brief Function handling loging
    /// @param request HTTP request
    /// @param response HTTP response
    void login(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept;
}