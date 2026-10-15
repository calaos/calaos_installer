#include "DeviceTemplates.h"

#include <ListeRoom.h>
#include <QFile>
#include <QJsonDocument>
#include <QRegularExpression>
#include <QSet>
#include <map>
#include <set>

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

QString formatNumber(double d)
{
    return QString::number(d, 'g', 15);
}

bool isRoom(const QVariant &v)
{
    return v.userType() == qMetaTypeId<Room *>();
}

//Decision 17, as the server applies it (InternalThermostat::claimOutputs): an IO
//linked as output_heat or output_cool belongs to one thermostat, the server ignores
//the link of any other. The only family rule here, because the server enforces it.
const char *const kThermostatImpl = "thermostat";

bool isThermostatOutput(const QString &role)
{
    return role == "output_heat" || role == "output_cool";
}

Device *thermostatOwning(const std::string &id)
{
    ListeRoom &lr = ListeRoom::Instance();
    for (int r = 0;r < lr.size();r++)
        for (Device *dev: lr.get_room(r)->get_devices())
        {
            if (u(dev->impl()).toLower() != kThermostatImpl)
                continue;
            for (const DeviceLink &l: dev->links())
                if (l.io == id && isThermostatOutput(u(l.name)))
                    return dev;
        }
    return nullptr;
}

}

QJsonArray DeviceTemplates::load(const QString &lang)
{
    for (const QString &l: { lang, QStringLiteral("en") })
    {
        QFile f(QStringLiteral(":/doc/%1/device_templates.json").arg(l));
        if (f.open(QIODevice::ReadOnly))
            return parse(f.readAll());
    }
    return QJsonArray();
}

QJsonArray DeviceTemplates::parse(const QByteArray &json)
{
    const QJsonObject root = QJsonDocument::fromJson(json).object();
    if (root["format"].toInt() != FORMAT)
        return QJsonArray();
    return root["templates"].toArray();
}

QJsonObject DeviceTemplates::field(const QJsonObject &tpl, const QString &name)
{
    for (const QJsonValue &f: tpl["fields"].toArray())
        if (f.toObject()["name"].toString() == name)
            return f.toObject();
    return QJsonObject();
}

QVariantMap DeviceTemplates::defaults(const QJsonObject &tpl, Room *room)
{
    QVariantMap values;
    for (const QJsonValue &f: tpl["fields"].toArray())
    {
        const QJsonObject def = f.toObject();
        const QString type = def["type"].toString();
        const QJsonValue d = def["default"];
        QVariant v;

        if (type == "string" || type == "io")
            v = d.toString();
        else if (type == "number")
        {
            double n = d.isDouble() ? d.toDouble() : (def.contains("min") ? def["min"].toDouble() : 0);
            if (def.contains("max") && n > def["max"].toDouble())
                n = def["max"].toDouble();
            v = n;
        }
        else if (type == "bool")
            v = d.toBool();
        else if (type == "enum")
            v = d.isString() ? d.toString() : def["values"].toArray().at(0).toObject()["value"].toString();
        else if (type == "multi")
        {
            QStringList l;
            for (const QJsonValue &e: d.toArray())
                l << e.toString();
            v = l;
        }
        else if (type == "room")
            v = QVariant::fromValue(room);

        if (v.isValid())
            values[def["name"].toString()] = v;
    }
    return values;
}

bool DeviceTemplates::isSet(const QVariant &value)
{
    if (!value.isValid())
        return false;
    if (value.userType() == QMetaType::Bool)
        return value.toBool();
    if (value.userType() == QMetaType::QString)
        return !value.toString().isEmpty();
    if (value.userType() == QMetaType::QStringList)
        return !value.toStringList().isEmpty();
    if (isRoom(value))
        return value.value<Room *>() != nullptr;
    //A number always has a value.
    return true;
}

bool DeviceTemplates::holds(const QJsonObject &entry, const QVariantMap &values)
{
    QString name = entry["when"].toString();
    if (name.isEmpty())
        return true;

    const bool negated = name.startsWith('!');
    if (negated)
        name = name.mid(1);

    return negated != isSet(values.value(name));
}

QString DeviceTemplates::render(const QJsonObject &fieldDef, const QVariant &value)
{
    if (value.userType() == QMetaType::Bool)
        return value.toBool() ? QStringLiteral("true") : QStringLiteral("false");
    if (value.userType() == QMetaType::Double || value.userType() == QMetaType::Int)
        return formatNumber(value.toDouble());
    if (isRoom(value))
    {
        Room *room = value.value<Room *>();
        return room ? u(room->get_name()) : QString();
    }
    if (value.userType() == QMetaType::QStringList)
    {
        //In the order of the field's values, whatever order they were picked in.
        const QStringList picked = value.toStringList();
        QStringList ordered;
        for (const QJsonValue &o: fieldDef["values"].toArray())
        {
            const QString v = o.toObject()["value"].toString();
            if (picked.contains(v))
                ordered << v;
        }
        return ordered.join(',');
    }
    return value.toString();
}

