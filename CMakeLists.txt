cmake_minimum_required(VERSION 3.1)

project(zera-resourcemanager LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(PROJECT_VERSION_MAJOR "2")
set(PROJECT_VERSION_MINOR "2")
set(PROJECT_VERSION_PATCH "0")
set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

include(FeatureSummary)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

find_package(Qt5 COMPONENTS Core Network Test CONFIG REQUIRED)
find_package(VeinFramework REQUIRED)
find_package(SCPI REQUIRED)

file(GLOB SOURCES
    resmanrunfacade.cpp
    resourcemanager.cpp
    rmprotobufwrapper.cpp
    resource/*.cpp
    server/*.cpp
    scpi/*.cpp
    )

file(GLOB HEADER
    *.h
    resource/*.h
    server/*.h
    scpi/*.h
    )

add_library(resman-lib STATIC
    ${SOURCES}
    ${HEADER}
    )

target_link_libraries(resman-lib
    PUBLIC
    Qt5::Core
    Qt5::Network
    Zera::xiqnet
    Zera::zeraprotobuf
    Zera::SCPI
    )

target_include_directories(resman-lib
    PUBLIC
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/zera-resman>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/resource>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/scpi>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/server>
    )

set_target_properties(resman-lib PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(resman-lib PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

write_basic_package_version_file(
    resman-libConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY SameMajorVersion
    )

configure_file(resman-libConfig.cmake.in resman-libConfig.cmake @ONLY)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/resman-libConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/resman-libConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/resman-lib
    )

install(EXPORT resman-libExport
    FILE resman-libTargets.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/resman-lib
    )

install(
    FILES ${HEADER}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/zera-resman
    )

install(TARGETS resman-lib
    EXPORT resman-libExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

add_executable(resman
    main.cpp
    )

target_link_libraries(resman
    PUBLIC
    resman-lib
    )

install(TARGETS resman
    EXPORT resmanExport
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

enable_testing()
add_subdirectory(tests)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
