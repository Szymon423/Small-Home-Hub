#include <Common/groups.hpp>
#include <Common/database.hpp>
#include <Common/log.hpp>

namespace Groups {
    nlohmann::json Group::ToJson() {
        return {
            { "GroupID", groupID },
            { "Name", name },
            { "Description", description }
        };
    }

    Group Group::FromJson(const nlohmann::json& j) {
        Group group;
        group.groupID = j["GroupID"];
        group.name = j["Name"];
        group.description = j["Description"];
        return group;
    }

    std::vector<Group> Group::GetFromDatabase() {
        Logger::trace("Reading Groups from DB.");
        std::vector<Group> groups;

        try {
            std::lock_guard<std::mutex> lock(ConfigDB::GetMutex());
            SQLite::Statement query(*ConfigDB::Get(), "SELECT * FROM Groups");
            
            while (query.executeStep()) {
                int cnt = 0;
                Group g;
                g.groupID = query.getColumn(cnt++).getInt();
                g.name = query.getColumn(cnt++).getString();
                g.description = query.getColumn(cnt++).getString();
                groups.push_back(g);
            }
        }
        catch (const SQLite::Exception& e) {
            Logger::error("SQLite error: {}", e.what());
        }
        catch (const std::exception& e) {
            Logger::error("std::exception: {}", e.what());
        }
        catch (...) {
            Logger::error("Unexpected error occurred while reaading groups");
        }
        return groups;
    }
}