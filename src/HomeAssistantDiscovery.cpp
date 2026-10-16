#include "HomeAssistantDiscovery.h"
#include "EsphomeDiscovery.h"
#include "IODoc.h"

#include <ListeRoom.h>
#include <QHash>
#include <QJsonDocument>
#include <QRegularExpression>
#include <algorithm>
#include <cmath>

using namespace Calaos;

namespace
{

QString u(const std::string &s)
{
    return QString::fromUtf8(s.c_str());
}

std::string s8(const QString &s)
{
    return s.toUtf8().toStdString();
}

using Table = QHash<QString, QString>;

//The abbreviations of Home Assistant this import reads; others are kept as they are.
const Table &entityAbbreviations()
{
    static const Table t = {
        { "act_t", "action_topic" }, { "act_tpl", "action_template" },
        { "avty", "availability" }, { "avty_mode", "availability_mode" },
        { "avty_t", "availability_topic" }, { "avty_tpl", "availability_template" },
        { "bri_cmd_t", "brightness_command_topic" }, { "bri_cmd_tpl", "brightness_command_template" },
        { "bri_scl", "brightness_scale" }, { "bri_stat_t", "brightness_state_topic" },
        { "bri_val_tpl", "brightness_value_template" },
        { "clrm", "color_mode" }, { "cmd_t", "command_topic" }, { "cmd_tpl", "command_template" },
        { "cmps", "components" },
        { "curr_temp_t", "current_temperature_topic" }, { "curr_temp_tpl", "current_temperature_template" },
        { "dev", "device" }, { "dev_cla", "device_class" }, { "ent_cat", "entity_category" },
        { "fan_mode_cmd_t", "fan_mode_command_topic" }, { "fan_mode_cmd_tpl", "fan_mode_command_template" },
        { "fan_mode_stat_t", "fan_mode_state_topic" }, { "fan_mode_stat_tpl", "fan_mode_state_template" },
        { "hs_cmd_t", "hs_command_topic" },
        { "mode_cmd_t", "mode_command_topic" }, { "mode_cmd_tpl", "mode_command_template" },
        { "mode_stat_t", "mode_state_topic" }, { "mode_stat_tpl", "mode_state_template" },
        { "o", "origin" }, { "obj_id", "object_id" }, { "opt", "optimistic" }, { "p", "platform" },
        { "pl_avail", "payload_available" }, { "pl_not_avail", "payload_not_available" },
        { "pl_cls", "payload_close" }, { "pl_open", "payload_open" }, { "pl_stop", "payload_stop" },
        { "pl_on", "payload_on" }, { "pl_off", "payload_off" },
        { "pos_t", "position_topic" }, { "set_pos_t", "set_position_topic" },
        { "rgb_cmd_t", "rgb_command_topic" },
        { "stat_cla", "state_class" }, { "stat_clsd", "state_closed" }, { "stat_open", "state_open" },
        { "stat_on", "state_on" }, { "stat_off", "state_off" },
        { "stat_t", "state_topic" }, { "stat_tpl", "state_template" }, { "stat_val_tpl", "state_value_template" },
        { "sug_dsp_prc", "suggested_display_precision" }, { "sup_clrm", "supported_color_modes" },
        { "swing_mode_cmd_t", "swing_mode_command_topic" }, { "swing_mode_cmd_tpl", "swing_mode_command_template" },
        { "swing_mode_stat_t", "swing_mode_state_topic" }, { "swing_mode_stat_tpl", "swing_mode_state_template" },
        { "t", "topic" },
        { "temp_cmd_t", "temperature_command_topic" }, { "temp_cmd_tpl", "temperature_command_template" },
        { "temp_stat_t", "temperature_state_topic" }, { "temp_stat_tpl", "temperature_state_template" },
        { "temp_hi_cmd_t", "temperature_high_command_topic" }, { "temp_hi_cmd_tpl", "temperature_high_command_template" },
        { "temp_hi_stat_t", "temperature_high_state_topic" }, { "temp_hi_stat_tpl", "temperature_high_state_template" },
        { "temp_lo_cmd_t", "temperature_low_command_topic" }, { "temp_lo_cmd_tpl", "temperature_low_command_template" },
        { "temp_lo_stat_t", "temperature_low_state_topic" }, { "temp_lo_stat_tpl", "temperature_low_state_template" },
        { "temp_unit", "temperature_unit" },
        { "uniq_id", "unique_id" }, { "unit_of_meas", "unit_of_measurement" }, { "val_tpl", "value_template" },
        { "xy_cmd_t", "xy_command_topic" },
    };
    return t;
}

const Table &deviceAbbreviations()
{
    static const Table t = {
        { "cns", "connections" }, { "ids", "identifiers" }, { "mf", "manufacturer" }, { "mdl", "model" },
        { "mdl_id", "model_id" }, { "hw", "hw_version" }, { "sw", "sw_version" }, { "sa", "suggested_area" },
        { "sn", "serial_number" }, { "cu", "configuration_url" }, { "url", "support_url" },
    };
    return t;
}

QJsonObject expandWith(const QJsonObject &in, const Table &table)
{
    QJsonObject out;
    for (auto it = in.begin();it != in.end();++it)
        out.insert(table.value(it.key(), it.key()), it.value());
    return out;
}

//"~" stands for the base topic at either end of a topic.
QString withBase(const QString &topic, const QString &base)
{
    if (base.isEmpty())
        return topic;
    if (topic.startsWith('~'))
        return base + topic.mid(1);
    if (topic.endsWith('~'))
        return topic.chopped(1) + base;
    return topic;
}

//Home Assistant takes numbers and booleans for its payloads, as their Python text.
QString text(const QJsonValue &v, const QString &def = QString())
{
    if (v.isString())
        return v.toString();
    if (v.isDouble())
    {
        const double d = v.toDouble();
        return d == std::floor(d) && std::fabs(d) < 1e15 ? QString::number(qint64(d)) : QString::number(d, 'g', 15);
    }
    if (v.isBool())
        return v.toBool() ? "True" : "False";
    return def;
}

double number(const QJsonValue &v, double def)
{
    if (v.isDouble())
        return v.toDouble();
    bool ok = false;
    const double d = v.toString().toDouble(&ok);
    return ok ? d : def;
}

QString valueAt(const QByteArray &payload, const QString &path)
{
    if (path.isEmpty())
        return QString::fromUtf8(payload).trimmed();
    QJsonValue v = QJsonDocument::fromJson(payload).object();
    for (const QString &part: path.split('/'))
    {
        if (part.startsWith('[') && part.endsWith(']'))
            v = v.toArray().at(part.mid(1, part.size() - 2).toInt());
        else
            v = v.toObject().value(part);
    }
    return text(v);
}

struct Avail
{
    QString topic, available, notAvailable, path;
    bool pathOk = true;
};

QList<Avail> availabilities(const QJsonObject &c)
{
    QList<Avail> l;
    if (c["availability_topic"].isString())
    {
        Avail a;
        a.topic = c["availability_topic"].toString();
        a.available = text(c["payload_available"], "online");
        a.notAvailable = text(c["payload_not_available"], "offline");
        a.path = HomeAssistantDiscovery::pathOf(c["availability_template"].toString(), &a.pathOk);
        l << a;
    }
    for (const QJsonValue &v: c["availability"].toArray())
    {
        const QJsonObject o = v.toObject();
        Avail a;
        a.topic = o["topic"].toString();
        a.available = text(o["payload_available"], "online");
        a.notAvailable = text(o["payload_not_available"], "offline");
        a.path = HomeAssistantDiscovery::pathOf(o["value_template"].toString(), &a.pathOk);
        if (!a.topic.isEmpty())
            l << a;
    }
    return l;
}

void addBroker(Params &p, const MqttBroker &b)
{
    p.Add("host", s8(b.host));
    p.Add("port", s8(QString::number(b.port)));
    //As the templates do: no empty credentials.
    if (!b.user.isEmpty())
        p.Add("user", s8(b.user));
    if (!b.password.isEmpty())
        p.Add("password", s8(b.password));
}

//The connected status of the IO from the availability Home Assistant reads.
void setConnectedStatus(Params &p, const QJsonObject &c)
{
    p.Delete("connected_status_topic");
    p.Delete("connected_status_path");
    p.Delete("connected_status_expr");
    //Zigbee2MQTT lists its bridge first and the device last: the device is the one.
    const QList<Avail> l = availabilities(c);
    if (l.isEmpty() || !l.last().pathOk || l.last().available.contains('\''))
        return;
    p.Add("connected_status_topic", s8(l.last().topic));
    if (!l.last().path.isEmpty())
        p.Add("connected_status_path", s8(l.last().path));
    p.Add("connected_status_expr", s8(QStringLiteral("value == '%1'").arg(l.last().available)));
}

QString styleFor(const QString &type, const QString &wanted)
{
    if (wanted.isEmpty())
        return QString();
    for (const QJsonValue &v: IODoc::getIOObject(type)["parameters"].toArray())
        if (v.toObject()["name"].toString() == "io_style")
            return v.toObject()["list_value"].toObject().contains(wanted) ? wanted : QString();
    return QString();
}

struct ClimateIo
{
    const char *key; //of the template's IO
    const char *what; //ESPHome's name, as EsphomeDiscovery::Climate::topics has it
    const char *command;
    const char *commandTemplate;
    const char *state;
    const char *stateTemplate;
};

const QList<ClimateIo> &climateIos()
{
    static const QList<ClimateIo> l = {
        { "mode", "mode", "mode_command_topic", "mode_command_template", "mode_state_topic", "mode_state_template" },
        { "current_temp", "current_temperature", nullptr, nullptr, "current_temperature_topic", "current_temperature_template" },
        { "target_temp", "target_temperature", "temperature_command_topic", "temperature_command_template",
          "temperature_state_topic", "temperature_state_template" },
        { "target_temp_low", "target_temperature_low", "temperature_low_command_topic", "temperature_low_command_template",
          "temperature_low_state_topic", "temperature_low_state_template" },
        { "target_temp_high", "target_temperature_high", "temperature_high_command_topic", "temperature_high_command_template",
          "temperature_high_state_topic", "temperature_high_state_template" },
        { "action", "action", nullptr, nullptr, "action_topic", "action_template" },
        { "fan_mode", "fan_mode", "fan_mode_command_topic", "fan_mode_command_template",
          "fan_mode_state_topic", "fan_mode_state_template" },
        { "swing_mode", "swing_mode", "swing_mode_command_topic", "swing_mode_command_template",
          "swing_mode_state_topic", "swing_mode_state_template" },
    };
    return l;
}

QJsonObject templateById(const QJsonArray &templates, const QString &id)
{
    for (const QJsonValue &t: templates)
        if (t.toObject()["id"].toString() == id)
            return t.toObject();
    return QJsonObject();
}

QStringList lowerList(const QJsonValue &v, const QStringList &def)
{
    if (!v.isArray())
        return def;
    QStringList l;
    for (const QJsonValue &e: v.toArray())
        l << text(e).toLower();
    return l;
}

//The template a climate fills, and the climate as the ESPHome discovery would have read it.
bool climateShape(const HomeAssistantDiscovery::Entity &e, const QJsonArray &templates, QJsonObject *tpl,
                  EsphomeDiscovery::Climate *c, QString *reason, QStringList *notes)
{
    const QJsonObject &cfg = e.config;
    auto has = [&](const char *k) { return k && !cfg[k].toString().isEmpty(); };

    if (cfg["temperature_unit"].toString().toUpper() == "F")
    {
        *reason = HomeAssistantDiscovery::tr("its temperatures are in °F, Calaos climates in °C");
        return false;
    }

    QSet<QString> present;
    for (const ClimateIo &io: climateIos())
    {
        if (!(io.command ? has(io.command) : has(io.state)))
            continue;
        const QString what = io.what;
        bool ok = true;
        HomeAssistantDiscovery::pathOf(cfg[io.stateTemplate].toString(), &ok);
        if (ok && io.commandTemplate)
            HomeAssistantDiscovery::dataOf(cfg[io.commandTemplate].toString(), &ok);
        const bool optional = what == "fan_mode" || what == "swing_mode" || what == "action";
        if (!ok || !has(io.state))
        {
            const QString why = !ok ? HomeAssistantDiscovery::tr("its %1 goes through a template Calaos cannot follow").arg(what)
                                    : HomeAssistantDiscovery::tr("it does not report its %1").arg(what);
            if (!optional)
            {
                *reason = why;
                return false;
            }
            if (notes)
                *notes << HomeAssistantDiscovery::tr("Not imported: %1 (%2).").arg(what, why);
            continue;
        }
        present.insert(what);
    }

    if (!present.contains("mode"))
    {
        *reason = HomeAssistantDiscovery::tr("it has no mode topics");
        return false;
    }
    const bool dual = present.contains("target_temperature_low") && present.contains("target_temperature_high");
    if (dual)
        present.remove("target_temperature");
    else
    {
        present.remove("target_temperature_low");
        present.remove("target_temperature_high");
    }
    if (!dual && !present.contains("target_temperature"))
    {
        *reason = HomeAssistantDiscovery::tr("it has no setpoint topics");
        return false;
    }

    const bool withSensor = present.contains("current_temperature");
    if (!withSensor && dual)
    {
        *reason = HomeAssistantDiscovery::tr("two setpoints without a current temperature: no template takes that");
        return false;
    }
    const QString id = withSensor ? "esphome_climate" : "esphome_climate_ir";
    *tpl = templateById(templates, id);
    if (tpl->isEmpty())
    {
        *reason = HomeAssistantDiscovery::tr("the device template %1 is missing from this installer").arg(id);
        return false;
    }
    if (!withSensor && present.contains("action") && notes)
        *notes << HomeAssistantDiscovery::tr("Not imported: action (the template without room temperature has none).");

    //ESPHome's layout gives its prefix and object id; any other layout gets its node.
    const QStringList s = cfg["mode_command_topic"].toString().split('/');
    const int n = s.size();
    if (n >= 5 && s[n - 4] == "climate" && s[n - 2] == "mode" && s[n - 1] == "command")
    {
        c->prefix = s.mid(0, n - 4).join('/');
        c->objectId = s[n - 3];
    }
    else
    {
        c->prefix = e.nodeId.isEmpty() ? e.deviceKey : e.nodeId;
        c->objectId = e.objectId;
    }
    c->name = e.ownName();
    c->configured = true;
    c->topics = present;
    //Home Assistant's defaults when the config lists nothing.
    c->modes = lowerList(cfg["modes"], { "auto", "off", "cool", "heat", "dry", "fan_only" });
    c->fanModes = lowerList(cfg["fan_modes"], { "auto", "low", "medium", "high" });
    c->swingModes = lowerList(cfg["swing_modes"], { "on", "off" });
    c->tempMin = number(cfg["min_temp"], 7);
    c->tempMax = number(cfg["max_temp"], 35);
    c->tempStep = number(cfg["temp_step"], 1);
    return true;
}

}

