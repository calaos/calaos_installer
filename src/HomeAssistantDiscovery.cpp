#include "HomeAssistantDiscovery.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QRegularExpression>
#include <QDebug>

namespace {

// Subset of HA's abbreviation table - the ones we actually map. Anything not
// listed is kept verbatim so unknown keys still flow through.
// Reference: https://www.home-assistant.io/integrations/mqtt/#mqtt-discovery
const QHash<QString, QString> &abbrevMap()
{
    static const QHash<QString, QString> m = {
        {"~", "topic_base"},
        {"t", "topic"},
        {"stat_t", "state_topic"},
        {"cmd_t", "command_topic"},
        {"avty", "availability"},
        {"avty_t", "availability_topic"},
        {"pl_on", "payload_on"},
        {"pl_off", "payload_off"},
        {"pl_open", "payload_open"},
        {"pl_cls", "payload_close"},
        {"pl_stop", "payload_stop"},
        {"pl_avail", "payload_available"},
        {"pl_not_avail", "payload_not_available"},
        {"uniq_id", "unique_id"},
        {"obj_id", "object_id"},
        {"dev_cla", "device_class"},
        {"stat_cla", "state_class"},
        {"unit_of_meas", "unit_of_measurement"},
        {"val_tpl", "value_template"},
        {"ic", "icon"},
        {"qos", "qos"},
        {"ret", "retain"},
        {"frc_upd", "force_update"},
        {"exp_aft", "expire_after"},
        {"dev", "device"},
        {"o", "origin"},
        {"cmps", "components"},
        {"ids", "identifiers"},
        {"cns", "connections"},
        {"mf", "manufacturer"},
        {"mdl", "model"},
        {"sw", "sw_version"},
        {"hw", "hw_version"},
        {"name", "name"},
        {"bri_cmd_t", "brightness_command_topic"},
        {"bri_stat_t", "brightness_state_topic"},
        {"bri_val_tpl", "brightness_value_template"},
        {"bri_scl", "brightness_scale"},
        {"rgb_cmd_t", "rgb_command_topic"},
        {"rgb_stat_t", "rgb_state_topic"},
        {"xy_cmd_t", "xy_command_topic"},
        {"xy_stat_t", "xy_state_topic"},
        {"hs_cmd_t", "hs_command_topic"},
        {"pos_t", "position_topic"},
        {"set_pos_t", "set_position_topic"},
        {"min", "min"},
        {"max", "max"},
        {"schema", "schema"},
        // climate
        {"mode_cmd_t", "mode_command_topic"},
        {"mode_stat_t", "mode_state_topic"},
        {"mode_stat_tpl", "mode_state_template"},
        {"modes", "modes"},
        {"curr_temp_t", "current_temperature_topic"},
        {"curr_temp_tpl", "current_temperature_template"},
        {"temp_cmd_t", "temperature_command_topic"},
        {"temp_stat_t", "temperature_state_topic"},
        {"temp_stat_tpl", "temperature_state_template"},
        {"min_temp", "min_temp"},
        {"max_temp", "max_temp"},
        {"temp_step", "temp_step"},
        {"temp_unit", "temperature_unit"},
        {"fan_mode_cmd_t", "fan_mode_command_topic"},
        {"fan_mode_stat_t", "fan_mode_state_topic"},
        {"fan_mode_stat_tpl", "fan_mode_state_template"},
        {"fan_modes", "fan_modes"},
        {"swing_mode_cmd_t", "swing_mode_command_topic"},
        {"swing_mode_stat_t", "swing_mode_state_topic"},
        {"act_t", "action_topic"},
        {"act_tpl", "action_template"},
        {"sug_dsp_prc", "suggested_display_precision"},
        {"ent_cat", "entity_category"},
    };
    return m;
}

QString expandAbbrev(const QString &key)
{
    auto it = abbrevMap().constFind(key);
    return it == abbrevMap().constEnd() ? key : it.value();
}

// Replace any "~" prefix in topic-like fields with the payload's `topic_base`.
QString resolveTopic(const QString &raw, const QString &base)
{
    if (raw.startsWith('~'))
        return base + raw.mid(1);
    if (raw.endsWith('~'))
        return raw.left(raw.size() - 1) + base;
    return raw;
}

QJsonValue expandValue(const QJsonValue &v)
{
    if (v.isObject())
        return HomeAssistantDiscovery::expandAbbreviations(v.toObject());
    if (v.isArray()) {
        QJsonArray out;
        for (const auto &item : v.toArray())
            out.append(expandValue(item));
        return out;
    }
    return v;
}

QString firstNonEmpty(std::initializer_list<QString> vals)
{
    for (const auto &v : vals)
        if (!v.isEmpty())
            return v;
    return QString();
}

} // namespace

