#pragma once

#include <memory>
#include <mutex>
#include <SQLiteCpp/SQLiteCpp.h>
#include <nlohmann/json.hpp>

/// @brief A static class for managing a SQLite database connection
class ConfigDB {
private:
    /// @brief Pointer to the SQLite database instance
    static std::unique_ptr<SQLite::Database> db_ptr;

    /// @brief Flag to ensure the database is initialized only once
    static std::once_flag init_flag;

    /// @brief Mutex for thread-safe database operations
    static std::mutex db_mutex;

public:
    /// @brief Initializes the database connection
    /// @param connection_string The SQLite database connection string
    static void Init(const std::string& connection_string);

    /// @brief Gets a reference to the SQLite database instance
    /// @return Reference to the unique_ptr holding the SQLite::Database instance
    static std::unique_ptr<SQLite::Database>& Get();
    
    /// @brief Gets a reference to the database mutex
    /// @return Reference to the mutex used for thread-safe database operations
    static std::mutex& GetMutex();
    
    /// @brief Default constructor (deleted)
    ConfigDB() = delete;

    /// @brief Copy constructor (deleted)
    ConfigDB(const ConfigDB&) = delete;

    /// @brief Copy assignment operator (deleted)
    ConfigDB& operator=(const ConfigDB&) = delete;
};