QString HomeAssistantDiscovery::Entity::displayName() const
{
    if (name.isEmpty())
        return deviceName.isEmpty() ? objectId : deviceName;
    if (deviceName.isEmpty() || name.startsWith(deviceName, Qt::CaseInsensitive))
        return name;
    return deviceName + " " + name;
}

QString HomeAssistantDiscovery::Entity::ownName() const
{
    if (!name.isEmpty())
        return name;
    return deviceName.isEmpty() ? objectId : deviceName;
}

HomeAssistantDiscovery::HomeAssistantDiscovery(const QString &prefix):
    m_prefix(normalizedPrefix(prefix))
{
}

QString HomeAssistantDiscovery::normalizedPrefix(const QString &prefix)
{
    QString p = prefix.trimmed();
    while (p.endsWith('/'))
        p.chop(1);
    return p.isEmpty() ? QStringLiteral("homeassistant") : p;
}

QStringList HomeAssistantDiscovery::filters() const
{
    //A broker that refuses # to this user still gives the configs.
    return { m_prefix + "/#", "#" };
}

void HomeAssistantDiscovery::clear()
{
    m_entities.clear();
    m_retained.clear();
    m_unreadable.clear();
}

QJsonObject HomeAssistantDiscovery::expand(const QJsonObject &config)
{
    //value(), not []: a [] read on a non-const object inserts a null key.
    QJsonObject c = expandWith(config, entityAbbreviations());
    if (c.value("device").isObject())
        c["device"] = expandWith(c.value("device").toObject(), deviceAbbreviations());
    if (c.value("origin").isObject())
        c["origin"] = expandWith(c.value("origin").toObject(), deviceAbbreviations());

    const QString base = c.value("~").toString();
    for (auto it = c.begin();it != c.end();++it)
        if (it.key().endsWith("_topic") && it.value().isString())
            it.value() = withBase(it.value().toString(), base);

    if (c.value("availability").isArray())
    {
        QJsonArray list;
        for (const QJsonValue &v: c.value("availability").toArray())
        {
            QJsonObject a = expandWith(v.toObject(), entityAbbreviations());
            if (a.value("topic").isString())
                a["topic"] = withBase(a.value("topic").toString(), base);
            list << a;
        }
        c["availability"] = list;
    }
    return c;
}

