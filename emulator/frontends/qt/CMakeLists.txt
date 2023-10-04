cmake_minimum_required(VERSION 3.12)
project(omam8emu-qt VERSION 3.0.0)

find_package(Qt6 REQUIRED COMPONENTS Widgets LinguistTools)
qt_standard_project_setup()

qt_add_executable(omam8emu-qt
    main.cpp

    emuwindow.ui
    emuwindow.cpp
)

set(TS_FILES
    i18n/ja_JP.ts
)

qt_add_lupdate(omam8emu-qt TS_FILES ${TS_FILES})
qt_add_lrelease(omam8emu-qt
    TS_FILES ${TS_FILES}
    QM_FILES_OUTPUT_VARIABLE qm_files
)

qt_add_resources(omam8emu-qt "i18n"
    PREFIX "/i18n"
    BASE "${CMAKE_CURRENT_BINARY_DIR}"
    FILES "${qm_files}"
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

target_link_libraries(omam8emu-qt
    PRIVATE
    Qt6::Widgets
)
