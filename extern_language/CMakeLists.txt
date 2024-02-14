cmake_minimum_required(VERSION 3.13.4)

project(cache)

find_package(Thorin REQUIRED)
find_package(AnyDSL_runtime REQUIRED)

add_library(extern_language
    SHARED
    extern_language.cpp
)

set_target_properties(extern_language PROPERTIES PREFIX "" CXX_STANDARD 17)

target_link_libraries(extern_language PUBLIC ${Thorin_LIBRARIES})
target_include_directories(extern_language PUBLIC ${Thorin_INCLUDE_DIRS})

anydsl_runtime_wrap(MAIN_PROGRAM
    FRONTEND "artic"
    CLANG_FLAGS ""
    ARTIC_FLAGS --plugin $<TARGET_FILE:extern_language>
    FILES main.art
)

#This output does not require a command, it is produced by the plugin.
add_custom_command(OUTPUT test.c DEPENDS ${MAIN_PROGRAM})

add_executable(main ${MAIN_PROGRAM} test.c)
target_link_libraries(main ${AnyDSL_runtime_LIBRARIES})
