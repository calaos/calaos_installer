#include "MqttBroker.h"
#include "ConfigOptions.h"

#include <ListeRoom.h>
#include <QHostAddress>
#include <QMap>
#include <QUrl>

using namespace Calaos;

namespace
{

QString u(const std::string &s)
{
    return QString::fromUtf8(s.c_str());
}

}

bool MqttBroker::isLoopback(const QString &host)
{
    const QString h = host.trimmed().toLower();
    if (h == "localhost")
        return true;
    QHostAddress a;
    return a.setAddress(h) && a.isLoopback();
}

QString MqttBroker::calaosHost()
{
    const QVector<CalaosAuth> auths = ConfigOptions::Instance().getAuthList();
    const int current = ConfigOptions::Instance().getCurrentAuthIndex();
    return current >= 0 && current < auths.size() ? auths.at(current).host : QString();
}

QString MqttBroker::reachedHost()
{
    ConfigOptions &opt = ConfigOptions::Instance();
    return opt.optionExists("calaos/mqtt_broker_host") ? opt.getMqttBrokerHost() : QString();
}

MqttBroker MqttBroker::ofProject()
{
    struct Seen
    {
        MqttBroker b;
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
            MqttBroker b;
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
    return best ? best->b : MqttBroker();
}

QString MqttBroker::hostOf(const QString &calaosHost)
{
    const QString h = calaosHost.trimmed();
    if (h.contains("://"))
        return QUrl(h).host();
    //host:port, but not an IPv6 address.
    if (h.count(':') == 1)
        return h.section(':', 0, 0);
    return h;
}

bool MqttBroker::sameBroker(const QString &probeHost, const QString &brokerHost, const QString &calaosHost)
{
    const QString probe = probeHost.trimmed().toLower();
    if (probe.isEmpty())
        return false;
    if (isLoopback(brokerHost))
        return probe == hostOf(calaosHost).toLower();
    return probe == brokerHost.trimmed().toLower();
}

QString MqttBroker::probeHost(const QString &brokerHost, const QString &reachedBroker, const QString &calaosHost)
{
    if (!isLoopback(brokerHost))
        return brokerHost.trimmed();
    if (!reachedBroker.trimmed().isEmpty() && !isLoopback(reachedBroker))
        return reachedBroker.trimmed();
    return hostOf(calaosHost);
}
