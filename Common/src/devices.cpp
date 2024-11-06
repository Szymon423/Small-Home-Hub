#include <Common/devices.hpp>
#include <Common/database.hpp>
#include <Common/log.hpp>

namespace Devices {
    nlohmann::json Signal::ToJson() const {
        return {
            { "IsAnalog", isAnalog },
            { "IsSteerable", isSteerable },
            { "Name", name },
            { "Unit", unit }
        };
    }

    Signal Signal::FromJson(const nlohmann::json& j) {
        Signal signal;
        signal.isAnalog = j["IsAnalog"];
        signal.isSteerable = j["IsSteerable"];
        signal.name = j["Name"];
        signal.unit = j["Unit"];
        return signal;
    }

    nlohmann::json Type::ToJson() {
        nlohmann::json type = {
            { "TypeID", typeID },
            { "Name", name }
        };
        nlohmann::json array = nlohmann::json::array();
        for (const auto& signal : signals) {
            array.push_back(signal.ToJson());
        }
        type["Signals"] = array;
        return type;
    }

    Type Type::FromJson(const nlohmann::json& j) {
        Type type;
        type.typeID = j["TypeID"];
        type.name = j["Name"];
        for (auto& signal : j["Signals"]) {
            type.signals.push_back(Signal::FromJson(signal));
        }
        return type;
    }

    std::vector<Type> Type::GetFromDatabase() {
        Logger::trace("Reading Device Types from DB.");
        std::vector<Type> types;

        try {
            std::lock_guard<std::mutex> lock(ConfigDB::GetMutex());
            SQLite::Statement query(*ConfigDB::Get(), "SELECT * FROM DeviceTypes");
            
            while (query.executeStep()) {
                int cnt = 0;
                Type t;
                t.typeID = query.getColumn(cnt++).getInt();
                t.name = query.getColumn(cnt++).getString();
                nlohmann::json signals = nlohmann::json::parse(query.getColumn(cnt++).getString());
                for (auto& signal : signals) {
                    t.signals.push_back(Signal::FromJson(signal));
                }
                types.push_back(t);
            }
        }
        catch (const SQLite::Exception& e) {
            Logger::error("SQLite error: {}", e.what());
        }
        catch (const std::exception& e) {
            Logger::error("std::exception: {}", e.what());
        }
        catch (...) {
            Logger::error("Unexpected error occurred while reaading device types");
        }
        return types;
    }

    nlohmann::json Device::ToJson() {
        return {
            { "DeviceID", deviceID },
            { "DeviceTypeID", deviceTypeID },
            { "Description", description }
        };
    }

    Device Device::FromJson(const nlohmann::json& j) {
        Device device;
        device.deviceID = j["DeviceID"];
        device.deviceTypeID = j["DeviceTypeID"];
        device.description = j["Description"];
        return device;
    }

    std::vector<Device> Device::GetFromDatabase() {
        Logger::trace("Reading Devices from DB.");
        std::vector<Device> devices;

        try {
            std::lock_guard<std::mutex> lock(ConfigDB::GetMutex());
            SQLite::Statement query(*ConfigDB::Get(), "SELECT * FROM Devices");
            
            while (query.executeStep()) {
                int cnt = 0;
                Device d;
                d.deviceID = query.getColumn(cnt++).getInt();
                d.deviceTypeID = query.getColumn(cnt++).getInt();
                d.description = query.getColumn(cnt++).getString();
                devices.push_back(d);
            }
        }
        catch (const SQLite::Exception& e) {
            Logger::error("SQLite error: {}", e.what());
        }
        catch (const std::exception& e) {
            Logger::error("std::exception: {}", e.what());
        }
        catch (...) {
            Logger::error("Unexpected error occurred while reaading devices");
        }
        return devices;
    }
}