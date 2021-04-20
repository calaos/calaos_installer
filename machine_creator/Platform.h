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

private slots:
    void usbChanged();

private:
    QVector<UsbDisk *> enumUsbDisk();

    UsbDiskModel *model = nullptr;
};

#endif // PLATFORM_H
