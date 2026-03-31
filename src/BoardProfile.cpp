#include "BoardProfile.h"
#include <QCoreApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QSysInfo>

static QString currentOsString()
{
#if defined(Q_OS_LINUX)
    return QStringLiteral("linux");
#elif defined(Q_OS_MACOS)
    return QStringLiteral("macos");
#elif defined(Q_OS_WIN)
    return QStringLiteral("windows");
#else
    return QString();
#endif
}

QString FlashConfig::resolvedToolUrl() const
{
    if (toolGithubRelease)
        return toolUrl;
    return toolUrls.value(currentOsString());
}

bool FlashConfig::hasDirectDownloadUrl() const
{
    return !toolGithubRelease && !resolvedToolUrl().isEmpty();
}

bool BoardProfile::isFlashSupportedOnCurrentOs() const
{
    if (flash.supportedOs.isEmpty())
        return true;

    return flash.supportedOs.contains(currentOsString(), Qt::CaseInsensitive);
}

QList<BoardProfile> BoardProfile::loadFromFile(const QString &path)
{
    QList<BoardProfile> boards;

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning() << "Cannot open boards file:" << path;
        return boards;
    }

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &err);
    if (err.error != QJsonParseError::NoError)
    {
        qWarning() << "JSON parse error in boards file:" << err.errorString();
        return boards;
    }

    QJsonArray arr = doc.object().value("boards").toArray();
    for (const QJsonValue &val : arr)
    {
        QJsonObject obj = val.toObject();
        BoardProfile bp;
        bp.id = obj.value("id").toString();
        bp.name = obj.value("name").toString();
        bp.platform = obj.value("platform").toString();
        bp.configPartitionName = obj.value("config_partition_name").toString();
        bp.firmwareRepo = obj.value("firmware_repo").toString();

        QJsonObject fl = obj.value("flash").toObject();
        bp.flash.tool = fl.value("tool").toString();

        QJsonValue toolUrlVal = fl.value("tool_url");
        if (toolUrlVal.isString())
        {
            bp.flash.toolUrl = toolUrlVal.toString();
        }
        else if (toolUrlVal.isObject())
        {
            QJsonObject urlObj = toolUrlVal.toObject();
            for (auto it = urlObj.begin();it != urlObj.end();++it)
                bp.flash.toolUrls.insert(it.key(), it.value().toString());
        }

        bp.flash.toolGithubRelease = fl.value("tool_github_release").toBool(false);
        bp.flash.requiresSerialPort = fl.value("requires_serial_port").toBool(false);

        QJsonArray osArr = fl.value("supported_os").toArray();
        for (const QJsonValue &osVal : osArr)
            bp.flash.supportedOs.append(osVal.toString());

        bp.flash.unsupportedOsDocUrl = fl.value("unsupported_os_doc_url").toString();

        boards.append(bp);
    }

    return boards;
}

QString BoardProfile::boardsFilePath()
{
    return QCoreApplication::applicationDirPath() + "/boards.json";
}
