#include "signal_values.hpp"
#include <Common/shared_memory.hpp>
#include <Common/log.hpp>
#include <nlohmann/json.hpp>
#include <fmt/format.h>

namespace SignalValues {
    void get_all_analogs(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept {
        try {
            Logger::trace("Reading current analog values.");

            nlohmann::json result = nlohmann::json::array();

            auto shmem = SharedMemory::Manager::Get();
            for (std::size_t i = 0; i < shmem->header.numberOfAnalogSignals; ++i) {
                nlohmann::json item = {
                    { "AnalogDefinitionID", shmem->analogDefinitions[i].analogDefinitionID },
                    { 
                        "Current", {
                            { "Value", shmem->currentAnalogValues[i].value },
                            { "Valid", shmem->currentAnalogValues[i].valid },
                            { "WarningLow", shmem->currentAnalogValues[i].warningLow },
                            { "WarningHigh", shmem->currentAnalogValues[i].warningHigh },
                            { "AlarmLow", shmem->currentAnalogValues[i].alarmLow },
                            { "AlarmHigh", shmem->currentAnalogValues[i].alarmHigh },
                            { "Timestamp", shmem->currentAnalogValues[i].timestamp.time_since_epoch().count() }
                        },
                    },
                    {
                        "Previous", {
                            { "Value", shmem->previousAnalogValues[i].value },
                            { "Valid", shmem->previousAnalogValues[i].valid },
                            { "WarningLow", shmem->previousAnalogValues[i].warningLow },
                            { "WarningHigh", shmem->previousAnalogValues[i].warningHigh },
                            { "AlarmLow", shmem->previousAnalogValues[i].alarmLow },
                            { "AlarmHigh", shmem->previousAnalogValues[i].alarmHigh },
                            { "Timestamp", shmem->previousAnalogValues[i].timestamp.time_since_epoch().count() }
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
            Logger::error("Error reading current analog values: {}", e.what());
        }
        catch (...) {
            // Catch any other unexpected exceptions
            response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json errorJson = {{"status", "error"}, {"message", "An unexpected error occurred"}};
            out << errorJson.dump();
            Logger::error("Unexpected error occurred while reding current analog values");
        }
    }

    void get_all_binaries(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept {
        try {
            Logger::trace("Reading current binary values.");

            nlohmann::json result = nlohmann::json::array();

            auto shmem = SharedMemory::Manager::Get();
            for (std::size_t i = 0; i < shmem->header.numberOfBinarySignals; ++i) {
                nlohmann::json item = {
                    { "BinaryDefinitionID", shmem->binaryDefinitions[i].binaryDefinitionID },
                    { 
                        "Current", {
                            { "Value", shmem->currentBinaryValues[i].value },
                            { "Valid", shmem->currentBinaryValues[i].valid },
                            { "AlarmLow", shmem->currentBinaryValues[i].alarmLow },
                            { "AlarmHigh", shmem->currentBinaryValues[i].alarmHigh },
                            { "Timestamp", shmem->currentAnalogValues[i].timestamp.time_since_epoch().count() }
                        },
                    },
                    {
                        "Previous", {
                            { "Value", shmem->previousBinaryValues[i].value },
                            { "Valid", shmem->previousBinaryValues[i].valid },
                            { "AlarmLow", shmem->previousBinaryValues[i].alarmLow },
                            { "AlarmHigh", shmem->previousBinaryValues[i].alarmHigh },
                            { "Timestamp", shmem->previousBinaryValues[i].timestamp.time_since_epoch().count() }
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
            Logger::error("Error reading current binary values: {}", e.what());
        }
        catch (...) {
            // Catch any other unexpected exceptions
            response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json errorJson = {{"status", "error"}, {"message", "An unexpected error occurred"}};
            out << errorJson.dump();
            Logger::error("Unexpected error occurred while reding current binary values");
        }
    }
}