QString HomeAssistantDiscovery::pathOf(const QString &valueTemplate, bool *ok)
{
    *ok = true;
    const QString t = valueTemplate.trimmed();
    if (t.isEmpty())
        return QString();

    //value_json and its accessors, then filters that do not change what is read.
    static const QRegularExpression rx(
        R"(^\{\{\s*(value_json((?:\s*\.\s*[A-Za-z_][A-Za-z0-9_]*|\s*\[\s*(?:'[^']*'|"[^"]*"|\d+)\s*\])+)|value)\s*)"
        R"(((?:\|\s*(?:float|int|string|round)(?:\s*\([^()]*\))?\s*)*)\}\}$)");
    const QRegularExpressionMatch m = rx.match(t);
    if (!m.hasMatch())
    {
        *ok = false;
        return QString();
    }
    static const QRegularExpression part(R"rx(\.\s*([A-Za-z_][A-Za-z0-9_]*)|\[\s*(?:'([^']*)'|"([^"]*)"|(\d+))\s*\])rx");
    QStringList parts;
    auto it = part.globalMatch(m.captured(2));
    while (it.hasNext())
    {
        const QRegularExpressionMatch p = it.next();
        if (!p.captured(1).isEmpty())
            parts << p.captured(1);
        else if (!p.captured(4).isEmpty())
            parts << "[" + p.captured(4) + "]";
        else
            parts << p.captured(2) + p.captured(3);
    }
    return parts.join('/');
}

