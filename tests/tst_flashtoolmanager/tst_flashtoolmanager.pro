QT += core network testlib

CONFIG += testcase c++17 console
CONFIG -= app_bundle

TARGET = tst_flashtoolmanager
TEMPLATE = app

INCLUDEPATH += $$PWD/../../src

# The unit tests only run on Linux (pr_ci.yml).
unix:!macx {
    INCLUDEPATH += /usr/include/KF6/KArchive
    LIBS += -lKF6Archive
}

HEADERS += \
    $$PWD/../../src/FlashToolManager.h \
    $$PWD/../../src/HttpClient.h \
    $$PWD/../../src/ConfigOptions.h \
    $$PWD/../../src/SimpleCrypt.h

SOURCES += \
    $$PWD/../../src/FlashToolManager.cpp \
    $$PWD/../../src/HttpClient.cpp \
    $$PWD/../../src/networkerrortext.cpp \
    $$PWD/../../src/EsptoolRelease.cpp \
    $$PWD/../../src/BoardProfile.cpp \
    $$PWD/../../src/ConfigOptions.cpp \
    $$PWD/../../src/SimpleCrypt.cpp \
    tst_flashtoolmanager.cpp
