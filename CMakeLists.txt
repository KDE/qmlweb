cmake_minimum_required(VERSION 2.8.11)

project(qmlweb)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

include(GenerateQrcFile)
enable_testing()

# QMLJSC
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_FLAGS "-std=c++0x")

find_package(Qt5 COMPONENTS Core)

# Components of qmlweb
add_subdirectory(src/qmljsc)
add_subdirectory(tests/auto/qmljsc)