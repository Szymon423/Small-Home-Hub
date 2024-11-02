#include "backend.hpp"
#include "system_configuration.hpp"
#include "signal_definitions.hpp"
#include "signal_values.hpp"
#include "users.hpp"
#include "events.hpp"
#include "authentication.hpp"
#include "diagnostics.hpp"
#include <Common/runtime.hpp>
#include <Common/configuration.hpp>
#include <Common/log.hpp>
#include <nlohmann/json.hpp>
#include <iostream>

std::map<std::pair<std::string, std::string>, MyRequestHandler::RouteHandler> MyRequestHandler::routes = {
    {{"GET",  "/api/configuration/get"}, std::bind(&SystemConfiguration::get_all, std::placeholders::_1, std::placeholders::_2)},
    {{"POST", "/api/configuration/import"}, std::bind(&SystemConfiguration::import_db, std::placeholders::_1, std::placeholders::_2)},
    {{"GET",  "/api/configuration/export"}, std::bind(&SystemConfiguration::export_db, std::placeholders::_1, std::placeholders::_2)},
    {{"POST", "/api/configuration/update_network"}, std::bind(&SystemConfiguration::update_network, std::placeholders::_1, std::placeholders::_2)},
    {{"POST", "/api/configuration/update_device_name"}, std::bind(&SystemConfiguration::update_device_name, std::placeholders::_1, std::placeholders::_2)},
    {{"POST", "/api/configuration/update_data_retention_period"}, std::bind(&SystemConfiguration::update_data_retention_period, std::placeholders::_1, std::placeholders::_2)},
    
    {{"GET",  "/api/signals/groups/get"}, std::bind(&GroupDefinitions::get_all, std::placeholders::_1, std::placeholders::_2)},
    {{"POST", "/api/signals/groups/add"}, std::bind(&GroupDefinitions::add_group, std::placeholders::_1, std::placeholders::_2)},
    {{"POST", "/api/signals/groups/update"}, std::bind(&GroupDefinitions::update_group, std::placeholders::_1, std::placeholders::_2)},
    {{"POST", "/api/signals/groups/delete"}, std::bind(&GroupDefinitions::delete_group, std::placeholders::_1, std::placeholders::_2)},
    {{"GET",  "/api/signals/definitions/get"}, std::bind(&SignalDefinitions::get_all, std::placeholders::_1, std::placeholders::_2)},
    {{"POST", "/api/signals/definitions/add"}, std::bind(&SignalDefinitions::add_signal, std::placeholders::_1, std::placeholders::_2)},
    {{"POST", "/api/signals/definitions/update"}, std::bind(&SignalDefinitions::update_signal, std::placeholders::_1, std::placeholders::_2)},
    {{"POST", "/api/signals/definitions/delete"}, std::bind(&SignalDefinitions::delete_signal, std::placeholders::_1, std::placeholders::_2)},
    {{"GET",  "/api/signals/values/get"}, std::bind(&SignalValues::get_all, std::placeholders::_1, std::placeholders::_2)},
    
    {{"GET",  "/api/events/get"}, std::bind(&EventsHandling::get_all, std::placeholders::_1, std::placeholders::_2)},
    {{"POST", "/api/events/acknowledge"}, std::bind(&EventsHandling::acknowledge, std::placeholders::_1, std::placeholders::_2)},

    {{"GET",  "/api/users/get"}, std::bind(&Users::get_all, std::placeholders::_1, std::placeholders::_2)},
    {{"POST", "/api/users/add"}, std::bind(&Users::add_user, std::placeholders::_1, std::placeholders::_2)},
    {{"POST", "/api/users/update"}, std::bind(&Users::update_user, std::placeholders::_1, std::placeholders::_2)},
    {{"POST", "/api/users/delete"}, std::bind(&Users::delete_user, std::placeholders::_1, std::placeholders::_2)},
    
    {{"POST", "/api/authentication/login"}, std::bind(&Authentication::login, std::placeholders::_1, std::placeholders::_2)},
    
    {{"GET",  "/api/diagnostics/status"}, std::bind(&Diagnostics::status, std::placeholders::_1, std::placeholders::_2)}
};

void MyRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
    Poco::URI uri(request.getURI());
    std::string path = uri.getPath();
    std::string method = request.getMethod();

    setCorsHeaders(response);

    if (method == "OPTIONS") {
        handleOptions(request, response);
        return;
    }

    auto route = routes.find({method, path});
    if (route != routes.end()) {
        route->second(request, response);
        return;
    }
    handleNotFound(request, response);
}

void MyRequestHandler::handleNotFound(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
    response.setStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
    response.setContentType("text/html");
    std::ostream& out = response.send();
    out << "<html><body><h1>404 Not Found</h1><p>This page was not found.</p></body></html>";
}

void MyRequestHandler::handleOptions(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
    response.send();
}

void MyRequestHandler::setCorsHeaders(Poco::Net::HTTPServerResponse& response) {
    response.set("Access-Control-Allow-Origin", "*");
    response.set("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    response.set("Access-Control-Allow-Headers", "Content-Type, Authorization");
    response.set("Access-Control-Allow-Credentials", "true");
}

Poco::Net::HTTPRequestHandler* MyRequestHandlerFactory::createRequestHandler(const Poco::Net::HTTPServerRequest& request) {
    return new MyRequestHandler;
}

int MyServerApp::run() {
    return Poco::Util::ServerApplication::run();
}

int MyServerApp::main(const std::vector<std::string>& args) {
    auto config = Configuration::Get();
    Poco::Net::HTTPServer s(new MyRequestHandlerFactory, config.backendServerPort);
    s.start();
    while (Runtime::Run()) {
        Poco::Thread::sleep(100);
    }

    Logger::info("Stopping backend server.");
    s.stop();
    return Application::EXIT_OK;
}

void backend_function() {
    Logger::info("Starting backend server.");
    MyServerApp app;
    app.run();
}