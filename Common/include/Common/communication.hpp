#pragma once

#include <Common/constants.hpp>
#include <Common/events.hpp>
#include <vector>
#include <future>
#include <atomic>
#include <string>
#include <memory>
#include <chrono>
#include <filesystem>
#include <zmq.hpp>

/// @brief Namespace for Communication related elements
namespace Communication {
    /// @brief Class responsible for encapsulation and handling communication context
    class Manager {
    private:
        static std::shared_ptr<zmq::context_t> contextPtr;   // context

    public:
        /// @brief Deleted constructor
        Manager() = delete;

        /// @brief Function to Initialize context
        /// @param threadsNumber number of threads taking part in communication
        static void Initialize(const int threadsNumber = 1);

        /// @brief Function to get context 
        /// @return instance of context
        static std::shared_ptr<zmq::context_t> GetInstance();
    };

    /// @brief Class responsible for handling Broker work
    class Broker {
    private:
        std::shared_ptr<zmq::context_t> context;    // ZMQ context for broker
        zmq::socket_t frontend;                     // ZMQ frontend socket for incomming connections
        zmq::socket_t backend;                      // ZMQ backend socket for outgoing connections
        zmq::socket_t control;                      // ZMQ control socket for managing broker
        std::future<void> proxyFuture;              // future for worker async work
        std::atomic<bool> running;                  // flag for controll of broker
        std::atomic<bool> initialized;              // flag for indication if broker was initialized

        /// @brief Function nahdling all broker work
        void proxyFunction();

    public:
        /// @brief Constructor with library context
        Broker();

        /// @brief Constructor with additional context
        /// @param context shared pointer to context
        Broker(std::shared_ptr<zmq::context_t> context);

        /// @brief Destructor
        ~Broker();

        /// @brief Function to bind broker sockets
        /// @throws Exception when could not connect to socket
        void Initialize();

        /// @brief Function to start broker
        void Start();

        /// @brief Function to stop broker
        void Stop();
    };

    /// @brief Class responsible for handling Publisher work
    class Publisher {
    private:
        std::shared_ptr<zmq::context_t> context;    // ZMQ context for publisher
        zmq::socket_t socket;                       // ZMQ socket for publisher
        std::atomic<bool> initialized;              // Flag for indication if Publisher was initialized

        /// @brief Private function to perform validation publishment
        void Validate();

    public:
        /// @brief Constructor with library context
        Publisher();

        /// @brief Constructor with additional context
        /// @param context shared pointer to context
        Publisher(std::shared_ptr<zmq::context_t> context);

        /// @brief  Destructor
        ~Publisher();

        /// @brief Function to bind publisher sockets
        /// @throws Exception when could not connect to socket
        void Initialize();

        /// @brief Function to publish string data to topic 
        /// @param topic Topic on which publish message
        /// @param message Message itself
        void Publish(const std::string& topic, const std::string& message);

        /// @brief Function to publish data to topic 
        /// @param topic Topic on which publish message
        /// @param message Buffer with data
        void Publish(const std::string& topic, const std::vector<std::byte>& message);
    };

    /// @brief Class responsible for handling communication subscriber
    class Subscriber {
    private:
        std::shared_ptr<zmq::context_t> context;    // ZMQ context for subscriber
        zmq::socket_t socket;                       // ZMQ socket for subscriber
        std::atomic<bool> initialized;              // Flag telling if subscriber is initialized
        std::atomic<bool> isCallbackSet;            // Flag telling if callback is set
        std::atomic<bool> running;                  // Flag telling if subscriber is running
        std::future<void> receiveFuture;            // Future for resoult of RecieveLoop
        std::string subscribedTopic;                // Currently subscribed topic
        std::function<void(const std::vector<std::byte>&)> callbackFunction;    // Callback beeing executed when message is recieved

        /// @brief Function running in background handling subscriber work
        void ReceiveLoop();

    public:
        /// @brief Constructor with library context
        Subscriber();

        /// @brief Constructor with additional context
        /// @param context shared pointer to context
        Subscriber(std::shared_ptr<zmq::context_t> context);

        /// @brief Destructor
        ~Subscriber();

        /// @brief Function to connect subscriber socket
        /// @throws Exception when could not connect to socket
        void Initialize();

        /// @brief Function to subscribe to a topic
        /// @param topic Topic to subscribe to
        /// @throws Exception when could not subscribe to topic
        void Subscribe(const std::string& topic);

        /// @brief Function to unsubscribe from a topic
        void Unsubscribe();

        /// @brief Function to set callback to be executed when recieved data
        /// @param newCallback Function to be called when a message is received with message as argument
        void SetCallback(const std::function<void(const std::vector<std::byte>&)>& newCallback);

        /// @brief Function to start receiving messages
        void Start();
        
        /// @brief Function to stop receiving messages
        void Stop();
    };

    /// @brief Namespace for messages exchanged via ZMQ communication
    namespace Messages {
        
    }
}