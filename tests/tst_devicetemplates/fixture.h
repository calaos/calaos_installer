#ifndef DEVICETEMPLATES_FIXTURE_H
#define DEVICETEMPLATES_FIXTURE_H

// A house to create devices in, shared by tst_devicetemplates and tst_dialognewdevice.

#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QStringList>
#include <QTemporaryDir>
#include <QXmlStreamReader>

#include "DeviceTemplates.h"
#include "projectmanager.h"
#include <ListeRoom.h>
#include <ListeRule.h>

namespace Fixture
{

inline QString u(const std::string &s)
{
    return QString::fromUtf8(s.c_str());
}

// Every IO kind an io_filter may ask for, in two rooms; a device whose owned IO
// may be linked too.
inline QByteArray houseXml()
{
    return R"(<?xml version="1.0"?>
<calaos:ioconfig xmlns:calaos="http://www.calaos.fr">
<calaos:home format="2">
<calaos:room name="Salon" type="salon" hits="0">
  <calaos:input id="temp_salon" type="MqttInputTemp" name="Salon temperature" host="h" port="1883" topic_sub="s/t"/>
  <calaos:input id="hum_salon" type="MqttInputAnalog" name="Salon humidity" host="h" port="1883" topic_sub="s/h"/>
  <calaos:input id="switch_salon" type="WIDigitalBP" name="Salon switch" host="h" port="502" var="1"/>
  <calaos:output id="relay_heat" type="WODigital" name="Heating relay" host="h" port="502" var="2"/>
  <calaos:output id="relay_cool" type="WODigital" name="Cooling relay" host="h" port="502" var="3"/>
  <calaos:internal id="flag" type="InternalBool" name="Flag"/>
  <calaos:output id="text_out" type="MqttOutputString" name="Text out" host="h" port="1883" topic_pub="s/x" topic_sub="s/x"/>
</calaos:room>
<calaos:room name="Cave" type="cave" hits="0">
  <calaos:input id="temp_cave" type="WITemp" name="Cave temperature" host="h" port="502" var="4"/>
  <calaos:device id="other" type="climate" impl="proxy" name="Other">
    <calaos:input id="other_temp" type="MqttInputTemp" name="Other temperature" host="h" port="1883" topic_sub="o/t" visible="false"/>
    <calaos:link name="current_temp" io="other_temp"/>
  </calaos:device>
</calaos:room>
</calaos:home>
</calaos:ioconfig>
)";
}

inline void resetModel()
{
    ListeRule::Instance().clear();
    ListeRoom::Instance().clear();
}

inline bool writeFile(const QString &path, const QByteArray &data)
{
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly))
        return false;
    return f.write(data) == data.size();
}

inline QByteArray readFile(const QString &path)
{
    QFile f(path);
    return f.open(QIODevice::ReadOnly) ? f.readAll() : QByteArray();
}

inline bool loadIo(const QString &path, const QByteArray &xml)
{
    resetModel();
    if (!writeFile(path, xml))
        return false;
    QString p = path;
    return ProjectManager::loadIOsFromFile(p);
}

inline QByteArray saveIo(const QString &path)
{
    QString p = path;
    return ProjectManager::saveIOsToFile(p) ? readFile(path) : QByteArray();
}

inline Calaos::Room *roomNamed(const QString &name)
{
    ListeRoom &lr = ListeRoom::Instance();
    for (int i = 0;i < lr.size();i++)
        if (u(lr.get_room(i)->get_name()) == name)
            return lr.get_room(i);
    return nullptr;
}

inline QJsonArray shippedTemplates(const QString &lang = "en")
{
    return DeviceTemplates::load(lang);
}

inline QJsonObject shippedTemplate(const QString &id)
{
    for (const QJsonValue &t: shippedTemplates())
        if (t.toObject()["id"].toString() == id)
            return t.toObject();
    return QJsonObject();
}

//What a user fills in: the defaults, a name, a value in every required text,
//and for each io field the first IO that fits and that no other field took.
inline QVariantMap filled(const QJsonObject &tpl)
{
    QVariantMap v = DeviceTemplates::defaults(tpl, roomNamed("Salon"));
    QStringList taken;
    for (const QJsonValue &e: tpl["fields"].toArray())
    {
        const QJsonObject f = e.toObject();
        const QString name = f["name"].toString();
        const QString type = f["type"].toString();
        if (type == "string" && f["required"].toBool() && v.value(name).toString().isEmpty())
            v[name] = name == "name" ? QString("Chauffage salon") : QString("tpl-%1").arg(name);
        if (type == "io")
        {
            for (Calaos::IOBase *io: DeviceTemplates::candidates(f["io_filter"].toObject()))
            {
                const QString id = u(io->get_param("id"));
                if (!taken.contains(id))
                {
                    taken << id;
                    v[name] = id;
                    break;
                }
            }
        }
    }
    return v;
}

// One line per element of the <calaos:device> of that id, attributes sorted:
// what the file says, whatever order the writer puts attributes in.
inline QStringList deviceElement(const QByteArray &xml, const QString &id)
{
    QStringList lines;
    QXmlStreamReader r(xml);
    int depth = -1;
    while (!r.atEnd())
    {
        r.readNext();
        if (r.isStartElement())
        {
            if (depth < 0 && r.name() == QLatin1String("device") && r.attributes().value("id") == id)
                depth = 0;
            if (depth < 0)
                continue;
            QStringList attrs;
            for (const QXmlStreamAttribute &a: r.attributes())
                attrs << QStringLiteral("%1=%2").arg(a.name().toString(), a.value().toString());
            attrs.sort();
            lines << QString(depth * 2, ' ') + r.name().toString() + " " + attrs.join(' ');
            depth++;
        }
        else if (r.isEndElement() && depth >= 0)
        {
            depth--;
            if (depth == 0)
                break;
        }
    }
    return lines;
}

}

#endif // DEVICETEMPLATES_FIXTURE_H
