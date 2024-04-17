cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(srv_cpu)

include_directories("../include/systemc/")
include_directories("${CMAKE_SOURCE_DIR}/common/")
include_directories("${CMAKE_SOURCE_DIR}/peq/")
include_directories("${CMAKE_SOURCE_DIR}/islip/")
include_directories("${CMAKE_SOURCE_DIR}/test/")

link_directories("../libs/systemc/")

add_executable(${PROJECT_NAME} 
    ${CMAKE_SOURCE_DIR}/main.cpp
    ${CMAKE_SOURCE_DIR}/islip/islip.cpp
    ${CMAKE_SOURCE_DIR}/test/test.cpp
)

target_link_libraries(${PROJECT_NAME} debug pthread            optimized pthread)
target_link_libraries(${PROJECT_NAME} debug libsystemc_debug.a optimized libsystemc.a)