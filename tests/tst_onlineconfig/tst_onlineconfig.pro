QT += core testlib

CONFIG += testcase c++17 console
CONFIG -= app_bundle

TARGET = tst_onlineconfig
TEMPLATE = app

INCLUDEPATH += $$PWD/../../src

SOURCES += \
    $$PWD/../../src/OnlineConfig.cpp \
    tst_onlineconfig.cpp
