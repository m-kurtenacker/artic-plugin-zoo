cmake_minimum_required(VERSION 3.23.3 FATAL_ERROR)

project (dynamic_arrays)

find_package(Thorin REQUIRED)

add_library(
    dynamic_arrays
    SHARED
    dynamic_arrays.cpp
)
set_target_properties(dynamic_arrays PROPERTIES PREFIX "" CXX_STANDARD 17)

target_include_directories(dynamic_arrays PUBLIC ${Thorin_INCLUDE_DIRS})
