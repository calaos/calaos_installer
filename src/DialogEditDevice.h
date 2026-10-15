#ifndef DIALOGEDITDEVICE_H
#define DIALOGEDITDEVICE_H

#include <QDialog>
#include <QList>

#include "DeviceEdit.h"

class QCheckBox;
class QComboBox;
class QLabel;
class QLineEdit;
class QScrollArea;

// Properties of a device: its links and its documented params, built from
// IODoc (DeviceEdit). The id, the owned IOs and the rules stay as they are.
class DialogEditDevice: public QDialog
{
    Q_OBJECT

public:
    explicit DialogEditDevice(Calaos::Device *device, QWidget *parent = nullptr);

    //What the editors hold, as DeviceEdit takes it.
    DeviceEdit::State state() const;
    //Accepted with a change written to the device.
    bool changed() const { return m_changed; }

private:
    struct LinkEditor
    {
        DeviceEdit::Role role;
        QLabel *label = nullptr;
        QComboBox *box = nullptr; //nullptr for a link to one of the device's own IOs
    };
    struct ParamEditor
    {
        DeviceEdit::Param param;
        QLineEdit *line = nullptr;
        QCheckBox *check = nullptr;
        QComboBox *list = nullptr;
        bool wasSet = false;
        QString raw;     //the value in the file
        QString initial; //editorValue() when the dialog opened
    };

    Calaos::Device *m_device;
    QScrollArea *m_scroll;
    QLabel *m_warning;
    QLabel *m_error;
    QList<LinkEditor> m_links;
    QList<ParamEditor> m_params;
    bool m_changed = false;

    QWidget *buildLinks(const DeviceEdit::State &start);
    QWidget *buildParams(const DeviceEdit::State &start);
    QWidget *buildOwnedIos();
    static QString editorValue(const ParamEditor &e);
    void refresh();
    void save();
};

#endif // DIALOGEDITDEVICE_H
