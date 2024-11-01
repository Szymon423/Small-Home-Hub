#pragma once

#include <Common/constants.hpp>
#include <nlohmann/json.hpp>
#include <ctime>
#include <cstdint>
#include <vector>
#include <chrono>
#include <variant>

/// @brief Namespace for stuff related to signals from configuratuion database
namespace Signals {
    /// @brief Enum class with all possible data types
    enum class DataType : std::int8_t {
        Unknown,        
        Analog,
        Binary,
        Text
    };

    /// @brief Function to convert int to DataType
    /// @param value int with associated DataType
    /// @return resoulting DataType
    DataType DataTypeFromInt(const std::int8_t value);

    /// @brief Function to convert DataType to int 
    /// @param value DataType
    /// @return resoulting int associated with DataType
    std::int8_t DataTypeToInt(const DataType value);

    /// @brief Class with data necessary for alarm handling
    class Alarm {
    public:
        bool enabled{ false };      // true - when this alarm can be raised
        double threshold{ 0.0 };    // treshhold over/below which alarm is raised
    };

    /// @brief Class with all necessary labels like unit, low/high flag
    class Label {
    public:
        char unit[SystemConstants::SignalLabelLength]{ "" };              // unit associated with signal
        char lowLabel[SystemConstants::SignalLabelLength]{ "False" };     // displayed label when signal is low
        char highLabel[SystemConstants::SignalLabelLength]{ "True" };     // displayed label when signal is high
    };

    /// @brief Class holding signal definition
    class Definition {
    public:
        std::uint16_t signalID{ 0 };            // ID of signal
        std::uint16_t groupID{ 0 };             // ID of associated group
        DataType dataType{ DataType::Unknown }; // Data type of this signal 
        Alarm alarmLow;                         // Alarm low thing
        Alarm alarmHigh;                        // Alarm high thing
        bool isArchived{ true };                // When set to true - signal is archived
        Label label;                            // All labels like unit, low/high flag text
        char name[SystemConstants::SignalNameLength]{ "Name" };                         // name of signal
        char description[SystemConstants::SignalDescriptionLength]{ "Description" };    // description of signal
        
        /// @brief Function to convert AnalogDefinition to JSON object
        /// @return JSON object with AnalogDefinition related variables
        nlohmann::json ToJson();
    
        /// @brief Function to read all SignalDefinition from Configuration Database
        /// @return Vector of SignalDefinition 
        static std::vector<SignalDefinition> GetFromDatabase();
    };
    
    /// @brief Structure holding value and flags of analog signal
    class Value {
    public:
        union {
            double analog;
            bool binary;
            char text[SystemConstants::SignalTextValueLength];
        };
        bool valid{ false };
        bool alarmLow{ false };
        bool alarmHigh{ false };
        std::chrono::system_clock::time_point timestamp{ std::chrono::system_clock::now() };
    };
}