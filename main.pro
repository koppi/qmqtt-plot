QT += core network gui webkitwidgets printsupport widgets

CONFIG += c++11

TEMPLATE = app

TARGET = qmqtt-plot

DEFINES += QMQTT_EXAMPLE

INCLUDEPATH += libqmqtt
LIBS += -lqmqtt

SOURCES = main.cpp \
    mainwindow.cpp ledindicator.cpp qcustomplot.cpp

FORMS += \
    mainwindow.ui

HEADERS += \
    mainwindow.h ledindicator.h qcustomplot.h
