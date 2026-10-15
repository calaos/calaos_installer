#include "EsphomeDiscovery.h"
#include "DeviceTemplates.h"

#include <ListeRoom.h>
#include <QHostAddress>
#include <QJsonArray>
#include <QJsonDocument>
#include <QUrl>
#include <algorithm>
#include <cmath>

using namespace Calaos;

namespace
{

QString u(const std::string &s)
{
    return QString::fromUtf8(s.c_str());
}

//<prefix>/climate/<object_id>/<what>/(state|command); the prefix may hold slashes.
bool parseClimateTopic(const QString &topic, QString *prefix, QString *objectId, QString *what, QString *way)
{
    const QStringList s = topic.split('/');
    const int n = s.size();
    if (n < 5 || s[n - 4] != "climate" || (s[n - 1] != "state" && s[n - 1] != "command"))
        return false;
    *prefix = s.mid(0, n - 4).join('/');
    *objectId = s[n - 3];
    *what = s[n - 2];
    *way = s[n - 1];
    return !prefix->isEmpty() && !objectId->isEmpty() && !what->isEmpty();
}

QStringList stringList(const QJsonValue &v)
{
    QStringList l;
    for (const QJsonValue &e: v.toArray())
        if (e.isString())
            l << e.toString().toLower();
    return l;
}

//ESPHome's heat_cool is the range Calaos calls auto (the template's auto_payload);
//ESPHome's auto runs a schedule of the appliance, which Calaos does not offer.
QString calaosMode(const QString &esphome)
{
    if (esphome == "heat_cool")
        return "auto";
    if (esphome == "auto")
        return QString();
    return esphome;
}

QStringList optionsOf(const QJsonObject &fieldDef)
{
    QStringList l;
    for (const QJsonValue &o: fieldDef["values"].toArray())
        l << o.toObject()["value"].toString();
    return l;
}

//wanted in the order of the field's options; what it does not offer goes to dropped.
QStringList pick(const QJsonObject &fieldDef, const QStringList &wanted, QStringList *dropped)
{
    const QStringList options = optionsOf(fieldDef);
    QStringList res;
    for (const QString &o: options)
        if (wanted.contains(o))
            res << o;
    for (const QString &w: wanted)
        if (!options.contains(w) && !dropped->contains(w))
            *dropped << w;
    return res;
}

bool isLoopback(const QString &host)
{
    const QString h = host.trimmed().toLower();
    if (h == "localhost")
        return true;
    QHostAddress a;
    return a.setAddress(h) && a.isLoopback();
}

}

QString EsphomeDiscovery::Climate::displayName() const
{
    if (!name.isEmpty())
        return name;
    if (!node.isEmpty())
        return node;
    return objectId;
}

QStringList EsphomeDiscovery::filters()
{
    return { "esphome/discover/#", "+/climate/+/#", "+/+/climate/+/#", "+/status", "+/+/status" };
}

void EsphomeDiscovery::clear()
{
    m_climates.clear();
    m_nodes.clear();
    m_status.clear();
}

EsphomeDiscovery::Climate &EsphomeDiscovery::climate(const QString &prefix, const QString &objectId)
{
    Climate &c = m_climates[prefix + "/climate/" + objectId];
    c.prefix = prefix;
    c.objectId = objectId;
    return c;
}

void EsphomeDiscovery::feed(const QString &topic, const QByteArray &payload)
{
    const QStringList s = topic.split('/');

    if (s.size() == 3 && s[0] == "esphome" && s[1] == "discover")
    {
        const QJsonObject o = QJsonDocument::fromJson(payload).object();
        const QString node = o["name"].toString(s[2]);
        m_nodes[node] = { o["friendly_name"].toString(), o["ip"].toString() };
        return;
    }

    if (s.size() >= 2 && s.last() == "status")
    {
        m_status[s.mid(0, s.size() - 1).join('/')] = QString::fromUtf8(payload).trimmed().toLower();
        return;
    }

    if (s.last() == "config")
    {
        const QJsonDocument doc = QJsonDocument::fromJson(payload);
        if (doc.isObject())
            feedConfig(doc.object());
        return;
    }

    QString prefix, objectId, what, way;
    if (!parseClimateTopic(topic, &prefix, &objectId, &what, &way))
        return;
    //An empty retained message deletes the retained one: nothing is published there any more.
    if (payload.isEmpty())
    {
        auto it = m_climates.find(prefix + "/climate/" + objectId);
        if (it == m_climates.end() || way != "state")
            return;
        it->topics.remove(what);
        it->states.remove(what);
        if (it->topics.isEmpty() && !it->configured)
            m_climates.erase(it);
        return;
    }
    Climate &c = climate(prefix, objectId);
    c.topics.insert(what);
    if (way == "state")
        c.states[what] = QString::fromUtf8(payload).trimmed();
}

