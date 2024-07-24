cmake_minimum_required(VERSION 3.13.4)

project(cache)

find_package(AnyDSL_runtime REQUIRED)

anydsl_runtime_wrap(MAIN_PROGRAM
    FRONTEND "artic"
    CLANG_FLAGS ""
    PLUGINS memory.cpp
    FILES main.art
)
add_executable(main ${MAIN_PROGRAM})
target_link_libraries(main ${AnyDSL_runtime_LIBRARIES})