QString DeviceTemplates::substitute(const QString &s, const QJsonObject &tpl, const QVariantMap &values,
                                    const QString &deviceId)
{
    static const QRegularExpression placeholder(QStringLiteral("\\{([A-Za-z0-9_]+)\\}"));

    QString out;
    qsizetype last = 0;
    QRegularExpressionMatchIterator it = placeholder.globalMatch(s);
    while (it.hasNext())
    {
        const QRegularExpressionMatch m = it.next();
        out += s.mid(last, m.capturedStart() - last);
        const QString name = m.captured(1);
        const QJsonObject def = field(tpl, name);
        if (name == "id")
            out += deviceId;
        else if (!def.isEmpty())
            out += render(def, values.value(name));
        else
            out += m.captured(0);
        last = m.capturedEnd();
    }
    out += s.mid(last);
    return out;
}

QList<DeviceTemplates::Problem> DeviceTemplates::validate(const QJsonObject &tpl, const QVariantMap &values)
{
    QList<Problem> problems;
    bool hasRoom = false;
    std::map<QString, QString> writableTargets; //IO id -> label of the field that picked it

    QSet<QString> thermostatOutputs; //fields whose IO becomes an output of this thermostat
    if (tpl["impl"].toString().toLower() == kThermostatImpl)
        for (const QJsonValue &l: tpl["links"].toArray())
            if (isThermostatOutput(l.toObject()["role"].toString()))
                thermostatOutputs.insert(l.toObject()["io"].toObject()["field"].toString());

    for (const QJsonValue &f: tpl["fields"].toArray())
    {
        const QJsonObject def = f.toObject();
        const QString name = def["name"].toString();
        const QString label = def["label"].toString();
        const QString type = def["type"].toString();
        const bool required = def["required"].toBool();
        const QVariant v = values.value(name);
        auto problem = [&](const QString &message) { problems.append({ name, message }); };

        if (type == "room")
            hasRoom = true;

        if (type == "string")
        {
            if (required && v.toString().trimmed().isEmpty())
                problem(tr("%1 is required.").arg(label));
        }
        else if (type == "number")
        {
            bool ok = v.isValid();
            const double n = v.toDouble(&ok);
            const QString unit = def["unit"].toString();
            auto bound = [&](const char *key)
            {
                return unit.isEmpty() ? formatNumber(def[key].toDouble())
                                      : QStringLiteral("%1 %2").arg(formatNumber(def[key].toDouble()), unit);
            };
            if (!ok)
                problem(tr("%1 must be a number.").arg(label));
            else if ((def.contains("min") && n < def["min"].toDouble()) || (def.contains("max") && n > def["max"].toDouble()))
            {
                if (def.contains("min") && def.contains("max"))
                    problem(tr("%1 must be between %2 and %3.").arg(label, bound("min"), bound("max")));
                else if (def.contains("min"))
                    problem(tr("%1 must be %2 or more.").arg(label, bound("min")));
                else
                    problem(tr("%1 must be %2 or less.").arg(label, bound("max")));
            }
        }
        else if (type == "bool")
        {
        }
        else if (type == "enum" || type == "multi")
        {
            QStringList known;
            for (const QJsonValue &o: def["values"].toArray())
                known << o.toObject()["value"].toString();

            const QStringList picked = type == "enum" ? QStringList { v.toString() } : v.toStringList();
            if (type == "multi" && required && picked.isEmpty())
                problem(tr("%1: choose one at least.").arg(label));
            for (const QString &p: picked)
                if (!known.contains(p))
                    problem(tr("%1: \"%2\" is not one of the choices.").arg(label, p));
        }
        else if (type == "room")
        {
            Room *room = isRoom(v) ? v.value<Room *>() : nullptr;
            if (!room || !ListeRoom::Instance().roomExists(room))
                problem(tr("%1: choose a room.").arg(label));
        }
        else if (type == "io")
        {
            const QString id = v.toString();
            const QJsonObject filter = def["io_filter"].toObject();
            IOBase *io = id.isEmpty() ? nullptr : ioById(s8(id));
            if (id.isEmpty())
            {
                if (required)
                    problem(tr("%1: choose an IO.").arg(label));
            }
            else if (!io)
                problem(tr("%1: no IO has the id \"%2\".").arg(label, id));
            else if (!fits(io, filter))
                problem(tr("%1: \"%2\" is not an IO of the expected kind.").arg(label, u(io->get_param("name"))));
            else if (Device *owner = thermostatOutputs.contains(name) ? thermostatOwning(s8(id)) : nullptr)
                problem(tr("%1: \"%2\" is already an output of thermostat \"%3\".")
                            .arg(label, u(io->get_param("name")), u(owner->get_param("name"))));
            else if (filter["writable"].toBool())
            {
                //A device writes an IO through one link: two would fight over it.
                auto taken = writableTargets.find(id);
                if (taken != writableTargets.end())
                    problem(tr("%1: \"%2\" is already chosen for %3.").arg(label, u(io->get_param("name")), taken->second));
                else
                    writableTargets[id] = label;
            }
        }
        else
            problem(tr("%1: this installer does not know the field type \"%2\".").arg(label, type));
    }

    if (!hasRoom)
        problems.append({ QString(), tr("The template has no room field.") });

    for (const QJsonValue &io: tpl["ios"].toArray())
    {
        const QString element = io.toObject()["element"].toString();
        if (element != "input" && element != "output" && element != "internal")
            problems.append({ QString(), tr("The template declares an IO of unknown kind \"%1\".").arg(element) });
    }

    return problems;
}

