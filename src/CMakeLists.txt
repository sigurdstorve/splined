cmake_minimum_required(VERSION 2.8.11)
project(QtSplineEditor)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)

set(QtSplineEditor_SOURCES
    SplineEditorApp.cpp
    main_gui.cpp
    widgets/SplineEditorNode.cpp
    widgets/SplineEditorScene.cpp
    widgets/SplineEditorView.cpp
    widgets/SplineEditorWidget.cpp
    SplineDataModel.cpp
    )

set(QtSplineEditor_HEADERS
    SplineEditorApp.hpp
    widgets/SplineEditorNode.hpp
    widgets/SplineEditorScene.hpp
    widgets/SplineEditorView.hpp
    widgets/SplineEditorWidget.hpp
    SplineDataModel.hpp
    )

add_executable(QtSplineEditor
               ${QtSplineEditor_SOURCES}
               ${QtSplineEditor_HEADERS}
               )

qt5_use_modules(QtSplineEditor Widgets OpenGL)               
