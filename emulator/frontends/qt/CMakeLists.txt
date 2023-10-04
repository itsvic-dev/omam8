cmake_minimum_required(VERSION 3.12)
project(omam8emu-qt VERSION 3.0.0)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

qt_add_executable(omam8emu-qt
    main.cpp

    emuwindow.ui
    emuwindow.cpp
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

target_link_libraries(omam8emu-qt
    PRIVATE
    Qt6::Widgets
)
