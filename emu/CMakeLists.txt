cmake_minimum_required(VERSION 3.15)

project(omam8emu)

find_library(SDL2 SDL2 REQUIRED)

set(SRCS main.c)

add_executable(${PROJECT_NAME} ${SRCS})
target_link_libraries(${PROJECT_NAME} ${SDL2})
