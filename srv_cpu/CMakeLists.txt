cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(srv_cpu)

include_directories("${CMAKE_SOURCE_DIR}/peq/")
include_directories("../include/systemc/")

link_directories("../libs/systemc/")

add_executable(${PROJECT_NAME} 
    ${CMAKE_SOURCE_DIR}/main.cpp
    ${CMAKE_SOURCE_DIR}/islip.cpp
)

target_link_libraries(${PROJECT_NAME} debug pthread            optimized pthread)
target_link_libraries(${PROJECT_NAME} debug libsystemc_debug.a optimized libsystemc.a)