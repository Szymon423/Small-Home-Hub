#pragma once

#include <Common/signals.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <mutex>
#include <atomic>
#include <stdexcept>
#include <functional>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#define SHMEM_NAME "SCCSharedMemory"

namespace bip = boost::interprocess;

/// @brief Namespace for Shared memory stuff
namespace SharedMemory {
    /// @brief Structure with information about signals number
    struct Header {
        std::uint16_t numberOfBinarySignals{ 0 };
        std::uint16_t numberOfAnalogSignals{ 0 };
    };

    // Save the current alignment setting and set the alignment to 1 byte
    #pragma pack(push, 1)   

    /// @brief Struture holding shared memory data 
    struct Data {
        /// @brief Mutex for locking shared memory acces
        bip::interprocess_mutex mtx;

        /// @brief Header with signal number information                    
        Header header;

        /// @brief Definitions of analog signals
        Signals::AnalogDefinition analogDefinitions[SystemConstants::AnalogSignalsNumber];
        
        /// @brief Definitions of binary signals
        Signals::BinaryDefinition binaryDefinitions[SystemConstants::BinarySignalsNumber];
        
        /// @brief Current analog signals values
        Signals::AnalogValue currentAnalogValues[SystemConstants::AnalogSignalsNumber];
        
        /// @brief Previous analog signals values
        Signals::AnalogValue previousAnalogValues[SystemConstants::AnalogSignalsNumber];
        
        /// @brief Current binary signals values
        Signals::BinaryValue currentBinaryValues[SystemConstants::BinarySignalsNumber];
        
        /// @brief Previous binary signals values
        Signals::BinaryValue previousBinaryValues[SystemConstants::BinarySignalsNumber];
    };

    // Restore the previous alignment setting
    #pragma pack(pop)  

    /// @brief Class managing shared memory operations
    class Manager {
    private:
        /// @brief Shared memory object
        static bip::shared_memory_object shm;

        /// @brief Mapped region of shared memory
        static bip::mapped_region region;

        // pointer to shared memory data
        static Data* data;

        /// @brief Mutex for safety
        static std::mutex mutex;
        
        /// @brief Flag to determine if shared memory was initialized first
        static std::atomic<bool> initialized;

        /// @brief Sets the content of the shared memory to null
        static void NullOut();

    public:
        /// @brief Signleton style
        Manager() = delete;
        ~Manager() = delete;
        Manager(Manager&) = delete;
        Manager(Manager&&) = delete;
        Manager& operator=(const Manager&) = delete;
        Manager& operator=(Manager&&) = delete;

        /// @brief Closes and releases the shared memory
        static void Close();

        /// @brief Function to create an instance of shared memory
        static void Create();

        /// @brief Function to connect to existing instance of shared memory
        static void Connect();

        /// @brief Function to get pointer to shared memory data
        /// @return pointer to shared memory data 
        static Data* Get();
    };
}