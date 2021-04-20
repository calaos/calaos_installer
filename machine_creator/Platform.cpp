#include "Platform.h"

#if defined(Q_OS_WIN)
#include "UsbMonitor_win.h"
#elif defined(Q_OS_LINUX)
#include "UsbMonitor_linux.h"
#elif defined(Q_OS_MAC)
#include "UsbMonitor_mac.h"
#endif

#include "drivelist/src/drivelist.hpp"

Platform::Platform(QObject *parent):
    QObject(parent),
    model(new UsbDiskModel(this))
{
#if defined(Q_OS_WIN)
    connect(UsbMonitor_win::Instance(), &UsbMonitor_win::usbDeviceChanged,
            this, &Platform::usbChanged);
#elif defined(Q_OS_LINUX)
    connect(UsbMonitor_linux::Instance(), &UsbMonitor_linux::usbDeviceChanged,
            this, &Platform::usbChanged);
#elif defined(Q_OS_MAC)
    connect(UsbMonitor_mac::Instance(), &UsbMonitor_mac::usbDeviceChanged,
            this, &Platform::usbChanged);
#endif

    model->loadModel(enumUsbDisk());
}

Platform::~Platform()
{
}

UsbDiskModel *Platform::getDiskModel()
{
    return model;
}

void Platform::usbChanged()
{
    qDebug() << "USB Changed: reloading disks";
    model->loadModel(enumUsbDisk());
}

QVector<UsbDisk *> Platform::enumUsbDisk()
{
    QVector<UsbDisk *> disks;
    auto lst = Drivelist::ListStorageDevices();

    for (auto &i: lst)
    {
        QStringList mountpoints;
        for (auto &s: i.mountpoints)
        {
            mountpoints.append(QString::fromStdString(s));
        }

        if (i.isSystem)
            continue;

        if (mountpoints.contains("/") || mountpoints.contains("C://"))
            continue;

        if (i.size == 0)
            continue;

#ifdef Q_OS_DARWIN
        if (i.isVirtual)
            continue;
#endif

        auto d = new UsbDisk();

#ifdef Q_OS_MAC
        // Write to RAW device, this is MUCH faster
        QString rawdev(QString::fromStdString(i.device));
        if (!rawdev.startsWith("/dev/r"))
            rawdev.replace("/dev/", "/dev/r");
        d->set_physicalDevice(rawdev);
#else
        d->set_physicalDevice(QString::fromStdString(i.device));
#endif

        d->set_name(QString::fromStdString(i.description));
        d->set_isRemovable(i.isRemovable);
        d->set_isSystem(i.isSystem);
        d->set_size(i.size);
        d->set_volumes(mountpoints);
        d->set_isUsb(i.isUSB);
        d->set_isSD(i.isCard);

        disks.append(d);
    }

    return disks;
}
