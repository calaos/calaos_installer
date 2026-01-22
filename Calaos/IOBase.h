/******************************************************************************
**  Copyright (c) 2007-2014, Calaos. All Rights Reserved.
**
**  This file is part of Calaos Home.
**
**  Calaos Home is free software; you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation; either version 3 of the License, or
**  (at your option) any later version.
**
**  Calaos Home is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with Foobar; if not, write to the Free Software
**  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
**
******************************************************************************/

#ifndef S_IOBASE_H
#define S_IOBASE_H

#include <Utils.h>
#include "TimeRange.h"
#include <QString>

using namespace std;
using namespace Utils;

namespace Calaos
{

class IOBase
{
private:
    //we store all params here
    Params param;

    // For RemoteUI: store the raw XML content of <calaos:pages> for roundtrip preservation
    QString remoteUIPagesXml;

    double dvalue = 1.234;
    bool bvalue = true;
    string svalue = "Sample string";

    string gui_type;
    DATA_TYPE basic_type;

    int io_type;

public:

    enum { IO_NONE = 0, IO_INPUT, IO_OUTPUT, IO_BOTH };

    IOBase(Params &p, string g, DATA_TYPE d, int i, string s = string()):
        param(p),
        gui_type(g),
        basic_type(d),
        io_type(i),
        scriptDemoValue(s)
    {
        range_months.set();
    }
    virtual ~IOBase() { }

    virtual DATA_TYPE get_type() { return basic_type; }

    virtual void set_param(std::string opt, std::string val)
    { param.Add(opt, val); }
    virtual std::string get_param(std::string opt)
    { return param[opt]; }
    virtual Params &get_params()
    { return param; }

    double get_value_double() { return dvalue; }
    bool get_value_bool() { return bvalue; }
    string get_value_string() { return svalue; }

    void set_value(double v) { dvalue = v; }
    void set_value(bool v) { bvalue = v; }
    void set_value(string v) { svalue = v; }

    string get_gui_type() { return gui_type; }
    void set_gui_type(string s) { gui_type = s; }

    bool is_input() { return io_type == IO_INPUT || io_type == IO_BOTH; }
    bool is_output() { return io_type == IO_OUTPUT || io_type == IO_BOTH; }
    bool is_inout() { return io_type == IO_BOTH; }

    // RemoteUI pages XML content
    QString getRemoteUIPagesXml() const { return remoteUIPagesXml; }
    void setRemoteUIPagesXml(const QString &xml) { remoteUIPagesXml = xml; }

    //For time ranges
    vector<TimeRange> range_monday;
    vector<TimeRange> range_tuesday;
    vector<TimeRange> range_wednesday;
    vector<TimeRange> range_thursday;
    vector<TimeRange> range_friday;
    vector<TimeRange> range_saturday;
    vector<TimeRange> range_sunday;

    //months where InPlageHoraire is activated
    enum { JANUARY = 0, FEBRUARY, MARCH, APRIL, MAY, JUNE, JULY, AUGUST, SEPTEMBER, OCTOBER, NOVEMBER, DECEMBER };
    bitset<12> range_months;

    //Input source mapping for AVReceiver
    typedef map<int ,string> AVRList;
    AVRList source_names;

    bool getDemoValueBool();
    double getDemoValueDouble();
    string getDemoValueString();
    string scriptDemoValue;

    void buildInputSourcesList();

    //AVReceiver Input Source types
    /* WARNING!! Must be kept in sync with calaos_server!!!
     */
    enum {
        AVR_UNKNOWN,

        /* AVR Zones */
        AVR_ZONE_MAIN, AVR_ZONE_2, AVR_ZONE_3,

        /* AVR Input sources */
        //WARNING: After first release, don't move these value it will mess the stored rule's actions
        //Instead, add new value to the end.
        AVR_INPUT_BD, AVR_INPUT_DVD, AVR_INPUT_TVSAT, AVR_INPUT_DVRBDR,
        AVR_INPUT_VIDEO_1, AVR_INPUT_VIDEO_2, AVR_INPUT_VIDEO_3, AVR_INPUT_VIDEO_4,
        AVR_INPUT_VIDEO_5, AVR_INPUT_VIDEO_6, AVR_INPUT_VIDEO_7, AVR_INPUT_VIDEO_8,
        AVR_INPUT_NETRADIO, AVR_INPUT_IPOD,
        AVR_INPUT_HDMI_1, AVR_INPUT_HDMI_2, AVR_INPUT_HDMI_3, AVR_INPUT_HDMI_4,
        AVR_INPUT_HDMI_5, AVR_INPUT_HDMI_6, AVR_INPUT_HDMI_7, AVR_INPUT_HDMI_8,
        AVR_INPUT_CD, AVR_INPUT_CDRTAPE, AVR_INPUT_TUNER, AVR_INPUT_PHONO, AVR_INPUT_MULTIIN,
        AVR_INPUT_APORT, AVR_INPUT_SIRIUS, AVR_INPUT_TV, AVR_INPUT_SAT, AVR_INPUT_GAME_1,
        AVR_INPUT_GAME_2, AVR_INPUT_AUX, AVR_INPUT_DOCK, AVR_INPUT_HDRADIO,
        AVR_INPUT_RHAPSODY, AVR_INPUT_NAPSTER, AVR_INPUT_PANDORA, AVR_INPUT_LASTFM,
        AVR_INPUT_FLICKR, AVR_INPUT_FAV, AVR_INPUT_SERVER, AVR_INPUT_NETWORK,
        AVR_INPUT_USB, AVR_INPUT_USB2, AVR_INPUT_USB3, AVR_INPUT_USB4,
        AVR_INPUT_USB5, AVR_INPUT_USB6, AVR_INPUT_USB7, AVR_INPUT_USB8,
        AVR_INPUT_VCR, AVR_INPUT_AUX1, AVR_INPUT_AUX2, AVR_INPUT_AUX3, AVR_INPUT_AUX4,
        AVR_INPUT_AUX5, AVR_INPUT_AUX6, AVR_INPUT_AUX7, AVR_INPUT_AUX8,
        AVR_INPUT_BLUETOOTH, AVR_INPUT_IPODUSB, AVR_INPUT_PC, AVR_INPUT_UAW
    };
};

}
#endif
