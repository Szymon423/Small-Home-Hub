#include <Common/signals.hpp>
#include <Common/database.hpp>
#include <Common/log.hpp>
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
        return reinterpret_cast<std::uint8_t>(value);
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

    std::vector<AnalogDefinition> AnalogDefinition::GetFromDatabase() {
        Logger::trace("Reading Analog Definitions from DB.");
        std::vector<AnalogDefinition> definitions;

        try {
            std::lock_guard<std::mutex> lock(ConfigDB::GetMutex());
            SQLite::Statement query(*ConfigDB::Get(), "SELECT * FROM Signals");
            
            while (query.executeStep()) {
                int cnt = 0;
                AnalogDefinition def;
                def.analogDefinitionID = query.getColumn(cnt++).getInt();
                copyStringToArray(query.getColumn(cnt++).getString(), def.name);
                copyStringToArray(query.getColumn(cnt++).getString(), def.description);
                cnt++;      // DataType
                def.isArchived = (query.getColumn(cnt++).getInt() == 1);
                def.groupID = query.getColumn(cnt++).getInt();
                def.sourceType = SourceTypeFromInt(query.getColumn(cnt++).getInt());
                def.sourceID = query.getColumn(cnt++).getInt();

                nlohmann::json alarms = nlohmann::json::parse(query.getColumn(cnt++).getString());
                def.alarmLowEnabled = alarms.value("AlarmLowEnabled", false);
                if (def.alarmLowEnabled) {
                    def.alarmLowThreshold = alarms.value("AlarmLowThreshold", 0.0);
                }
                def.alarmHighEnabled = alarms.value("AlarmHighEnabled", false);
                if (def.alarmHighEnabled) {
                    def.alarmHighThreshold = alarms.value("AlarmHighThreshold", 0.0);
                }

                nlohmann::json warnings = nlohmann::json::parse(query.getColumn(cnt++).getString());
                def.warningLowEnabled = warnings.value("WarningLowEnabled", false);
                if (def.warningLowEnabled) {
                    def.warningLowThreshold = warnings.value("warningLowThreshold", 0.0);
                }
                def.warningHighEnabled = warnings.value("WarningHighEnabled", false);
                if (def.warningHighEnabled) {
                    def.warningHighThreshold = warnings.value("WarningHighThreshold", 0.0);
                }

                nlohmann::json labels = nlohmann::json::parse(query.getColumn(cnt++).getString());
                copyStringToArray(labels.value("Unit", ""), def.unit);

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

    nlohmann::json BinaryDefinition::ToJson() {
        return {
            { "BinaryDefinitionID", binaryDefinitionID },
            { "IsArchived", isArchived },
            { "GroupID", groupID },
            { "SourceType", SourceTypeToInt(sourceType) },
            { "SourceID", sourceID },
            { "AlarmLowEnabled", alarmLowEnabled },
            { "AlarmHighEnabled", alarmHighEnabled },
            { "LowLabel", lowLabel },
            { "HighLabel", highLabel },
            { "Name", name },
            { "Description", description }
        };
    }

    std::vector<BinaryDefinition> BinaryDefinition::GetFromDatabase() {
        Logger::trace("Reading Binary Definitions from DB.");
        std::vector<BinaryDefinition> definitions;

        try {
            std::lock_guard<std::mutex> lock(ConfigDB::GetMutex());
            SQLite::Statement query(*ConfigDB::Get(), "SELECT * FROM Signals WHERE DataType = 1");
            
            while (query.executeStep()) {
                int cnt = 0;
                BinaryDefinition def;
                def.binaryDefinitionID = query.getColumn(cnt++).getInt();
                copyStringToArray(query.getColumn(cnt++).getString(), def.name);
                copyStringToArray(query.getColumn(cnt++).getString(), def.description);
                cnt++;      // DataType
                def.isArchived = (query.getColumn(cnt++).getInt() == 1);
                def.groupID = query.getColumn(cnt++).getInt();
                def.sourceType = SourceTypeFromInt(query.getColumn(cnt++).getInt());
                def.sourceID = query.getColumn(cnt++).getInt();

                nlohmann::json alarms = nlohmann::json::parse(query.getColumn(cnt++).getString());
                def.alarmLowEnabled = alarms.value("AlarmLowEnabled", false);
                def.alarmHighEnabled = alarms.value("AlarmHighEnabled", false);

                cnt++;      // Warnings
                nlohmann::json labels = nlohmann::json::parse(query.getColumn(cnt++).getString());
                copyStringToArray(labels.value("LowLabel", "Stan niski"), def.lowLabel);
                copyStringToArray(labels.value("HighLabel", "Stan wysoki"), def.highLabel);

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
            Logger::error("Unexpected error occurred while reaading BinaryDefinition");
        }
        return definitions;
    }
}