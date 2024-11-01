cmake_minimum_required(VERSION 3.5)

set(CMAKE_TOOLCHAIN_FILE /usr/local/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "")

project(Common VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")

file(GLOB COMMON_SOURCES 
    "src/*.cpp"
    "src/*.c"
)

file(GLOB COMMON_HEADERS
    "include/Common/*.hpp"
    "include/Common/*.h"
)

set_source_files_properties("src/shared_memory.cpp" PROPERTIES COMPILE_FLAGS "-fPIC")

find_package(spdlog REQUIRED)

add_library(Common ${COMMON_SOURCES} ${COMMON_HEADERS})

target_link_libraries(Common PRIVATE 
    spdlog::spdlog
    pthread
    rt
    cppzmq
)

target_include_directories(Common PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)