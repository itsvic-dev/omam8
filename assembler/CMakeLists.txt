cmake_minimum_required(VERSION 3.12)

project(omam8asm VERSION 3.0.0)

add_executable(${PROJECT_NAME}
    src/assembler.cpp
    src/main.cpp
    src/rom.cpp

    ${SHARED_SRC}
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
