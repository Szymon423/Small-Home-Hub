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
        namespace Report {
            std::atomic<std::uint32_t> Query::uniqueId = 0;

            Query::Query() {
                uniqueId += 1;
                id = uniqueId;
            }

            std::vector<std::byte> Query::ToBuffer() const {
                // Calculate size for data
                std::size_t size = (analogSignalIds.size() + binarySignalIds.size()) * sizeof(std::uint16_t) + 
                    sizeof(id) + 
                    sizeof(timerangeStart) +
                    sizeof(timerangeStop) + 
                    sizeof(reportType) + 
                    sizeof(std::uint16_t) + 
                    sizeof(std::uint16_t);

                // Allocate memory for the buffer
                std::vector<std::byte> buffer(size);
                std::byte* ptr = buffer.data();
                std::size_t offset = 0;

                // Serialize id
                std::memcpy(&ptr[offset], &id, sizeof(id));
                offset += sizeof(id);

                // Serialize timerangeStart
                std::memcpy(&ptr[offset], &timerangeStart, sizeof(timerangeStart));
                offset += sizeof(timerangeStart);

                // Serialize timerangeStop
                std::memcpy(&ptr[offset], &timerangeStop, sizeof(timerangeStop));
                offset += sizeof(timerangeStop);

                // Serialize reportType
                std::memcpy(&ptr[offset], &reportType, sizeof(reportType));
                offset += sizeof(reportType);

                // Serialize analogSignalIds size
                std::uint16_t analogSize = analogSignalIds.size();
                std::memcpy(&ptr[offset], &analogSize, sizeof(analogSize));
                offset += sizeof(analogSize);

                // Serialize binarySignalIds size
                std::uint16_t binarySize = binarySignalIds.size();
                std::memcpy(&ptr[offset], &binarySize, sizeof(binarySize));
                offset += sizeof(binarySize);

                // Copy vectors data
                if (!analogSignalIds.empty()) {
                    std::memcpy(&ptr[offset], analogSignalIds.data(), analogSignalIds.size() * sizeof(std::uint16_t));
                    offset += analogSignalIds.size() * sizeof(std::uint16_t);
                }

                if (!binarySignalIds.empty()) {
                    std::memcpy(&ptr[offset], binarySignalIds.data(), binarySignalIds.size() * sizeof(std::uint16_t));
                }

                return buffer;
            }

            Query Query::FromBuffer(const std::vector<std::byte>& buffer) {
                Query result;
                const std::byte* data = buffer.data();
                std::size_t size = buffer.size();
                std::uint16_t analogVectorSize = 0;
                std::uint16_t binaryVectorSize = 0;

                // Calculate base size
                std::size_t baseSize = sizeof(result.id) + 
                    sizeof(result.timerangeStart) + 
                    sizeof(result.timerangeStop) + 
                    sizeof(result.reportType) + 
                    sizeof(analogVectorSize) + 
                    sizeof(binaryVectorSize);

                if (size < baseSize) {
                    throw std::runtime_error("Buffer too small to contain Query");
                }

                // Copy id
                std::memcpy(&result.id, data, sizeof(result.id));
                data += sizeof(result.id);

                // Copy timerangeStart
                std::memcpy(&result.timerangeStart, data, sizeof(result.timerangeStart));
                data += sizeof(result.timerangeStart);

                // Copy timerangeStop
                std::memcpy(&result.timerangeStop, data, sizeof(result.timerangeStop));
                data += sizeof(result.timerangeStop);

                // Copy reportType
                std::memcpy(&result.reportType, data, sizeof(result.reportType));
                data += sizeof(result.reportType);

                // Copy size of analogSignalIds
                std::memcpy(&analogVectorSize, data, sizeof(analogVectorSize));
                data += sizeof(analogVectorSize);

                // Copy size of binarySignalIds
                std::memcpy(&binaryVectorSize, data, sizeof(binaryVectorSize));
                data += sizeof(binaryVectorSize);

                // Calculate remaining size
                size -= baseSize;

                std::size_t analogSize = analogVectorSize * sizeof(std::uint16_t);
                std::size_t binarySize = binaryVectorSize * sizeof(std::uint16_t);

                if (size < analogSize + binarySize) {
                    throw std::runtime_error("Buffer too small to contain all vector data");
                }

                // Prepare vectors
                result.analogSignalIds.resize(analogVectorSize);
                result.binarySignalIds.resize(binaryVectorSize);

                // Copy vectors data
                if (!result.analogSignalIds.empty()) {
                    std::memcpy(result.analogSignalIds.data(), data, analogSize);
                    data += analogSize;
                }
                if (!result.binarySignalIds.empty()) {
                    std::memcpy(result.binarySignalIds.data(), data, binarySize);
                }

                return result;
            }

            std::vector<std::byte> Response::ToBuffer() const {
                std::string_view pathStr = reportPath.string();
                std::size_t pathSize = pathStr.size();
                std::size_t bufferSize = sizeof(id) + sizeof(pathSize) + pathSize + sizeof(Code);

                std::vector<std::byte> buffer(bufferSize);
                std::byte* ptr = buffer.data();

                // Copy id
                std::memcpy(ptr, &id, sizeof(id));
                ptr += sizeof(id);

                // Copy pathSize
                std::memcpy(ptr, &pathSize, sizeof(pathSize));
                ptr += sizeof(pathSize);
                
                // Copy path
                std::memcpy(ptr, pathStr.data(), pathSize);
                ptr += pathSize;

                // Copy code
                auto codeValue = static_cast<std::underlying_type_t<Code>>(code);
                std::memcpy(ptr, &codeValue, sizeof(codeValue));

                return buffer;
            }

            Response Response::FromBuffer(const std::vector<std::byte>& buffer) {
                Response result;
                const std::byte* data = buffer.data();
                std::size_t remainingSize = buffer.size();

                // Read id
                if (remainingSize < sizeof(Response::id)) {
                    throw std::runtime_error("Buffer too short to contain id.");
                }
                std::memcpy(&result.id, data, sizeof(Response::id));
                data += sizeof(Response::id);
                remainingSize -= sizeof(Response::id);

                // Read pathSize
                if (remainingSize < sizeof(std::size_t)) {
                    throw std::runtime_error("Buffer too short to contain path size.");
                }
                std::size_t pathSize;
                std::memcpy(&pathSize, data, sizeof(pathSize));
                data += sizeof(pathSize);
                remainingSize -= sizeof(pathSize);

                // Read path
                if (remainingSize < pathSize) {
                    throw std::runtime_error("Buffer too short to contain full path.");
                }
                result.reportPath = std::filesystem::path(std::string(reinterpret_cast<const char*>(data), pathSize));
                data += pathSize;
                remainingSize -= pathSize;

                // Read code
                if (remainingSize < sizeof(Code)) {
                    throw std::runtime_error("Buffer too short to contain code.");
                }
                std::memcpy(&result.code, data, sizeof(Code));

                return result;
            }
        }

        namespace Events {
            std::atomic<bool> Registration::isUniqueIdUpdated = false;
            std::atomic<std::uint64_t> Registration::uniqueId = 0;

            void Registration::GetRecentID() {
                if (!isUniqueIdUpdated.load()) {  // Poprawka literówki
                    Logger::debug("Reading latest EventId from database.");
                    std::string queryID = "SELECT MAX(EventID) AS MaxEventID FROM Events;";
                    cpr::Response r = cpr::Get(
                        cpr::Url{"http://localhost:9000/exec"},
                        cpr::Parameters{{"query", queryID}}
                    );

                    if (r.status_code == 0) {
                        Logger::error("Could not connect to database.");
                        throw std::runtime_error("Could not connect to database.");
                    }

                    if (r.status_code == 400) {
                        uniqueId.store(0);
                        isUniqueIdUpdated.store(true);
                        Logger::debug("Setting new UniqueID as: {}", uniqueId.load());
                        return;
                    }

                    try {
                        nlohmann::json dbResponse = nlohmann::json::parse(r.text);
                        if (dbResponse.contains("dataset")) {
                            auto dataset = dbResponse["dataset"];
                            if (dataset.is_array() && dataset.size() == 1 && dataset[0].is_array() && dataset[0].size() == 1) {
                                std::uint64_t value = dataset[0][0];
                                uniqueId.store(value + 1);
                                Logger::debug("Setting new UniqueID as: {}", uniqueId.load());
                                isUniqueIdUpdated.store(true);
                            } 
                            else {
                                throw std::runtime_error("Unexpected dataset structure.");
                            }
                        } 
                        else {
                            throw std::runtime_error("Response does not contain \"dataset\" element.");
                        }
                    } 
                    catch (const std::exception& e) {
                        Logger::error("Error parsing database response: {}", e.what());
                        throw;
                    }
                }
            }


            std::uint64_t Registration::GetNextID() {
                auto id = uniqueId.load();
                ++uniqueId;
                return id;
            }

            std::vector<std::byte> Registration::ToBuffer() const {
                // Calculate size for data
                std::uint16_t messageSize = message.size();
                std::size_t size = sizeof(type) + 
                    sizeof(messageSize) + messageSize +
                    sizeof(issueTimestamp) + 
                    sizeof(associatedSignalID);

                // Allocate memory for the buffer
                std::vector<std::byte> buffer(size);
                std::byte* ptr = buffer.data();
                std::size_t offset = 0;

                // Serialize type
                std::memcpy(&ptr[offset], &type, sizeof(type));
                offset += sizeof(type);

                // Serialize message size
                std::memcpy(&ptr[offset], &messageSize, sizeof(messageSize));
                offset += sizeof(messageSize);

                // Serialize message
                std::memcpy(&ptr[offset], message.c_str(), messageSize);
                offset += messageSize;

                // Serialize issue timestamp
                std::memcpy(&ptr[offset], &issueTimestamp, sizeof(issueTimestamp));
                offset += sizeof(issueTimestamp);

                // Serialize issue associated signal id
                std::memcpy(&ptr[offset], &associatedSignalID, sizeof(associatedSignalID));
                offset += sizeof(associatedSignalID);

                return buffer;
            }

            Registration Registration::FromBuffer(const std::vector<std::byte>& buffer) {
                Registration result;
                const std::byte* data = buffer.data();
                std::size_t size = buffer.size();
                std::uint16_t messageSize = 0;

                // Calculate base size
                std::size_t baseSize = sizeof(result.type) + 
                    sizeof(result.issueTimestamp) + 
                    sizeof(result.associatedSignalID) + 
                    sizeof(messageSize);

                if (size < baseSize) {
                    throw std::runtime_error("Buffer too small to contain Registration");
                }

                // Copy type
                std::memcpy(&result.type, data, sizeof(result.type));
                data += sizeof(result.type);

                // Copy message size
                std::memcpy(&messageSize, data, sizeof(messageSize));
                data += sizeof(messageSize);

                // Copy message
                result.message.assign(reinterpret_cast<const char*>(data), messageSize);
                data += messageSize;

                // Copy issue timestamp
                std::memcpy(&result.issueTimestamp, data, sizeof(result.issueTimestamp));
                data += sizeof(result.issueTimestamp);

                // Copy issue associated signal id
                std::memcpy(&result.associatedSignalID, data, sizeof(result.associatedSignalID));
                data += sizeof(result.associatedSignalID);

                return result;
            }
        }
    }
} 