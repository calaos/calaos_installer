#ifndef DIALOGDEVICEINFO_H
#define DIALOGDEVICEINFO_H

#include <QDialog>

#include <Device.h>

class QTreeWidget;
class QTreeWidgetItem;

// Read-only view of a device: its parameters, links and owned IOs.
// Editing arrives with the device wizard.
class DialogDeviceInfo: public QDialog
{
    Q_OBJECT

public:
    explicit DialogDeviceInfo(Calaos::Device *device, QWidget *parent = nullptr);

private:
    QTreeWidget *m_tree;

    QTreeWidgetItem *addGroup(const QString &title);
};

#endif // DIALOGDEVICEINFO_H
