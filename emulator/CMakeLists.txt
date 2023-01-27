cmake_minimum_required(VERSION 3.12)

project(omam8emu)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/core.cpp
    src/rom.cpp
    src/opcodes/jumps.cpp
    src/opcodes/math.cpp
    src/opcodes/simple.cpp
    src/opcodes/stack.cpp

    ${SHARED_SRC}
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

target_include_directories(${PROJECT_NAME} PRIVATE include)
