#include "MqttListener.h"

#include <QAbstractSocket>
#include <QLocale>
#include <QtMqtt/QMqttClient>
#include <QtMqtt/QMqttSubscription>
#include <QRandomGenerator>

MqttListener::MqttListener(QObject *parent):
    QObject(parent)
{
    m_timer.setSingleShot(true);
    connect(&m_timer, &QTimer::timeout, this, [this]()
    {
        if (m_state == LISTENING)
            finish(QString());
        else
            finish(tr("No answer from the broker at %1 within %2 s.").arg(where(), QLocale().toString(m_answerTimeoutMs / 1000.0)));
    });
}

QString MqttListener::where() const
{
    return QStringLiteral("%1:%2").arg(m_settings.host).arg(m_settings.port);
}

void MqttListener::start(const Settings &settings, const QStringList &filters, int listenMs, int answerTimeoutMs)
{
    stop();
    m_settings = settings;
    m_filters = filters;
    m_listenMs = listenMs;
    m_answerTimeoutMs = answerTimeoutMs;
    m_pending = m_granted = 0;

    //A client per run: QMqttClient keeps its transport from one connection to the next.
    m_client = new QMqttClient(this);
    m_client->setProtocolVersion(QMqttClient::MQTT_3_1_1);
    m_client->setHostname(settings.host);
    m_client->setPort(settings.port);
    m_client->setCleanSession(true);
    m_client->setKeepAlive(60);
    m_client->setClientId(QStringLiteral("calaos-installer-%1")
                          .arg(QRandomGenerator::global()->generate(), 8, 16, QLatin1Char('0')));
    //3.1.1 allows a password only with a user name.
    if (!settings.user.isEmpty())
    {
        m_client->setUsername(settings.user);
        m_client->setPassword(settings.password);
    }

    connect(m_client, &QMqttClient::connected, this, &MqttListener::connected);
    connect(m_client, &QMqttClient::errorChanged, this, [this](QMqttClient::ClientError e)
    {
        clientError(int(e));
    });
    connect(m_client, &QMqttClient::disconnected, this, [this]()
    {
        if (m_state != IDLE)
            clientError(int(m_client->error()));
    });
    connect(m_client, &QMqttClient::messageReceived, this, [this](const QByteArray &payload, const QMqttTopicName &topic)
    {
        emit message(topic.name(), payload);
    });

    m_state = CONNECTING;
    m_timer.start(answerTimeoutMs);
    m_client->connectToHost();
}

void MqttListener::stop()
{
    m_timer.stop();
    m_state = IDLE;
    if (m_client)
    {
        m_client->disconnect(this);
        m_client->disconnectFromHost();
        m_client->deleteLater();
        m_client = nullptr;
    }
}

void MqttListener::connected()
{
    if (m_state != CONNECTING)
        return;
    m_state = SUBSCRIBING;
    m_timer.start(m_answerTimeoutMs);
    m_pending = m_filters.size();
    for (const QString &f: std::as_const(m_filters))
    {
        QMqttSubscription *sub = m_client->subscribe(QMqttTopicFilter(f), 0);
        if (!sub)
        {
            subscriptionSettled(false);
            continue;
        }
        connect(sub, &QMqttSubscription::stateChanged, this, [this](QMqttSubscription::SubscriptionState s)
        {
            if (s == QMqttSubscription::Subscribed)
                subscriptionSettled(true);
            else if (s == QMqttSubscription::Error)
                subscriptionSettled(false);
        });
    }
}

void MqttListener::subscriptionSettled(bool granted)
{
    if (m_state != SUBSCRIBING)
        return;
    m_granted += granted ? 1 : 0;
    if (--m_pending > 0)
        return;
    if (m_granted == 0)
    {
        finish(tr("The broker at %1 does not let this user read those topics.").arg(where()));
        return;
    }
    m_state = LISTENING;
    m_timer.start(m_listenMs);
    emit listening();
}

void MqttListener::clientError(int error)
{
    if (m_state == IDLE)
        return;

    switch (QMqttClient::ClientError(error))
    {
    case QMqttClient::NoError:
        finish(tr("The broker at %1 closed the connection.").arg(where()));
        return;
    case QMqttClient::InvalidProtocolVersion:
        finish(tr("The broker at %1 does not speak MQTT 3.1.1.").arg(where()));
        return;
    case QMqttClient::BadUsernameOrPassword:
        finish(tr("The broker at %1 refused the user name or the password.").arg(where()));
        return;
    case QMqttClient::NotAuthorized:
        finish(tr("The broker at %1 does not let this user in: check the user and the password.").arg(where()));
        return;
    case QMqttClient::IdRejected:
    case QMqttClient::ServerUnavailable:
        finish(tr("The broker at %1 refused the connection (code %2).").arg(where()).arg(error));
        return;
    case QMqttClient::ProtocolViolation:
        finish(tr("The broker at %1 sent data that is not MQTT.").arg(where()));
        return;
    case QMqttClient::TransportInvalid:
        break;
    default:
        finish(tr("Could not reach the broker at %1 (error %2).").arg(where()).arg(error));
        return;
    }

    //QMqttClient handles the socket error before anyone else: ask the socket.
    QAbstractSocket *socket = qobject_cast<QAbstractSocket *>(m_client->transport());
    switch (socket ? socket->error() : QAbstractSocket::UnknownSocketError)
    {
    case QAbstractSocket::HostNotFoundError:
        finish(tr("The broker %1 was not found: check its address.").arg(m_settings.host));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        finish(tr("Nothing answers on %1: check that the broker runs and listens on this port.").arg(where()));
        break;
    case QAbstractSocket::RemoteHostClosedError:
        if (m_state == CONNECTING)
            finish(tr("The broker at %1 closed the connection: check the user and the password.").arg(where()));
        else
            finish(tr("The broker at %1 closed the connection.").arg(where()));
        break;
    default:
        finish(tr("Could not reach the broker at %1.").arg(where()));
        break;
    }
}

void MqttListener::finish(const QString &error)
{
    if (m_state == IDLE)
        return;
    stop();
    emit finished(error);
}
