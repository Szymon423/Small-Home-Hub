#include "backend.hpp"
#include "data_collecting.hpp"
#include "authentication.hpp"
#include "diagnostics.hpp"
#include <Common/runtime.hpp>
#include <Common/configuration.hpp>
#include <Common/log.hpp>
#include <nlohmann/json.hpp>
#include <iostream>

std::map<std::pair<std::string, std::string>, MyRequestHandler::RouteHandler> MyRequestHandler::routes = {
    {{"POST", "/api/data/collect"}, std::bind(&DataCollecting::collect, std::placeholders::_1, std::placeholders::_2)},
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
    Poco::Net::HTTPServer s(new MyRequestHandlerFactory, config.backend_server_port);
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