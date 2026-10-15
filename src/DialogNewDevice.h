#ifndef DIALOGNEWDEVICE_H
#define DIALOGNEWDEVICE_H

#include <QDialog>
#include <QJsonArray>
#include <QJsonObject>
#include <QList>
#include <QMap>
#include <QStringList>
#include <QVariantMap>

#include <Device.h>
#include <Room.h>

class QCheckBox;
class QComboBox;
class QDialogButtonBox;
class QLabel;
class QScrollArea;
class WidgetEsphomeDiscovery;

// "New device from a template": one form per template, built from its fields
// (DeviceTemplates). On Create, the values are validated and the device, its
// owned IOs, links and params are added to the model.
class DialogNewDevice: public QDialog
{
    Q_OBJECT

public:
    //room: the one a room field starts on.
    DialogNewDevice(const QJsonArray &templates, Calaos::Room *room, QWidget *parent = nullptr);

    bool selectTemplate(const QString &id);
    QJsonObject currentTemplate() const;
    //Values of the fields, as DeviceTemplates takes them.
    QVariantMap values() const;
    //Sets the editors of the current template, as if the user did.
    void setValues(const QVariantMap &values);

    //Shown for the templates EsphomeDiscovery fills; nullptr without Qt MQTT.
    WidgetEsphomeDiscovery *discovery() const { return m_discovery; }

    //Set once Create succeeded.
    Calaos::Device *device() const { return m_device; }
    Calaos::Room *room() const { return m_room; }
    QStringList notes() const { return m_notes; }

    //Texts of the template to read once the device exists (markdown).
    static QDialog *createNotesDialog(const QString &deviceName, const QStringList &notes, QWidget *parent);

private:
    struct FieldEditor
    {
        QJsonObject def;
        QWidget *editor = nullptr;
        QList<QCheckBox *> choices; //multi
    };

    QJsonArray m_templates;
    Calaos::Room *m_startRoom;

    QComboBox *m_templateBox;
    QLabel *m_description;
    WidgetEsphomeDiscovery *m_discovery = nullptr;
    QScrollArea *m_scroll;
    QLabel *m_error;
    QDialogButtonBox *m_buttons;

    QList<FieldEditor> m_fields;
    //Values the user set, by field name, whatever template they were set in.
    QVariantMap m_changed;

    Calaos::Device *m_device = nullptr;
    Calaos::Room *m_room = nullptr;
    QStringList m_notes;

    void buildForm();
    void remember(const QString &name);
    QWidget *createEditor(FieldEditor &f, const QVariant &value);
    QVariant valueOf(const FieldEditor &f) const;
    void create();
    void updateDiscoveryHosts();
};

#endif // DIALOGNEWDEVICE_H
