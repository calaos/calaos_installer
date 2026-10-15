QT += core gui widgets xml testlib
equals(QT_MAJOR_VERSION, 6): QT += core5compat

CONFIG += testcase c++17 console
CONFIG -= app_bundle

TARGET = tst_dialognewdevice
TEMPLATE = app

include($$PWD/../../calaos_model.pri)

RESOURCES += dialognewdevice.qrc

SOURCES += \
    $$PWD/../../src/DialogNewDevice.cpp \
    $$PWD/../../src/DialogDeviceInfo.cpp \
    $$PWD/../../src/DialogNewRemoteUIRelay.cpp \
    tst_dialognewdevice.cpp

HEADERS += \
    $$PWD/../tst_devicetemplates/fixture.h \
    $$PWD/../../src/DialogNewDevice.h \
    $$PWD/../../src/DialogDeviceInfo.h \
    $$PWD/../../src/DialogNewRemoteUIRelay.h

# An existing dialog next to the new one in the screenshots.
FORMS += $$PWD/../../src/DialogNewRemoteUIRelay.ui
