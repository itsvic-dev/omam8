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

add_subdirectory(frontends/cli)
