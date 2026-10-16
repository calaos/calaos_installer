#ifndef DEVICECOMMAND_H
#define DEVICECOMMAND_H

#include <QHash>
#include <QJsonObject>
#include <QJsonValue>
#include <QList>
#include <QSet>
#include <QString>
#include <QStringList>

namespace Calaos
{
class Device;
}

// The JSON command of a device (spec §5.2, §11): which fields an instance
// offers, and the text they are stored as in a rule action.
class DeviceCommand
{
public:
    enum FieldType { Enum, Number };

    struct Field
    {
        QString name;
        QString description;
        FieldType type = Enum;
        QStringList values;
        double min = 0;
        double max = 0;
        double step = 1;
        int decimals = 0;
        QString unit;
        // Why a value is not in the list, for one stored by an older version of the rule.
        QString valuesParam;                          // the list is this param
        QHash<QString, QString> excludedByParam;      // value -> param that leaves it out
        QHash<QString, QString> excludedByDefault;    // value -> linked role whose default list leaves it out
        QHash<QString, QStringList> excludedByNeeds;  // value -> values it needs and the device does not offer
        QHash<QString, QStringList> excludedByLinks;  // value -> roles it lacks or cannot write
    };

    // Bounds a linked IO sets with value_min, value_max, step and unit.
    struct Bounds
    {
        bool hasMin = false, hasMax = false, hasStep = false;
        double min = 0, max = 0, step = 0;
        QString unit;
    };

    // What the form needs to know about one device.
    struct Instance
    {
        QSet<QString> links;
        QSet<QString> writable; // what the server's isWritable accepts
        QHash<QString, QString> params;
        QHash<QString, Bounds> linkBounds;

        static Instance of(Calaos::Device *device);
    };

    struct Offer
    {
        QList<Field> fields;
        QStringList hidden;     // documented fields without a usable link
        QStringList unwritable; // among them, those whose link exists but cannot be written
    };

    enum TextKind
    {
        Empty,
        Object,    // a JSON object, what the form edits
        PlainMode, // "off": equivalent to {"mode":"off"}, kept as text
        Other
    };

    static bool isJson(const QJsonObject &deviceDoc);
    static Offer offer(const QJsonObject &deviceDoc, const Instance &instance);

    static TextKind classify(const QString &text, const QJsonObject &deviceDoc);
    // False when text is not a JSON object; an empty text gives an empty object.
    static bool parse(const QString &text, QJsonObject &object);

    // Declaration order of the fields, then the other keys by name; no spaces.
    static QString compose(const QList<Field> &fields, const QHash<QString, QJsonValue> &values,
                           const QJsonObject &kept);

    static QStringList splitList(const QString &list);
};

#endif // DEVICECOMMAND_H