bool DeviceTemplates::fits(IOBase *io, const QJsonObject &ioFilter)
{
    if (!io || dynamic_cast<Device *>(io) || io->get_param("id").empty())
        return false;

    const QString value = ioFilter["value"].toString();
    const DATA_TYPE t = io->get_type();
    const bool typeFits = (value == "number" && t == TINT) ||
                          (value == "bool" && t == TBOOL) ||
                          (value == "string" && t == TSTRING);
    if (!typeFits)
        return false;

    return !ioFilter["writable"].toBool() || io->is_output();
}

std::vector<IOBase *> DeviceTemplates::candidates(const QJsonObject &ioFilter)
{
    std::vector<IOBase *> res;
    std::set<IOBase *> seen;
    ListeRoom &lr = ListeRoom::Instance();
    for (int r = 0;r < lr.size();r++)
    {
        Room *room = lr.get_room(r);
        auto consider = [&](IOBase *io)
        {
            if (seen.insert(io).second && fits(io, ioFilter))
                res.push_back(io);
        };
        for (int i = 0;i < room->get_size_in();i++)
            consider(room->get_input(i));
        for (int i = 0;i < room->get_size_out();i++)
            consider(room->get_output(i));
    }
    return res;
}

IOBase *DeviceTemplates::ioById(const std::string &id)
{
    if (IOBase *io = ListeRoom::Instance().get_output(id))
        return io;
    return ListeRoom::Instance().get_input(id);
}

bool DeviceTemplates::isTakenInHouse(const std::string &id)
{
    ListeRoom &lr = ListeRoom::Instance();
    if (lr.get_input(id) || lr.get_output(id) || ListeRoom::isPendingIOId(id))
        return true;

    //A dangling link would bind to a new IO holding its target id.
    for (int r = 0;r < lr.size();r++)
        for (Device *dev: lr.get_room(r)->get_devices())
            for (const DeviceLink &l: dev->links())
                if (l.io == id || l.commandIo == id)
                    return true;

    return false;
}

std::string DeviceTemplates::freeId(const std::string &base, const IdTaken &taken)
{
    if (!taken(base))
        return base;

    for (int n = 0;;n++)
    {
        const std::string candidate = base + "_" + std::to_string(n);
        if (!taken(candidate))
            return candidate;
    }
}

std::string DeviceTemplates::idBaseFor(const QString &name)
{
    std::string id;
    bool gap = false;
    for (const QChar c: name.normalized(QString::NormalizationForm_KD))
    {
        if (c.category() == QChar::Mark_NonSpacing)
            continue;

        const char16_t code = c.unicode();
        const bool alnum = code < 128 && ((code >= 'a' && code <= 'z') || (code >= 'A' && code <= 'Z') ||
                                          (code >= '0' && code <= '9'));
        if (!alnum)
        {
            gap = true;
            continue;
        }
        if (gap && !id.empty())
            id += '_';
        gap = false;
        id += static_cast<char>(code >= 'A' && code <= 'Z' ? code - 'A' + 'a' : code);
    }
    return id.empty() ? "device" : id;
}

