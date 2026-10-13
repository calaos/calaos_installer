QT += core gui widgets xml testlib
equals(QT_MAJOR_VERSION, 6): QT += core5compat

CONFIG += testcase c++17 console
CONFIG -= app_bundle

TARGET = tst_servertypes
TEMPLATE = app

include($$PWD/../../calaos_model.pri)

# The suite reads the sources and the shipped catalogue to prove nothing offers a dead type.
DEFINES += REPO_ROOT=\\\"$$PWD/../..\\\" SUITE_DIR=\\\"$$PWD\\\"

RESOURCES += $$PWD/../../data/resources.qrc

SOURCES += \
    $$PWD/../../src/HomeTree.cpp \
    $$PWD/../../src/HomeTreeMove.cpp \
    $$PWD/../../src/hometreewidget.cpp \
    $$PWD/../../src/DialogDeleteImpact.cpp \
    $$PWD/../../src/DialogDeviceInfo.cpp \
    tst_servertypes.cpp

HEADERS += \
    $$PWD/../../src/HomeTree.h \
    $$PWD/../../src/HomeTreeMove.h \
    $$PWD/../../src/hometreewidget.h \
    $$PWD/../../src/DialogDeleteImpact.h \
    $$PWD/../../src/DialogDeviceInfo.h \
    $$PWD/../../src/qtreewidget_addition.h
