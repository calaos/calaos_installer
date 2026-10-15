#ifndef DEVICEEDIT_H
#define DEVICEEDIT_H

#include <QCoreApplication>
#include <QJsonObject>
#include <QList>
#include <QMap>
#include <QString>
#include <QStringList>
#include <vector>

#include <Device.h>

// Changes the links and the documented params of an existing device, from what
// IODoc says of its impl: nothing here knows a device family. The id, the owned
// IOs and the rules are never touched; a link whose target is one of the
// device's own IOs is left as it is.
class DeviceEdit
{
    Q_DECLARE_TR_FUNCTIONS(DeviceEdit)

public:
    struct Role
    {
        QString name;
        QString description;
        bool required = false;
        //{link: role, param: name, value: v}: required while every given key holds.
        QJsonObject requiredWhen;
        bool writable = false;
        //number, bool or string; empty when nothing tells it.
        QString valueType;
    };

    struct Param
    {
        QString name;
        QString type; //string, bool, int, float, list
        QString description;
        QString defaultValue;
        QString min, max;
        QJsonObject listValues;
        //Written as <calaos:param> rather than as an attribute of <calaos:device>.
        bool deviceParam = false;
        bool mandatory = false;
    };

    //role -> IO id ("" removes the link); param -> value (absent: not written).
    struct State
    {
        QMap<QString, QString> links;
        QMap<QString, QString> params;
    };

    struct Problem
    {
        QString field; //a role or a param name
        QString message;
    };

    //Documented roles, in IODoc order; empty for an impl IODoc does not know.
    static QList<Role> roles(Calaos::Device *dev);
    //Documented params the user may change (not readonly).
    static QList<Param> params(Calaos::Device *dev);
    static bool isDocumented(Calaos::Device *dev);

    //The role's link targets one of the device's own IOs: it is not offered for editing.
    static bool isOwnedRole(Calaos::Device *dev, const QString &role);

    //Never one of dev's own IOs: a thermostat would regulate on its own setpoint.
    static bool fits(Calaos::IOBase *io, const Role &role, Calaos::Device *dev);
    static std::vector<Calaos::IOBase *> candidates(const Role &role, Calaos::Device *dev);

    static State current(Calaos::Device *dev);
    static bool isRequired(Calaos::Device *dev, const Role &role, const State &state);
    //Checks what differs from the device: a value the file already held is not refused.
    static QList<Problem> validate(Calaos::Device *dev, const State &state);
    //Required roles left without an IO: the device loads, without that capability.
    static QStringList missingRequired(Calaos::Device *dev, const State &state);

    static bool isUnchanged(Calaos::Device *dev, const State &state);
    //Writes a validated state; false (nothing changed) when it does not validate.
    static bool apply(Calaos::Device *dev, const State &state);
};

#endif // DEVICEEDIT_H
