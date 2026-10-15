QT += core gui widgets xml testlib
equals(QT_MAJOR_VERSION, 6): QT += core5compat

CONFIG += testcase c++17 console
CONFIG -= app_bundle

TARGET = tst_deviceedit
TEMPLATE = app

include($$PWD/../../calaos_model.pri)

# The shipped IO documentation and templates: roles, params and filters come from them.
RESOURCES += deviceedit.qrc

SOURCES += \
    $$PWD/../../src/DeviceEdit.cpp \
    $$PWD/../../src/DialogEditDevice.cpp \
    tst_deviceedit.cpp

HEADERS += \
    $$PWD/../tst_devicetemplates/fixture.h \
    house.h \
    $$PWD/../../src/DeviceEdit.h \
    $$PWD/../../src/DialogEditDevice.h \
    $$PWD/../../src/WheelGuard.h
