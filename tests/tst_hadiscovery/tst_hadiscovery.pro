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
    tst_hadiscovery.cpp

HEADERS += \
    $$PWD/../tst_devicetemplates/fixture.h \
    $$PWD/../../src/HomeAssistantDiscovery.h \
    $$PWD/../../src/MqttBroker.h \
    $$PWD/../../src/EsphomeDiscovery.h

# The dialog needs Qt MQTT, as in the application.
qtHaveModule(mqtt) {
    QT += mqtt
    DEFINES += QT_MQTT_AVAILABLE
    SOURCES += $$PWD/../../src/MqttListener.cpp $$PWD/../../src/DialogHomeAssistantMqtt.cpp \
        $$PWD/../tst_esphomediscovery/FakeMqttBroker.cpp
    HEADERS += $$PWD/../../src/MqttListener.h $$PWD/../../src/DialogHomeAssistantMqtt.h \
        $$PWD/../tst_esphomediscovery/FakeMqttBroker.h
}
