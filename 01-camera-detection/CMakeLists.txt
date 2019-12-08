cmake_minimum_required(VERSION 3.5)

project(01-camera-detection LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5Core)
find_package(Qt5Multimedia)

add_executable(01-camera-detection main.cpp)
target_link_libraries(01-camera-detection Qt5::Core)
target_link_libraries(01-camera-detection Qt5::Multimedia)
