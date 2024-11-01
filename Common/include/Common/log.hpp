#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <memory>

/// @brief Class responsible for handling logger actions
class Logger {
public:
    /// @brief Initializes the logger
    /// @param file_name Name of the log file
    /// @param program_name Name of the program for logging context
    /// @param max_size Max file size in bytes, default is 5MB
    /// @param max_files Max rotating file number, default is 3 files
    static void init(const std::string& file_name, const std::string& program_name, std::size_t max_size = 1024 * 1024 * 5, std::size_t max_files = 3);

    /// @brief Gets the logger instance
    /// @return Shared pointer to the logger instance
    static std::shared_ptr<spdlog::logger>& getLogger();

    /// @brief Logs a trace message
    /// @tparam Args Types of the format arguments
    /// @param fmt Format string
    /// @param args Format arguments
    template<typename... Args>
    static void trace(spdlog::format_string_t<Args...> fmt, Args &&... args) {
        getLogger()->trace(fmt, std::forward<Args>(args)...);
    }

    /// @brief Logs a debug message
    /// @tparam Args Types of the format arguments
    /// @param fmt Format string
    /// @param args Format arguments
    template<typename... Args>
    static void debug(spdlog::format_string_t<Args...> fmt, Args &&... args) {
        getLogger()->debug(fmt, std::forward<Args>(args)...);
    }

    /// @brief Logs an info message
    /// @tparam Args Types of the format arguments
    /// @param fmt Format string
    /// @param args Format arguments
    template<typename... Args>
    static void info(spdlog::format_string_t<Args...> fmt, Args &&... args) {
        getLogger()->info(fmt, std::forward<Args>(args)...);
    }

    /// @brief Logs a warning message
    /// @tparam Args Types of the format arguments
    /// @param fmt Format string
    /// @param args Format arguments
    template<typename... Args>
    static void warn(spdlog::format_string_t<Args...> fmt, Args &&... args) {
        getLogger()->warn(fmt, std::forward<Args>(args)...);
    }

    /// @brief Logs an error message
    /// @tparam Args Types of the format arguments
    /// @param fmt Format string
    /// @param args Format arguments
    template<typename... Args>
    static void error(spdlog::format_string_t<Args...> fmt, Args &&... args) {
        getLogger()->error(fmt, std::forward<Args>(args)...);
    }

    /// @brief Logs a critical message
    /// @tparam Args Types of the format arguments
    /// @param fmt Format string
    /// @param args Format arguments
    template<typename... Args>
    static void critical(spdlog::format_string_t<Args...> fmt, Args &&... args) {
        getLogger()->critical(fmt, std::forward<Args>(args)...);
    }

private:
    /// @brief Shared pointer to the logger instance
    static std::shared_ptr<spdlog::logger> logger;

    /// @brief Name of the program for logging context
    static std::string programName;
};

#define LOG_TRACE(...) Logger::getLogger()->trace(__VA_ARGS__)
#define LOG_DEBUG(...) Logger::getLogger()->debug(__VA_ARGS__)
#define LOG_INFO(...)  Logger::getLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)  Logger::getLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...) Logger::getLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...) Logger::getLogger()->critical(__VA_ARGS__) 