#include "DeviceCommand.h"

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

DeviceCommand::Bounds boundsOf(const std::string &ioId)
{
    DeviceCommand::Bounds b;

    IOBase *io = ListeRoom::Instance().get_output(ioId);
    if (!io)
        io = ListeRoom::Instance().get_input(ioId);
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

bool allLinks(const QJsonArray &roles, const QSet<QString> &links)
{
    for (const QJsonValue &r : roles)
    {
        if (!links.contains(r.toString()))
            return false;
    }
    return true;
}

QStringList modeValuesFor(const QJsonObject &fieldDoc, const QJsonObject &deviceDoc, const DeviceCommand::Instance &inst)
{
    QStringList values;
    for (const QJsonValue &v : fieldDoc["values"].toArray())
        values << v.toString();

    // The modes param narrows the six modes of a proxy; a device that does not
    // document it ignores it, and so does the form.
    if (fieldDoc["name"].toString() != "mode")
        return values;

    bool documented = false;
    for (const QJsonValue &p : deviceDoc["parameters"].toArray())
    {
        if (p.toObject()["name"].toString() == "modes")
            documented = true;
    }

    const QStringList subset = DeviceCommand::splitList(inst.params.value("modes"));
    if (documented && !subset.isEmpty())
    {
        QStringList kept;
        for (const QString &v : values)
        {
            if (subset.contains(v))
                kept << v;
        }
        return kept;
    }

    return values;
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
        inst.links.insert(role);
        inst.linkBounds.insert(role, boundsOf(link.io));
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

        if (!allLinks(jf["requires_links"].toArray(), instance.links))
        {
            offer.hidden << f.name;
            continue;
        }

        if (type == "enum")
        {
            f.type = Enum;

            if (jf.contains("values_param"))
                f.values = splitList(instance.params.value(jf["values_param"].toString()));
            else
                f.values = modeValuesFor(jf, deviceDoc, instance);

            const QJsonObject need = jf["value_requires_links"].toObject();
            QStringList allowed;
            for (const QString &v : f.values)
            {
                if (allLinks(need[v].toArray(), instance.links))
                    allowed << v;
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
    if (text.trimmed().isEmpty())
        return true;

    QJsonParseError err;
    const QJsonDocument doc = QJsonDocument::fromJson(text.toUtf8(), &err);
    if (err.error != QJsonParseError::NoError || !doc.isObject())
        return false;

    object = doc.object();
    return true;
}

DeviceCommand::TextKind DeviceCommand::classify(const QString &text, const QJsonObject &deviceDoc)
{
    const QString t = text.trimmed();
    if (t.isEmpty())
        return Empty;

    QJsonObject obj;
    if (parse(text, obj))
        return Object;

    // A plain mode is a bare word: what starts a JSON text is never one.
    if (!t.startsWith('{') && !t.startsWith('['))
    {
        for (const QJsonValue &jv : deviceDoc["command_fields"].toArray())
        {
            const QJsonObject jf = jv.toObject();
            if (jf["name"].toString() != "mode" || jf["type"].toString() != "enum")
                continue;
            for (const QJsonValue &v : jf["values"].toArray())
            {
                if (v.toString() == t)
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
