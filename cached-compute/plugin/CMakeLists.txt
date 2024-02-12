cmake_minimum_required(VERSION 3.23.3 FATAL_ERROR)

project (cache)

find_package(Thorin REQUIRED)

add_library(
    cache
    SHARED
    cache.cpp
)
set_target_properties(cache PROPERTIES PREFIX "" CXX_STANDARD 17)

target_include_directories(cache PUBLIC ${Thorin_INCLUDE_DIRS})
