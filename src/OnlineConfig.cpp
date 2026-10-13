#include "OnlineConfig.h"

#include <QCoreApplication>
#include <QHash>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QXmlStreamReader>

namespace OnlineConfig
{

static QJsonObject baseRequest(const QString &user, const QString &pass, const char *type)
{
    QJsonObject r;
    r["cn_user"] = user;
    r["cn_pass"] = pass;
    r["action"] = QStringLiteral("config");
    r["type"] = QString::fromLatin1(type);
    return r;
}

QByteArray buildGetRequest(const QString &user, const QString &pass)
{
    return QJsonDocument(baseRequest(user, pass, "get")).toJson();
}

QByteArray buildPutRequest(const QString &user, const QString &pass,
                           const QString &ioXml, const QString &rulesXml)
{
    QJsonObject r = baseRequest(user, pass, "put");
    QJsonObject files;
    files["io.xml"] = ioXml;
    files["rules.xml"] = rulesXml;
    r["config_files"] = files;
    // The server cannot tell from the file alone that the last device was deleted on purpose.
    r["io_format"] = kPutIoFormat;
    return QJsonDocument(r).toJson();
}

static bool isSuccess(const QJsonObject &o)
{
    // Servers answer with the string "true"; accept a real boolean too.
    const QJsonValue v = o.value("success");
    return v.isBool() ? v.toBool() : v.toString() == "true";
}

GetResult parseGetResponse(const QByteArray &body)
{
    GetResult res;
    QJsonParseError err;
    const QJsonObject o = QJsonDocument::fromJson(body, &err).object();
    if (err.error != QJsonParseError::NoError)
        return res;
    if (!isSuccess(o))
    {
        res.error = o.value("error").toString();
        return res;
    }

    res.ok = true;
    const QJsonObject files = o.value("config_files").toObject();
    res.hasFiles = files.contains("io.xml") && files.contains("rules.xml");
    res.ioXml = files.value("io.xml").toString();
    res.rulesXml = files.value("rules.xml").toString();

    // Only a whole number counts: 2.5 or "2" must not unlock devices.
    const QJsonValue f = o.value("io_format");
    if (f.isDouble())
    {
        const double d = f.toDouble();
        if (d >= 1 && d <= 1000 && d == static_cast<int>(d))
            res.ioFormat = static_cast<int>(d);
    }
    return res;
}

PutResult parsePutResponse(const QByteArray &body)
{
    PutResult res;
    QJsonParseError err;
    const QJsonObject o = QJsonDocument::fromJson(body, &err).object();
    if (err.error != QJsonParseError::NoError)
        return res;

    res.ok = isSuccess(o);
    if (!res.ok)
        res.error = o.value("error").toString();
    return res;
}

static QHash<QString, int> &formats()
{
    static QHash<QString, int> h;
    return h;
}

void rememberServerFormat(const QString &host, int format)
{
    formats().insert(host, format);
}

int serverFormat(const QString &host)
{
    return formats().value(host, kFormatUnknown);
}

bool serverAcceptsDevices(int format)
{
    return format >= kDevicesMinFormat;
}

static void scanDevices(const QString &ioXml, QStringList &ids, int &idless)
{
    QXmlStreamReader xml(ioXml);
    while (!xml.atEnd())
    {
        if (xml.readNext() == QXmlStreamReader::StartElement &&
            xml.qualifiedName() == QLatin1String("calaos:device"))
        {
            const QString id = xml.attributes().value("id").toString();
            if (id.isEmpty())
                idless++;
            else if (!ids.contains(id))
                ids << id;
        }
    }
}

QStringList deviceIds(const QString &ioXml)
{
    QStringList ids;
    int idless = 0;
    scanDevices(ioXml, ids, idless);
    return ids;
}

QStringList devicesLostByPut(const QString &serverIoXml, const QString &sentIoXml)
{
    QStringList serverIds, sentIds;
    int serverIdless = 0, sentIdless = 0;
    scanDevices(serverIoXml, serverIds, serverIdless);
    scanDevices(sentIoXml, sentIds, sentIdless);

    QStringList lost;
    for (const QString &id : serverIds)
        if (!sentIds.contains(id))
            lost << id;

    // Without an id they cannot be matched one by one; only the count can shrink.
    for (int i = sentIdless; i < serverIdless; i++)
        lost << QCoreApplication::translate("OnlineConfig", "(device without id)");
    return lost;
}

}
