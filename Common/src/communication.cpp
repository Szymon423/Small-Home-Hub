#include <Common/communication.hpp>
#include <Common/log.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include <string_view>
#include <cpr/cpr.h>

namespace Communication {
    std::shared_ptr<zmq::context_t> Manager::contextPtr = nullptr;

    void Manager::Initialize(const int threadsNumber) {
        contextPtr = std::make_shared<zmq::context_t>(threadsNumber);
    }

    std::shared_ptr<zmq::context_t> Manager::GetInstance() {
        if (!contextPtr) {
            throw std::runtime_error("Context not initialized. Call Manager::Initialize() first.");
        }
        return contextPtr;
    }

    Broker::Broker() : Broker(Manager::GetInstance()) {

    }

    Broker::Broker(std::shared_ptr<zmq::context_t> context) : 
        context(context),
        frontend(*context, zmq::socket_type::xsub),
        backend(*context, zmq::socket_type::xpub),
        control(*context, zmq::socket_type::pair),
        running(false),
        initialized(false)
    {
    }

    Broker::~Broker() {
        Stop();
    }

    void Broker::Initialize() {
        if (initialized.load()) {
            Logger::warn("Broker is already initialized.");
            return;
        }

        try {
            frontend.bind("tcp://*:" + std::to_string(SystemConstants::ZmqFrontentPort));
            backend.bind("tcp://*:" + std::to_string(SystemConstants::ZmqBackendPort));
            control.bind("inproc://broker_control");
            initialized.store(true);
        } 
        catch (const zmq::error_t& e) {
            Logger::error("Failed to initialize broker: {}", e.what());
            throw;  
        }
    }

    void Broker::proxyFunction() {
        try {
            zmq::proxy_steerable(frontend, backend, zmq::socket_ref(), control);
        }
        catch (const zmq::error_t& e) {
            if (running) {
                Logger::error("Exception in broker thread: {}", e.what());
            }
        }
        Logger::debug("Broker proxy finished work");
    }

    void Broker::Start() {
        if (!initialized.load()) {
            Logger::error("Cannot start uninitialized broker. Call Broker::Initialize() first.");
            return;
        }

        if (!running.load()) {
            running.store(true);
            proxyFuture = std::async(std::launch::async, &Broker::proxyFunction, this);
        } 
        else {
            Logger::info("Broker is already running.");
        }
    }

    void Broker::Stop() {
        if (running.load()) {
            running.store(false);
            Logger::info("Stopping broker.");
            
            zmq::socket_t controlPair(*context, zmq::socket_type::pair);
            controlPair.connect("inproc://broker_control");
            controlPair.send(zmq::str_buffer("TERMINATE"));
            
            if (proxyFuture.valid()) {
                proxyFuture.wait();
            }
            Logger::info("Broker stopped.");
        }
    }

    Publisher::Publisher() : Publisher(Manager::GetInstance()) {

    }

    Publisher::Publisher(std::shared_ptr<zmq::context_t> context) : 
        context(context), 
        socket(*context, zmq::socket_type::pub),
        initialized(false)
    {
    }

    Publisher::~Publisher() {
        socket.close();
    }

