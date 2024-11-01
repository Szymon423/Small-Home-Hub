#include <Common/utilities.hpp>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <fmt/chrono.h>

namespace Utilities {
    namespace Time {
        std::string ToString(const std::chrono::system_clock::time_point& timestamp, const Zone zone) {
            // I am using this fucking piece of shit becouse RPI OS at the time of writing this code
            // is still only on version bullseye, thus forcing me to use g++ in version 12.x,
            // at which not all C++ v20 features are implemented like chrono::zoned_time, chrono::parse
            std::time_t timeT = std::chrono::system_clock::to_time_t(timestamp);

            // Convert time t to local time
            std::tm timeInfo;
            if (zone == Zone::Local) {
                localtime_r(&timeT, &timeInfo);
            } 
            else { 
                gmtime_r(&timeT, &timeInfo);
            }

            // Using format from fmt, not from std becouse of g++
            return fmt::format("{:%Y-%m-%d %H:%M:%S}", timeInfo);
        }

        std::chrono::system_clock::time_point FromString(const std::string& timestamp, const Zone zone) {
            // Stream to parse the input string
            std::istringstream in(timestamp);
            std::tm t = {};
            in >> std::get_time(&t, "%Y-%m-%d %H:%M:%S");

            if (in.fail()) {
                throw std::runtime_error("Failed to parse time string");
            }
            
            std::time_t timeT;
            if (zone == Zone::Local) {
                // This will make sure that the Polish daylight saving time will be chosen automatically
                t.tm_isdst = -1;
                timeT = std::mktime(&t);
            } 
            else { 
                timeT = timegm(&t);
            }
            return std::chrono::system_clock::from_time_t(timeT);
        }
    }
}