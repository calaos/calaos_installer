#include "DeviceEdit.h"
#include "DeviceTemplates.h"
#include "IODoc.h"

#include <ListeRoom.h>
#include <QJsonArray>
#include <algorithm>
#include <cmath>
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

QString valueTypeOf(IOBase *io)
{
    switch (io->get_type())
    {
    case TINT: return QStringLiteral("number");
    case TBOOL: return QStringLiteral("bool");
    case TSTRING: return QStringLiteral("string");
    default: return QString();
    }
}

const DeviceLink *linkOf(Device *dev, const QString &role)
{
    for (const DeviceLink &l: dev->links())
        if (u(l.name) == role)
            return &l;
    return nullptr;
}

//The IO id a role targets in the state, or in the device for a role the state leaves out.
QString targetOf(Device *dev, const DeviceEdit::State &state, const QString &role)
{
    if (state.links.contains(role))
        return state.links.value(role);
    const DeviceLink *l = linkOf(dev, role);
    return l ? u(l->io) : QString();
}

//An unresolved link gives the server nothing, like a missing one.
bool resolves(const QString &id)
{
    return !id.isEmpty() && DeviceTemplates::ioById(s8(id));
}

//The io_filter the templates of this impl give a role picked from the house.
QString templateValueType(const QString &impl, const QString &role)
{
    for (const QJsonValue &t: DeviceTemplates::load(QStringLiteral("en")))
    {
        const QJsonObject tpl = t.toObject();
        if (tpl["impl"].toString().compare(impl, Qt::CaseInsensitive) != 0)
            continue;
        for (const QJsonValue &l: tpl["links"].toArray())
        {
            const QJsonObject link = l.toObject();
            const QString field = link["io"].toObject()["field"].toString();
            if (link["role"].toString() == role && !field.isEmpty())
                return DeviceTemplates::field(tpl, field)["io_filter"].toObject()["value"].toString();
        }
    }
    return QString();
}

bool isNumber(const QString &v, bool integer)
{
    bool ok = false;
    if (integer)
    {
        v.toLongLong(&ok);
        return ok;
    }
    const double d = v.toDouble(&ok);
    return ok && std::isfinite(d);
}

}

bool DeviceEdit::isDocumented(Device *dev)
{
    return !IODoc::getDeviceObject(u(dev->impl())).isEmpty();
}

QList<DeviceEdit::Role> DeviceEdit::roles(Device *dev)
{
    QList<Role> res;
    const QString impl = u(dev->impl());
    for (const QJsonValue &v: IODoc::getDeviceObject(impl)["link_roles"].toArray())
    {
        const QJsonObject o = v.toObject();
        Role r;
        r.name = o["role"].toString();
        r.description = o["description"].toString();
        r.required = o["required"].toBool();
        r.requiredWhen = o["required_when"].toObject();
        r.writable = o["writable"].toBool();
        r.valueType = templateValueType(impl, r.name);
        if (r.valueType.isEmpty())
        {
            const DeviceLink *l = linkOf(dev, r.name);
            if (IOBase *io = l ? DeviceTemplates::ioById(l->io) : nullptr)
                r.valueType = valueTypeOf(io);
        }
        res.append(r);
    }
    return res;
}

QList<DeviceEdit::Param> DeviceEdit::params(Device *dev)
{
    QList<Param> res;
    for (const QJsonValue &v: IODoc::getDeviceObject(u(dev->impl()))["parameters"].toArray())
    {
        const QJsonObject o = v.toObject();
        if (o["readonly"].toString() == "true")
            continue;
        Param p;
        p.name = o["name"].toString();
        p.type = o["type"].toString();
        p.description = o["description"].toString();
        p.defaultValue = o["default"].toString();
        p.min = o["min"].toString();
        p.max = o["max"].toString();
        p.listValues = o["list_value"].toObject();
        p.deviceParam = o["storage"].toString() == "calaos:param";
        p.mandatory = o["mandatory"].toString() == "true";
        res.append(p);
    }
    return res;
}

bool DeviceEdit::isOwnedRole(Device *dev, const QString &role)
{
    const DeviceLink *l = linkOf(dev, role);
    if (!l)
        return false;
    IOBase *io = DeviceTemplates::ioById(l->io);
    return io && io->getOwnerDevice() == dev;
}

