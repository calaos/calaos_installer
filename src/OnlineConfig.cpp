#include "OnlineConfig.h"

#include <QCoreApplication>
#include <QHash>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QXmlStreamReader>

namespace OnlineConfig
{

static QJsonObject baseRequest(const QString &user, const QString &pass, const char *type,
                               const QString &adminSession)
{
    QJsonObject r;
    r["cn_user"] = user;
    r["cn_pass"] = pass;
    r["action"] = QStringLiteral("config");
    r["type"] = QString::fromLatin1(type);
    // Body only: haproxy logs URLs.
    if (!adminSession.isEmpty())
        r["admin_session"] = adminSession;
    return r;
}

QByteArray buildGetRequest(const QString &user, const QString &pass, const QString &adminSession)
{
    return QJsonDocument(baseRequest(user, pass, "get", adminSession)).toJson();
}

QByteArray buildPutRequest(const QString &user, const QString &pass,
                           const QString &ioXml, const QString &rulesXml,
                           const QString &adminSession)
{
    QJsonObject r = baseRequest(user, pass, "put", adminSession);
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

static QString refusalText(const QJsonObject &o);

GetResult parseGetResponse(const QByteArray &body)
{
    GetResult res;
    QJsonParseError err;
    const QJsonObject o = QJsonDocument::fromJson(body, &err).object();
    if (err.error != QJsonParseError::NoError)
        return res;
    if (!isSuccess(o))
    {
        res.error = refusalText(o);
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

QByteArray buildUnlockRequest(const QString &user, const QString &pass, const QString &adminPass)
{
    QJsonObject r;
    r["cn_user"] = user;
    r["cn_pass"] = pass;
    r["action"] = QStringLiteral("admin_unlock");
    r["admin_pass"] = adminPass;
    return QJsonDocument(r).toJson();
}

static QJsonObject refusal(const QByteArray &body)
{
    QJsonParseError err;
    const QJsonObject o = QJsonDocument::fromJson(body, &err).object();
    if (err.error != QJsonParseError::NoError || isSuccess(o))
        return QJsonObject();
    return o;
}

QString refusalCode(const QByteArray &body)
{
    return refusal(body).value("error").toString();
}

QString parseUnlockResponse(const QByteArray &body)
{
    QJsonParseError err;
    const QJsonObject o = QJsonDocument::fromJson(body, &err).object();
    if (err.error != QJsonParseError::NoError || !isSuccess(o))
        return QString();
    return o.value("admin_session").toString();
}

static QString translated(const char *text)
{
    return QCoreApplication::translate("OnlineConfig", text);
}

static QString refusalText(const QJsonObject &o)
{
    static const struct
    {
        const char *code;
        const char *text;
    } texts[] = {
        { "bad_session", QT_TRANSLATE_NOOP("OnlineConfig",
          "This server has an admin password. Enter it in the \"Admin password\" field "
          "(again if the previous admin session expired).") },
        { "bad_password", QT_TRANSLATE_NOOP("OnlineConfig", "Wrong admin password.") },
        { "https_required", QT_TRANSLATE_NOOP("OnlineConfig",
          "This server only accepts this request over https. "
          "Use its address alone (or https://address/api.php) instead of http://.") },
        { "admin_not_set", QT_TRANSLATE_NOOP("OnlineConfig",
          "No admin password is set on this server: leave the \"Admin password\" field empty.") },
        { "admin_unavailable", QT_TRANSLATE_NOOP("OnlineConfig",
          "Administration is unavailable (calaos-container cannot be reached; "
          "it is only available on Calaos OS). Try again in a moment.") },
        { "daemon_unreachable", QT_TRANSLATE_NOOP("OnlineConfig",
          "The server cannot reach its system service (calaos-container) to check the admin "
          "password. Try again in a moment.") },
        { "daemon_too_old", QT_TRANSLATE_NOOP("OnlineConfig",
          "Administration is being enabled on this box. Try again in a moment; "
          "restart the box if this lasts more than 2 minutes.") },
        { "upgrade_in_progress", QT_TRANSLATE_NOOP("OnlineConfig",
          "An update is already in progress on this box.") },
        { "household_password_required", QT_TRANSLATE_NOOP("OnlineConfig",
          "Change the household password of this box first.") },
        { "admin_already_set", QT_TRANSLATE_NOOP("OnlineConfig",
          "Someone has just set the admin password of this box.") },
        { "internal", QT_TRANSLATE_NOOP("OnlineConfig",
          "Internal server error. The details are in the calaos_server log.") },
    };

    const QString code = o.value("error").toString();
    for (const auto &t : texts)
        if (code == QLatin1String(t.code))
            return translated(t.text);

    if (code == "rate_limited")
    {
        const QString after = o.value("retry_after").toString();
        return after.isEmpty() ? translated(QT_TRANSLATE_NOOP("OnlineConfig", "Too many attempts, try again later."))
                               : translated(QT_TRANSLATE_NOOP("OnlineConfig", "Too many attempts, try again in %1 s.")).arg(after);
    }
    if (code == "password_too_short")
        return translated(QT_TRANSLATE_NOOP("OnlineConfig", "The admin password must have at least %1 characters."))
            .arg(o.value("min_length").toString());
    // The contract makes these texts safe to show; they say which field or action is wrong.
    if (code == "not_allowed" || code == "invalid_request" || code == "package_not_found")
    {
        const QString msg = o.value("msg").toString();
        return msg.isEmpty() ? code : msg;
    }
    return code;
}

QString refusalText(const QByteArray &body)
{
    return refusalText(refusal(body));
}

QUrl serverUrl(const QString &host)
{
    if (host.startsWith("http"))
        return QUrl(host);
    return QUrl(QString("https://" + host + "/api.php"));
}

QUrl httpsRetryUrl(const QUrl &url, int httpsPort)
{
    QUrl u;
    u.setScheme("https");
    u.setHost(url.host());
    u.setPort(httpsPort);
    u.setPath("/api.php");
    if (u.host().isEmpty() || u == url)
        return QUrl();
    return u;
}

bool shouldRetryOverHttps(const QUrl &url, const QString &code)
{
    if (code == "https_required")
        return true;
    return code == "bad_session" && url.scheme() == "http" && url.port() == 5454;
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
        res.error = refusalText(o);
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
