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
#include "UsbMonitor_mac.h"

#include <Cocoa/Cocoa.h>
#include <IOKit/IOBSD.h>

void UsbDeviceAddedCallback(void *refCon, io_iterator_t iterator)
{
    while (IOIteratorNext(iterator)) {}; // Run out the iterator or notifications won't start
    UsbMonitor_mac *m = reinterpret_cast<UsbMonitor_mac*>(refCon);
    emit m->usbDeviceChanged();
}

void UsbDeviceRemovedCallback(void *refCon, io_iterator_t iterator)
{
    while (IOIteratorNext(iterator)) {}; // Run out the iterator or notifications won't start
    UsbMonitor_mac *m = reinterpret_cast<UsbMonitor_mac*>(refCon);
    emit m->usbDeviceChanged();
}

UsbMonitor_mac::UsbMonitor_mac():
    QObject()
{
    IONotificationPortRef notificationPort = IONotificationPortCreate(kIOMasterPortDefault);
    CFRunLoopAddSource(CFRunLoopGetCurrent(),
                       IONotificationPortGetRunLoopSource(notificationPort),
                       kCFRunLoopDefaultMode);

    // If we monitor USB devices, notification comes too early, and the new disk device is not present yet.
    // So, instead we monitor for the new disks.
    CFMutableDictionaryRef matchingDict = IOServiceMatching("IOMediaBSDClient");
    CFRetain(matchingDict); // Need to use it twice and IOServiceAddMatchingNotification() consumes a reference

    io_iterator_t portIterator = 0;
    // Register for notifications when a serial port is added to the system
    kern_return_t result = IOServiceAddMatchingNotification(notificationPort,
                                                            kIOPublishNotification,
                                                            matchingDict,
                                                            UsbDeviceAddedCallback,
                                                            this,
                                                            &portIterator);
    while (IOIteratorNext(portIterator)) {}; // Run out the iterator or notifications won't start (you can also use it to iterate the available devices).

    // Also register for removal notifications
    IONotificationPortRef terminationNotificationPort = IONotificationPortCreate(kIOMasterPortDefault);
    CFRunLoopAddSource(CFRunLoopGetCurrent(),
                       IONotificationPortGetRunLoopSource(terminationNotificationPort),
                       kCFRunLoopDefaultMode);
    result = IOServiceAddMatchingNotification(terminationNotificationPort,
                                              kIOTerminatedNotification,
                                              matchingDict,
                                              UsbDeviceRemovedCallback,
                                              this,
                                              &portIterator);

    while (IOIteratorNext(portIterator)) {}; // Run out the iterator or notifications won't start (you can also use it to iterate the available devices).
}

UsbMonitor_mac::~UsbMonitor_mac()
{
}
