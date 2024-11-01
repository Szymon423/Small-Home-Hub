#include <Common/users.hpp>
#include <Common/database.hpp>
#include <Common/log.hpp>

namespace Users {
    std::uint8_t PrivilegesToInt(const Privileges& p) {
        return static_cast<std::uint8_t>(p);
    }

    Privileges PrivilegesFromInt(const int p) {
        switch (p) {
            case 0: return Privileges::Service;
            case 1: return Privileges::Admin;
            case 2: return Privileges::Operator;
            default: return Privileges::Unknown;
        }
    }

    nlohmann::json User::ToJson() {
        return {
            { "UserID", userID },
            { "Name", name },
            { "Surname", surname },
            { "Login", login },
            { "Password", password },
            { "Privileges", PrivilegesToInt(privileges) }
        };
    }

    User User::FromJson(const nlohmann::json& j) {
        User user;
        user.userID = j["UserID"];
        user.name = j["Name"];
        user.surname = j["Surname"];
        user.login = j["Login"];
        user.password = j["Password"];
        user.privileges = PrivilegesFromInt(j["Privileges"].get<int>());
        return user;
    }

    std::vector<User> User::GetFromDatabase() {
        Logger::trace("Reading Users from DB.");
        std::vector<User> users;

        try {
            std::lock_guard<std::mutex> lock(ConfigDB::GetMutex());
            SQLite::Statement query(*ConfigDB::Get(), "SELECT * FROM Users");
            
            while (query.executeStep()) {
                int cnt = 0;
                User u;
                u.userID = query.getColumn(cnt++).getInt();
                u.name = query.getColumn(cnt++).getString();
                u.surname = query.getColumn(cnt++).getString();
                u.login = query.getColumn(cnt++).getString();
                u.password = query.getColumn(cnt++).getString();
                u.privileges = PrivilegesFromInt(query.getColumn(cnt++).getInt());
                users.push_back(u);
            }
        }
        catch (const SQLite::Exception& e) {
            Logger::error("SQLite error: {}", e.what());
        }
        catch (const std::exception& e) {
            Logger::error("std::exception: {}", e.what());
        }
        catch (...) {
            Logger::error("Unexpected error occurred while reaading Users");
        }
        return users;
    }
}