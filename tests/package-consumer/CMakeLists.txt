cmake_minimum_required(VERSION 3.16)
project(QtUiStylePackageConsumer LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)

find_package(QtUiStyle 0.1 CONFIG REQUIRED)

add_executable(qt_ui_style_package_consumer main.cpp)
target_link_libraries(qt_ui_style_package_consumer PRIVATE QtUiStyle::Dialogs)
