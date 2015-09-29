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
        mainwindow.cpp \
    cvisablevertex.cpp

HEADERS  += mainwindow.h \
    cvisablevertex.h
