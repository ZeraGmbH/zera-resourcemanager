cmake_minimum_required(VERSION 3.1)

project(zera-resourcemanager LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(resman_VERSION_MAJOR "2")
set(resman_VERSION_MINOR "2")
set(resman_VERSION_PATCH "0")
set(PROJECT_VERSION "${resman_VERSION_MAJOR}.${resman_VERSION_MINOR}.${resman_VERSION_PATCH}")

include(FeatureSummary)
include(GNUInstallDirs)

find_package(Qt5 COMPONENTS Core Gui Network CONFIG REQUIRED)
find_package(xiqnet REQUIRED)
find_package(zeraprotobuf REQUIRED)
find_package(SCPI REQUIRED)

file(GLOB SOURCES
    ${PROJECT_SOURCE_DIR}/*.cpp
    ${PROJECT_SOURCE_DIR}/resource/*.cpp
    ${PROJECT_SOURCE_DIR}/server/*.cpp
    ${PROJECT_SOURCE_DIR}/scpi/*.cpp
    )

file(GLOB HEADER
    ${PROJECT_SOURCE_DIR}/*.h
    ${PROJECT_SOURCE_DIR}/resource/*.h
    ${PROJECT_SOURCE_DIR}/server/*.h
    ${PROJECT_SOURCE_DIR}/scpi/*.h
    )

add_executable(resman
    ${SOURCES}
    ${HEADER}
    )

target_link_libraries(resman
    PUBLIC
    Qt5::Core
    Qt5::Gui
    Qt5::Network
    VeinMeta::xiqnet
    VeinMeta::zeraprotobuf 
    Zera::SCPI
    )

target_include_directories(resman
    PUBLIC
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/resman>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/resource>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/scpi>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/server>
    )

install(TARGETS resman
    EXPORT resmanExport
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
