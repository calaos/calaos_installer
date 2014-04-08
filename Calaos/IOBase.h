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

using namespace std;
using namespace Utils;

namespace Calaos
{

class Horaire
{
public:
    std::string shour, smin, ssec;
    std::string ehour, emin, esec;

    Horaire():shour("0"),smin("0"),ssec("0"),ehour("0"),emin("0"),esec("0"){;}
};

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

    enum { IO_INPUT, IO_OUTPUT, IO_BOTH };

    IOBase(Params &p, string g, DATA_TYPE d, int i):
        param(p),
        gui_type(g),
        basic_type(d),
        io_type(i)
    { }
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

    vector<Horaire> plg_lundi;
    vector<Horaire> plg_mardi;
    vector<Horaire> plg_mercredi;
    vector<Horaire> plg_jeudi;
    vector<Horaire> plg_vendredi;
    vector<Horaire> plg_samedi;
    vector<Horaire> plg_dimanche;

    void clear()
    {
            plg_lundi.clear();
            plg_mardi.clear();
            plg_mercredi.clear();
            plg_jeudi.clear();
            plg_vendredi.clear();
            plg_samedi.clear();
            plg_dimanche.clear();
    }
};

}
#endif
