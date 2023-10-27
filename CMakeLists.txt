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

find_package(ZenuxLegacyComponents REQUIRED)

add_executable(resman
    main.cpp
    )

target_link_libraries(resman
    PRIVATE
    Zera::resman-lib
    )

install(TARGETS resman
    EXPORT resmanExport
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
