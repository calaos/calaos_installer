QT += core gui widgets xml testlib
equals(QT_MAJOR_VERSION, 6): QT += core5compat

CONFIG += testcase c++17 console
CONFIG -= app_bundle

TARGET = tst_analogbounds
TEMPLATE = app

include($$PWD/../../calaos_model.pri)

SOURCES += \
    $$PWD/../../src/formanalogproperties.cpp \
    $$PWD/../../src/dialognewanalog.cpp \
    tst_analogbounds.cpp

HEADERS += \
    $$PWD/../../src/formanalogproperties.h \
    $$PWD/../../src/dialognewanalog.h

FORMS += \
    $$PWD/../../data/formanalogproperties.ui \
    $$PWD/../../data/DialogNewAnalog.ui
