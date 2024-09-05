#ifndef PLATFORM_H
#define PLATFORM_H

#include <QtCore>
#include "UsbDisk.h"

class Platform: public QObject
{
    Q_OBJECT

public:
    explicit Platform(QObject *parent = nullptr);
    virtual ~Platform();

    UsbDiskModel *getDiskModel();

    static QVector<UsbDisk *> enumUsbDisk();

    void setReloadEnabled(bool en);

private slots:
    void usbChanged();

private:
    UsbDiskModel *model = nullptr;

    bool reloadEnabled = true;
};

#endif // PLATFORM_H
