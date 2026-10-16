QT += core gui widgets xml testlib
equals(QT_MAJOR_VERSION, 6): QT += core5compat

CONFIG += testcase c++17 console
CONFIG -= app_bundle

TARGET = tst_replacedroles
TEMPLATE = app

include($$PWD/../../calaos_model.pri)

# The thermostat of a server that publishes `replaces` (zone valve), whatever data/doc ships.
RESOURCES += replacedroles.qrc

SOURCES += \
    $$PWD/../../src/DeviceEdit.cpp \
    $$PWD/../../src/DialogEditDevice.cpp \
    $$PWD/../../src/DeviceCommand.cpp \
    $$PWD/../../src/DeviceCommandForm.cpp \
    tst_replacedroles.cpp

HEADERS += \
    $$PWD/../tst_devicetemplates/fixture.h \
    house.h \
    $$PWD/../../src/DeviceEdit.h \
    $$PWD/../../src/DialogEditDevice.h \
    $$PWD/../../src/DeviceCommand.h \
    $$PWD/../../src/DeviceCommandForm.h \
    $$PWD/../../src/WheelGuard.h
