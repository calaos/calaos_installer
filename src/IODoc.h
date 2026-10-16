#ifndef IODOC_H
#define IODOC_H

#include <QJsonObject>
#include <QString>
#include <QStringList>

class IODoc
{
public:
    // Returns the JSON object for the given IO type (with alias resolution).
    // Returns an empty QJsonObject if the type is not found in io_doc.json.
    static QJsonObject getIOObject(const QString &type);

    // Returns devices[impl] of io_doc.json: family, link_roles, parameters and, for a device
    // that takes a JSON command, command_format and command_fields. Empty if unknown.
    static QJsonObject getDeviceObject(const QString &impl);

    // Entry of devices[impl].link_roles / .parameters; empty when the impl or the entry is unknown.
    static QJsonObject getLinkRole(const QString &impl, const QString &role);
    static QJsonObject getDeviceParam(const QString &impl, const QString &name);

    // `label` of an entry (already translated by the server), the id made readable (put_back_delay: Put back delay) when it has none.
    static QString labelOf(const QJsonObject &entry, const QString &id);

    // value_type of a link role: number, bool or string. Empty when absent or anything
    // else, so that an older or unknown document filters nothing.
    static QString valueTypeOf(const QJsonObject &linkRole);

    // Roles of deviceDoc whose `replaces` lists role (a zone valve for output_heat): linking
    // one of them meets a requirement on role, linking both is a configuration error.
    static QStringList replacersOf(const QJsonObject &deviceDoc, const QString &role);

private:
    static const QJsonObject &rootObject();
};

#endif // IODOC_H
