cmake_minimum_required(VERSION 3.13.4)

project(cache)

find_package(Thorin REQUIRED)
find_package(AnyDSL_runtime REQUIRED)

add_library(cache_plugin
    SHARED
    cache-plugin.cpp
)

set_target_properties(cache_plugin PROPERTIES PREFIX "" CXX_STANDARD 17)

target_link_libraries(cache_plugin PUBLIC ${Thorin_LIBRARIES})
target_include_directories(cache_plugin PUBLIC ${Thorin_INCLUDE_DIRS})

anydsl_runtime_wrap(MAIN_PROGRAM
    FRONTEND "artic"
    CLANG_FLAGS ""
    ARTIC_FLAGS --plugin $<TARGET_FILE:cache_plugin>
    FILES main.art
)
add_executable(main ${MAIN_PROGRAM} cache-util.cpp)
target_link_libraries(main ${AnyDSL_runtime_LIBRARIES})
