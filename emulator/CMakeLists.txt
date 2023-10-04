cmake_minimum_required(VERSION 3.12)
project(omam8emu-core VERSION 3.0.0)

find_package(boost_program_options REQUIRED)

add_library(${PROJECT_NAME} STATIC
    src/core.cpp
    src/io.cpp
    src/rom.cpp

    src/io/serial.cpp

    src/opcodes/io.cpp
    src/opcodes/jumps.cpp
    src/opcodes/math.cpp
    src/opcodes/simple.cpp
    src/opcodes/stack.cpp

    ${SHARED_SRC}
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME}
    Boost::program_options
    omam8-shared
)

option(EMU_QT "Enable omam8emu's Qt frontend" OFF)
option(EMU_CLI "Enable omam8emu's CLI frontend" ON)

if (EMU_CLI)
    message(STATUS "Building omam8emu-cli")
    add_subdirectory(frontends/cli)
endif()

if (EMU_QT)
    message(STATUS "Building omam8emu-qt")
    add_subdirectory(frontends/qt)
endif()
