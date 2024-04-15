cmake_minimum_required(VERSION 3.9)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
project(classs)

add_executable(${PROJECT_NAME} main.cpp)
add_compile_options(${PROJECT_NAME} PULIC "-g")