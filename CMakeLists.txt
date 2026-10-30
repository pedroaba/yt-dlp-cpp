cmake_minimum_required(VERSION 3.24)

project(ytdl VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(YTDL_BUILD_CLI "Build the Hello World command-line application" ON)
option(YTDL_WARNINGS_AS_ERRORS "Treat project compiler warnings as errors" OFF)

# Register library sources and headers here. Quality tools reuse these lists.
set(YTDL_SOURCES
    src/greeting.cpp
)
set(YTDL_HEADERS
    include/ytdl/greeting.hpp
)

function(ytdl_configure_target target)
    target_compile_features(${target} PUBLIC cxx_std_20)
    set_target_properties(${target} PROPERTIES CXX_EXTENSIONS OFF)

    if(MSVC)
        target_compile_options(${target} PRIVATE /W4 /permissive- /utf-8)
        if(YTDL_WARNINGS_AS_ERRORS)
            target_compile_options(${target} PRIVATE /WX)
        endif()
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        target_compile_options(${target} PRIVATE
            -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wshadow
        )
        if(YTDL_WARNINGS_AS_ERRORS)
            target_compile_options(${target} PRIVATE -Werror)
        endif()
    endif()
endfunction()

add_library(ytdl_core STATIC ${YTDL_SOURCES} ${YTDL_HEADERS})
add_library(ytdl::core ALIAS ytdl_core)
target_include_directories(ytdl_core
    PUBLIC "${PROJECT_SOURCE_DIR}/include"
    PRIVATE "${PROJECT_SOURCE_DIR}/src"
)
set_target_properties(ytdl_core PROPERTIES POSITION_INDEPENDENT_CODE ON)
ytdl_configure_target(ytdl_core)

set(YTDL_QUALITY_SOURCES ${YTDL_SOURCES})
set(YTDL_FORMAT_FILES ${YTDL_SOURCES} ${YTDL_HEADERS})

if(YTDL_BUILD_CLI)
    add_executable(ytdl src/main.cpp)
    target_link_libraries(ytdl PRIVATE ytdl::core)
    ytdl_configure_target(ytdl)
    list(APPEND YTDL_QUALITY_SOURCES src/main.cpp)
    list(APPEND YTDL_FORMAT_FILES src/main.cpp)
endif()

# Formatting and static analysis are optional for a normal build.
set(YTDL_LLVM_HINTS
    "$ENV{LLVM_ROOT}/bin"
    "/opt/homebrew/opt/llvm/bin"
    "/usr/local/opt/llvm/bin"
)

find_program(YTDL_CLANG_FORMAT
    NAMES clang-format clang-format-18
    HINTS ${YTDL_LLVM_HINTS}
)
find_program(YTDL_CLANG_TIDY
    NAMES clang-tidy clang-tidy-18
    HINTS ${YTDL_LLVM_HINTS}
)

if(YTDL_CLANG_FORMAT)
    add_custom_target(format
        COMMAND "${YTDL_CLANG_FORMAT}" -i ${YTDL_FORMAT_FILES}
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
        COMMENT "Formatting C++ sources"
        VERBATIM
    )
    add_custom_target(format-check
        COMMAND "${YTDL_CLANG_FORMAT}" --dry-run --Werror ${YTDL_FORMAT_FILES}
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
        COMMENT "Checking C++ formatting"
        VERBATIM
    )
else()
    message(STATUS "clang-format not found: format and format-check are unavailable")
endif()

if(YTDL_CLANG_TIDY AND CMAKE_GENERATOR MATCHES "Ninja|Makefiles")
    # Homebrew clang-tidy does not inherit AppleClang's SDK header search paths.
    # Reuse the paths detected by CMake instead of hardcoding a local SDK version.
    set(YTDL_TIDY_EXTRA_ARGS)
    if(APPLE AND CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
        foreach(directory IN LISTS CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES)
            list(APPEND YTDL_TIDY_EXTRA_ARGS "--extra-arg=-isystem${directory}")
        endforeach()
    endif()

    add_custom_target(lint COMMENT "Analyzing C++ sources with clang-tidy")
    foreach(source IN LISTS YTDL_QUALITY_SOURCES)
        add_custom_command(TARGET lint POST_BUILD
            COMMAND "${YTDL_CLANG_TIDY}" ${YTDL_TIDY_EXTRA_ARGS}
                -p "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/${source}"
            WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
            VERBATIM
        )
    endforeach()
else()
    message(STATUS "lint requires clang-tidy and a Ninja or Makefiles generator")
endif()