QString HomeAssistantDiscovery::dataOf(const QString &commandTemplate, bool *ok)
{
    *ok = true;
    if (commandTemplate.trimmed().isEmpty())
        return "__##VALUE##__";
    if (commandTemplate.contains("{%") || commandTemplate.contains("{#"))
    {
        *ok = false;
        return QString();
    }
    static const QRegularExpression rx(R"(\{\{\s*value\s*(?:\|\s*(?:float|int|string|round)(?:\s*\([^()]*\))?\s*)*\}\})");
    QString data = commandTemplate;
    data.replace(rx, "__##VALUE##__");
    if (data.contains("{{"))
    {
        *ok = false;
        return QString();
    }
    return data;
}

void HomeAssistantDiscovery::feed(const QString &topic, const QByteArray &payload)
{
    QStringList parts;
    if (topic.startsWith(m_prefix + "/") && topic.endsWith("/config"))
        parts = topic.mid(m_prefix.size() + 1).split('/');
    if (parts.size() != 3 && parts.size() != 4)
    {
        //Availability and the like, read by the configs.
        if (payload.isEmpty() || payload.size() > 1024)
            m_retained.remove(topic);
        else
            m_retained[topic] = payload;
        return;
    }

    //A device config holds several entities, which go with it.
    for (auto it = m_entities.begin();it != m_entities.end();)
    {
        if (it.key() == topic || it.key().startsWith(topic + "#"))
            it = m_entities.erase(it);
        else
            ++it;
    }
    m_unreadable.remove(topic);
    //An empty retained config is a deleted entity.
    if (payload.trimmed().isEmpty())
        return;

    const QJsonDocument doc = QJsonDocument::fromJson(payload);
    if (!doc.isObject())
    {
        m_unreadable.insert(topic);
        return;
    }

    const QJsonObject config = expand(doc.object());
    const QString component = parts[0];
    const QString nodeId = parts.size() == 4 ? parts[1] : QString();
    const QString objectId = parts[parts.size() - 2];

    auto add = [&](const QString &key, const QString &comp, const QString &object, const QJsonObject &c)
    {
        Entity e;
        e.topic = key;
        e.component = comp;
        e.nodeId = nodeId;
        e.objectId = object;
        e.config = c;
        //A null name: the entity is the device itself.
        e.name = c["name"].isString() ? c["name"].toString().trimmed() : QString();
        if (!c.contains("name"))
            e.name = object;
        const QJsonObject dev = c["device"].toObject();
        e.deviceName = dev["name"].toString().trimmed();
        const QJsonValue ids = dev["identifiers"];
        e.deviceKey = ids.isArray() ? text(ids.toArray().first()) : text(ids);
        if (e.deviceKey.isEmpty() && !dev["connections"].toArray().isEmpty())
        {
            QStringList l;
            for (const QJsonValue &v: dev["connections"].toArray().first().toArray())
                l << text(v);
            e.deviceKey = l.join(':');
        }
        if (e.deviceKey.isEmpty())
            e.deviceKey = !e.deviceName.isEmpty() ? e.deviceName : !nodeId.isEmpty() ? nodeId : key;
        m_entities[key] = e;
    };

    if (component != "device")
    {
        add(topic, component, objectId, config);
        return;
    }
    //Device discovery: the shared keys, and one entry per component.
    QJsonObject shared = config;
    shared.remove("components");
    const QJsonObject cmps = config["components"].toObject();
    for (auto it = cmps.begin();it != cmps.end();++it)
    {
        QJsonObject c = shared;
        const QJsonObject own = expand(it.value().toObject());
        for (auto o = own.begin();o != own.end();++o)
            c[o.key()] = o.value();
        //~ of the device applies to its components.
        const QString base = c.value("~").toString();
        for (auto t = c.begin();t != c.end();++t)
            if (t.key().endsWith("_topic") && t.value().isString())
                t.value() = withBase(t.value().toString(), base);
        add(topic + "#" + it.key(), c.value("platform").toString(), it.key(), c);
    }
}

