cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(srv_cpu)
enable_testing()

include_directories("../include/")
include_directories("../include/systemc/")
include_directories("../include/spdlog/")
include_directories("../include/gtest/")
include_directories("../include/gmock/")
include_directories("${CMAKE_SOURCE_DIR}/common/")
include_directories("${CMAKE_SOURCE_DIR}/peq/")
include_directories("${CMAKE_SOURCE_DIR}/islip/")
include_directories("${CMAKE_SOURCE_DIR}/dpa/")
include_directories("${CMAKE_SOURCE_DIR}/test/")
include_directories("${CMAKE_SOURCE_DIR}/network/")
include_directories("${CMAKE_SOURCE_DIR}/sch/")

link_directories("../libs/systemc/")
link_directories("../libs/spdlog/")
link_directories("../libs/gtest/")
link_directories("../libs/gmock/")

add_executable(${PROJECT_NAME} 
    ${CMAKE_SOURCE_DIR}/main.cpp
    ${CMAKE_SOURCE_DIR}/src.cpp
    ${CMAKE_SOURCE_DIR}/tgt.cpp
    ${CMAKE_SOURCE_DIR}/islip/islip.cpp
    ${CMAKE_SOURCE_DIR}/dpa/dpa.cpp
    ${CMAKE_SOURCE_DIR}/test/test.cpp
    ${CMAKE_SOURCE_DIR}/network/network.cpp
    ${CMAKE_SOURCE_DIR}/sch/sch.cpp
)

target_link_libraries(${PROJECT_NAME} debug gtest              optimized gtest)
target_link_libraries(${PROJECT_NAME} debug pthread            optimized pthread)
target_link_libraries(${PROJECT_NAME} debug libsystemc_debug.a optimized libsystemc.a)
target_link_libraries(${PROJECT_NAME} debug libspdlog.a        optimized libspdlog.a)