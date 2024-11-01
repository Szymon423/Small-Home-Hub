#include <Common/signals.hpp>
#include <Common/database.hpp>
#include <Common/log.hpp>
#include <Common/utilities.hpp>
#include <mutex>

namespace Signals {
    DataType DataTypeFromInt(const std::int8_t value) {
        switch (value) {
            case 1: return DataType::Analog;
            case 2: return DataType::Binary;
            case 3: return DataType::Text;
            default: return DataType::Unknown; 
        }
    }

    std::int8_t DataTypeToInt(const DataType value) {
        return static_cast<std::uint8_t>(value);
    }

    nlohmann::json Definition::ToJson() {
        return {
            { "ID", ID },
            { "GroupID", groupID },
            { "DataType", DataTypeToInt(dataType) },
            { "IsArchived", isArchived },
            { "AlarmLow", {
                    { "Enabled", alarmLow.enabled },
                    { "Threshold", alarmLow.threshold },
                }
            },
            { "AlarmHigh", {
                    { "Enabled", alarmHigh.enabled },
                    { "Threshold", alarmHigh.threshold },
                }
            },
            { "Label", {
                    { "Unit", label.unit },
                    { "LowLabel", label.lowLabel },
                    { "HighLabel", label.highLabel },
                }
            },
            { "Name", name },
            { "Description", description }
        };
    }

    std::vector<Definition> Definition::GetFromDatabase() {
        Logger::trace("Reading signals definitions from DB.");
        std::vector<Definition> definitions;

        try {
            std::lock_guard<std::mutex> lock(ConfigDB::GetMutex());
            SQLite::Statement query(*ConfigDB::Get(), "SELECT * FROM SignalDefinitions");
            using namespace Utilities::Text;
            while (query.executeStep()) {
                int cnt = 0;
                Definition def;
                def.ID = query.getColumn(cnt++).getInt();
                CopyStringToArray(query.getColumn(cnt++).getString(), def.name);
                CopyStringToArray(query.getColumn(cnt++).getString(), def.description);
                def.dataType = DataTypeFromInt(query.getColumn(cnt++).getInt());
                def.isArchived = (query.getColumn(cnt++).getInt() == 1);
                def.groupID = query.getColumn(cnt++).getInt();
                
                nlohmann::json alarms = nlohmann::json::parse(query.getColumn(cnt++).getString());
                def.alarmLow.enabled = alarms.value("LowEnabled", false);
                if (def.alarmLow.enabled) {
                    def.alarmLow.threshold = alarms.value("LowThreshold", 0.0);
                }
                def.alarmHigh.enabled = alarms.value("HighEnabled", false);
                if (def.alarmHigh.enabled) {
                    def.alarmHigh.threshold = alarms.value("HighThreshold", 0.0);
                }

                nlohmann::json labels = nlohmann::json::parse(query.getColumn(cnt++).getString());
                CopyStringToArray(labels.value("Unit", ""), def.label.unit);
                CopyStringToArray(labels.value("LowLabel", ""), def.label.lowLabel);
                CopyStringToArray(labels.value("HighLabel", ""), def.label.highLabel);

                definitions.push_back(def);
            }
        }
        catch (const SQLite::Exception& e) {
            Logger::error("SQLite error: {}", e.what());
        }
        catch (const std::exception& e) {
            Logger::error("std::exception: {}", e.what());
        }
        catch (...) {
            Logger::error("Unexpected error occurred while reaading AnalogDefinition");
        }
        return definitions;
    }
}