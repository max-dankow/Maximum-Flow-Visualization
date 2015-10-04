#-------------------------------------------------
#
# Project created by QtCreator 2015-09-29T19:57:49
#
#-------------------------------------------------
CONFIG += c++11
QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FlowVisualization
TEMPLATE = app


SOURCES += main.cpp\
    algorithmrelabeltofront.cpp \
    network.cpp \
    visablevertex.cpp \
    maxflowvisualizer.cpp \
    forcedirectedgraphdrawing.cpp

HEADERS  += \
    algorithmrelabeltofront.h \
    network.h \
    visablevertex.h \
    maxflowvisualizer.h \
    forcedirectedgraphdrawing.h
