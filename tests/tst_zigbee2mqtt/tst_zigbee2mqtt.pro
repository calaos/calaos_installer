QT += core gui widgets xml network testlib
equals(QT_MAJOR_VERSION, 6): QT += core5compat

CONFIG += testcase c++17 console
CONFIG -= app_bundle

TARGET = tst_zigbee2mqtt
TEMPLATE = app

include($$PWD/../../calaos_model.pri)

SOURCES += \
    $$PWD/../../src/DialogZigbee2mqtt.cpp \
    $$PWD/../../src/qanimationlabel.cpp \
    $$PWD/../../src/MqttPacket.cpp \
    $$PWD/../../src/MqttClient.cpp \
    $$PWD/../../src/MqttListener.cpp \
    $$PWD/../tst_esphomediscovery/FakeMqttBroker.cpp \
    tst_zigbee2mqtt.cpp

HEADERS += \
    $$PWD/../../src/DialogZigbee2mqtt.h \
    $$PWD/../../src/qanimationlabel.h \
    $$PWD/../../src/MqttPacket.h \
    $$PWD/../../src/MqttClient.h \
    $$PWD/../../src/MqttListener.h \
    $$PWD/../tst_esphomediscovery/FakeMqttBroker.h

FORMS += $$PWD/../../src/DialogZigbee2mqtt.ui
