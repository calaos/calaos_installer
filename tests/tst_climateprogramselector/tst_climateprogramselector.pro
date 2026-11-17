QT += core gui widgets xml testlib
equals(QT_MAJOR_VERSION, 6): QT += core5compat

CONFIG += testcase c++17 console
CONFIG -= app_bundle

TARGET = tst_climateprogramselector
TEMPLATE = app

include($$PWD/../../calaos_model.pri)

INCLUDEPATH += $$PWD/../../src

SOURCES += \
    $$PWD/../../src/DialogIOSelector.cpp \
    $$PWD/../../src/ClearLineEdit.cpp \
    tst_climateprogramselector.cpp

HEADERS += \
    $$PWD/../../src/DialogIOSelector.h \
    $$PWD/../../src/ClearLineEdit.h
