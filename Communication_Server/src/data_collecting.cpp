#include "data_collecting.hpp"
#include <Common/log.hpp>
#include <Common/database.hpp>
#include <Common/groups.hpp>
#include <Common/signals.hpp>
#include <Common/shared_memory.hpp>
#include <Common/utilities.hpp>
#include <nlohmann/json.hpp>
#include <SQLiteCpp/SQLiteCpp.h>
#include <exception>
#include <tuple>
#include <map>
#include <string>

namespace DataCollecting {
    /// @brief Helper function to initialize id map
    void initializeIDMap(std::map<int, int> &IDToPosition) {
        static std::atomic<bool> isMapInitialized{ false };
        if (!isMapInitialized) {
            auto shmem = SharedMemory::Manager::Get();
            IDToPosition.clear();
            for (int i = 0; i < shmem->header.numberOfSignals; ++i) {
                IDToPosition[shmem->definitions[i].ID] = i;
            }
            isMapInitialized = true;
        }
    }

    void collect(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) noexcept {
        try {
            // Parse JSON from request body
            nlohmann::json requestBody;
            try {
                requestBody = nlohmann::json::parse(request.stream());
            } 
            catch (const nlohmann::json::parse_error& e) {
                throw std::runtime_error("Invalid JSON: " + std::string(e.what()));
            }

            // Validate required fields
            static const std::vector<std::tuple<std::string, Utilities::JSON::FieldDataType, bool>> required_fields {
                { "Timestamp", Utilities::JSON::FieldDataType::Integer, true },
                { "Data", Utilities::JSON::FieldDataType::Array, true }
            };
            Utilities::JSON::Validate(required_fields, requestBody);

            // Prepare map for faster finding positions of signals ID
            auto shmem = SharedMemory::Manager::Get();
            static std::map<int, int> IDToPosition;
            initializeIDMap(IDToPosition);

            for (const auto& item : data) {
                // Validate ID
                Utilities::JSON::Validate({{ "ID", Utilities::JSON::FieldDataType::Integer, true }}, item);
                const int ID = item["ID"].get<int>();
                auto it = IDToPosition.find(ID);
                if (it == IDToPosition.end()) {
                    throw std::runtime_error("Trying to access not existing signal in system with ID: " + std::to_string(ID));
                }
                const int position = it->second;

                shmem->previousValues[position] = shmem->currentValues[position];
                switch (shmem->definitions[position].dataType) {
                    case Signals::DataType::Analog: {
                        Utilities::JSON::Validate({{ "Value", Utilities::JSON::FieldDataType::Integer, true }}, item);
                        const double value = item["Value"].get<double>();
                        Logger::trace("Updating signal with ID: {} with value: {}", ID, value);
                        shmem->currentValues[position].analog = value;
                        break;
                    }
                    case Signals::DataType::Binary: {
                        Utilities::JSON::Validate({{ "Value", Utilities::JSON::FieldDataType::Boolean, true }}, item);
                        const bool value = item["Value"].get<bool>();
                        Logger::trace("Updating signal with ID: {} with value: {}", ID, value);
                        shmem->currentValues[position].binary = value;
                        break;
                    }
                    case Signals::DataType::Text: {
                        Utilities::JSON::Validate({{ "Value", Utilities::JSON::FieldDataType::String, true }}, item);
                        const std::string value = item["Value"].get<std::string>();
                        Logger::trace("Updating signal with ID: {} with value: {}", ID, value);
                        shmem->currentValues[position].text = value;
                        break;
                    }
                    default: {
                        throw std::runtime_error("Signal in system with ID: " + std::to_string(ID) + " has invalid data type.");
                    }
                }
            }
            shmem->currentValues[position].timestamp = std::chrono::system_clock::now();

            nlohmann::json j = { "message", "Succesfully updated signal."};
            response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            out << j.dump();
        }
        catch (const std::exception& e) {
            response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            nlohmann::json errorJson = {{"status", "error"}, {"message", e.what()}};
            out << errorJson.dump();
            Logger::error("Error retrieving signal definitions: {}", e.what());
        }
    }
}