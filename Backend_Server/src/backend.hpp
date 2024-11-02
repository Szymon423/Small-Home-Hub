#pragma once

#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/MessageHeader.h>
#include <Poco/Net/PartHandler.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Util/ServerApplication.h>
#include <Poco/CountingStream.h>
#include <Poco/UUIDGenerator.h>
#include <Poco/StreamCopier.h>
#include <Poco/NullStream.h>
#include <Poco/Exception.h>
#include <Poco/File.h>
#include <Poco/URI.h>
#include <functional>
#include <filesystem>
#include <fstream>
#include <string>
#include <map>

/// @brief Class handling HTTP requests
class MyRequestHandler : public Poco::Net::HTTPRequestHandler {
public:
    /// @brief Handles incoming HTTP requests
    /// @param request HTTP request object
    /// @param response HTTP response object
    void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) override;

private:
    /// @brief Type definition for route handler functions
    using RouteHandler = std::function<void(Poco::Net::HTTPServerRequest&, Poco::Net::HTTPServerResponse&)>;

    /// @brief Static map storing routes and their corresponding handlers
    /// Key: pair of (HTTP method, URL path)
    /// Value: function handling the route
    static std::map<std::pair<std::string, std::string>, RouteHandler> routes;

    /// @brief Handles requests for non-existent routes (404)
    /// @param request HTTP request object
    /// @param response HTTP response object
    void handleNotFound(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);

    /// @brief Handles requests for OPTIONS
    /// @param request HTTP request object
    /// @param response HTTP response object
    void handleOptions(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);

    /// @brief Sets CORS headers for the response
    /// @param response HTTP response object
    void setCorsHeaders(Poco::Net::HTTPServerResponse& response);
};

/// @brief Factory class for creating MyRequestHandler objects
class MyRequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory {
public:
    /// @brief Creates a new MyRequestHandler object for each request
    /// @param request HTTP request object
    /// @return Pointer to a new MyRequestHandler object
    Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request) override;
};

/// @brief Main server application class
class MyServerApp : public Poco::Util::ServerApplication {
public:
    /// @brief Runs the server application
    /// @return Exit code of the application
    int run();

protected:
    /// @brief Main method of the application
    /// @param args Command line arguments
    /// @return Exit code of the application
    int main(const std::vector<std::string>& args) override;
};

/// @brief Function to start the backend server
void backend_function();
