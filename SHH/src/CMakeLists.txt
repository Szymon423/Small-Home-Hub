file(GLOB SOURCES
     "*.cpp"
     "*.hpp"
     "*.h"
)

add_library(SHH_lib ${SOURCES})

target_link_libraries(SHH_lib PRIVATE 
     spdlog::spdlog
     Boost::system
     Boost::process
     Boost::interprocess
     cpr::cpr
)

target_include_directories(SHH_lib PUBLIC 
     ${CMAKE_CURRENT_SOURCE_DIR}
     ${CMAKE_SOURCE_DIR}/../Common/include
)