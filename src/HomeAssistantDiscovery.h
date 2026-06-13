#ifndef HOMEASSISTANTDISCOVERY_H
#define HOMEASSISTANTDISCOVERY_H

#include <QObject>
#include <QHash>
#include <QList>
#include <QJsonObject>
#include <QString>
#include "Params.h"

// Parses Home Assistant MQTT discovery payloads and converts them to Calaos IO Params.
// See https://www.home-assistant.io/integrations/mqtt/#mqtt-discovery
class HomeAssistantDiscovery : public QObject
{
    Q_OBJECT
public:
    struct Entity {
        QString topic;          // full discovery topic (.../config)
        QString component;      // sensor, binary_sensor, switch, light, cover, number...
        QString objectId;       // last path segment before /config
        QString uniqueId;       // payload.unique_id (or objectId fallback)
        QString name;           // payload.name (resolved)
        QString deviceKey;      // device.identifiers[0] or device.name
        QString deviceName;     // device.name (resolved)
        QJsonObject payload;    // fully expanded payload (abbreviations -> long form)
    };

    struct BrokerCreds {
        QString host;
        int port = 1883;
        QString user;
        QString password;
    };

    explicit HomeAssistantDiscovery(QObject *parent = nullptr);

    // Returns true if topic looks like "<prefix>/<component>/[<node>/]<obj>/config".
    static bool isDiscoveryTopic(const QString &topic, const QString &prefix);

    // Add or remove an entity based on a discovery message.
    // Empty payload on a discovery topic removes the entity. Returns true if registry changed.
    bool handleMessage(const QString &topic, const QByteArray &payload);

    void clear();

    // Iterate currently-known entities grouped by deviceKey.
    QHash<QString, QList<Entity>> entitiesByDevice() const;

    // Convert one discovered entity into one or more Params objects suitable for
    // ListeRoom::createInput/Output. Most components yield a single IO; "climate"
    // is split into several (current temp, setpoint, mode, fan mode, action...).
    // Returns true on success and appends to `out`. Sets `reason` when unsupported.
    static bool toCalaosParams(const Entity &e, const BrokerCreds &b, QList<Params> &out, QString &reason);

    // Expose for unit tests.
    static QJsonObject expandAbbreviations(const QJsonObject &in);
    static QString jsonPointerFromTemplate(const QString &valueTemplate);

signals:
    void entityAddedOrUpdated(const HomeAssistantDiscovery::Entity &e);
    void entityRemoved(const QString &uniqueId);

private:
    QHash<QString, Entity> entities; // keyed by uniqueId
};

#endif // HOMEASSISTANTDISCOVERY_H
