/******************************************************************************
**  Copyright (c) 2007-2008, Calaos. All Rights Reserved.
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
//-----------------------------------------------------------------------------
#ifndef S_IOBASE_H
#define S_IOBASE_H
//-----------------------------------------------------------------------------
#include <iostream>
#include <Utils.h>
#include <Params.h>
//-----------------------------------------------------------------------------
using namespace std;
using namespace Utils;
//-----------------------------------------------------------------------------
namespace Calaos
{

class IOBase
{
        private:
                //we store all params here
                Params param;

        public:
                IOBase(Params &p): param(p) { /* nothing */ }
                virtual ~IOBase() { /* nothing */ }

                virtual DATA_TYPE get_type() = 0;

                virtual void set_param(std::string opt, std::string val)
                        { param.Add(opt, val); }
                virtual std::string get_param(std::string opt)
                        { return param[opt]; }
                virtual Params &get_params()
                        { return param; }


                static bool isCameraType(string type)
                {
                        if (type == "axis" || type == "gadspot" ||
                            type == "planet" || type == "standard_mjpeg")
                                return true;

                        return false;
                }

                static bool isAudioType(string type)
                {
                        if (type == "slim")
                                return true;

                        return false;
                }
};

class Input: public IOBase
{
        public:
                Input(Params &p): IOBase(p) {}
};

class Output: public IOBase
{
        public:
                Output(Params &p): IOBase(p) {}
};

class InputTime: public Input
{
        public:
                InputTime(Params &p): Input(p) {}
                virtual DATA_TYPE get_type() { return TBOOL; }
};

class InputTimer: public Input, public Output
{
        public:
                InputTimer(Params &p): Input(p), Output(p) {}
                virtual DATA_TYPE get_type() { return TBOOL; }

                virtual void set_param(std::string opt, std::string val)
                        { Input::set_param(opt, val); }
                virtual std::string get_param(std::string opt)
                        { return Input::get_param(opt); }
                virtual Params &get_params()
                        { return Input::get_params(); }
};

class WIDigitalBP: public Input
{
        public:
                WIDigitalBP(Params &p): Input(p) {}
                virtual DATA_TYPE get_type() { return TBOOL; }
};

class WIDigitalTriple: public Input
{
        public:
                WIDigitalTriple(Params &p): Input(p) {}
                virtual DATA_TYPE get_type() { return TINT; }
};

class WITemp: public Input
{
        public:
                WITemp(Params &p): Input(p) {}
                virtual DATA_TYPE get_type() { return TINT; }
};

class Scenario: public Input, public Output
{
        public:
                Scenario(Params &p): Input(p), Output(p) {}
                virtual DATA_TYPE get_type() { return TBOOL; }

                virtual void set_param(std::string opt, std::string val)
                        { Input::set_param(opt, val); }
                virtual std::string get_param(std::string opt)
                        { return Input::get_param(opt); }
                virtual Params &get_params()
                        { return Input::get_params(); }
};

class Horaire
{
        public:
                std::string shour, smin, ssec;
                std::string ehour, emin, esec;

                Horaire():shour("0"),smin("0"),ssec("0"),ehour("0"),emin("0"),esec("0"){;}
};
class InPlageHoraire: public Input
{
        public:
                InPlageHoraire(Params &p): Input(p) {}
                virtual DATA_TYPE get_type() { return TBOOL; }

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

class Internal: public Input, public Output
{
        public:
                Internal(Params &p): Input(p), Output(p) {}
                virtual DATA_TYPE get_type()
                {
                        if (Input::get_param("type") == "InternalBool") return TBOOL;
                        if (Input::get_param("type") == "InternalInt") return TINT;
                        if (Input::get_param("type") == "InternalString") return TSTRING;

                        return TBOOL;
                }

                virtual void set_param(std::string opt, std::string val)
                        { Input::set_param(opt, val); }
                virtual std::string get_param(std::string opt)
                        { return Input::get_param(opt); }
                virtual Params &get_params()
                        { return Input::get_params(); }
};

class OutputFake: public Output
{
        public:
                OutputFake(Params &p): Output(p) {}
                virtual DATA_TYPE get_type() { return TBOOL; }
};

class OutTouchscreen: public Output
{
        public:
                OutTouchscreen(Params &p): Output(p) {}
                virtual DATA_TYPE get_type() { return TSTRING; }
};

class WODigital: public Output
{
        public:
                WODigital(Params &p): Output(p) {}
                virtual DATA_TYPE get_type() { return TBOOL; }
};

class WONeon: public Output
{
        public:
                WONeon(Params &p): Output(p) {}
                virtual DATA_TYPE get_type() { return TSTRING; }
};

class WOVolet: public Output
{
        public:
                WOVolet(Params &p): Output(p) {}
                virtual DATA_TYPE get_type() { return TSTRING; }
};

class WOVoletSmart: public Output
{
        public:
                WOVoletSmart(Params &p): Output(p) {}
                virtual DATA_TYPE get_type() { return TSTRING; }
};

class WODali: public Output
{
        public:
                WODali(Params &p): Output(p) {}
                virtual DATA_TYPE get_type() { return TSTRING; }
};

class WODaliRVB: public Output
{
        public:
                WODaliRVB(Params &p): Output(p) {}
                virtual DATA_TYPE get_type() { return TSTRING; }
};

class Audio: public Input, public Output
{
        public:
                Audio(Params &p): Input(p), Output(p) {}
                virtual DATA_TYPE get_type() { return TSTRING; }

                virtual void set_param(std::string opt, std::string val)
                        { Input::set_param(opt, val); }
                virtual std::string get_param(std::string opt)
                        { return Input::get_param(opt); }
                virtual Params &get_params()
                        { return Input::get_params(); }
};

class Camera: public Input, public Output
{
        public:
                Camera(Params &p): Input(p), Output(p) {}
                virtual DATA_TYPE get_type() { return TSTRING; }

                virtual void set_param(std::string opt, std::string val)
                        { Input::set_param(opt, val); }
                virtual std::string get_param(std::string opt)
                        { return Input::get_param(opt); }
                virtual Params &get_params()
                        { return Input::get_params(); }
};

}
#endif
