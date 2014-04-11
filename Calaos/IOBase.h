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

using namespace std;
using namespace Utils;

namespace Calaos
{

class IOBase
{
private:
    //we store all params here
    Params param;

    double dvalue;
    bool bvalue;
    string svalue;

    string gui_type;
    DATA_TYPE basic_type;

    int io_type;

public:

    enum { IO_NONE = 0, IO_INPUT, IO_OUTPUT, IO_BOTH };

    IOBase(Params &p, string g, DATA_TYPE d, int i):
        param(p),
        gui_type(g),
        basic_type(d),
        io_type(i)
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
};

}
#endif
