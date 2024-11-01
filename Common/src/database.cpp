#include <Common/database.hpp>
#include <stdexcept>

std::unique_ptr<SQLite::Database> ConfigDB::db_ptr;
std::once_flag ConfigDB::init_flag;
std::mutex ConfigDB::db_mutex;

void ConfigDB::Init(const std::string& connection_string) {
    std::call_once(init_flag, [&]() {
        std::lock_guard<std::mutex> lock(db_mutex);
        try {
            db_ptr = std::make_unique<SQLite::Database>(connection_string, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        } 
        catch (const SQLite::Exception& e) {
            throw std::runtime_error("Failed to initialize database");
        }
    });
}

std::unique_ptr<SQLite::Database>& ConfigDB::Get() {
    if (!db_ptr) {
        throw std::runtime_error("Database not initialized. Call Init() first.");
    }
    return db_ptr;
}

std::mutex& ConfigDB::GetMutex() {
    return db_mutex; 
}