QT += core gui widgets xml testlib
equals(QT_MAJOR_VERSION, 6): QT += core5compat

CONFIG += testcase c++17 console
CONFIG -= app_bundle

TARGET = tst_hometree
TEMPLATE = app

include($$PWD/../../calaos_model.pri)

SOURCES += \
    $$PWD/../../src/HomeTree.cpp \
    $$PWD/../../src/HomeTreeMove.cpp \
    $$PWD/../../src/hometreewidget.cpp \
    $$PWD/../../src/DialogDeleteImpact.cpp \
    $$PWD/../../src/DialogDeviceInfo.cpp \
    tst_hometree.cpp

HEADERS += \
    $$PWD/../../src/HomeTree.h \
    $$PWD/../../src/HomeTreeMove.h \
    $$PWD/../../src/hometreewidget.h \
    $$PWD/../../src/DialogDeleteImpact.h \
    $$PWD/../../src/DialogDeviceInfo.h \
    $$PWD/../../src/qtreewidget_addition.h
