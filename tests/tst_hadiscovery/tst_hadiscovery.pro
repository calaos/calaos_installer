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
    $$PWD/../../src/EsphomeDiscovery.cpp \
    $$PWD/../../src/MqttBroker.cpp \
    $$PWD/../../src/MqttPacket.cpp \
    $$PWD/../../src/MqttClient.cpp \
    $$PWD/../../src/MqttListener.cpp \
    $$PWD/../../src/DialogHomeAssistantMqtt.cpp \
    $$PWD/../tst_esphomediscovery/FakeMqttBroker.cpp \
    tst_hadiscovery.cpp

HEADERS += \
    $$PWD/../tst_devicetemplates/fixture.h \
    $$PWD/../../src/HomeAssistantDiscovery.h \
    $$PWD/../../src/MqttBroker.h \
    $$PWD/../../src/MqttPacket.h \
    $$PWD/../../src/MqttClient.h \
    $$PWD/../../src/MqttListener.h \
    $$PWD/../../src/DialogHomeAssistantMqtt.h \
    $$PWD/../tst_esphomediscovery/FakeMqttBroker.h \
    $$PWD/../../src/EsphomeDiscovery.h
