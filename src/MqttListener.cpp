#include "MqttListener.h"
#include "MqttClient.h"

#include <QLocale>

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

    //One client for the listener's life: a slot of finished() may open a modal
    //dialog, whose event loop would run a deleteLater() of the client under its feet.
    m_client = new MqttClient(this);
    connect(m_client, &MqttClient::connected, this, &MqttListener::onConnected);
    connect(m_client, &MqttClient::subscribed, this, &MqttListener::subscribed);
    connect(m_client, &MqttClient::errorOccurred, this, &MqttListener::clientError);
    connect(m_client, &MqttClient::messageReceived, this, [this](const QString &topic, const QByteArray &payload)
    {
        if (m_state != IDLE)
            emit message(topic, payload);
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
    m_subscription = 0;

    m_client->setHost(settings.host);
    m_client->setPort(settings.port);
    m_client->setUser(settings.user);
    m_client->setPassword(settings.password);

    m_state = CONNECTING;
    m_timer.start(answerTimeoutMs);
    m_client->connectToHost();
}

void MqttListener::stop()
{
    m_timer.stop();
    m_state = IDLE;
    m_client->disconnectFromHost();
}

MqttListener::~MqttListener()
{
    stop();
}

void MqttListener::onConnected()
{
    if (m_state != CONNECTING)
        return;
    m_state = SUBSCRIBING;
    m_timer.start(m_answerTimeoutMs);
    emit connected();
    if (m_state != SUBSCRIBING)
        return;
    for (const QString &f: std::as_const(m_filters))
        if (!MqttPacket::isValidFilter(f))
        {
            finish(tr("%1 is not a valid MQTT topic filter.").arg(f));
            return;
        }
    m_subscription = m_client->subscribe(m_filters);
    if (!m_subscription)
        finish(tr("The broker at %1 does not let this user read those topics.").arg(where()));
}

void MqttListener::subscribed(quint16 id, const QList<bool> &granted)
{
    if (m_state != SUBSCRIBING || id != m_subscription)
        return;
    if (!granted.contains(true))
    {
        finish(tr("The broker at %1 does not let this user read those topics.").arg(where()));
        return;
    }
    m_state = LISTENING;
    if (m_listenMs > 0)
        m_timer.start(m_listenMs);
    else
        m_timer.stop();
    emit listening();
}

void MqttListener::clientError()
{
    if (m_state == IDLE)
        return;

    switch (m_client->error())
    {
    case MqttClient::CONNECTION_REFUSED:
        switch (m_client->connackCode())
        {
        case 1:
            finish(tr("The broker at %1 does not speak MQTT 3.1.1.").arg(where()));
            return;
        case 4:
            finish(tr("The broker at %1 refused the user name or the password.").arg(where()));
            return;
        case 5:
            finish(tr("The broker at %1 does not let this user in: check the user and the password.").arg(where()));
            return;
        default:
            finish(tr("The broker at %1 refused the connection (code %2).").arg(where()).arg(m_client->connackCode()));
            return;
        }
    case MqttClient::PROTOCOL_VIOLATION:
        finish(tr("The broker at %1 sent data that is not MQTT.").arg(where()));
        return;
    case MqttClient::PACKET_TOO_LARGE:
        finish(tr("The broker at %1 sent a message too large to be read.").arg(where()));
        return;
    case MqttClient::PING_TIMED_OUT:
    case MqttClient::CONNECT_TIMED_OUT:
        finish(tr("The broker at %1 stopped answering.").arg(where()));
        return;
    case MqttClient::SOCKET_FAILED:
    case MqttClient::NO_CLIENT_ERROR:
        break;
    }

    switch (m_client->socketError())
    {
    case QAbstractSocket::HostNotFoundError:
        finish(tr("The broker %1 was not found: check its address.").arg(m_settings.host));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        finish(tr("Nothing answers on %1: check that the broker runs and listens on this port.").arg(where()));
        break;
    case QAbstractSocket::RemoteHostClosedError:
        if (m_client->failedBeforeConnack())
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
