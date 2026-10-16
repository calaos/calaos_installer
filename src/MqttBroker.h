#ifndef MQTTBROKER_H
#define MQTTBROKER_H

#include <QString>

// The MQTT broker of a project, as the Calaos server reaches it, and the address
// to reach it from this computer. Shared by the broker discoveries.
struct MqttBroker
{
    QString host;
    int port = 1883;
    QString user;
    QString password;
    bool found = false;

    //The broker most MQTT IOs of the project use: the server's view of it.
    static MqttBroker ofProject();

    //The host to reach the broker from this computer. A loopback address names the
    //box as the server sees it: the broker this computer last reached (Zigbee2MQTT
    //dialog), else the box last opened online.
    static QString probeHost(const QString &brokerHost, const QString &reachedBroker, const QString &calaosHost);
    //Host part of what the "open online" dialog keeps (an address, host:port or a URL).
    static QString hostOf(const QString &calaosHost);
    //Whether probeHost is the broker the project's IOs name brokerHost (from the server).
    static bool sameBroker(const QString &probeHost, const QString &brokerHost, const QString &calaosHost);
    static bool isLoopback(const QString &host);

    //The box last opened online, and the broker the Zigbee2MQTT dialog last reached.
    static QString calaosHost();
    static QString reachedHost();
};

#endif // MQTTBROKER_H
