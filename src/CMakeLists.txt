aux_source_directory(. DIR_SRCS)

add_executable(cxxtest main.cpp)

target_link_libraries(cxxtest pthread boost_thread boost_atomic)