HomeAssistantDiscovery::Availability HomeAssistantDiscovery::availabilityOf(const QJsonObject &config) const
{
    bool online = false, offline = false;
    for (const Avail &a: availabilities(config))
    {
        auto it = m_retained.constFind(a.topic);
        if (it == m_retained.constEnd() || !a.pathOk)
            continue;
        const QString v = valueAt(it.value(), a.path);
        online |= v == a.available;
        offline |= v == a.notAvailable;
    }
    if (offline && !(online && config["availability_mode"].toString() == "any"))
        return OFFLINE;
    return online ? ONLINE : UNKNOWN;
}

QList<HomeAssistantDiscovery::Entity> HomeAssistantDiscovery::entities() const
{
    QList<Entity> l;
    for (Entity e: m_entities)
    {
        e.availability = availabilityOf(e.config);
        l << e;
    }
    std::stable_sort(l.begin(), l.end(), [](const Entity &a, const Entity &b)
    {
        const int d = QString::compare(a.deviceName, b.deviceName, Qt::CaseInsensitive);
        if (d != 0)
            return d < 0;
        if (a.deviceKey != b.deviceKey)
            return a.deviceKey < b.deviceKey;
        return QString::compare(a.displayName(), b.displayName(), Qt::CaseInsensitive) < 0;
    });
    return l;
}

