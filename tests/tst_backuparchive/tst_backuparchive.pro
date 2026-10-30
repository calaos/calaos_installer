QT += core testlib

CONFIG += testcase c++17 console
CONFIG -= app_bundle

TARGET = tst_backuparchive
TEMPLATE = app

INCLUDEPATH += $$PWD/../../src

# The unit tests only run on Linux (pr_ci.yml).
unix:!macx {
    INCLUDEPATH += /usr/include/KF6/KArchive
    LIBS += -lKF6Archive
}

SOURCES += \
    $$PWD/../../src/BackupArchive.cpp \
    tst_backuparchive.cpp

HEADERS += \
    $$PWD/../../src/BackupArchive.h
