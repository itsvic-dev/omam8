cmake_minimum_required(VERSION 3.12)
project(omam8emu-cli VERSION 3.0.0)

add_executable(omam8emu-cli
    main.cpp
    platform.cpp
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

target_link_libraries(omam8emu-cli omam8emu-core)
