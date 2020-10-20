cmake_minimum_required(VERSION 3.10)
project(Render)

message(STATUS "cpp version 11 is requried")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(Third_Part ${CMAKE_SOURCE_DIR}/third_part)
message(STATUS "third part dir locates in ${Third_Part}")
set(Resource_Path "${PROJECT_SOURCE_DIR}/resources")
configure_file(
        "${PROJECT_SOURCE_DIR}/config.h.in"
        "${PROJECT_BINARY_DIR}/config.h"
        )
add_subdirectory(render)