void EsphomeDiscovery::feedConfig(QJsonObject config)
{
    //"~" stands for the base topic at either end of a topic.
    const QString base = config.value("~").toString();
    QString prefix, objectId;
    QSet<QString> whats;
    for (auto it = config.begin();it != config.end();++it)
    {
        if (!it.value().isString())
            continue;
        QString t = it.value().toString();
        if (!base.isEmpty() && t.startsWith('~'))
            t = base + t.mid(1);
        else if (!base.isEmpty() && t.endsWith('~'))
            t = t.chopped(1) + base;

        QString p, o, what, way;
        if (!parseClimateTopic(t, &p, &o, &what, &way))
            continue;
        if (prefix.isEmpty())
        {
            prefix = p;
            objectId = o;
        }
        if (p == prefix && o == objectId)
            whats.insert(what);
    }
    //Not a climate of ESPHome's topic layout.
    if (prefix.isEmpty() || !whats.contains("mode"))
        return;

    Climate &c = climate(prefix, objectId);
    c.configured = true;
    c.topics.unite(whats);
    c.modes = stringList(config["modes"]);
    c.fanModes = stringList(config["fan_modes"]);
    c.swingModes = stringList(config["swing_modes"]);
    if (config["min_temp"].isDouble())
        c.tempMin = config["min_temp"].toDouble();
    if (config["max_temp"].isDouble())
        c.tempMax = config["max_temp"].toDouble();
    for (const char *k: { "temp_step", "temperature_step" })
        if (config[k].isDouble())
            c.tempStep = config[k].toDouble();
    c.name = config["name"].toString();
    if (c.name.isEmpty())
        c.name = config.contains("dev") ? config["dev"].toObject()["name"].toString()
                                        : config["device"].toObject()["name"].toString();
}

QList<EsphomeDiscovery::Climate> EsphomeDiscovery::climates() const
{
    QSet<QString> discoveredPrefixes;
    for (const Climate &c: m_climates)
        if (c.configured)
            discoveredPrefixes.insert(c.prefix);

    QList<Climate> res;
    for (Climate c: m_climates)
    {
        c.status = m_status.value(c.prefix);
        //A renamed entity leaves its retained states behind, on a node that may
        //still be online: next to a discovered sibling, they are the old one.
        c.stale = !c.configured && (c.status != "online" || discoveredPrefixes.contains(c.prefix));

        //A node's prefix is its name unless its YAML sets another one.
        auto n = m_nodes.constFind(c.prefix);
        if (n != m_nodes.constEnd())
        {
            c.node = n->friendlyName;
            c.ip = n->ip;
        }
        res << c;
    }
    std::stable_partition(res.begin(), res.end(), [](const Climate &c) { return !c.stale; });
    return res;
}

bool EsphomeDiscovery::appliesTo(const QJsonObject &tpl)
{
    for (const char *f: { "topic_prefix", "object_id", "mqtt_host" })
        if (DeviceTemplates::field(tpl, f).isEmpty())
            return false;
    return true;
}

