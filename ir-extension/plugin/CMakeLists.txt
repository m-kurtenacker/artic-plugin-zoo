cmake_minimum_required(VERSION 3.23.3 FATAL_ERROR)

project (extension)

find_package(Thorin REQUIRED)

add_library(
    extension
    SHARED
    extension.cpp
)
set_target_properties(extension PROPERTIES PREFIX "" CXX_STANDARD 17)

target_include_directories(extension PUBLIC ${Thorin_INCLUDE_DIRS})
