#include "ClimateProgram.h"

#include <ListeRoom.h>
#include <QCoreApplication>
#include <QRegularExpression>
#include <algorithm>

namespace Calaos
{
namespace ClimateProgram
{

namespace
{

const char *setName(Set set)
{
    return set == Set::Heat ? "heat" : "cool";
}

const char *countParam(Set set)
{
    return set == Set::Heat ? "heat_levels" : "cool_levels";
}

QString tr(const char *s)
{
    return QCoreApplication::translate("ClimateProgram", s);
}

void eraseLinks(Device *dev, const std::string &role)
{
    auto &links = dev->links();
    links.erase(std::remove_if(links.begin(), links.end(),
                               [&](const DeviceLink &l) { return l.name == role; }),
                links.end());
}

}

bool supports(Device *dev)
{
    if (!dev || dev->family() != "climate")
        return false;
    const std::string impl = QString::fromStdString(dev->impl()).toLower().toStdString();
    return impl == "thermostat" || impl == "proxy";
}

bool isProgramParam(const std::string &name)
{
    return name == "heat_levels" || name == "cool_levels" || name == "program_min_gap" ||
           name == "heat_default_setpoint" || name == "cool_default_setpoint" ||
           name == "cool_default_off";
}

std::string param(Device *dev, const std::string &name, const std::string &def)
{
    for (const auto &p: dev->deviceParams())
        if (p.first == name)
            return p.second;
    return def;
}

void setParam(Device *dev, const std::string &name, const std::string &value)
{
    for (auto &p: dev->deviceParams())
    {
        if (p.first == name)
        {
            p.second = value;
            return;
        }
    }
    dev->deviceParams().push_back({ name, value });
}

int levels(Device *dev, Set set)
{
    bool ok = false;
    const int n = QString::fromStdString(param(dev, countParam(set), "0")).trimmed().toInt(&ok);
    if (!ok || n < 0)
        return 0;
    return std::min(n, kMaxLevels);
}

void setLevels(Device *dev, Set set, int count)
{
    setParam(dev, countParam(set), std::to_string(std::clamp(count, 0, kMaxLevels)));
}

std::string levelRole(Set set, int level)
{
    return std::string(setName(set)) + "_level" + std::to_string(level);
}

std::string setpointRole(Set set, int level)
{
    return levelRole(set, level) + "_setpoint";
}

std::string defaultRole(Set set)
{
    return std::string(setName(set)) + "_default";
}

IOBase *ioForRole(Device *dev, const std::string &role)
{
    for (const DeviceLink &l: dev->links())
    {
        if (l.name != role)
            continue;
        for (IOBase *io: dev->ownedIos())
            if (io->get_param("id") == l.io)
                return io;
    }
    return nullptr;
}

bool isProgramRole(const std::string &role)
{
    static const QRegularExpression re("^((heat|cool)_level[1-6](_setpoint)?|(heat|cool)_default|"
                                       "cool_default_off|program_enabled)$");
    return re.match(QString::fromStdString(role)).hasMatch();
}

std::string roleOf(IOBase *io)
{
    Device *dev = io ? io->getOwnerDevice() : nullptr;
    if (!supports(dev))
        return std::string();
    const std::string id = io->get_param("id");
    for (const DeviceLink &l: dev->links())
        if (l.io == id && isProgramRole(l.name))
            return l.name;
    return std::string();
}

bool isProgramIO(IOBase *io)
{
    return !roleOf(io).empty();
}

std::vector<IOBase *> programIos(Device *dev)
{
    std::vector<IOBase *> res;
    for (IOBase *io: dev->ownedIos())
        if (isProgramIO(io))
            res.push_back(io);
    return res;
}

bool canDeleteAlone(IOBase *io)
{
    return !isProgramIO(io);
}

QString roleLabel(const std::string &role)
{
    static const QRegularExpression level("^(heat|cool)_level([1-6])(_setpoint)?$");
    const QRegularExpressionMatch m = level.match(QString::fromStdString(role));
    if (m.hasMatch())
    {
        const bool heat = m.captured(1) == "heat";
        if (m.captured(3).isEmpty())
            return (heat ? tr("Heating level %1") : tr("Cooling level %1")).arg(m.captured(2));
        return (heat ? tr("Heating level %1 setpoint") : tr("Cooling level %1 setpoint")).arg(m.captured(2));
    }
    if (role == "heat_default")
        return tr("Heating default setpoint");
    if (role == "cool_default")
        return tr("Cooling default setpoint");
    if (role == "cool_default_off")
        return tr("Cooling off by default");
    if (role == "program_enabled")
        return tr("Program enabled");
    return QString::fromStdString(role);
}

QString ruleLabel(IOBase *io)
{
    const std::string role = roleOf(io);
    if (role.empty())
        return QString::fromUtf8(io->get_param("name").c_str());

    QString label = roleLabel(role);
    //A range is a condition on the level being the active one.
    if (io->get_param("type") == "InPlageHoraire")
        label = tr("%1 active").arg(label);
    return QString::fromUtf8(io->getOwnerDevice()->get_param("name").c_str()) +
           QString::fromUtf8(" › ") + label;
}

std::vector<IOBase *> iosRemovedBy(Device *dev, int heat, int cool)
{
    std::vector<IOBase *> res;
    for (Set set: { Set::Heat, Set::Cool })
    {
        const int keep = std::clamp(set == Set::Heat ? heat : cool, 0, kMaxLevels);
        for (int n = keep + 1;n <= kMaxLevels;n++)
            for (const std::string &role: { levelRole(set, n), setpointRole(set, n) })
                if (IOBase *io = ioForRole(dev, role))
                    res.push_back(io);
    }
    return res;
}

std::vector<IOBase *> applyLevels(Device *dev, int heat, int cool)
{
    const std::vector<IOBase *> gone = iosRemovedBy(dev, heat, cool);
    for (IOBase *io: gone)
    {
        if (io->is_input())
            ListeRoom::Instance().deleteIOInput(io);
        else
            ListeRoom::Instance().deleteIOOutput(io);
    }

    //Links are erased even when their IO is missing: the server drops the role.
    for (Set set: { Set::Heat, Set::Cool })
    {
        const int keep = std::clamp(set == Set::Heat ? heat : cool, 0, kMaxLevels);
        for (int n = keep + 1;n <= kMaxLevels;n++)
        {
            eraseLinks(dev, levelRole(set, n));
            eraseLinks(dev, setpointRole(set, n));
        }
    }

    setLevels(dev, Set::Heat, heat);
    setLevels(dev, Set::Cool, cool);
    return gone;
}

double ioValue(IOBase *io, double def)
{
    bool ok = false;
    const double v = QString::fromStdString(io->get_param("value")).trimmed().toDouble(&ok);
    return ok ? v : def;
}

void setIoValue(IOBase *io, double value)
{
    io->set_param("value", QString::number(value).toStdString());
}

bool ioBool(IOBase *io, bool def)
{
    const std::string v = io->get_param("value");
    if (v == "true")
        return true;
    if (v == "false")
        return false;
    return def;
}

void setIoBool(IOBase *io, bool value)
{
    io->set_param("value", value ? "true" : "false");
}

}
}
