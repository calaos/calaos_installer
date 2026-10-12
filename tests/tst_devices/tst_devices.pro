QT += core gui widgets xml testlib
equals(QT_MAJOR_VERSION, 6): QT += core5compat

CONFIG += testcase c++17 console
CONFIG -= app_bundle

TARGET = tst_devices
TEMPLATE = app

include($$PWD/../../calaos_model.pri)

SOURCES += tst_devices.cpp
