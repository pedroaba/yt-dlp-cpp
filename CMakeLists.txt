cmake_minimum_required(VERSION 3.20)

project(vidx LANGUAGES CXX)

# ==========================================
# C++ Configuration
# ==========================================

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Generate compile_commands.json for Clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ==========================================
# Dependencies
# ==========================================

find_package(CURL 7.62 REQUIRED)
find_package(LibXml2 REQUIRED)

# ==========================================
# Vidx Library
# ==========================================

add_library(vidx_core STATIC
    src/url.cpp
    src/core/http/client.cpp
    src/core/extractor/html_title.cpp
)

add_library(vidx::core ALIAS vidx_core)

target_include_directories(vidx_core
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

set_target_properties(vidx_core PROPERTIES POSITION_INDEPENDENT_CODE ON)

# ==========================================
# Vidx Executable
# ==========================================

add_executable(vidx
    src/main.cpp
)

# ==========================================
# Link Libraries
# ==========================================

target_link_libraries(vidx_core PRIVATE
    CURL::libcurl
)

target_link_libraries(vidx PRIVATE
    vidx::core
)

target_link_libraries(vidx_core PRIVATE
    CURL::libcurl
    LibXml2::LibXml2
)