bool HomeAssistantDiscovery::ioParams(const Entity &e, const MqttBroker &broker, Params &out, QString *reason,
                                      QStringList *notes)
{
    const QJsonObject &c = e.config;
    const QString state = c["state_topic"].toString();
    const QString command = c["command_topic"].toString();
    Params p;
    QStringList said;

    auto fail = [&](const QString &why)
    {
        if (reason)
            *reason = why;
        return false;
    };
    auto pathFrom = [&](const char *key, QString *path)
    {
        bool ok = true;
        *path = pathOf(c[key].toString(), &ok);
        return ok;
    };
    auto common = [&](const char *type, const char *ioType)
    {
        p.Add("type", type);
        p.Add("io_type", ioType);
        p.Add("name", s8(e.displayName()));
        addBroker(p, broker);
    };
    auto setPath = [&](const QString &path)
    {
        if (!path.isEmpty())
            p.Add("path", s8(path));
    };

    QString path;
    const QString comp = e.component;
    if (comp == "binary_sensor")
    {
        if (state.isEmpty())
            return fail(tr("it has no state topic"));
        if (!pathFrom("value_template", &path))
            return fail(tr("its value goes through a template Calaos cannot follow"));
        common("MqttInputSwitch", "input");
        p.Add("topic_sub", s8(state));
        setPath(path);
        p.Add("on_value", s8(text(c["payload_on"], "ON")));
        p.Add("off_value", s8(text(c["payload_off"], "OFF")));
        QString cls = c["device_class"].toString();
        if (cls == "opening" || cls == "window")
            cls = "door";
        else if (cls == "moisture")
            cls = "water";
        const QString style = styleFor("MqttInputSwitch", cls);
        if (!style.isEmpty())
            p.Add("io_style", s8(style));
    }
    else if (comp == "sensor")
    {
        if (state.isEmpty())
            return fail(tr("it has no state topic"));
        if (!pathFrom("value_template", &path))
            return fail(tr("its value goes through a template Calaos cannot follow"));
        const QString cls = c["device_class"].toString();
        const QString unit = c["unit_of_measurement"].toString();
        const bool isText = cls == "enum" || cls == "timestamp" || cls == "date";
        const bool numeric = !isText && (!unit.isEmpty() || c.contains("state_class") || !cls.isEmpty());
        if (cls == "temperature")
        {
            common("MqttInputTemp", "input");
            if (!unit.isEmpty() && unit != "°C")
                p.Add("unit", s8(unit));
        }
        else if (numeric)
        {
            common("MqttInputAnalog", "input");
            if (!unit.isEmpty())
                p.Add("unit", s8(unit));
            static const QHash<QString, QString> styles = {
                { "humidity", "humidity" }, { "moisture", "humidity" }, { "pressure", "pressure" },
                { "atmospheric_pressure", "pressure" }, { "voltage", "voltage" }, { "current", "current" },
                { "power", "watt" }, { "illuminance", "luminosity" }, { "speed", "speed" }, { "wind_speed", "speed" },
            };
            const QString style = styleFor("MqttInputAnalog", styles.value(cls));
            if (!style.isEmpty())
                p.Add("io_style", s8(style));
            if (c["suggested_display_precision"].isDouble())
                p.Add("precision", s8(QString::number(c["suggested_display_precision"].toInt())));
        }
        else
            common("MqttInputString", "input");
        p.Add("topic_sub", s8(state));
        setPath(path);
    }
    else if (comp == "switch")
    {
        if (command.isEmpty())
            return fail(tr("it has no command topic"));
        if (state.isEmpty())
            return fail(tr("it does not report its state"));
        if (!pathFrom("value_template", &path))
            return fail(tr("its value goes through a template Calaos cannot follow"));
        const QString on = text(c["payload_on"], "ON"), off = text(c["payload_off"], "OFF");
        //Calaos reads the state with the values it sends.
        if (text(c["state_on"], on) != on || text(c["state_off"], off) != off)
            return fail(tr("it reports other values than the ones it takes"));
        common("MqttOutputLight", "output");
        p.Add("topic_sub", s8(state));
        p.Add("topic_pub", s8(command));
        setPath(path);
        p.Add("on_value", s8(on));
        p.Add("off_value", s8(off));
        p.Add("data", "__##VALUE##__");
        p.Add("io_style", c["device_class"].toString() == "outlet" ? "outlet" : "light");
    }
    else if (comp == "light")
    {
        const QString schema = c["schema"].toString("default");
        const double scale = number(c["brightness_scale"], 255);
        const QString factor = DeviceTemplates::formatNumber(scale / 100.0);
        auto scaled = [&]()
        {
            p.Add("out_expr", s8("x * " + factor));
            p.Add("in_expr", s8("x / " + factor));
        };

        if (schema == "json")
        {
            if (command.isEmpty())
                return fail(tr("it has no command topic"));
            if (state.isEmpty())
                return fail(tr("it does not report its state"));
            QStringList modes = lowerList(c["supported_color_modes"], {});
            for (const char *legacy: { "xy", "rgb", "hs" })
                if (c[legacy].toBool())
                    modes << legacy;
            if (c["brightness"].toBool())
                modes << "brightness";
            const bool dims = std::any_of(modes.begin(), modes.end(), [](const QString &m) { return m != "onoff"; });

            //The RGB output reads the colour back as x and y, brightness 0 to 255.
            if (modes.contains("xy") && scale == 255)
            {
                common("MqttOutputLightRGB", "output");
                p.Add("data", R"({"state":"ON","color":{"x":__##VALUE_X##__,"y":__##VALUE_Y##__},"brightness":__##VALUE_BRIGHTNESS##__})");
                p.Add("path_x", "color/x");
                p.Add("path_y", "color/y");
                p.Add("path_brightness", "brightness");
            }
            else if (dims)
            {
                common("MqttOutputLightDimmer", "output");
                p.Add("data", R"({"state":"ON","brightness":__##VALUE##__})");
                p.Add("path", "brightness");
                scaled();
                if (modes.contains("rgb") || modes.contains("hs") || modes.contains("xy"))
                    said << tr("Its colour is not imported: Calaos reads colours as x and y on a 0 to 255 brightness.");
            }
            else
            {
                common("MqttOutputLight", "output");
                p.Add("data", R"({"state":"__##VALUE##__"})");
                p.Add("path", "state");
                p.Add("on_value", "ON");
                p.Add("off_value", "OFF");
            }
            p.Add("topic_sub", s8(state));
            p.Add("topic_pub", s8(command));
        }
        else if (schema == "default" || schema == "basic")
        {
            const QString briCommand = c["brightness_command_topic"].toString();
            if (!briCommand.isEmpty())
            {
                const QString briState = c["brightness_state_topic"].toString();
                if (briState.isEmpty())
                    return fail(tr("it does not report its brightness"));
                bool ok = true;
                const QString data = dataOf(c["brightness_command_template"].toString(), &ok);
                if (!ok || !pathFrom("brightness_value_template", &path))
                    return fail(tr("its brightness goes through a template Calaos cannot follow"));
                common("MqttOutputLightDimmer", "output");
                p.Add("topic_sub", s8(briState));
                p.Add("topic_pub", s8(briCommand));
                setPath(path);
                p.Add("data", s8(data));
                scaled();
                said << tr("Driven by its brightness topic: off is brightness 0.");
            }
            else
            {
                if (command.isEmpty())
                    return fail(tr("it has no command topic"));
                if (state.isEmpty())
                    return fail(tr("it does not report its state"));
                if (!pathFrom("state_value_template", &path))
                    return fail(tr("its state goes through a template Calaos cannot follow"));
                common("MqttOutputLight", "output");
                p.Add("topic_sub", s8(state));
                p.Add("topic_pub", s8(command));
                setPath(path);
                p.Add("on_value", s8(text(c["payload_on"], "ON")));
                p.Add("off_value", s8(text(c["payload_off"], "OFF")));
                p.Add("data", "__##VALUE##__");
            }
            if (c.contains("rgb_command_topic") || c.contains("hs_command_topic") || c.contains("xy_command_topic"))
                said << tr("Its colour is not imported: Calaos reads colours as x and y on a 0 to 255 brightness.");
        }
        else
            return fail(tr("lights of the %1 schema are not imported").arg(schema));
    }
    else if (comp == "cover")
    {
        if (command.isEmpty())
            return fail(tr("it has no command topic"));
        if (!pathFrom("value_template", &path))
            return fail(tr("its state goes through a template Calaos cannot follow"));
        common("MqttOutputShutter", "output");
        p.Add("topic_pub", s8(command));
        p.Add("payload_open", s8(text(c["payload_open"], "OPEN")));
        p.Add("payload_close", s8(text(c["payload_close"], "CLOSE")));
        p.Add("payload_stop", s8(text(c["payload_stop"], "STOP")));
        if (!state.isEmpty())
        {
            p.Add("topic_sub", s8(state));
            setPath(path);
            p.Add("state_open", s8(text(c["state_open"], "open")));
            p.Add("state_close", s8(text(c["state_closed"], "closed")));
        }
        else
            said << tr("It reports no state: Calaos times its travel.");
    }
    else if (comp == "number")
    {
        if (command.isEmpty())
            return fail(tr("it has no command topic"));
        if (state.isEmpty())
            return fail(tr("it does not report its value"));
        bool ok = true;
        const QString data = dataOf(c["command_template"].toString(), &ok);
        if (!ok || !pathFrom("value_template", &path))
            return fail(tr("its value goes through a template Calaos cannot follow"));
        common("MqttOutputAnalog", "output");
        p.Add("topic_sub", s8(state));
        p.Add("topic_pub", s8(command));
        setPath(path);
        p.Add("data", s8(data));
        //Home Assistant's own range when the config gives none.
        p.Add("value_min", s8(DeviceTemplates::formatNumber(number(c["min"], 1))));
        p.Add("value_max", s8(DeviceTemplates::formatNumber(number(c["max"], 100))));
        p.Add("step", s8(DeviceTemplates::formatNumber(number(c["step"], 1))));
        if (!c["unit_of_measurement"].toString().isEmpty())
            p.Add("unit", s8(c["unit_of_measurement"].toString()));
    }
    else if (comp == "climate")
        return fail(tr("a climate is a device"));
    else
        return fail(tr("Calaos does not import %1 entities").arg(comp));

    setConnectedStatus(p, c);
    out = p;
    if (notes)
        *notes += said;
    return true;
}

bool HomeAssistantDiscovery::climatePlan(const Entity &e, const QJsonArray &templates, const MqttBroker &broker,
                                         Room *room, ClimatePlan *out, QString *reason)
{
    QStringList notes;
    EsphomeDiscovery::Climate c;
    QString why;
    if (!climateShape(e, templates, &out->tpl, &c, &why, &notes))
    {
        if (reason)
            *reason = why;
        return false;
    }

    QVariantMap v = DeviceTemplates::defaults(out->tpl, room);
    v["mqtt_host"] = broker.host;
    v["mqtt_port"] = double(broker.port);
    v["mqtt_user"] = broker.user;
    v["mqtt_password"] = broker.password;
    v["name"] = QString();
    QStringList summary;
    out->values = EsphomeDiscovery::prefill(out->tpl, c, v, &summary);
    out->summary = summary + notes;

    const QList<DeviceTemplates::Problem> problems = DeviceTemplates::validate(out->tpl, out->values);
    if (!problems.isEmpty())
    {
        if (reason)
            *reason = problems.first().message;
        return false;
    }
    return true;
}

void HomeAssistantDiscovery::adjustClimate(const Entity &e, DeviceTemplates::Plan &plan)
{
    const QJsonObject &cfg = e.config;
    for (DeviceTemplates::OwnedIo &o: plan.ios)
    {
        auto io = std::find_if(climateIos().begin(), climateIos().end(),
                               [&](const ClimateIo &c) { return o.key == c.key; });
        if (io == climateIos().end())
            continue;
        bool ok = true;
        o.params.Add("topic_sub", s8(cfg[io->state].toString()));
        const QString path = pathOf(cfg[io->stateTemplate].toString(), &ok);
        o.params.Delete("path");
        if (!path.isEmpty())
            o.params.Add("path", s8(path));
        if (io->command)
        {
            o.params.Add("topic_pub", s8(cfg[io->command].toString()));
            o.params.Add("data", s8(dataOf(cfg[io->commandTemplate].toString(), &ok)));
        }
        setConnectedStatus(o.params, cfg);
    }
}

QSet<QString> HomeAssistantDiscovery::projectKeys()
{
    QSet<QString> keys;
    ListeRoom &lr = ListeRoom::Instance();
    for (int r = 0;r < lr.size();r++)
    {
        Room *room = lr.get_room(r);
        auto consider = [&](IOBase *io)
        {
            if (!u(io->get_param("type")).startsWith("Mqtt"))
                return;
            const QString sub = u(io->get_param("topic_sub"));
            keys.insert(sub.isEmpty() ? "pub:" + u(io->get_param("topic_pub")) : sub + "|" + u(io->get_param("path")));
        };
        for (int i = 0;i < room->get_size_in();i++)
            consider(room->get_input(i));
        for (int i = 0;i < room->get_size_out();i++)
            consider(room->get_output(i));
    }
    return keys;
}

QString HomeAssistantDiscovery::keyOf(const Entity &e, const MqttBroker &broker)
{
    if (e.isClimate())
    {
        bool ok = true;
        return e.config["mode_state_topic"].toString() + "|" + pathOf(e.config["mode_state_template"].toString(), &ok);
    }
    Params p;
    if (!ioParams(e, broker, p, nullptr))
        return QString();
    const QString sub = u(p["topic_sub"]);
    return sub.isEmpty() ? "pub:" + u(p["topic_pub"]) : sub + "|" + u(p["path"]);
}

HomeAssistantDiscovery::Mapping HomeAssistantDiscovery::mappingOf(const Entity &e, const QJsonArray &templates)
{
    Mapping m;
    if (e.isClimate())
    {
        m.device = true;
        QJsonObject tpl;
        EsphomeDiscovery::Climate c;
        if (climateShape(e, templates, &tpl, &c, &m.reason, nullptr))
            m.what = tpl["name"].toString();
        return m;
    }
    Params p;
    if (ioParams(e, MqttBroker(), p, &m.reason))
        m.what = u(p["type"]);
    return m;
}

HomeAssistantDiscovery::Imported HomeAssistantDiscovery::import(const QList<Entity> &entities,
                                                                const QJsonArray &templates,
                                                                const MqttBroker &broker, Room *room)
{
    Imported res;
    QSet<QString> keys = projectKeys();
    ListeRoom &lr = ListeRoom::Instance();

    for (const Entity &e: entities)
    {
        const QString key = keyOf(e, broker);
        if (!key.isEmpty() && keys.contains(key))
        {
            res.notes << tr("%1: already in the project, skipped.").arg(e.displayName());
            continue;
        }

        QString reason;
        QStringList said;
        if (e.isClimate())
        {
            ClimatePlan plan;
            Device *dev = nullptr;
            if (climatePlan(e, templates, broker, room, &plan, &reason))
            {
                QStringList tplNotes;
                dev = DeviceTemplates::create(plan.tpl, plan.values, &tplNotes,
                                              [&e](DeviceTemplates::Plan &p) { adjustClimate(e, p); });
            }
            if (!dev)
            {
                res.notes << tr("%1: not imported, %2.").arg(e.displayName(), reason);
                continue;
            }
            res.devices.push_back(dev);
            said = plan.summary;
        }
        else
        {
            Params p;
            if (!ioParams(e, broker, p, &reason, &said))
            {
                res.notes << tr("%1: not imported, %2.").arg(e.displayName(), reason);
                continue;
            }
            p.Add("id", DeviceTemplates::freeId(DeviceTemplates::idBaseFor(e.displayName()),
                                                &DeviceTemplates::isTakenInHouse));
            res.ios.push_back(p["io_type"] == "input" ? lr.createInput(p, room) : lr.createOutput(p, room));
        }
        keys.insert(key);
        if (!said.isEmpty())
            res.notes << QStringLiteral("%1: %2").arg(e.displayName(), said.join(' '));
    }
    return res;
}
