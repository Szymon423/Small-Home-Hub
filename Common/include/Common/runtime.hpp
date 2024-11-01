#pragma once

#include <atomic>
#include <csignal>

/// @brief Class managing global work flags for controlling program execution
class Runtime {
private:
    /// @brief Atomic boolean flag indicating whether the program should continue running
    static std::atomic_bool run;

public:
    /// @brief Clears run flag, sets it to state when program should continiue working 
    static void Clear();

    /// @brief Checks if work should continue
    /// @return True if work should continue, false otherwise
    static bool Run();

    /// @brief Stops the work by setting the run flag to false
    static void Stop();

    /// @brief Function handling signals recieving
    /// @param signalNumber recieved signal
    static void SignalHandler(int signalNumber);
};
