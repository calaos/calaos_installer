#ifndef MQTTLISTENER_H
#define MQTTLISTENER_H

#include <QByteArray>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QTimer>

class MqttClient;

// Listens to an MQTT broker for a few seconds or until stopped: connect, subscribe, collect what
// is published (retained messages first), disconnect. Says in words why it failed.
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
    ~MqttListener() override;

    //listenMs <= 0 listens until stop().
    void start(const Settings &settings, const QStringList &filters, int listenMs = 3000,
               int answerTimeoutMs = 5000);
    //Ends the listening now, without finished().
    void stop();
    bool isRunning() const { return m_state != IDLE; }

signals:
    //CONNACK accepted the user: subscriptions follow.
    void connected();
    //The broker took the subscriptions: messages follow.
    void listening();
    void message(const QString &topic, const QByteArray &payload);
    //error is empty when the listener ran for its whole time.
    void finished(const QString &error);

private:
    enum State { IDLE, CONNECTING, SUBSCRIBING, LISTENING };

    MqttClient *m_client;
    QTimer m_timer;
    State m_state = IDLE;
    Settings m_settings;
    QStringList m_filters;
    int m_listenMs = 0;
    int m_answerTimeoutMs = 0;
    quint16 m_subscription = 0;

    void onConnected();
    void subscribed(quint16 id, const QList<bool> &granted);
    void clientError();
    void finish(const QString &error);
    QString where() const;
};

#endif // MQTTLISTENER_H
