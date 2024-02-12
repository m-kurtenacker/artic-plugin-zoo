cmake_minimum_required(VERSION 3.23.3 FATAL_ERROR)

project (extern_language)

find_package(Thorin REQUIRED)

add_library(
    extern_language
    SHARED
    extern_language.cpp
)
set_target_properties(extern_language PROPERTIES PREFIX "" CXX_STANDARD 17)

target_include_directories(extern_language PUBLIC ${Thorin_INCLUDE_DIRS})
