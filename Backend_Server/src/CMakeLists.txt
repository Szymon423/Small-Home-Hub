file(GLOB SOURCES
     "*.cpp"
     "*.hpp"
     "*.h"
)

add_library(Backend_Server_lib ${SOURCES})

target_link_libraries(Backend_Server_lib PRIVATE 
     spdlog::spdlog
     fmt::fmt
     Poco::Net
	Poco::Util
	Poco::JWT
     ${COMMON_LIB}
     SQLiteCpp
     cpr::cpr
)

target_include_directories(Backend_Server_lib PUBLIC 
     ${CMAKE_CURRENT_SOURCE_DIR}
     ${CMAKE_SOURCE_DIR}/../Common/include
)