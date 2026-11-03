QT += core gui widgets testlib

CONFIG += testcase c++17 console
CONFIG -= app_bundle

TARGET = tst_toolupdateprompt
TEMPLATE = app

INCLUDEPATH += $$PWD/../../src

HEADERS += $$PWD/../../src/ToolUpdatePrompt.h
SOURCES += \
    $$PWD/../../src/ToolUpdatePrompt.cpp \
    tst_toolupdateprompt.cpp
