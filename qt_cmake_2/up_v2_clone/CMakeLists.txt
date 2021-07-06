cmake_minimum_required(VERSION 2.8.11)

project(testproject)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

aux_source_directory(. SOURCE_DIR)

find_package(Qt5 REQUIRED Widgets Core Gui PrintSupport)

set(up_test_successful mainwindow)

add_executable(up_test_successful ${SOURCE_DIR} )
target_link_libraries(up_test_successful Qt5::Widgets  Qt5::Gui Qt5::Core Qt5::PrintSupport)