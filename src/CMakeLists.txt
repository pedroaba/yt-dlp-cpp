add_library(ytdl_core STATIC core/greeting.cpp)
add_library(ytdl::core ALIAS ytdl_core)

target_include_directories(ytdl_core PUBLIC "${PROJECT_SOURCE_DIR}/include")
set_target_properties(ytdl_core PROPERTIES POSITION_INDEPENDENT_CODE ON)
ytdl_configure_target(ytdl_core)
