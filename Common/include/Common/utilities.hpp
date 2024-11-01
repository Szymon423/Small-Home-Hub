#pragma once

#include <chrono>
#include <string>

namespace Utilities {
    namespace Time {
        /// @brief Namespace defining which time is beeing taken into account
        enum class Zone {
            UTC,
            Local
        };

        /// @brief Function to convert timestamp to it's string representation
        /// @param timestamp refference to timestamp
        /// @param zone by default time is converted to Local zone string
        /// @return string representation of timestamp
        std::string ToString(const std::chrono::system_clock::time_point& timestamp, const Zone zone = Zone::Local);

        /// @brief Function to covert string with timestamp to timestamp
        /// @param timestamp string representation of timestamp
        /// @param zone by default time is converted from Local zone string
        /// @return timestamp based on provided string
        std::chrono::system_clock::time_point FromString(const std::string& timestamp, const Zone zone = Zone::Local);
    }

    namespace Text {
        /// @brief Helper function to copy string to char array
        /// @tparam N size of array
        /// @param str source string
        /// @param arr destination char array
        template<std::size_t N>
        void CopyStringToArray(const std::string& str, char (&arr)[N]) {
            std::size_t copySize = (str.size() < N - 1) ? str.size() : (N - 1);
            str.copy(arr, copySize);
            arr[copySize] = '\0';
        }
    }
}