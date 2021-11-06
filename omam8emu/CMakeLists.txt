cmake_minimum_required(VERSION 3.21)

project(omam8emu)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(Boost COMPONENTS program_options REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(
    include
    ${Boost_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIR}
    ${SDL2_TTF_INCLUDE_DIR}
)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/core.cpp
)

target_link_libraries(${PROJECT_NAME}
    ${Boost_LIBRARIES}
    ${SDL2_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
)
