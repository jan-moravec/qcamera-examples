cmake_minimum_required(VERSION 3.5)

project(03-display-frames LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets Multimedia REQUIRED)

add_executable(03-display-frames
  main.cpp
  mainwindow.cpp
  mainwindow.h
  mainwindow.ui
  videosurface.cpp
)

target_link_libraries(03-display-frames PRIVATE Qt5::Widgets Qt5::Multimedia)
