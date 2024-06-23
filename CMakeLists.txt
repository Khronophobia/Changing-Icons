cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_ARCHITECTURES "x86_64")
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

project(ChangingIcons VERSION 1.0.0)

add_library(${PROJECT_NAME} SHARED
    src/main.cpp
    src/CIUtilities.cpp
    src/class/CIManager.cpp
    src/class/CCMenuItemTriToggler.cpp
    src/class/CIConfigPopup.cpp
    src/class/IconCell.cpp
    src/class/IconListView.cpp
    src/hooks/GJGarageLayer.cpp
)

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

target_include_directories(${PROJECT_NAME} PRIVATE src/)

setup_geode_mod(${PROJECT_NAME})
