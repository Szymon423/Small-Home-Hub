cmake_minimum_required(VERSION 3.1)

set(CMAKE_TOOLCHAIN_FILE /usr/local/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "")

project(SHH VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(SHH main.cpp)

find_package(spdlog REQUIRED)
find_package(SQLiteCpp REQUIRED)
find_package(cppzmq REQUIRED)
find_package(Boost REQUIRED COMPONENTS process interprocess system)
find_package(cpr REQUIRED)

add_subdirectory(src)
include_directories(${PROJECT_SOURCE_DIR}/src)

set(COMMON_LIB "${CMAKE_SOURCE_DIR}/../Common/Build/${PLATFORM_TYPE}/${CMAKE_BUILD_TYPE}/libCommon.a")

target_link_libraries(SHH PRIVATE 
    SHH_lib
    ${COMMON_LIB}
    spdlog::spdlog
    Boost::system
    Boost::process
    Boost::interprocess
    rt
    SQLiteCpp
    pthread
    cppzmq
    cpr::cpr
)

target_include_directories(SHH PUBLIC 
    ${CMAKE_SOURCE_DIR}/../Common/include
)