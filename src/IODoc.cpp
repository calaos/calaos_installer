#include "IODoc.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>

#include "common/Utils.h"

namespace
{

// Reserved key of io_doc.json: it holds the devices by impl and is not an IO type.
const char *kDevicesKey = "devices";

}

const QJsonObject &IODoc::rootObject()
{
    static QJsonObject root;
    static bool loaded = false;

    if (!loaded)
    {
        loaded = true;

        QString lang = Utils::GetLocale();
        QString rsc = QString(":/doc/%1/io_doc.json").arg(lang);

        QFile f(rsc);
        if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            // Fallback to English version
            f.setFileName(":/doc/en/io_doc.json");
            if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
                return root;
        }

        QJsonParseError jerr;
        QJsonDocument jdoc = QJsonDocument::fromJson(f.readAll(), &jerr);
        if (jerr.error != QJsonParseError::NoError || !jdoc.isObject())
            return root;

        QJsonObject obj = jdoc.object();
        // Insert lowercase keys so lookups are case-insensitive
        for (auto it = obj.begin(); it != obj.end(); it++)
            obj.insert(it.key().toLower(), it.value());

        root = obj;
    }

    return root;
}

QJsonObject IODoc::getIOObject(const QString &type)
{
    const QJsonObject &root = rootObject();
    if (root.isEmpty())
        return QJsonObject();

    QString ltype = type.toLower();
    if (ltype == kDevicesKey)
        return QJsonObject();

    if (root.contains(ltype))
        return root[ltype].toObject();

    // Search in aliases
    for (auto it = root.constBegin(); it != root.constEnd(); it++)
    {
        if (it.key() == kDevicesKey)
            continue;

        QJsonObject o = it.value().toObject();
        QJsonArray jalias = o["alias"].toArray();
        for (int i = 0; i < jalias.size(); i++)
        {
            if (jalias.at(i).toString().toLower() == ltype)
                return o;
        }
    }

    return QJsonObject();
}

QJsonObject IODoc::getDeviceObject(const QString &impl)
{
    const QJsonObject devices = rootObject()[kDevicesKey].toObject();

    const QString limpl = impl.toLower();
    for (auto it = devices.constBegin(); it != devices.constEnd(); it++)
    {
        if (it.key().toLower() == limpl)
            return it.value().toObject();
    }

    return QJsonObject();
}
