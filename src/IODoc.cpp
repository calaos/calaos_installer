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

        // Insert lowercase keys so lookups are case-insensitive. Iterate a copy:
        // inserting into the object being walked invalidates its iterators.
        const QJsonObject file = jdoc.object();
        QJsonObject obj = file;
        for (auto it = file.constBegin(); it != file.constEnd(); it++)
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

QJsonObject IODoc::getLinkRole(const QString &impl, const QString &role)
{
    for (const QJsonValue &v: getDeviceObject(impl)["link_roles"].toArray())
        if (v.toObject()["role"].toString() == role)
            return v.toObject();
    return QJsonObject();
}

QJsonObject IODoc::getDeviceParam(const QString &impl, const QString &name)
{
    for (const QJsonValue &v: getDeviceObject(impl)["parameters"].toArray())
        if (v.toObject()["name"].toString() == name)
            return v.toObject();
    return QJsonObject();
}

QString IODoc::labelOf(const QJsonObject &entry, const QString &id)
{
    const QString label = entry["label"].toString();
    if (!label.isEmpty())
        return label;
    QString made = id;
    made.replace('_', ' ');
    if (!made.isEmpty())
        made[0] = made[0].toUpper();
    return made;
}

QString IODoc::valueTypeOf(const QJsonObject &linkRole)
{
    const QString t = linkRole["value_type"].toString();
    return (t == "number" || t == "bool" || t == "string") ? t : QString();
}