bool DeviceEdit::fits(IOBase *io, const Role &role, Device *dev)
{
    if (!io || dynamic_cast<Device *>(io) || io->get_param("id").empty() || !io->isSupportedByServer() ||
        io->getOwnerDevice() == dev)
        return false;
    if (!role.valueType.isEmpty() && valueTypeOf(io) != role.valueType)
        return false;
    return !role.writable || io->is_output();
}

std::vector<IOBase *> DeviceEdit::candidates(const Role &role, Device *dev)
{
    std::vector<IOBase *> res;
    std::set<IOBase *> seen;
    ListeRoom &lr = ListeRoom::Instance();
    for (int r = 0;r < lr.size();r++)
    {
        Room *room = lr.get_room(r);
        auto consider = [&](IOBase *io)
        {
            if (seen.insert(io).second && fits(io, role, dev))
                res.push_back(io);
        };
        for (int i = 0;i < room->get_size_in();i++)
            consider(room->get_input(i));
        for (int i = 0;i < room->get_size_out();i++)
            consider(room->get_output(i));
    }
    return res;
}

DeviceEdit::State DeviceEdit::current(Device *dev)
{
    State s;
    for (const Role &r: roles(dev))
    {
        if (isOwnedRole(dev, r.name))
            continue;
        const DeviceLink *l = linkOf(dev, r.name);
        s.links[r.name] = l ? u(l->io) : QString();
    }

    for (const Param &p: params(dev))
    {
        if (p.deviceParam)
        {
            for (const auto &dp: dev->deviceParams())
                if (u(dp.first) == p.name)
                    s.params[p.name] = u(dp.second);
        }
        else if (dev->get_params().Exists(s8(p.name)))
            s.params[p.name] = u(dev->get_param(s8(p.name)));
    }
    return s;
}

bool DeviceEdit::isRequired(Device *dev, const Role &role, const State &state)
{
    if (role.required)
        return true;
    if (role.requiredWhen.isEmpty())
        return false;

    if (role.requiredWhen.contains("link"))
    {
        const QString other = role.requiredWhen["link"].toString();
        if (!resolves(targetOf(dev, state, other)))
            return false;
    }
    if (role.requiredWhen.contains("param"))
    {
        const QString name = role.requiredWhen["param"].toString();
        QString value = state.params.value(name);
        if (!state.params.contains(name))
            for (const Param &p: params(dev))
                if (p.name == name)
                    value = p.defaultValue;
        if (value != role.requiredWhen["value"].toString())
            return false;
    }
    return true;
}

QStringList DeviceEdit::missingRequired(Device *dev, const State &state)
{
    QStringList res;
    for (const Role &r: roles(dev))
    {
        const QString id = targetOf(dev, state, r.name);
        if (resolves(id) || !isRequired(dev, r, state))
            continue;
        res << (id.isEmpty() ? r.name : tr("%1 (\"%2\" not found)").arg(r.name, id));
    }
    return res;
}

