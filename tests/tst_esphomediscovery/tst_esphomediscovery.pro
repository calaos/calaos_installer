QT += core gui widgets xml network testlib
equals(QT_MAJOR_VERSION, 6): QT += core5compat

CONFIG += testcase c++17 console
CONFIG -= app_bundle

TARGET = tst_esphomediscovery
TEMPLATE = app

include($$PWD/../../calaos_model.pri)

RESOURCES += ../tst_dialognewdevice/dialognewdevice.qrc

SOURCES += \
    $$PWD/../../src/EsphomeDiscovery.cpp \
    $$PWD/../../src/MqttBroker.cpp \
    $$PWD/../../src/MqttPacket.cpp \
    $$PWD/../../src/MqttClient.cpp \
    $$PWD/../../src/MqttListener.cpp \
    FakeMqttBroker.cpp \
    tst_esphomediscovery.cpp

HEADERS += \
    $$PWD/../tst_devicetemplates/fixture.h \
    $$PWD/../../src/MqttBroker.h \
    $$PWD/../../src/MqttPacket.h \
    $$PWD/../../src/MqttClient.h \
    $$PWD/../../src/MqttListener.h \
    FakeMqttBroker.h \
    $$PWD/../../src/EsphomeDiscovery.h
