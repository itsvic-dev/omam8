cmake_minimum_required(VERSION 3.12)

project(omam8asm)

add_executable(${PROJECT_NAME}
    src/assembler.cpp
    src/main.cpp
    src/rom.cpp

    ${SHARED_SRC}
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