QList<DeviceEdit::Problem> DeviceEdit::validate(Device *dev, const State &state)
{
    QList<Problem> problems;
    const State before = current(dev);
    const bool thermostat = DeviceTemplates::isThermostatImpl(u(dev->impl()));
    //IO id -> the writable role that took it, and whether that link is a change.
    std::map<QString, std::pair<QString, bool>> writableTargets;
    for (const Role &r: roles(dev))
        if (r.writable && isOwnedRole(dev, r.name))
            writableTargets[u(linkOf(dev, r.name)->io)] = { r.name, false };

    for (const Role &r: roles(dev))
    {
        if (!state.links.contains(r.name) || isOwnedRole(dev, r.name))
            continue;
        const QString id = state.links.value(r.name);
        if (id.isEmpty())
            continue;
        IOBase *io = DeviceTemplates::ioById(s8(id));
        const bool changed = id != before.links.value(r.name);
        auto problem = [&](const QString &m) { problems.append({ r.name, m }); };

        //A device writes an IO through one link: two would fight over it.
        if (r.writable)
        {
            auto taken = writableTargets.find(id);
            if (taken != writableTargets.end())
            {
                if (changed || taken->second.second)
                    problem(tr("%1: \"%2\" is already linked as %3.")
                                .arg(r.name, io ? u(io->get_param("name")) : id, taken->second.first));
                continue;
            }
            writableTargets[id] = { r.name, changed };
        }

        if (!changed)
            continue;
        if (!io)
            problem(tr("%1: no IO has the id \"%2\".").arg(r.name, id));
        else if (io->getOwnerDevice() == dev)
            problem(tr("%1: \"%2\" is one of this device's own IOs.").arg(r.name, u(io->get_param("name"))));
        else if (!fits(io, r, dev))
            problem(tr("%1: \"%2\" is not an IO of the expected kind.").arg(r.name, u(io->get_param("name"))));
        else if (Device *owner = thermostat && DeviceTemplates::isThermostatOutput(r.name) ?
                                     DeviceTemplates::thermostatOwning(s8(id), dev) : nullptr)
            problem(tr("%1: \"%2\" is already an output of thermostat \"%3\".")
                        .arg(r.name, u(io->get_param("name")), u(owner->get_param("name"))));
    }

    for (const Param &p: params(dev))
    {
        const QString v = state.params.value(p.name);
        auto problem = [&](const QString &m) { problems.append({ p.name, m }); };

        if (p.mandatory && v.trimmed().isEmpty())
        {
            problem(tr("%1 is required.").arg(p.name));
            continue;
        }
        if (!state.params.contains(p.name) || v == before.params.value(p.name))
            continue;

        if (p.type == "int" || p.type == "float")
        {
            if (!isNumber(v, p.type == "int"))
            {
                problem(p.type == "int" ? tr("%1 must be a whole number.").arg(p.name)
                                        : tr("%1 must be a number.").arg(p.name));
                continue;
            }
            const double d = v.toDouble();
            if ((!p.min.isEmpty() && d < p.min.toDouble()) || (!p.max.isEmpty() && d > p.max.toDouble()))
                problem(tr("%1 must be between %2 and %3.").arg(p.name, p.min.isEmpty() ? "-" : p.min,
                                                                  p.max.isEmpty() ? "-" : p.max));
        }
        else if (p.type == "bool" && v != "true" && v != "false")
            problem(tr("%1 must be true or false.").arg(p.name));
        else if (p.type == "list" && !p.listValues.isEmpty() && !p.listValues.contains(v))
            problem(tr("%1: \"%2\" is not one of the choices.").arg(p.name, v));
    }

    return problems;
}

bool DeviceEdit::isUnchanged(Device *dev, const State &state)
{
    const State now = current(dev);
    return now.links == state.links && now.params == state.params;
}

bool DeviceEdit::apply(Device *dev, const State &state)
{
    if (!validate(dev, state).isEmpty())
        return false;

    std::vector<DeviceLink> &links = dev->links();
    for (const Role &r: roles(dev))
    {
        if (!state.links.contains(r.name) || isOwnedRole(dev, r.name))
            continue;
        const std::string role = s8(r.name);
        const std::string id = s8(state.links.value(r.name));
        auto it = std::find_if(links.begin(), links.end(), [&](const DeviceLink &l) { return l.name == role; });
        if (id.empty())
        {
            if (it != links.end())
                links.erase(it);
        }
        else if (it != links.end())
        {
            //The commands went to the old IO's partner, not to the new one.
            if (it->io != id)
                it->commandIo.clear();
            it->io = id;
        }
        else
            links.push_back({ role, id, std::string() });
    }

    auto &deviceParams = dev->deviceParams();
    for (const Param &p: params(dev))
    {
        const std::string name = s8(p.name);
        const bool set = state.params.contains(p.name);
        const std::string value = s8(state.params.value(p.name));
        if (!p.deviceParam)
        {
            if (set)
                dev->set_param(name, value);
            else
                dev->get_params().Delete(name);
            continue;
        }

        auto it = std::find_if(deviceParams.begin(), deviceParams.end(), [&](const auto &e) { return e.first == name; });
        if (!set)
        {
            if (it != deviceParams.end())
                deviceParams.erase(it);
        }
        else if (it != deviceParams.end())
            it->second = value;
        else
            deviceParams.emplace_back(name, value);
    }
    return true;
}
