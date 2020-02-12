cmake_minimum_required(VERSION 3.1)

#set project name and language
project(zera-resourcemanager LANGUAGES CXX)

#Automoc and RCC ON. Needed in QT projects
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
#C++ 11 Flag
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Set project Version
set(resman_VERSION_MAJOR "2")
set(resman_VERSION_MINOR "2")
set(resman_VERSION_PATCH "0")

set(PROJECT_VERSION "${resman_VERSION_MAJOR}.${resman_VERSION_MINOR}.${resman_VERSION_PATCH}")

#Adding cmake functions like feature_summary()
include(FeatureSummary)
include(GNUInstallDirs)

#Find dependecies
find_package(Qt5 COMPONENTS Core Gui Network CONFIG REQUIRED)
find_package(xiqnet REQUIRED)
find_package(zeraprotobuf REQUIRED)
find_package(SCPI REQUIRED)


#sum up project Files 
file(GLOB SOURCES
    ${PROJECT_SOURCE_DIR}/*.cpp 
    ${PROJECT_SOURCE_DIR}/resource/*.cpp
    ${PROJECT_SOURCE_DIR}/server/*.cpp
    ${PROJECT_SOURCE_DIR}/scpi/*.cpp
    )

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/resource)
include_directories(${PROJECT_SOURCE_DIR}/server)
include_directories(${PROJECT_SOURCE_DIR}/scpi)


file(GLOB HEADER
    ${PROJECT_SOURCE_DIR}/*.h  
    ${PROJECT_SOURCE_DIR}/resource/*.h
    ${PROJECT_SOURCE_DIR}/server/*.h
    ${PROJECT_SOURCE_DIR}/scpi/*.h
    )

file(GLOB RESOURCES 
    ${PROJECT_SOURCE_DIR}/*.qrc
    ${PROJECT_SOURCE_DIR}/resource/*.qrc
    ${PROJECT_SOURCE_DIR}/server/*.qrc
    ${PROJECT_SOURCE_DIR}/scpi/*.qrc
    )

#create executable
add_executable(resman
    ${SOURCES}
    ${HEADER}
    ${RESOURCES}
    )

#link dependencies
target_link_libraries(resman
    PRIVATE
    Qt5::Core
    Qt5::Gui
    Qt5:Network
    VeinMeta::xiqnet
    VeinMeta::zeraprotobuf 
    Zera::SCPI
    )	





# announce headers - target perspective
target_include_directories(resman
    PUBLIC
    $<BUILD_INTERFACE:${MY_LIBRARY_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/resman>
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    )



#install resmanto bin 
install(TARGETS resman
    EXPORT resmanExport
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

# spawn out some info on configuration
feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)






