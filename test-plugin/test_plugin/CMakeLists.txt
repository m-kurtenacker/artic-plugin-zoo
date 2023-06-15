cmake_minimum_required(VERSION 3.13.4 FATAL_ERROR)

project (test)

find_package(Thorin REQUIRED)

add_library(
    test
    SHARED
    test.cpp
)

set_target_properties(test PROPERTIES PREFIX "" CXX_STANDARD 17)

target_link_libraries(test PUBLIC ${Thorin_LIBRARIES})
target_include_directories(test PUBLIC ${Thorin_INCLUDE_DIRS})
