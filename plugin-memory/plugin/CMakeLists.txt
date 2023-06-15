cmake_minimum_required(VERSION 3.23.3 FATAL_ERROR)

project (memory)

find_package(Thorin REQUIRED)

add_library(
    memory
    SHARED
    memory.cpp
)
set_target_properties(memory PROPERTIES PREFIX "" CXX_STANDARD 17)

target_include_directories(memory PUBLIC ${Thorin_INCLUDE_DIRS})
