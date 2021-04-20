/******************************************************************************
 **  Copyright (c) Raoul Hecky. All Rights Reserved.
 **
 **  Calaos is free software; you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation; either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Calaos is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Foobar; if not, write to the Free Software
 **  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 **
 ******************************************************************************/
#ifndef USBMONITOR_LINUX_H
#define USBMONITOR_LINUX_H

#include <QtCore>

class UsbMonitor_linux: public QObject
{
    Q_OBJECT
public:
    static UsbMonitor_linux *Instance()
    {
        static UsbMonitor_linux inst;
        return &inst;
    }
    ~UsbMonitor_linux();

signals:
    void usbDeviceChanged();

private slots:
    void monitorUSB(int fd);

private:
    UsbMonitor_linux();

    void* m_udevLib;

    QSocketNotifier *sockMonitor = nullptr;
    struct udev* m_udev = nullptr;
    struct udev_monitor* mon = nullptr;

    static QString ADD_ACTION;
    static QString REMOVE_ACTION;
};

#endif // USBMONITOR_LINUX_H
