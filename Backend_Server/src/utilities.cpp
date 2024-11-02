#include "utilities.hpp"
#include <Common/log.hpp>
#include <Common/runtime.hpp>
#include <csignal>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <string>
#include <tuple>

int get_utc_offset() {
    std::time_t now = std::time(nullptr);
    std::tm tm_local = {};
    std::tm tm_gmt = {};
    
    localtime_r(&now, &tm_local);
    gmtime_r(&now, &tm_gmt);
    
    std::time_t local_time = std::mktime(&tm_local);
    std::time_t gmt_time = std::mktime(&tm_gmt);
    
    return std::difftime(local_time, gmt_time) / 3600;
}

std::string time_t_to_string(const time_t& time, const std::string& format) {
    std::tm tm = {};
    if (localtime_r(&time, &tm) == nullptr) {
        throw std::runtime_error("Nie udało się skonwertować time_t na struct tm");
    }
    std::ostringstream oss;
    oss << std::put_time(&tm, format.c_str());
    return oss.str();
}

std::time_t string_to_time_t(const std::string& time_str, const std::string& format) {
    std::tm tm = {};
    std::istringstream iss(time_str);
    
    // Specjalne traktowanie dla formatu z mikrosekundami
    if (format == "%Y-%m-%dT%H:%M:%S.%fZ") {
        char dummy;
        int year, month, day, hour, minute, second;
        long microsecond;
        iss >> year >> dummy >> month >> dummy >> day >> dummy
            >> hour >> dummy >> minute >> dummy >> second >> dummy >> microsecond;
        
        if (iss.fail()) {
            throw std::runtime_error("Could not parse string to time_t");
        }
        
        tm.tm_year = year - 1900;
        tm.tm_mon = month - 1;
        tm.tm_mday = day;
        tm.tm_hour = hour;
        tm.tm_min = minute;
        tm.tm_sec = second;
    } 
    else {
        iss >> std::get_time(&tm, format.c_str());
        if (iss.fail()) {
            throw std::runtime_error("Could not parse string to time_t");
        }
    }
    
    // set to -1 so OS will determine summer or winter time
    tm.tm_isdst = -1;
    
    std::time_t result = std::mktime(&tm);
    if (result == -1) {
        throw std::runtime_error("Could not convert struct tm to time_t");
    }
    
    return result;
}

void validate_fields(const std::vector<std::tuple<std::string, FieldDataType, bool>>& required_fields, const nlohmann::json& j) {
    for (const auto& [field, expected_type, is_required] : required_fields) {
        if (!j.contains(field)) {
            if (is_required) {
                throw std::runtime_error("Missing required field: " + field);
            }
            // Pole nie jest wymagane, więc przechodzimy do następnego
            continue;  
        }

        // Sprawdź typ pola
        switch (expected_type) {
            case FieldDataType::String:
                if (!j[field].is_string()) {
                    throw std::runtime_error("Field '" + field + "' should be a string");
                }
                break;
            case FieldDataType::Integer:
                if (!j[field].is_number_integer()) {
                    throw std::runtime_error("Field '" + field + "' should be an integer");
                }
                break;
            case FieldDataType::Float:
                if (!j[field].is_number_float()) {
                    throw std::runtime_error("Field '" + field + "' should be a float");
                }
                break;
            case FieldDataType::Boolean:
                if (!j[field].is_boolean()) {
                    throw std::runtime_error("Field '" + field + "' should be a boolean");
                }
                break;
            case FieldDataType::Object:
                if (!j[field].is_object()) {
                    throw std::runtime_error("Field '" + field + "' should be an object");
                }
                break;
            case FieldDataType::Array:
                if (!j[field].is_array()) {
                    throw std::runtime_error("Field '" + field + "' should be an array");
                }
                break;
            default:
                throw std::runtime_error("Unknown field type for field: " + field);
        }
    }
}
