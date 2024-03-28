cmake_minimum_required(VERSION 3.13.4)

project(cache)

find_package(Thorin REQUIRED)
find_package(AnyDSL_runtime REQUIRED)

add_library(serdes
    SHARED
    serdes.cpp
)

set_target_properties(serdes PROPERTIES PREFIX "" CXX_STANDARD 17)

target_link_libraries(serdes PUBLIC ${Thorin_LIBRARIES})
target_include_directories(serdes PUBLIC ${Thorin_INCLUDE_DIRS})

anydsl_runtime_wrap(MAIN_PROGRAM
    FRONTEND "artic"
    CLANG_FLAGS ""
    ARTIC_FLAGS --plugin $<TARGET_FILE:serdes>
    FILES main.art
)
add_executable(main ${MAIN_PROGRAM} util.c)
target_link_libraries(main ${AnyDSL_runtime_LIBRARIES})
