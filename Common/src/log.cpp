#include <Common/log.hpp>

std::shared_ptr<spdlog::logger> Logger::logger;
std::string Logger::programName;

void Logger::init(const std::string& file_name, const std::string& program_name, std::size_t max_size, std::size_t max_files) {
    programName = program_name;

    std::vector<spdlog::sink_ptr> sinks;
    sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    
    // Użyj rotating_file_sink zamiast basic_file_sink
    auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
        file_name, max_size, max_files
    );
    sinks.push_back(rotating_sink);

    logger = std::make_shared<spdlog::logger>(program_name, begin(sinks), end(sinks));
    spdlog::register_logger(logger);

    // Ustaw format logów
    logger->set_pattern("[%Y-%m-%d %H:%M:%S] [%n] [%l] %v");

    logger->set_level(spdlog::level::trace);
    logger->flush_on(spdlog::level::trace);
}

std::shared_ptr<spdlog::logger>& Logger::getLogger() {
    return logger;
}
