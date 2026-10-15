QT += core gui widgets xml testlib
equals(QT_MAJOR_VERSION, 6): QT += core5compat

CONFIG += testcase c++17 console
CONFIG -= app_bundle

TARGET = tst_devicetemplates
TEMPLATE = app

include($$PWD/../../calaos_model.pri)

# The shipped templates and IO documentation, and a marked French file.
RESOURCES += devicetemplates.qrc

SOURCES += \
    $$PWD/../../src/IODoc.cpp \
    $$PWD/../../src/HomeTree.cpp \
    $$PWD/../../src/HomeTreeMove.cpp \
    tst_devicetemplates.cpp

HEADERS += \
    fixture.h \
    $$PWD/../../src/IODoc.h \
    $$PWD/../../src/HomeTree.h \
    $$PWD/../../src/HomeTreeMove.h \
    $$PWD/../../src/qtreewidget_addition.h
