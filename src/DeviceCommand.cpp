#include "DeviceCommand.h"
#include "IODoc.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <cmath>

#include <Device.h>
#include <ListeRoom.h>

using namespace Calaos;

namespace
{

bool parseNumber(const std::string &s, double &out)
{
    bool ok = false;
    const double d = QString::fromStdString(s).trimmed().toDouble(&ok);
    if (!ok || !std::isfinite(d))
        return false;
    out = d;
    return true;
}

int decimalsOf(double step)
{
    for (int d = 0;d < 3;d++)
    {
        const double scaled = step * std::pow(10.0, d);
        if (std::fabs(scaled - std::round(scaled)) < 1e-9)
            return d;
    }
    return 3;
}

IOBase *findIo(const std::string &id)
{
    IOBase *io = ListeRoom::Instance().get_output(id);
    return io ? io : ListeRoom::Instance().get_input(id);
}

// What the server writes for a role: its command_io, else its io.
IOBase *writeTarget(const DeviceLink &link)
{
    IOBase *io = link.commandIo.empty() ? nullptr : findIo(link.commandIo);
    return io ? io : findIo(link.io);
}

DeviceCommand::Bounds boundsOf(IOBase *io)
{
    DeviceCommand::Bounds b;
    if (!io)
        return b;

    Params &p = io->get_params();
    b.hasMin = p.Exists("value_min") && parseNumber(p["value_min"], b.min);
    b.hasMax = p.Exists("value_max") && parseNumber(p["value_max"], b.max);
    b.hasStep = p.Exists("step") && parseNumber(p["step"], b.step) && b.step > 0;
    b.unit = QString::fromUtf8(p["unit"].c_str());

    // An inverted pair is a typo of the installer, the documented bounds stay.
    if (b.hasMin && b.hasMax && b.min > b.max)
        b.hasMin = b.hasMax = false;

    return b;
}

enum LinkState { LinksOk, LinksMissing, LinksUnwritable };

// A role the device only reads (current_temp, humidity) never needs to be writable.
bool roleIsWritten(const QJsonObject &deviceDoc, const QString &role)
{
    for (const QJsonValue &r : deviceDoc["link_roles"].toArray())
    {
        if (r.toObject()["role"].toString() == role)
            return r.toObject()["writable"].toBool(true);
    }
    return true;
}

LinkState linkState(const QJsonArray &roles, const QJsonObject &deviceDoc,
                    const DeviceCommand::Instance &inst, QStringList *lacking = nullptr)
{
    LinkState state = LinksOk;
    for (const QJsonValue &jr : roles)
    {
        QString r = jr.toString();
        // A role that replaces this one meets the requirement (a zone valve for output_heat).
        if (!inst.links.contains(r))
        {
            for (const QString &other : IODoc::replacersOf(deviceDoc, jr.toString()))
            {
                if (inst.links.contains(other))
                    r = other;
            }
        }
        if (!inst.links.contains(r))
        {
            state = LinksMissing;
            if (lacking)
                *lacking << r;
        }
        else if (!inst.writable.contains(r) && roleIsWritten(deviceDoc, r))
        {
            if (state == LinksOk)
                state = LinksUnwritable;
            if (lacking)
                *lacking << r;
        }
    }
    return state;
}

}

DeviceCommand::Instance DeviceCommand::Instance::of(Device *device)
{
    Instance inst;
    if (!device)
        return inst;

    for (const DeviceLink &link : device->links())
    {
        if (link.io.empty())
            continue;
        const QString role = QString::fromStdString(link.name);
        IOBase *target = writeTarget(link);
        inst.links.insert(role);
        // Proxy::isWritable: what is written has to be an output.
        if (target && target->is_output())
            inst.writable.insert(role);
        inst.linkBounds.insert(role, boundsOf(target));
    }

    for (const auto &p : device->deviceParams())
        inst.params.insert(QString::fromStdString(p.first), QString::fromStdString(p.second));

    return inst;
}

bool DeviceCommand::isJson(const QJsonObject &deviceDoc)
{
    return deviceDoc["command_format"].toString() == "json";
}

QStringList DeviceCommand::splitList(const QString &list)
{
    QStringList out;
    for (const QString &s : list.split(',', Qt::SkipEmptyParts))
    {
        const QString t = s.trimmed();
        if (!t.isEmpty() && !out.contains(t))
            out << t;
    }
    return out;
}

