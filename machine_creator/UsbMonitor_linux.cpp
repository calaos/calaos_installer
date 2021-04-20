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
#include "UsbMonitor_linux.h"
#include <QSocketNotifier>
#include <libudev.h>
#include <dlfcn.h>
#include <QTimer>

// Declare required functions as weak so that they were not reported as missing at compile time.
// In runtime it is required to ensure they are defined: we do it by checking that libudev is loaded.
#pragma weak udev_device_unref
#pragma weak udev_monitor_enable_receiving
#pragma weak udev_monitor_filter_add_match_subsystem_devtype
#pragma weak udev_monitor_get_fd
#pragma weak udev_monitor_new_from_netlink
#pragma weak udev_monitor_receive_device
#pragma weak udev_monitor_unref
#pragma weak udev_new
#pragma weak udev_unref

UsbMonitor_linux::UsbMonitor_linux()
{
    m_udevLib = dlopen("libudev.so.1", RTLD_NOW | RTLD_GLOBAL);
    if (m_udevLib == nullptr)
        m_udevLib = dlopen("libudev.so.0", RTLD_NOW | RTLD_GLOBAL);

    if (!m_udevLib)
        return;

    m_udev = udev_new();
    mon = udev_monitor_new_from_netlink(m_udev, "udev");

    //Filter hidraw devices
    udev_monitor_filter_add_match_subsystem_devtype(mon, "block", "disk");
    udev_monitor_enable_receiving(mon);

    int fd = udev_monitor_get_fd(mon);

    sockMonitor = new QSocketNotifier(fd, QSocketNotifier::Read);
    connect(sockMonitor, &QSocketNotifier::activated, this, &UsbMonitor_linux::monitorUSB);
}

UsbMonitor_linux::~UsbMonitor_linux()
{
    delete sockMonitor;
    if (mon) udev_monitor_unref(mon);
    if (m_udev) udev_unref(m_udev);
    if (m_udevLib) dlclose(m_udevLib);
}

void UsbMonitor_linux::monitorUSB(int fd)
{
    Q_UNUSED(fd);
    const auto dev = udev_monitor_receive_device(mon);
    if (dev)
    {
        udev_device_unref(dev);
        emit usbDeviceChanged();
    }
}
