#ifndef MQTTLISTENER_H
#define MQTTLISTENER_H

#include <QByteArray>
#include <QObject>
#include <QPointer>
#include <QString>
#include <QStringList>
#include <QTimer>

class QMqttClient;

// Listens to an MQTT broker for a few seconds: connect, subscribe, collect what
// is published (retained messages first), disconnect. QMqttClient reports every
// network failure as TransportInvalid; the socket underneath says which one.
class MqttListener: public QObject
{
    Q_OBJECT

public:
    struct Settings
    {
        QString host;
        quint16 port = 1883;
        QString user;
        QString password;
    };

    explicit MqttListener(QObject *parent = nullptr);

    void start(const Settings &settings, const QStringList &filters, int listenMs = 3000,
               int answerTimeoutMs = 5000);
    //Ends the listening now, without finished().
    void stop();
    bool isRunning() const { return m_state != IDLE; }

signals:
    //The broker took the subscriptions: messages follow.
    void listening();
    void message(const QString &topic, const QByteArray &payload);
    //error is empty when the listener ran for its whole time.
    void finished(const QString &error);

private:
    enum State { IDLE, CONNECTING, SUBSCRIBING, LISTENING };

    QPointer<QMqttClient> m_client;
    QTimer m_timer;
    State m_state = IDLE;
    Settings m_settings;
    QStringList m_filters;
    int m_listenMs = 0;
    int m_answerTimeoutMs = 0;
    int m_pending = 0;
    int m_granted = 0;

    void connected();
    void subscriptionSettled(bool granted);
    void clientError(int error);
    void finish(const QString &error);
    QString where() const;
};

#endif // MQTTLISTENER_H