HomeAssistantDiscovery::HomeAssistantDiscovery(QObject *parent)
    : QObject(parent)
{
}

QJsonObject HomeAssistantDiscovery::expandAbbreviations(const QJsonObject &in)
{
    QJsonObject out;
    for (auto it = in.begin(); it != in.end(); ++it)
        out.insert(expandAbbrev(it.key()), expandValue(it.value()));
    return out;
}

bool HomeAssistantDiscovery::isDiscoveryTopic(const QString &topic, const QString &prefix)
{
    if (!topic.startsWith(prefix + "/"))
        return false;
    if (!topic.endsWith("/config"))
        return false;
    // Minimum: prefix / component / object_id / config => 4 segments
    return topic.count('/') >= 3;
}

QString HomeAssistantDiscovery::jsonPointerFromTemplate(const QString &valueTemplate)
{
    // Best-effort: handle "{{ value_json.foo }}", "{{ value_json.foo.bar }}",
    // "{{ value_json['foo'] }}". Anything more complex -> empty (use raw payload).
    static const QRegularExpression rx(
        R"(\{\{\s*value_json((?:\.[A-Za-z_][A-Za-z0-9_]*|\[['"][^'"]+['"]\])+)\s*\}\})");
    auto m = rx.match(valueTemplate);
    if (!m.hasMatch())
        return QString();

    QString accessors = m.captured(1);
    static const QRegularExpression part(R"(\.([A-Za-z_][A-Za-z0-9_]*)|\[['"]([^'"]+)['"]\])");
    auto it = part.globalMatch(accessors);
    QStringList parts;
    while (it.hasNext()) {
        auto pm = it.next();
        parts << (pm.captured(1).isEmpty() ? pm.captured(2) : pm.captured(1));
    }
    return parts.join('/');
}

bool HomeAssistantDiscovery::handleMessage(const QString &topic, const QByteArray &payload)
{
    // Parse topic: <prefix>/<component>/[<node>/]<object_id>/config
    QStringList parts = topic.split('/');
    if (parts.size() < 4 || parts.last() != "config")
        return false;
    QString component = parts.at(1);
    QString objectId = parts.at(parts.size() - 2);

    // Empty payload = removal of a retained discovery entry.
    if (payload.trimmed().isEmpty()) {
        QString removedKey;
        for (auto it = entities.begin(); it != entities.end(); ++it) {
            if (it.value().topic == topic) { removedKey = it.key(); break; }
        }
        if (removedKey.isEmpty())
            return false;
        entities.remove(removedKey);
        emit entityRemoved(removedKey);
        return true;
    }

    QJsonParseError err;
    QJsonDocument jdoc = QJsonDocument::fromJson(payload, &err);
    if (err.error != QJsonParseError::NoError || !jdoc.isObject()) {
        qWarning() << "HA discovery: bad JSON on" << topic << err.errorString();
        return false;
    }
    QJsonObject raw = expandAbbreviations(jdoc.object());

    // Resolve "~" topic_base shortcuts.
    QString base = raw.value("topic_base").toString();
    if (!base.isEmpty()) {
        QJsonObject expanded = raw;
        for (auto it = raw.begin(); it != raw.end(); ++it) {
            if (it.value().isString() && it.key().endsWith("_topic"))
                expanded.insert(it.key(), resolveTopic(it.value().toString(), base));
        }
        raw = expanded;
    }

    Entity e;
    e.topic = topic;
    e.component = component;
    e.objectId = objectId;
    e.payload = raw;
    e.uniqueId = firstNonEmpty({raw.value("unique_id").toString(), objectId});
    e.name = raw.value("name").toString();

    QJsonObject dev = raw.value("device").toObject();
    QString devId;
    auto ids = dev.value("identifiers");
    if (ids.isArray() && !ids.toArray().isEmpty())
        devId = ids.toArray().first().toString();
    else if (ids.isString())
        devId = ids.toString();
    e.deviceName = dev.value("name").toString();
    e.deviceKey = firstNonEmpty({devId, e.deviceName, e.uniqueId});
    if (e.deviceName.isEmpty())
        e.deviceName = e.deviceKey;

    entities.insert(e.uniqueId, e);
    emit entityAddedOrUpdated(e);
    return true;
}

void HomeAssistantDiscovery::clear()
{
    entities.clear();
}

QHash<QString, QList<HomeAssistantDiscovery::Entity>> HomeAssistantDiscovery::entitiesByDevice() const
{
    QHash<QString, QList<Entity>> out;
    for (const auto &e : entities)
        out[e.deviceKey].append(e);
    return out;
}

bool HomeAssistantDiscovery::toCalaosParams(const Entity &e, const BrokerCreds &b,
                                            QList<Params> &outList, QString &reason)
{
    const QJsonObject &p = e.payload;
    const QString comp = e.component;

    // Working Params for the single-IO components below. Climate ignores it and
    // appends its own Params directly to outList.
    Params out;

    auto addBroker = [&](Params &io) {
        io.Add("host", b.host.toStdString());
        io.Add("port", QString::number(b.port).toStdString());
        io.Add("user", b.user.toStdString());
        io.Add("password", b.password.toStdString());
    };

    auto displayName = [&]() {
        QString n = e.name;
        if (n.isEmpty()) n = e.objectId;
        if (!e.deviceName.isEmpty() && e.deviceName != n)
            return QStringLiteral("%1 - %2").arg(e.deviceName, n);
        return n;
    };

    QString stateTopic = p.value("state_topic").toString();
    QString cmdTopic = p.value("command_topic").toString();
    QString valTpl = p.value("value_template").toString();
    QString path = jsonPointerFromTemplate(valTpl);
    QString payloadOn = p.value("payload_on").toString("ON");
    QString payloadOff = p.value("payload_off").toString("OFF");

    if (comp == "binary_sensor")
    {
        out.Add("type", "MqttInputSwitch");
        out.Add("io_type", "input");
        out.Add("name", displayName().toStdString());
        addBroker(out);
        out.Add("topic_sub", stateTopic.toStdString());
        if (!path.isEmpty()) out.Add("path", path.toStdString());
        out.Add("on_value", payloadOn.toStdString());
        out.Add("off_value", payloadOff.toStdString());
        outList.append(out);
        return true;
    }
    if (comp == "switch")
    {
        out.Add("type", "MqttOutputLight");
        out.Add("io_type", "output");
        out.Add("name", displayName().toStdString());
        addBroker(out);
        if (!stateTopic.isEmpty()) out.Add("topic_sub", stateTopic.toStdString());
        out.Add("topic_pub", cmdTopic.toStdString());
        if (!path.isEmpty()) out.Add("path", path.toStdString());
        out.Add("on_value", payloadOn.toStdString());
        out.Add("off_value", payloadOff.toStdString());
        out.Add("data", "__##VALUE##__");
        outList.append(out);
        return true;
    }
    if (comp == "sensor")
    {
        out.Add("io_type", "input");
        out.Add("name", displayName().toStdString());
        addBroker(out);
        out.Add("topic_sub", stateTopic.toStdString());
        if (!path.isEmpty()) out.Add("path", path.toStdString());

        QString unit = p.value("unit_of_measurement").toString();
        QString devClass = p.value("device_class").toString();

        if (devClass == "temperature") {
            out.Add("type", "MqttInputTemp");
        } else if (!unit.isEmpty() || !devClass.isEmpty()) {
            out.Add("type", "MqttInputAnalog");
            if (!unit.isEmpty()) out.Add("unit", unit.toStdString());
            if (devClass == "humidity")          out.Add("io_style", "humidity");
            else if (devClass == "pressure")     out.Add("io_style", "pressure");
            else if (devClass == "voltage")      out.Add("io_style", "voltage");
            else if (devClass == "current")      out.Add("io_style", "current");
            else if (devClass == "power" || devClass == "energy")
                                                  out.Add("io_style", "watt");
            else if (devClass == "illuminance")  out.Add("io_style", "luminosity");
        } else {
            out.Add("type", "MqttInputString");
        }
        outList.append(out);
        return true;
    }
    if (comp == "cover")
    {
        out.Add("type", "MqttOutputShutter");
        out.Add("io_type", "output");
        out.Add("name", displayName().toStdString());
        addBroker(out);
        if (!stateTopic.isEmpty()) out.Add("topic_sub", stateTopic.toStdString());
        out.Add("topic_pub", cmdTopic.toStdString());
        out.Add("on_value", p.value("payload_open").toString("OPEN").toStdString());
        out.Add("off_value", p.value("payload_close").toString("CLOSE").toStdString());
        out.Add("stop_value", p.value("payload_stop").toString("STOP").toStdString());
        out.Add("data", "__##VALUE##__");
        outList.append(out);
        return true;
    }
    if (comp == "light")
    {
        out.Add("io_type", "output");
        out.Add("name", displayName().toStdString());
        addBroker(out);
        if (!stateTopic.isEmpty()) out.Add("topic_sub", stateTopic.toStdString());
        if (!cmdTopic.isEmpty())   out.Add("topic_pub", cmdTopic.toStdString());

        bool hasRgb = p.contains("rgb_command_topic") || p.contains("xy_command_topic") ||
                      p.contains("hs_command_topic");
        bool hasBri = p.contains("brightness_command_topic") || p.value("brightness").toBool();
        QString schema = p.value("schema").toString("default");

        if (schema == "json") {
            if (hasRgb) {
                out.Add("type", "MqttOutputLightRGB");
                out.Add("data", R"({"state":"__##VALUE##__","brightness":__##VALUE_BRIGHTNESS##__,"color":{"r":__##VALUE_R##__,"g":__##VALUE_G##__,"b":__##VALUE_B##__}})");
                out.Add("path", "state");
                out.Add("path_brightness", "brightness");
                out.Add("path_x", "color/r");
                out.Add("path_y", "color/g");
            } else if (hasBri) {
                out.Add("type", "MqttOutputLightDimmer");
                out.Add("data", R"({"state":"ON","brightness":__##VALUE##__})");
                out.Add("path", "brightness");
            } else {
                out.Add("type", "MqttOutputLight");
                out.Add("on_value", "ON");
                out.Add("off_value", "OFF");
                out.Add("data", R"({"state":"__##VALUE##__"})");
                out.Add("path", "state");
            }
        } else if (hasRgb) {
            out.Add("type", "MqttOutputLightRGB");
            out.Add("data", "__##VALUE_R##__,__##VALUE_G##__,__##VALUE_B##__");
        } else if (hasBri) {
            out.Add("type", "MqttOutputLightDimmer");
            // separate brightness topic - override topic_pub to brightness command topic
            QString briCmd = p.value("brightness_command_topic").toString();
            if (!briCmd.isEmpty()) {
                out.Add("topic_pub", briCmd.toStdString());
                QString briState = p.value("brightness_state_topic").toString();
                if (!briState.isEmpty()) out.Add("topic_sub", briState.toStdString());
            }
            out.Add("data", "__##VALUE##__");
        } else {
            out.Add("type", "MqttOutputLight");
            out.Add("on_value", payloadOn.toStdString());
            out.Add("off_value", payloadOff.toStdString());
            out.Add("data", "__##VALUE##__");
        }
        outList.append(out);
        return true;
    }
    if (comp == "number")
    {
        out.Add("type", "MqttOutputAnalog");
        out.Add("io_type", "output");
        out.Add("name", displayName().toStdString());
        addBroker(out);
        if (!stateTopic.isEmpty()) out.Add("topic_sub", stateTopic.toStdString());
        out.Add("topic_pub", cmdTopic.toStdString());
        QString unit = p.value("unit_of_measurement").toString();
        if (!unit.isEmpty()) out.Add("unit", unit.toStdString());
        out.Add("data", "__##VALUE##__");
        outList.append(out);
        return true;
    }
    if (comp == "climate")
    {
        // One HA/ESPHome climate entity exposes several independent topics. There is
        // no native Calaos "thermostat" IO, so we split it into multiple IOs.
        const QString base = displayName();

        QString currTempTopic = p.value("current_temperature_topic").toString();
        QString tempCmdTopic  = p.value("temperature_command_topic").toString();
        QString tempStatTopic = p.value("temperature_state_topic").toString();
        QString modeCmdTopic  = p.value("mode_command_topic").toString();
        QString modeStatTopic = p.value("mode_state_topic").toString();
        QString fanStatTopic  = p.value("fan_mode_state_topic").toString();
        QString actionTopic   = p.value("action_topic").toString();

        auto pathFromTpl = [](const QJsonObject &obj, const QString &key) {
            return jsonPointerFromTemplate(obj.value(key).toString());
        };

        // Current temperature (read-only).
        if (!currTempTopic.isEmpty()) {
            Params io;
            io.Add("type", "MqttInputTemp");
            io.Add("io_type", "input");
            io.Add("name", QStringLiteral("%1 - Current temperature").arg(base).toStdString());
            addBroker(io);
            io.Add("topic_sub", currTempTopic.toStdString());
            QString tpl = pathFromTpl(p, "current_temperature_template");
            if (!tpl.isEmpty()) io.Add("path", tpl.toStdString());
            outList.append(io);
        }

        // Target temperature setpoint (read/write).
        if (!tempCmdTopic.isEmpty()) {
            Params io;
            io.Add("type", "MqttOutputAnalog");
            io.Add("io_type", "output");
            io.Add("name", QStringLiteral("%1 - Target temperature").arg(base).toStdString());
            addBroker(io);
            io.Add("topic_pub", tempCmdTopic.toStdString());
            if (!tempStatTopic.isEmpty()) io.Add("topic_sub", tempStatTopic.toStdString());
            QString tpl = pathFromTpl(p, "temperature_state_template");
            if (!tpl.isEmpty()) io.Add("path", tpl.toStdString());
            io.Add("io_style", "temperature");
            io.Add("unit", p.value("temperature_unit").toString("C") == "F" ? "°F" : "°C");
            if (p.contains("min_temp")) io.Add("min", QString::number(p.value("min_temp").toDouble()).toStdString());
            if (p.contains("max_temp")) io.Add("max", QString::number(p.value("max_temp").toDouble()).toStdString());
            if (p.contains("temp_step")) io.Add("step", QString::number(p.value("temp_step").toDouble()).toStdString());
            io.Add("data", "__##VALUE##__");
            outList.append(io);
        }

        // Mode on/off control. No native enum output: map ON->first cooling/heating
        //    mode, OFF->"off". User can tweak on_value afterwards.
        if (!modeCmdTopic.isEmpty()) {
            QString onMode = "cool";
            auto modes = p.value("modes").toArray();
            for (const auto &m : modes) {
                QString s = m.toString();
                if (s != "off" && s != "auto") { onMode = s; break; }
            }
            Params io;
            io.Add("type", "MqttOutputLight");
            io.Add("io_type", "output");
            io.Add("name", QStringLiteral("%1 - Mode").arg(base).toStdString());
            addBroker(io);
            io.Add("topic_pub", modeCmdTopic.toStdString());
            if (!modeStatTopic.isEmpty()) io.Add("topic_sub", modeStatTopic.toStdString());
            QString tpl = pathFromTpl(p, "mode_state_template");
            if (!tpl.isEmpty()) io.Add("path", tpl.toStdString());
            io.Add("on_value", onMode.toStdString());
            io.Add("off_value", "off");
            io.Add("data", "__##VALUE##__");
            outList.append(io);
        }

        // Mode readback (read-only string) when there's a state topic, useful to see
        //    the real mode even when controlled as on/off above.
        if (!modeStatTopic.isEmpty()) {
            Params io;
            io.Add("type", "MqttInputString");
            io.Add("io_type", "input");
            io.Add("name", QStringLiteral("%1 - Mode (state)").arg(base).toStdString());
            addBroker(io);
            io.Add("topic_sub", modeStatTopic.toStdString());
            QString tpl = pathFromTpl(p, "mode_state_template");
            if (!tpl.isEmpty()) io.Add("path", tpl.toStdString());
            outList.append(io);
        }

        // Fan mode (read-only string).
        if (!fanStatTopic.isEmpty()) {
            Params io;
            io.Add("type", "MqttInputString");
            io.Add("io_type", "input");
            io.Add("name", QStringLiteral("%1 - Fan mode").arg(base).toStdString());
            addBroker(io);
            io.Add("topic_sub", fanStatTopic.toStdString());
            QString tpl = pathFromTpl(p, "fan_mode_state_template");
            if (!tpl.isEmpty()) io.Add("path", tpl.toStdString());
            outList.append(io);
        }

        // Action (read-only string: heating/cooling/idle/fan...).
        if (!actionTopic.isEmpty()) {
            Params io;
            io.Add("type", "MqttInputString");
            io.Add("io_type", "input");
            io.Add("name", QStringLiteral("%1 - Action").arg(base).toStdString());
            addBroker(io);
            io.Add("topic_sub", actionTopic.toStdString());
            QString tpl = pathFromTpl(p, "action_template");
            if (!tpl.isEmpty()) io.Add("path", tpl.toStdString());
            outList.append(io);
        }

        if (outList.isEmpty()) {
            reason = QObject::tr("Climate entity has no usable topics");
            return false;
        }
        return true;
    }

    reason = QObject::tr("Unsupported component: %1").arg(comp);
    return false;
}
