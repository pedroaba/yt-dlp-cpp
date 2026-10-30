cmake_minimum_required(VERSION 3.24)

project(ytdl VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(YTDL_BUILD_CLI "Build the Hello World command-line application" ON)
option(YTDL_WARNINGS_AS_ERRORS "Treat project compiler warnings as errors" OFF)

include(cmake/ProjectOptions.cmake)

add_subdirectory(src)

if(YTDL_BUILD_CLI)
    add_subdirectory(apps/cli)
endif()

include(cmake/CodeQuality.cmake)
