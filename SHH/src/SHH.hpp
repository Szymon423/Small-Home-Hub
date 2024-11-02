#pragma once

#include <Common/shared_memory.hpp>
#include "SQLiteCpp/SQLiteCpp.h"
#include <chrono>

/// @brief function to update all signals descriptions in shared memory based on database
/// @return true if succes
bool UpdateSignalDefinitionsInSharedMemory();

/// @brief function to get last database modification time
/// @return time of last modification
std::chrono::system_clock::time_point GetLastUpdateTime();