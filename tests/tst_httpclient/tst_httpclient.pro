QT += core network testlib

CONFIG += testcase c++17 console
CONFIG -= app_bundle

TARGET = tst_httpclient
TEMPLATE = app

INCLUDEPATH += $$PWD/../../src

HEADERS += $$PWD/../../src/HttpClient.h

SOURCES += \
    $$PWD/../../src/HttpClient.cpp \
    $$PWD/../../src/networkerrortext.cpp \
    tst_httpclient.cpp
