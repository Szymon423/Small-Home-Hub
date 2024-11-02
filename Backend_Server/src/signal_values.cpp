#include "signal_values.hpp"
#include <Common/shared_memory.hpp>
#include <Common/log.hpp>
#include <nlohmann/json.hpp>
#include <fmt/format.h>

namespace SignalValues {
    void get_all(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept {
        try {
            Logger::trace("Reading current signal values.");

            nlohmann::json result = nlohmann::json::array();
            auto shmem = SharedMemory::Manager::Get();
            for (std::size_t i = 0; i < shmem->header.numberOfSignals; ++i) {
                
                nlohmann::json currentValue;
                switch (shmem->definitions[i].dataType) {
                    case Signals::DataType::Analog: currentValue["value"] = shmem->currentValues[i].analog; break;
                    case Signals::DataType::Binary: currentValue["value"] = shmem->currentValues[i].binary; break;
                    case Signals::DataType::Text: currentValue["value"] = shmem->currentValues[i].text; break;
                    default: currentValue["value"] = "?????"; break;
                }

                nlohmann::json previousValue;
                switch (shmem->definitions[i].dataType) {
                    case Signals::DataType::Analog: previousValue["value"] = shmem->previousValues[i].analog; break;
                    case Signals::DataType::Binary: previousValue["value"] = shmem->previousValues[i].binary; break;
                    case Signals::DataType::Text: previousValue["value"] = shmem->previousValues[i].text; break;
                    default: previousValue["value"] = "?????"; break;
                }

                nlohmann::json item = {
                    { "ID", shmem->definitions[i].ID },
                    { 
                        "Current", {
                            currentValue,
                            { "Valid", shmem->currentValues[i].valid },
                            { "AlarmLow", shmem->currentValues[i].alarmLow },
                            { "AlarmHigh", shmem->currentValues[i].alarmHigh },
                            { "Timestamp", shmem->currentValues[i].timestamp.time_since_epoch().count() }
                        },
                    },
                    {
                        "Previous", {
                            previousValue,
                            { "Valid", shmem->previousValues[i].valid },
                            { "AlarmLow", shmem->previousValues[i].alarmLow },
                            { "AlarmHigh", shmem->previousValues[i].alarmHigh },
                            { "Timestamp", shmem->previousValues[i].timestamp.time_since_epoch().count() }
                        }
                    }
                };
                result.push_back(item);
            }

            response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            out << result.dump();
        }
        catch (const std::exception& e) {
            response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json errorJson = {{"status", "error"}, {"message", e.what()}};
            out << errorJson.dump();
            Logger::error("Error reading values: {}", e.what());
        }
        catch (...) {
            // Catch any other unexpected exceptions
            response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json errorJson = {{"status", "error"}, {"message", "An unexpected error occurred"}};
            out << errorJson.dump();
            Logger::error("Unexpected error occurred while reding values");
        }
    }
}