    void Publisher::Initialize() {
        if (initialized.load()) {
            Logger::warn("Publisher is already initialized.");
            return;
        }

        try {
            socket.connect("tcp://localhost:" + std::to_string(SystemConstants::ZmqFrontentPort));
            std::atomic<bool> validationSuccess{ false };
            Subscriber validationSub;
            validationSub.Initialize();
            validationSub.Subscribe("VALIDATION");
            validationSub.SetCallback([&validationSuccess](const std::vector<std::byte>& data) {
                std::string message(reinterpret_cast<const char*>(data.data()), data.size());
                if (message == "READY") {
                    validationSuccess.store(true);
                }
            });
            validationSub.Start();

            constexpr int validationTrialsNumber{ 10 };
            for (int i{ 0 }; i < validationTrialsNumber; ++i) {
                Validate();
                if (validationSuccess.load()) {
                    initialized.store(true);
                    break;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }

            validationSub.Stop();
            if (!validationSuccess.load()) {
                throw std::runtime_error("Could not initialize publisher after " + std::to_string(validationTrialsNumber) + " trials.");
            }
        }
        catch (const zmq::error_t& e) {
            Logger::error("Failed to initialize publisher: {}", e.what());
            throw;
        }
        catch (const std::exception& e) {
            Logger::error("Failed to initialize publisher: {}", e.what());
            throw;
        }
    }

    void Publisher::Validate() {
        try {
            auto result1 = socket.send(zmq::str_buffer("VALIDATION"), zmq::send_flags::sndmore);
            auto result2 = socket.send(zmq::str_buffer("READY"), zmq::send_flags::none);
            if (!result1 || !result2) {
                throw std::runtime_error("Failed to send message on topic \"VALIDATION\"");
            }
        } 
        catch (const zmq::error_t& e) {
            Logger::error("Failed to publish message: \"{}\"", e.what());
            throw;
        }
    }

    void Publisher::Publish(const std::string& topic, const std::string& message) {
        if (!initialized.load()) {
            Logger::error("Cannot publish when Publisher is not initialized. Call Publisher::Initialize() first.");
            return;
        }

        try {
            auto result1 = socket.send(zmq::buffer(topic), zmq::send_flags::sndmore);
            auto result2 = socket.send(zmq::buffer(message), zmq::send_flags::none);
            if (!result1 || !result2) {
                Logger::error("Failed to send message on topic \"{}\"", topic);
                throw std::runtime_error("Failed to send message on topic \"" + topic + "\"");
            }
        } 
        catch (const zmq::error_t& e) {
            Logger::error("Failed to publish message: \"{}\"", e.what());
            throw;
        }
    }

    void Publisher::Publish(const std::string& topic, const std::vector<std::byte>& message) {
        if (!initialized.load()) {
            Logger::error("Cannot publish when Publisher is not initialized. Call Publisher::Initialize() first.");
            return;
        }

        try {
            auto result1 = socket.send(zmq::buffer(topic), zmq::send_flags::sndmore);
            auto result2 = socket.send(zmq::buffer(message), zmq::send_flags::none);
            if (!result1 || !result2) {
                Logger::error("Failed to send message on topic \"{}\"", topic);
                throw std::runtime_error("Failed to send message on topic \"" + topic + "\"");
            }
        } 
        catch (const zmq::error_t& e) {
            Logger::error("Failed to publish buffer with data: \"{}\"", e.what());
            throw;
        }
    }

    Subscriber::Subscriber() : Subscriber(Manager::GetInstance()) {
        
    }

    Subscriber::Subscriber(std::shared_ptr<zmq::context_t> context) : 
        context(context), 
        socket(*context, zmq::socket_type::sub),
        initialized(false),
        isCallbackSet(false),
        running(false)
    {
    }

    Subscriber::~Subscriber() {
        Stop();
        socket.close();
    }

    void Subscriber::Initialize() {
        if (initialized.load()) {
            Logger::warn("Subscriber is already initialized.");
            return;
        }

        try {
            socket.connect("tcp://localhost:" + std::to_string(SystemConstants::ZmqBackendPort));
            socket.set(zmq::sockopt::rcvtimeo, 100);
            initialized.store(true);
        } 
        catch (const zmq::error_t& e) {
            Logger::error("Failed to initialize subscriber: {}", e.what());
            throw;
        }
    }

    void Subscriber::Subscribe(const std::string& topic) {
        if (!initialized.load()) {
            Logger::error("Cannot subscribe when Subscriber is not initialized. Call Subscriber::Initialize() first.");
            return;
        }

        if (!subscribedTopic.empty()) {
            Logger::warn("Already subscribed to topic: \"{}\". Unsubscribe first.", subscribedTopic);
            throw std::runtime_error("Already subscribed to topic: " + subscribedTopic + ". Unsubscribe first.");
        }

        try {
            subscribedTopic = topic;
            socket.set(zmq::sockopt::subscribe, subscribedTopic);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        catch (const zmq::error_t& e) {
            Logger::error("Failed to subscribe to topic \"{}\": {}", subscribedTopic, e.what());
            throw;
        }
    }

    void Subscriber::Unsubscribe() {
        if (!initialized.load()) {
            Logger::error("Cannot unsubscribe when Subscriber is not initialized.");
            return;
        }

        try {
            socket.set(zmq::sockopt::unsubscribe, subscribedTopic);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            subscribedTopic.clear();
        }
        catch (const zmq::error_t& e) {
            Logger::error("Failed to unsubscribe from topic \"{}\": {}", subscribedTopic, e.what());
            throw;
        }
    }

    void Subscriber::SetCallback(const std::function<void(const std::vector<std::byte>&)>& newCallback) {
        if (!initialized.load()) {
            Logger::error("Cannot set callback when Subscriber is not initialized. Call Subscriber::Initialize() first.");
            return;
        }

        if (running.load()) {
            Logger::warn("Subscriber is already receiving messages.");
            return;
        }

        callbackFunction = newCallback;
        isCallbackSet.store(true);
    }

    void Subscriber::Start() {
        if (!initialized.load()) {
            Logger::error("Cannot start when Subscriber is not initialized. Call Subscriber::Initialize() first.");
            return;
        }

        if (!isCallbackSet.load()) {
            Logger::error("Cannot start Subscriber when no callback is set. Call Subscriber::Initialize() first.");
            return;
        }

        if (running.load()) {
            Logger::warn("Subscriber is already receiving messages.");
            return;
        }

        running.store(true);
        receiveFuture = std::async(std::launch::async, &Subscriber::ReceiveLoop, this);
    }

    void Subscriber::Stop() {
        running.store(false);
        if (receiveFuture.valid()) {
            receiveFuture.wait();
        }
    }

    void Subscriber::ReceiveLoop() {
        while (running.load()) {
            try {
                zmq::message_t topicMsg;
                zmq::message_t contentMsg;

                if (socket.recv(topicMsg, zmq::recv_flags::dontwait)) {
                    if (socket.recv(contentMsg, zmq::recv_flags::none)) {
                        std::vector<std::byte> contentBuffer(static_cast<const std::byte*>(contentMsg.data()), 
                            static_cast<const std::byte*>(contentMsg.data()) + contentMsg.size());
                        callbackFunction(contentBuffer);
                    } 
                    else {
                        Logger::warn("Received topic but failed to receive content");
                    }
                }
            } 
            catch (const zmq::error_t& e) {
                if (e.num() != EAGAIN) {
                    Logger::error("Error receiving message: {}", e.what());
                }
            }
        }
    }

    namespace Messages {
        
    }
} 