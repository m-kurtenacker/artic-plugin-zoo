cmake_minimum_required(VERSION 3.13.4)

project(cache)

find_package(AnyDSL_runtime REQUIRED)

anydsl_runtime_wrap(MAIN_PROGRAM
    FRONTEND "artic"
    CLANG_FLAGS ""
    PLUGINS extern_language.cpp
    FILES main.art
)

#This output does not require a command, it is produced by the plugin.
add_custom_command(OUTPUT test.c DEPENDS ${MAIN_PROGRAM})

add_executable(main ${MAIN_PROGRAM} test.c)
target_link_libraries(main ${AnyDSL_runtime_LIBRARIES})
