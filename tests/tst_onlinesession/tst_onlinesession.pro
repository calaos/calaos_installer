QT += core network testlib

CONFIG += testcase c++17 console
CONFIG -= app_bundle

TARGET = tst_onlinesession
TEMPLATE = app

INCLUDEPATH += $$PWD/../../src

# Self-signed certificate of the fake https server, valid for 127.0.0.1 only.
RESOURCES += onlinesession.qrc

SOURCES += \
    $$PWD/../../src/OnlineConfig.cpp \
    $$PWD/../../src/OnlineSession.cpp \
    $$PWD/../../src/AdminCache.cpp \
    tst_onlinesession.cpp

HEADERS += \
    FakeApi.h \
    $$PWD/../../src/AdminCache.h \
    $$PWD/../../src/OnlineSession.h