QVariantMap EsphomeDiscovery::prefill(const QJsonObject &tpl, const Climate &c, const QVariantMap &current,
                                      QStringList *summary)
{
    QVariantMap v = current;
    QStringList said;
    auto has = [&](const char *f) { return !DeviceTemplates::field(tpl, f).isEmpty(); };

    v["topic_prefix"] = c.prefix;
    v["object_id"] = c.objectId;
    if (has("name") && current.value("name").toString().trimmed().isEmpty() && !c.displayName().isEmpty())
        v["name"] = c.displayName();

    const bool single = c.topics.contains("target_temperature");
    const bool dual = c.topics.contains("target_temperature_low") || c.topics.contains("target_temperature_high");
    if (has("two_point") && (single || dual))
    {
        v["two_point"] = dual;
        said << (dual ? tr("Two setpoints (target_temperature_low and _high).")
                      : tr("One setpoint (target_temperature)."));
    }

    if (!c.complete())
    {
        said << tr("Its mode was not read: modes, fan and swing are left as they are.");
        if (summary)
            *summary = said;
        return v;
    }

    if (has("with_action"))
        v["with_action"] = c.topics.contains("action");
    if (has("with_current_temp"))
        v["with_current_temp"] = c.topics.contains("current_temperature");

    QStringList dropped;
    if (has("modes"))
    {
        QStringList wanted;
        const QStringList seen = c.modes.isEmpty() ? QStringList(c.states.value("mode")) : c.modes;
        for (const QString &m: seen)
            if (!calaosMode(m.toLower()).isEmpty())
                wanted << calaosMode(m.toLower());
            else if (!dropped.contains(m))
                dropped << m;
        //Without its list, the mode read is added to what the form had.
        if (c.modes.isEmpty())
            wanted += current.value("modes").toStringList();
        const QStringList modes = pick(DeviceTemplates::field(tpl, "modes"), wanted, &dropped);
        if (!modes.isEmpty())
            v["modes"] = modes;
        said << (c.modes.isEmpty() ? tr("Mode read: %1.").arg(c.states.value("mode"))
                                   : tr("Modes: %1.").arg(modes.join(", ")));
    }

    struct Optional
    {
        const char *field;
        const char *topic;
        QStringList listed;
    };
    for (const Optional &o: { Optional { "fan_modes", "fan_mode", c.fanModes },
                              Optional { "swing_modes", "swing_mode", c.swingModes } })
    {
        if (!has(o.field))
            continue;
        const QJsonObject def = DeviceTemplates::field(tpl, o.field);
        if (!c.topics.contains(o.topic))
        {
            v[o.field] = QStringList();
            continue;
        }
        QStringList wanted = o.listed;
        if (wanted.isEmpty())
            wanted = current.value(o.field).toStringList() + QStringList(c.states.value(o.topic).toLower());
        v[o.field] = pick(def, wanted, &dropped);
        said << (o.field == QLatin1String("fan_modes") ? tr("Fan modes: %1.") : tr("Swing modes: %1."))
                    .arg(v[o.field].toStringList().join(", "));
    }

    auto bounded = [&](const char *f, double d)
    {
        const QJsonObject def = DeviceTemplates::field(tpl, f);
        if (def.isEmpty() || std::isnan(d))
            return;
        if (def.contains("min"))
            d = std::max(d, def["min"].toDouble());
        if (def.contains("max"))
            d = std::min(d, def["max"].toDouble());
        v[f] = d;
    };
    bounded("temp_min", c.tempMin);
    bounded("temp_max", c.tempMax);
    bounded("temp_step", c.tempStep);
    if (!std::isnan(c.tempMin) && !std::isnan(c.tempMax))
        said << tr("Setpoint from %1 to %2 °C.").arg(DeviceTemplates::formatNumber(c.tempMin),
                                                       DeviceTemplates::formatNumber(c.tempMax));

    dropped.removeAll(QString());
    if (!dropped.isEmpty())
        said << tr("Not offered by this template: %1.").arg(dropped.join(", "));
    if (summary)
        *summary = said;
    return v;
}

EsphomeDiscovery::Broker EsphomeDiscovery::projectBroker()
{
    struct Seen
    {
        Broker b;
        int count = 0;
        int order = 0;
    };
    QMap<QString, Seen> seen;
    ListeRoom &lr = ListeRoom::Instance();
    for (int r = 0;r < lr.size();r++)
    {
        Room *room = lr.get_room(r);
        auto consider = [&](IOBase *io)
        {
            if (!u(io->get_param("type")).startsWith("Mqtt") || io->get_param("host").empty())
                return;
            Broker b;
            b.host = u(io->get_param("host"));
            b.port = io->get_param("port").empty() ? 1883 : u(io->get_param("port")).toInt();
            b.user = u(io->get_param("user"));
            b.password = u(io->get_param("password"));
            b.found = true;
            const QString key = QStringLiteral("%1:%2@%3").arg(b.host).arg(b.port).arg(b.user);
            Seen &s = seen[key];
            if (s.count++ == 0)
            {
                s.b = b;
                s.order = seen.size();
            }
        };
        for (int i = 0;i < room->get_size_in();i++)
            consider(room->get_input(i));
        for (int i = 0;i < room->get_size_out();i++)
            consider(room->get_output(i));
    }

    const Seen *best = nullptr;
    for (const Seen &s: seen)
        if (!best || s.count > best->count || (s.count == best->count && s.order < best->order))
            best = &s;
    return best ? best->b : Broker();
}

QString EsphomeDiscovery::hostOf(const QString &calaosHost)
{
    const QString h = calaosHost.trimmed();
    if (h.contains("://"))
        return QUrl(h).host();
    //host:port, but not an IPv6 address.
    if (h.count(':') == 1)
        return h.section(':', 0, 0);
    return h;
}

bool EsphomeDiscovery::sameBroker(const QString &probeHost, const QString &brokerHost, const QString &calaosHost)
{
    const QString probe = probeHost.trimmed().toLower();
    if (probe.isEmpty())
        return false;
    if (isLoopback(brokerHost))
        return probe == hostOf(calaosHost).toLower();
    return probe == brokerHost.trimmed().toLower();
}

QString EsphomeDiscovery::probeHost(const QString &brokerHost, const QString &reachedBroker, const QString &calaosHost)
{
    if (!isLoopback(brokerHost))
        return brokerHost.trimmed();
    if (!reachedBroker.trimmed().isEmpty() && !isLoopback(reachedBroker))
        return reachedBroker.trimmed();
    return hostOf(calaosHost);
}
