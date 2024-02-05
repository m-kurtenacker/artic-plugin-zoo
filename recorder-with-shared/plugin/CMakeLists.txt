cmake_minimum_required(VERSION 3.23.3 FATAL_ERROR)

project (recorder)

find_package(Thorin REQUIRED)

add_library(
    recorder
    SHARED
    recorder.cpp
)
set_target_properties(recorder PROPERTIES PREFIX "" CXX_STANDARD 17)

target_include_directories(recorder PUBLIC ${Thorin_INCLUDE_DIRS})
