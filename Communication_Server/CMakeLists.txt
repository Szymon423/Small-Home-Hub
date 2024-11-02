cmake_minimum_required(VERSION 3.5)

set(CMAKE_TOOLCHAIN_FILE /usr/local/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "")

project(Communication_Server VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(Communication_Server main.cpp)

find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)
find_package(Poco REQUIRED COMPONENTS Util Net JWT)
find_package(SQLiteCpp REQUIRED)
find_package(cpr REQUIRED)

add_subdirectory(src)
include_directories(${PROJECT_SOURCE_DIR}/src)

set(COMMON_LIB "${CMAKE_SOURCE_DIR}/../Common/Build/${PLATFORM_TYPE}/${CMAKE_BUILD_TYPE}/libCommon.a")

target_link_libraries(${PROJECT_NAME} PRIVATE 
    Communication_Server_lib
    ${COMMON_LIB}
    spdlog::spdlog
    Poco::Net
	Poco::Util
	Poco::JWT
    rt
    SQLiteCpp
    cpr::cpr
)

target_include_directories(Communication_Server PUBLIC 
    ${CMAKE_SOURCE_DIR}/../Common/include
)