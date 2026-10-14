#ifndef IODOC_H
#define IODOC_H

#include <QJsonObject>
#include <QString>

class IODoc
{
public:
    // Returns the JSON object for the given IO type (with alias resolution).
    // Returns an empty QJsonObject if the type is not found in io_doc.json.
    static QJsonObject getIOObject(const QString &type);

    // Returns devices[impl] of io_doc.json: family, link_roles, parameters and, for a device
    // that takes a JSON command, command_format and command_fields. Empty if unknown.
    static QJsonObject getDeviceObject(const QString &impl);

private:
    static const QJsonObject &rootObject();
};

#endif // IODOC_H
