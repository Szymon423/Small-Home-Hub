#pragma once

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

namespace Users {
    /// @brief Retrieves all Users
    /// @param request HTTP request
    /// @param response HTTP response
    void get_all(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept;

    /// @brief Adds new record to Users table in database
    /// @param request HTTP request
    /// @param response HTTP response
    void add_user(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept;

    /// @brief Updates record in Users table in database
    /// @param request HTTP request
    /// @param response HTTP response
    void update_user(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept;

    /// @brief Deletes record in Users table in database
    /// @param request HTTP request
    /// @param response HTTP response
    void delete_user(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept;
}