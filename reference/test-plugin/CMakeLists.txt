cmake_minimum_required(VERSION 3.13.4)

project(test)

find_package(AnyDSL_runtime REQUIRED)

anydsl_runtime_wrap(MAIN_PROGRAM
    FRONTEND "artic"
    CLANG_FLAGS ""
    FILES main.art
)
add_executable(main ${MAIN_PROGRAM})
target_link_libraries(main ${AnyDSL_runtime_LIBRARIES})
