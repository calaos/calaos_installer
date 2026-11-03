QT += core testlib

CONFIG += testcase c++17 console
CONFIG -= app_bundle

TARGET = tst_esptoolrelease
TEMPLATE = app

DEFINES += ESPTOOL_FIXTURE_DIR=\\\"$$clean_path($$PWD)\\\"

INCLUDEPATH += $$PWD/../../src

SOURCES += \
    $$PWD/../../src/EsptoolRelease.cpp \
    tst_esptoolrelease.cpp
