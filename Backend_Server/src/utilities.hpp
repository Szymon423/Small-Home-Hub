#pragma once

#include <string>
#include <ctime>
#include <vector>
#include <utility>
#include <nlohmann/json.hpp>

/// @brief Function handling interrupts
/// @param signum signal number
void signal_handler(int signum);

/// @brief Function to convert time_t to string
/// @param time time value
/// @param format format in which time must be interpreted
/// @return string with time
std::string time_t_to_string(const time_t& time, const std::string& format);

/// @brief Function to convert string to time_t
/// @param time_str time with string
/// @param format format in which time_str must be interpreted
/// @return time value
std::time_t string_to_time_t(const std::string& time_str, const std::string& format);

/// @brief Helping enum with data types for JSON fields
enum class FieldDataType : std::uint8_t {
    String,
    Integer,
    Float,
    Boolean,
    Object,
    Array
};

/// @brief Function to check if all fields are inside of JSON with proper datatype, bool (when true) defines that item is required
/// @param required_fields fields to examine as { "test", FieldDataType::String, true } - test must be string and it is required
/// @param j json to examine
void validate_fields(const std::vector<std::tuple<std::string, FieldDataType, bool>>& required_fields, const nlohmann::json& j);