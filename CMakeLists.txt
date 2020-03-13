cmake_minimum_required(VERSION 3.0)

project(cxxtest)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_STANDARD 11)

add_compile_options("-g")

add_subdirectory(./src)


