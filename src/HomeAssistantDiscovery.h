#ifndef HOMEASSISTANTDISCOVERY_H
#define HOMEASSISTANTDISCOVERY_H

#include <QCoreApplication>
#include <QJsonArray>
#include <QJsonObject>
#include <QList>
#include <QMap>
#include <QSet>
#include <QString>
#include <QStringList>
#include <QVariantMap>
#include <vector>

#include "DeviceTemplates.h"
#include "MqttBroker.h"
#include <Params.h>

// Entities announced by Home Assistant MQTT discovery
// (<prefix>/<component>/[<node_id>/]<object_id>/config, see
// https://www.home-assistant.io/integrations/mqtt/#mqtt-discovery) and what
// Calaos makes of them: an MQTT IO for a sensor, a switch, a light, a cover or a
// number; a climate device of the ESPHome climate templates for a climate.
class HomeAssistantDiscovery
{
    Q_DECLARE_TR_FUNCTIONS(HomeAssistantDiscovery)

public:
    enum Availability { UNKNOWN, ONLINE, OFFLINE };

    struct Entity
    {
        QString topic; //of its config, which identifies it
        QString component;
        QString nodeId;
        QString objectId;
        QString name;
        QString deviceKey;
        QString deviceName;
        //Abbreviations expanded, ~ replaced.
        QJsonObject config;
        Availability availability = UNKNOWN;

        //The device name first, as Home Assistant shows an entity.
        QString displayName() const;
        //Without the device name: an ESPHome climate is named after itself.
        QString ownName() const;
        bool isClimate() const { return component == "climate"; }
    };

    explicit HomeAssistantDiscovery(const QString &prefix = QString());

    //Trimmed, without a trailing slash; homeassistant when empty.
    static QString normalizedPrefix(const QString &prefix);
    QString prefix() const { return m_prefix; }
    //The configs, and every topic: availability topics may be anywhere.
    QStringList filters() const;

    void feed(const QString &topic, const QByteArray &payload);
    void clear();
    //Config topics whose payload is not a JSON object.
    QStringList unreadable() const { return QStringList(m_unreadable.begin(), m_unreadable.end()); }
    //By device, then by name.
    QList<Entity> entities() const;

    static QJsonObject expand(const QJsonObject &config);
    //"{{ value_json.a['b'][0] }}" gives a/b/[0], the path of the MQTT IOs; empty for
    //no template or "{{ value }}". ok is false for a template Calaos cannot follow.
    static QString pathOf(const QString &valueTemplate, bool *ok);
    //A command template with {{ value }} gives the data of the MQTT IOs.
    static QString dataOf(const QString &commandTemplate, bool *ok);

    //What an entity becomes: an IO type or a template name; reason says why not.
    struct Mapping
    {
        bool device = false;
        QString what;
        QString reason;
        bool ok() const { return reason.isEmpty(); }
    };
    static Mapping mappingOf(const Entity &e, const QJsonArray &templates);

    //Params of the IO for an entity that is not a climate, id excepted.
    static bool ioParams(const Entity &e, const MqttBroker &broker, Params &out, QString *reason,
                         QStringList *notes = nullptr);

    struct ClimatePlan
    {
        QJsonObject tpl;
        QVariantMap values;
        QStringList summary;
    };
    //The template and its values for a climate entity, as the ESPHome discovery fills them.
    static bool climatePlan(const Entity &e, const QJsonArray &templates, const MqttBroker &broker,
                            Calaos::Room *room, ClimatePlan *out, QString *reason);
    //The entity's own topics in the IOs of the template, whatever their layout.
    static void adjustClimate(const Entity &e, DeviceTemplates::Plan &plan);

    //Topic and path an IO reads, the topic it drives for an IO that reads nothing.
    static QSet<QString> projectKeys();
    static QString keyOf(const Entity &e, const MqttBroker &broker);

    struct Imported
    {
        std::vector<Calaos::IOBase *> ios;
        std::vector<Calaos::Device *> devices;
        QStringList notes;
    };
    static Imported import(const QList<Entity> &entities, const QJsonArray &templates, const MqttBroker &broker,
                           Calaos::Room *room);

private:
    QString m_prefix;
    QMap<QString, Entity> m_entities; //by config topic
    QMap<QString, QByteArray> m_retained;
    QSet<QString> m_unreadable;

    Availability availabilityOf(const QJsonObject &config) const;
};

#endif // HOMEASSISTANTDISCOVERY_H
