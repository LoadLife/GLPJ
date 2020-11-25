cmake_minimum_required(VERSION 3.10)
project(Render)

message("")
message(STATUS "-------------------------configure msg start----------------------------")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
message(STATUS "cpp version 11 is requried")
message(STATUS "Operate System : ${CMAKE_SYSTEM}")
message(STATUS "CXX Compiler : ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "build type:" ${CMAKE_BUILD_TYPE})

#configure third part lib && include dir
set(Third_Part ${CMAKE_SOURCE_DIR}/third_part)
set(Resource_Path "${PROJECT_SOURCE_DIR}/resources")
message(STATUS "third part dir locates in ${Third_Part}")
message(STATUS "resource path dir locates in ${Resource_Path}")
#configure file
configure_file(
        "${PROJECT_SOURCE_DIR}/config.h.in"
        "${PROJECT_BINARY_DIR}/config.h"
        )
#install dll
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    if("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
        install(FILES "${Third_Part}/dll/freetype.dll" DESTINATION "${PROJECT_BINARY_DIR}/render/Release")
    else()
        install(FILES "${Third_Part}/dll/freetype.dll" DESTINATION "${PROJECT_BINARY_DIR}/render/Debug")
    endif()
endif()
#Todo : On GNU
message(STATUS "-------------------------configure msg done-----------------------------")
message("")

add_subdirectory(render)
