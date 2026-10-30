# Keep the file list explicit so adding a source is a deliberate build change.
set(YTDL_QUALITY_SOURCES "${PROJECT_SOURCE_DIR}/src/core/greeting.cpp")
set(YTDL_FORMAT_FILES
    ${YTDL_QUALITY_SOURCES}
    "${PROJECT_SOURCE_DIR}/include/ytdl/core/greeting.hpp"
)

if(YTDL_BUILD_CLI)
    list(APPEND YTDL_QUALITY_SOURCES "${PROJECT_SOURCE_DIR}/apps/cli/main.cpp")
    list(APPEND YTDL_FORMAT_FILES "${PROJECT_SOURCE_DIR}/apps/cli/main.cpp")
endif()

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
        COMMENT "Formatting C++ sources"
        VERBATIM
    )
    add_custom_target(format-check
        COMMAND "${YTDL_CLANG_FORMAT}" --dry-run --Werror ${YTDL_FORMAT_FILES}
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
                -p "${PROJECT_BINARY_DIR}" "${source}"
            WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
            VERBATIM
        )
    endforeach()
else()
    message(STATUS "lint requires clang-tidy and a Ninja or Makefiles generator")
endif()
