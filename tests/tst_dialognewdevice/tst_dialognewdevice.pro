QT += core gui widgets xml network testlib
equals(QT_MAJOR_VERSION, 6): QT += core5compat

CONFIG += testcase c++17 console
CONFIG -= app_bundle

TARGET = tst_dialognewdevice
TEMPLATE = app

include($$PWD/../../calaos_model.pri)

RESOURCES += dialognewdevice.qrc

SOURCES += \
    $$PWD/../../src/DialogNewDevice.cpp \
    $$PWD/../../src/EsphomeDiscovery.cpp \
    $$PWD/../../src/MqttBroker.cpp \
    $$PWD/../../src/DialogDeviceInfo.cpp \
    $$PWD/../../src/DialogNewRemoteUIRelay.cpp \
    tst_dialognewdevice.cpp

HEADERS += \
    $$PWD/../tst_devicetemplates/fixture.h \
    $$PWD/../../src/DialogNewDevice.h \
    $$PWD/../../src/MqttBroker.h \
    $$PWD/../../src/EsphomeDiscovery.h \
    $$PWD/../../src/DialogDeviceInfo.h \
    $$PWD/../../src/DialogNewRemoteUIRelay.h

# An existing dialog next to the new one in the screenshots.
FORMS += $$PWD/../../src/DialogNewRemoteUIRelay.ui

# The broker discovery of the ESPHome templates needs Qt MQTT, as in the application.
qtHaveModule(mqtt) {
    QT += mqtt
    DEFINES += QT_MQTT_AVAILABLE
    SOURCES += $$PWD/../../src/MqttListener.cpp $$PWD/../../src/WidgetEsphomeDiscovery.cpp $$PWD/../tst_esphomediscovery/FakeMqttBroker.cpp
    HEADERS += $$PWD/../../src/MqttListener.h $$PWD/../../src/WidgetEsphomeDiscovery.h $$PWD/../tst_esphomediscovery/FakeMqttBroker.h
}
