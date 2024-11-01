#pragma once

/// @brief Namespace for system related constant values
namespace SystemConstants {
    constexpr int MaxSignalsNumber{ 1000 };             // Max number of signals 
    constexpr int ZmqBackendPort{ 5559 };               // Port for communication over zmq for backend layer
    constexpr int ZmqFrontentPort{ 5560 };              // Port for communication over zmq for frontend layer
    constexpr int SignalLabelLength{ 50 };              // Max text length for signal label
    constexpr int SignalTextValueLength{ 50 };          // Max text length for signal label
    constexpr int SignalNameLength{ 50 };               // Max text length for signal name
    constexpr int SignalDescriptionLength{ 100 };       // Max text length for signal description
}
