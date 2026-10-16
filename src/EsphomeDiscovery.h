#ifndef ESPHOMEDISCOVERY_H
#define ESPHOMEDISCOVERY_H

#include <QCoreApplication>
#include <QJsonObject>
#include <QList>
#include <QMap>
#include <QSet>
#include <QString>
#include <QStringList>
#include <QVariantMap>
#include <QtNumeric>

// ESPHome climates found on an MQTT broker, and the template values they imply.
// An ESPHome MQTT climate publishes <prefix>/climate/<object_id>/<what>/state and
// listens on .../<what>/command; with discovery on, it also publishes a Home
// Assistant config (its modes, setpoint range, topics) and esphome/discover/<node>.
class EsphomeDiscovery
{
    Q_DECLARE_TR_FUNCTIONS(EsphomeDiscovery)

public:
    struct Climate
    {
        QString prefix;
        QString objectId;
        QString name; //of the entity, from the discovery config
        QString node; //friendly name of the node
        QString ip;
        //<what> of the topics seen, with the last state when one was read.
        QSet<QString> topics;
        QMap<QString, QString> states;
        bool configured = false; //a discovery config was read
        QStringList modes, fanModes, swingModes;
        double tempMin = qQNaN(), tempMax = qQNaN(), tempStep = qQNaN();
        //Last <prefix>/status: "online", "offline" or empty.
        QString status;
        //Neither discovered nor online, or a sibling of a discovered one: likely the
        //retained leftovers of a renamed entity.
        bool stale = false;

        QString key() const { return prefix + "/climate/" + objectId; }
        //What it publishes tells which of its features exist.
        bool complete() const { return configured || states.contains("mode"); }
        QString displayName() const;
    };

    //Wildcards also match homeassistant/climate/<node>/<id>/config, the discovery
    //config; <prefix>/status is the availability of the node.
    static QStringList filters();

    void feed(const QString &topic, const QByteArray &payload);
    QList<Climate> climates() const;
    void clear();

    //Templates whose fields this fills: they ask for a prefix, an object id and a broker.
    static bool appliesTo(const QJsonObject &tpl);

    //Values of the template's fields for this climate, from current. Only what the
    //climate tells is changed; summary gets one line per fact used or dropped.
    static QVariantMap prefill(const QJsonObject &tpl, const Climate &c, const QVariantMap &current,
                               QStringList *summary = nullptr);


private:
    QMap<QString, Climate> m_climates;
    struct Node
    {
        QString friendlyName;
        QString ip;
    };
    QMap<QString, Node> m_nodes;
    QMap<QString, QString> m_status; //by prefix

    Climate &climate(const QString &prefix, const QString &objectId);
    void feedConfig(QJsonObject config);
};

#endif // ESPHOMEDISCOVERY_H
