cmake_minimum_required(VERSION 3.12)

project(omam8)

if (${YY_DEBUG} MATCHES "no")
    unset(YY_DEBUG)
endif()

if (DEFINED YY_DEBUG)
    add_definitions(-DYY_DEBUG)
endif()

add_subdirectory(emulator)
add_subdirectory(assembler)
