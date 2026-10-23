QT += core gui widgets xml network testlib
equals(QT_MAJOR_VERSION, 6): QT += core5compat

CONFIG += testcase c++17 console
CONFIG -= app_bundle

TARGET = tst_onlinedialogs
TEMPLATE = app

include($$PWD/../../calaos_model.pri)

INCLUDEPATH += $$PWD/../tst_onlinesession

RESOURCES += \
    $$PWD/../tst_onlinesession/onlinesession.qrc \
    $$PWD/../../data/resources.qrc

FORMS += \
    $$PWD/../../data/DialogOpenOnline.ui \
    $$PWD/../../data/DialogSaveOnline.ui

SOURCES += \
    $$PWD/../../src/OnlineSession.cpp \
    $$PWD/../../src/dialogopenonline.cpp \
    $$PWD/../../src/dialogsaveonline.cpp \
    $$PWD/../../src/qanimationlabel.cpp \
    tst_onlinedialogs.cpp

HEADERS += \
    $$PWD/../tst_onlinesession/FakeApi.h \
    $$PWD/../../src/OnlineSession.h \
    $$PWD/../../src/dialogopenonline.h \
    $$PWD/../../src/dialogsaveonline.h \
    $$PWD/../../src/qanimationlabel.h
