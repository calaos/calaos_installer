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
#ifndef USBMONITOR_MAC_H
#define USBMONITOR_MAC_H

#include <QtCore>

class UsbMonitor_mac: public QObject
{
    Q_OBJECT
public:
    static UsbMonitor_mac *Instance()
    {
        static UsbMonitor_mac inst;
        return &inst;
    }
    ~UsbMonitor_mac();

signals:
    void usbDeviceChanged();

private:
    UsbMonitor_mac();
};

#endif // USBMONITOR_MAC_H
