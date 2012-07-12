#-------------------------------------------------
#
# Project created by QtCreator 2011-12-15T09:24:23
#
#-------------------------------------------------

QT       += core network

TARGET = ResourceManager
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    resourcemanager.cpp \
    server/serverinterface.cpp \
    server/client.cpp \
    resource/resourceobject.cpp \
    server/implementations/scpi.cpp

HEADERS += \
    resourcemanager.h \
    server/serverinterface.h \
    server/client.h \
    resource/resourceobject.h \
    server/implementations/scpi.h
