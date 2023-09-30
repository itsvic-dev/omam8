cmake_minimum_required(VERSION 3.12)

project(omam8)

option(DEBUG "debug stuff" OFF)

if (DEBUG)
    add_definitions(-DDEBUG -DYY_DEBUG)
endif()

include_directories(shared)

set(SHARED_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/helpers.cpp
)

add_subdirectory(emulator)
add_subdirectory(assembler)
