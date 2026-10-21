QT += core network testlib

CONFIG += testcase c++17 console
CONFIG -= app_bundle

TARGET = tst_mqttclient
TEMPLATE = app

INCLUDEPATH += $$PWD/../../src $$PWD/../tst_esphomediscovery

SOURCES += \
    $$PWD/../../src/MqttPacket.cpp \
    $$PWD/../../src/MqttClient.cpp \
    $$PWD/../tst_esphomediscovery/FakeMqttBroker.cpp \
    tst_mqttclient.cpp

HEADERS += \
    $$PWD/../../src/MqttPacket.h \
    $$PWD/../../src/MqttClient.h \
    $$PWD/../tst_esphomediscovery/FakeMqttBroker.h