DeviceTemplates::Plan DeviceTemplates::plan(const QJsonObject &tpl, const QVariantMap &values, const IdTaken &taken)
{
    Plan p;
    const QVariantMap &v = values;

    std::set<std::string> assigned;
    IdTaken used = [&](const std::string &id) { return assigned.count(id) || (taken && taken(id)); };
    auto claim = [&](const std::string &base)
    {
        const std::string id = freeId(base, used);
        assigned.insert(id);
        return id;
    };

    const QJsonObject attributes = tpl["attributes"].toObject();
    const std::string deviceId = claim(idBaseFor(substitute(attributes["name"].toString(), tpl, v, QString())));
    const QString qid = u(deviceId);

    p.device.Add("id", deviceId);
    p.device.Add("type", s8(tpl["family"].toString()));
    p.device.Add("impl", s8(tpl["impl"].toString()));
    for (auto it = attributes.constBegin();it != attributes.constEnd();++it)
    {
        const QString value = substitute(it.value().toString(), tpl, v, qid);
        if (!value.isEmpty() && it.key() != "id" && it.key() != "type" && it.key() != "impl")
            p.device.Add(s8(it.key()), s8(value));
    }

    std::map<std::string, std::string> owned;
    for (const QJsonValue &e: tpl["ios"].toArray())
    {
        const QJsonObject io = e.toObject();
        if (!holds(io, v))
            continue;

        OwnedIo o;
        o.key = s8(io["key"].toString());
        o.element = s8(io["element"].toString());
        const std::string id = claim(deviceId + "_" + o.key);
        owned[o.key] = id;

        o.params.Add("id", id);
        o.params.Add("type", s8(io["type"].toString()));
        const QJsonObject params = io["params"].toObject();
        for (auto it = params.constBegin();it != params.constEnd();++it)
        {
            const QString value = substitute(it.value().toString(), tpl, v, qid);
            if (!value.isEmpty() && it.key() != "id" && it.key() != "type")
                o.params.Add(s8(it.key()), s8(value));
        }
        p.ios.push_back(o);
    }

    auto resolve = [&](const QJsonObject &ref) -> std::string
    {
        if (ref.contains("owned"))
        {
            auto it = owned.find(s8(ref["owned"].toString()));
            return it == owned.end() ? std::string() : it->second;
        }
        const QString name = ref["field"].toString();
        return s8(render(field(tpl, name), v.value(name)));
    };

    for (const QJsonValue &e: tpl["links"].toArray())
    {
        const QJsonObject l = e.toObject();
        if (!holds(l, v))
            continue;

        const std::string io = resolve(l["io"].toObject());
        const bool hasCommand = l.contains("command_io");
        const std::string command = hasCommand ? resolve(l["command_io"].toObject()) : std::string();
        if (io.empty() || (hasCommand && command.empty()))
            continue;

        p.links.push_back({ s8(l["role"].toString()), io, command });
    }

    //The chosen option of an enum sets the params of the same name.
    std::map<QString, QString> preset;
    for (const QJsonValue &f: tpl["fields"].toArray())
    {
        const QJsonObject def = f.toObject();
        if (def["type"].toString() != "enum" || !v.contains(def["name"].toString()))
            continue;
        const QString chosen = v.value(def["name"].toString()).toString();
        for (const QJsonValue &o: def["values"].toArray())
        {
            if (o.toObject()["value"].toString() != chosen)
                continue;
            const QJsonObject params = o.toObject()["params"].toObject();
            for (auto it = params.constBegin();it != params.constEnd();++it)
                preset[it.key()] = it.value().toString();
        }
    }

    for (const QJsonValue &e: tpl["params"].toArray())
    {
        const QJsonObject param = e.toObject();
        if (!holds(param, v))
            continue;

        const QString name = param["name"].toString();
        auto it = preset.find(name);
        const QString value = substitute(it != preset.end() ? it->second : param["value"].toString(), tpl, v, qid);
        if (!value.isEmpty())
            p.params.emplace_back(s8(name), s8(value));
    }

    for (const QJsonValue &e: tpl["notes"].toArray())
    {
        const QJsonObject note = e.toObject();
        if (holds(note, v))
            p.notes << substitute(note["text"].toString(), tpl, v, qid);
    }

    return p;
}

Room *DeviceTemplates::roomOf(const QJsonObject &tpl, const QVariantMap &values)
{
    for (const QJsonValue &f: tpl["fields"].toArray())
    {
        const QJsonObject def = f.toObject();
        if (def["type"].toString() != "room")
            continue;
        const QVariant v = values.value(def["name"].toString());
        return isRoom(v) ? v.value<Room *>() : nullptr;
    }
    return nullptr;
}

Device *DeviceTemplates::create(const QJsonObject &tpl, const QVariantMap &values, QStringList *notes)
{
    Room *room = roomOf(tpl, values);
    if (!room || !validate(tpl, values).isEmpty())
        return nullptr;

    const Plan p = plan(tpl, values, &DeviceTemplates::isTakenInHouse);
    ListeRoom &lr = ListeRoom::Instance();

    Device *dev = lr.createDevice(p.device, room);
    for (const OwnedIo &o: p.ios)
    {
        //As the reader does for the elements of a <calaos:device>.
        IOBase *io = o.element == "output" ? lr.createOutput(o.params, room) : lr.createInput(o.params, room);
        dev->adoptIO(io);
    }
    dev->links() = p.links;
    dev->deviceParams() = p.params;

    if (notes)
        *notes = p.notes;
    return dev;
}
