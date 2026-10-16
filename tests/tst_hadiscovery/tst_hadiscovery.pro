QT += core gui widgets xml network testlib
equals(QT_MAJOR_VERSION, 6): QT += core5compat

CONFIG += testcase c++17 console
CONFIG -= app_bundle

TARGET = tst_hadiscovery
TEMPLATE = app

include($$PWD/../../calaos_model.pri)

RESOURCES += ../tst_dialognewdevice/dialognewdevice.qrc

SOURCES += \
    $$PWD/../../src/HomeAssistantDiscovery.cpp \
    tst_hadiscovery.cpp

HEADERS += \
    $$PWD/../tst_devicetemplates/fixture.h \
    $$PWD/../../src/HomeAssistantDiscovery.h
