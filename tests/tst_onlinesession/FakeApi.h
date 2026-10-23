#ifndef FAKEAPI_H
#define FAKEAPI_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <functional>

namespace
{
const QString kSession = QString(64, 'a');

struct Request
{
    QByteArray requestLine;
    QByteArray raw;
    QJsonObject body;
};

// Handler answers that are not JSON: drop the connection, or never answer.
const QJsonObject kAbort{ { "__fake", "abort" } };
const QJsonObject kHang{ { "__fake", "hang" } };

// Minimal /api.php: one request per connection, JSON answers from the test's handler.
class FakeApi: public QObject
{
public:
    using Handler = std::function<QJsonObject(const QJsonObject &)>;

    explicit FakeApi(QTcpServer *server, Handler handler):
        m_server(server), m_handler(std::move(handler))
    {
        connect(m_server, &QTcpServer::pendingConnectionAvailable, this, [this]()
        {
            while (QTcpSocket *s = m_server->nextPendingConnection())
            {
                auto *buffer = new QByteArray;
                connect(s, &QObject::destroyed, s, [buffer]() { delete buffer; });
                connect(s, &QTcpSocket::readyRead, s, [this, s, buffer]() { readSome(s, *buffer); });
                connect(s, &QTcpSocket::disconnected, s, &QObject::deleteLater);
                if (s->bytesAvailable())
                    readSome(s, *buffer);
            }
        });
    }

    bool listen() { return m_server->listen(QHostAddress::LocalHost, 0); }
    quint16 port() const { return m_server->serverPort(); }

    QList<Request> requests;

    int count(const QString &action, const QString &type = QString()) const
    {
        int n = 0;
        for (const Request &r : requests)
            if (r.body.value("action").toString() == action &&
                (type.isEmpty() || r.body.value("type").toString() == type))
                n++;
        return n;
    }

private:
    void readSome(QTcpSocket *s, QByteArray &buffer)
    {
        buffer += s->readAll();
        const int end = buffer.indexOf("\r\n\r\n");
        if (end < 0)
            return;
        const QByteArray head = buffer.left(end);
        int length = 0;
        for (const QByteArray &line : head.split('\n'))
            if (line.toLower().startsWith("content-length:"))
                length = line.mid(15).trimmed().toInt();
        if (buffer.size() < end + 4 + length)
            return;

        Request r;
        r.requestLine = head.left(head.indexOf("\r\n"));
        r.raw = buffer.mid(end + 4, length);
        r.body = QJsonDocument::fromJson(r.raw).object();
        requests << r;

        const QJsonObject reply = m_handler(r.body);
        if (reply == kHang)
            return;
        if (reply == kAbort)
        {
            s->abort();
            return;
        }
        const QByteArray answer = QJsonDocument(reply).toJson(QJsonDocument::Compact);
        s->write("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nConnection: close\r\nContent-Length: " +
                 QByteArray::number(answer.size()) + "\r\n\r\n" + answer);
        s->disconnectFromHost();
    }

    QTcpServer *m_server;
    Handler m_handler;
};

QJsonObject refusal(const QString &code)
{
    return QJsonObject{ { "success", "false" }, { "error", code }, { "msg", "English text" } };
}

QJsonObject configAnswer(const QJsonObject &req)
{
    if (req.value("type").toString() == "put")
        return QJsonObject{ { "success", "true" } };
    return QJsonObject{ { "success", "true" }, { "io_format", 2 },
                        { "config_files", QJsonObject{ { "io.xml", "<io/>" }, { "rules.xml", "<rules/>" } } } };
}

// calaos_server once an admin is set, reached over https.
QJsonObject adminServer(const QJsonObject &req)
{
    const QString action = req.value("action").toString();
    if (action == "admin_unlock")
    {
        if (req.value("admin_pass").toString() != "correct horse battery")
            return refusal("bad_password");
        return QJsonObject{ { "success", "true" }, { "admin_session", kSession }, { "expires_in", "900" } };
    }
    if (req.value("admin_session").toString() != kSession)
        return refusal("bad_session");
    return configAnswer(req);
}
}

#endif // FAKEAPI_H