DeviceCommand::Offer DeviceCommand::offer(const QJsonObject &deviceDoc, const Instance &instance)
{
    Offer offer;
    if (!isJson(deviceDoc))
        return offer;

    for (const QJsonValue &jv : deviceDoc["command_fields"].toArray())
    {
        const QJsonObject jf = jv.toObject();

        Field f;
        f.name = jf["name"].toString();
        f.description = jf["description"].toString();
        const QString type = jf["type"].toString();
        if (f.name.isEmpty() || (type != "enum" && type != "number"))
            continue;

        const LinkState state = linkState(jf["requires_links"].toArray(), deviceDoc, instance);
        if (state != LinksOk)
        {
            offer.hidden << f.name;
            if (state == LinksUnwritable)
                offer.unwritable << f.name;
            continue;
        }

        if (type == "enum")
        {
            f.type = Enum;
            QJsonObject valueNeeds;

            if (jf.contains("values_param"))
            {
                f.valuesParam = jf["values_param"].toString();
                f.values = splitList(instance.params.value(f.valuesParam));
            }
            else
            {
                for (const QJsonValue &v : jf["values"].toArray())
                    f.values << v.toString();

                // A device that does not document the modes param ignores it, and so does the form.
                QJsonObject modesParam;
                bool documented = false;
                for (const QJsonValue &p : deviceDoc["parameters"].toArray())
                {
                    if (p.toObject()["name"].toString() == "modes")
                    {
                        modesParam = p.toObject();
                        documented = true;
                    }
                }

                // list_rule is what the server does with the list; without it, the list is the whole list.
                const QJsonObject rule = modesParam["list_rule"].toObject();
                if (f.name == "mode" && documented)
                {
                    bool narrowed = instance.params.contains("modes");
                    QStringList subset = splitList(instance.params.value("modes"));
                    if (!narrowed)
                    {
                        const QJsonObject defaults = rule["default_when_linked"].toObject();
                        for (const QString &role : defaults.keys())
                        {
                            if (!instance.links.contains(role))
                                continue;
                            narrowed = true;
                            for (const QJsonValue &v : defaults[role].toArray())
                                subset << v.toString();
                        }
                    }
                    if (narrowed)
                    {
                        for (const QJsonValue &v : rule["always_includes"].toArray())
                            subset << v.toString();
                        QStringList kept;
                        for (const QString &v : f.values)
                        {
                            if (subset.contains(v))
                                kept << v;
                            else
                                f.excludedByParam.insert(v, "modes");
                        }
                        f.values = kept;
                    }
                    valueNeeds = rule["value_requires_values"].toObject();
                }
            }

            const QJsonObject need = jf["value_requires_links"].toObject();
            QStringList allowed;
            for (const QString &v : f.values)
            {
                QStringList lacking;
                if (linkState(need[v].toArray(), deviceDoc, instance, &lacking) == LinksOk)
                    allowed << v;
                else
                    f.excludedByLinks.insert(v, lacking);
            }
            // A value that needs others is judged on what is offered, as the server does.
            for (const QString &v : valueNeeds.keys())
            {
                bool met = true;
                for (const QJsonValue &n : valueNeeds[v].toArray())
                    met = met && allowed.contains(n.toString());
                if (!met && allowed.removeAll(v))
                    f.excludedByParam.insert(v, "modes");
            }
            f.values = allowed;

            if (f.values.isEmpty())
            {
                offer.hidden << f.name;
                continue;
            }
        }
        else
        {
            f.type = Number;
            f.min = jf["min"].toDouble(-1000000);
            f.max = jf["max"].toDouble(1000000);
            f.step = jf["step"].toDouble(1);
            f.unit = jf["unit"].toString();

            // The linked IO of the field's own role sets the bounds of this instance.
            QString role = f.name;
            const QJsonArray need = jf["requires_links"].toArray();
            if (!instance.links.contains(role) && !need.isEmpty())
                role = need.first().toString();

            const Bounds b = instance.linkBounds.value(role);
            if (b.hasMin)
                f.min = b.min;
            if (b.hasMax)
                f.max = b.max;
            if (b.hasStep)
                f.step = b.step;
            if (!b.unit.isEmpty())
                f.unit = b.unit;

            if (f.min > f.max)
                std::swap(f.min, f.max);
            if (!(f.step > 0))
                f.step = 1;
            f.decimals = decimalsOf(f.step);
        }

        offer.fields << f;
    }

    return offer;
}

bool DeviceCommand::parse(const QString &text, QJsonObject &object)
{
    object = QJsonObject();
    if (text.isEmpty())
        return true;

    // The server takes a JSON command only when the text starts with {.
    if (!text.startsWith('{'))
        return false;

    QJsonParseError err;
    const QJsonDocument doc = QJsonDocument::fromJson(text.toUtf8(), &err);
    if (err.error != QJsonParseError::NoError || !doc.isObject())
        return false;

    object = doc.object();
    return true;
}

DeviceCommand::TextKind DeviceCommand::classify(const QString &text, const QJsonObject &deviceDoc)
{
    if (text.isEmpty())
        return Empty;

    QJsonObject obj;
    if (parse(text, obj))
        return Object;

    // A plain mode is a bare word: what starts a JSON text is never one.
    if (!text.startsWith('{') && !text.startsWith('['))
    {
        for (const QJsonValue &jv : deviceDoc["command_fields"].toArray())
        {
            const QJsonObject jf = jv.toObject();
            if (jf["name"].toString() != "mode" || jf["type"].toString() != "enum")
                continue;
            for (const QJsonValue &v : jf["values"].toArray())
            {
                if (v.toString() == text)
                    return PlainMode;
            }
        }
    }

    return Other;
}

QString DeviceCommand::compose(const QList<Field> &fields, const QHash<QString, QJsonValue> &values,
                               const QJsonObject &kept)
{
    QStringList parts;

    auto add = [&](const QString &key, const QJsonValue &value)
    {
        QJsonObject one;
        one.insert(key, value);
        const QString s = QString::fromUtf8(QJsonDocument(one).toJson(QJsonDocument::Compact));
        parts << s.mid(1, s.size() - 2);
    };

    for (const Field &f : fields)
    {
        if (values.contains(f.name))
            add(f.name, values.value(f.name));
    }

    for (auto it = kept.constBegin(); it != kept.constEnd(); it++)
    {
        if (!values.contains(it.key()))
            add(it.key(), it.value());
    }

    if (parts.isEmpty())
        return QString();

    return "{" + parts.join(',') + "}";
}
