cmake_minimum_required(VERSION 3.12)

project(omam8)

option(DEBUG "debug stuff" OFF)

if (DEBUG)
    add_definitions(-DDEBUG -DYY_DEBUG)
endif()

add_library(omam8-shared STATIC
    shared/helpers.cpp
)

target_include_directories(omam8-shared PUBLIC shared)

add_subdirectory(emulator)
add_subdirectory(assembler)
