cmake_minimum_required(VERSION 3.12)

project(omam8)

if (${YY_DEBUG} MATCHES "no")
    unset(YY_DEBUG)
endif()

if (DEFINED YY_DEBUG)
    add_definitions(-DYY_DEBUG)
endif()

include_directories(shared)

set(SHARED_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/helpers.cpp
)

add_subdirectory(emulator)
add_subdirectory(assembler)
