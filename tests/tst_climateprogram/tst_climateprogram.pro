QT += core gui widgets xml testlib
equals(QT_MAJOR_VERSION, 6): QT += core5compat

CONFIG += testcase c++17 console
CONFIG -= app_bundle

TARGET = tst_climateprogram
TEMPLATE = app

include($$PWD/../../calaos_model.pri)

SOURCES += \
    $$PWD/../../src/HomeTree.cpp \
    $$PWD/../../src/HomeTreeMove.cpp \
    tst_climateprogram.cpp

HEADERS += $$PWD/../../src/HomeTree.h $$PWD/../../src/HomeTreeMove.h $$PWD/../../src/qtreewidget_addition.h
