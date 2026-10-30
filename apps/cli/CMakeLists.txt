add_executable(ytdl main.cpp)
target_link_libraries(ytdl PRIVATE ytdl::core)
ytdl_configure_target(ytdl)
