cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(sc_fifo_with_delay)

add_executable(${PROJECT_NAME} 
    ${CMAKE_SOURCE_DIR}/main.cpp
)

find_library(SYSTEMC_LIB systemc PATHS /00_ycl/02_install/systemc-2.3.3/lib-linux64)
target_link_libraries(${PROJECT_NAME} PUBLIC ${SYSTEMC_LIB})  

target_include_directories(${PROJECT_NAME} PUBLIC
                          "${CMAKE_SOURCE_DIR}/include/"
                          #"/00_ycl/02_install/systemc-2.3.3/include"
                          "/00_ycl/02_install/systemc-2.3.3/include"
                          )
                          
target_compile_options(${PROJECT_NAME} PRIVATE "-g")                           