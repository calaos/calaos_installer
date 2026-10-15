#ifndef FAKEMQTTBROKER_H
#define FAKEMQTTBROKER_H

#include <QByteArray>
#include <QList>
#include <QPair>
#include <QStringList>
#include <QTcpServer>

class QTcpSocket;

// The broker side of MQTT 3.1.1, written apart from MqttListener so that a mistake
// in one is not repeated in the other. Serves retained messages on SUBSCRIBE.
class FakeMqttBroker: public QTcpServer
{
    Q_OBJECT

public:
    explicit FakeMqttBroker(QObject *parent = nullptr);

    quint16 start();

    //Behaviour.
    QList<QPair<QString, QByteArray>> retained;
    QString requiredUser, requiredPassword; //empty: anyone
    int connackCode = 0;
    bool answerConnect = true;
    bool closeAfterConnect = false;
    bool refuseSubscriptions = false;
    bool deliverAtQos1 = false;
    bool byteByByte = false;

    //What the client did.
    int connects = 0;
    QString user, password, clientId;
    bool hadPassword = false;
    bool cleanSession = false;
    int keepAlive = -1;
    QStringList filters;
    int pubacks = 0;
    int disconnects = 0;

    void publish(const QString &topic, const QByteArray &payload);
    //Sends raw bytes to the connected client.
    void sendRaw(const QByteArray &bytes);

    static bool matches(const QString &filter, const QString &topic);

private:
    QTcpSocket *m_client = nullptr;
    QByteArray m_in;

    void onData();
    void handle(quint8 header, const QByteArray &body);
    void send(quint8 header, const QByteArray &body);
};

#endif // FAKEMQTTBROKER_H
