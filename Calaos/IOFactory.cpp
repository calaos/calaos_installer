/******************************************************************************
*  Copyright (c) 2007-2008, Calaos. All Rights Reserved.
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
#include <IOFactory.h>
//-----------------------------------------------------------------------------
using namespace Calaos;
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
Input *IOFactory::CreateInput(std::string type, Params &params)
{
        Input *in = NULL;

        if (type == "InputTime")
        {
                in = new InputTime(params);
        }
        else if (type == "InputTimer")
        {
                in = new InputTimer(params);
        }
        else if (type == "WIDigitalBP" || type == "WIDigital" || type == "WIDigitalTriple" || type == "WIDigitalLong")
        {
                in = new WIDigital(params);
        }
        else if (type == "GpioInputSwitch" || type == "GpioInputSwitchLongPress" || type == "GpioInputSwitchTriple")
        {
                printf("INPUT : %s\n", type.c_str());
                in = new GpioInput(params);
        }
        else if (type == "WITemp" || type == "OWTemp")
        {
                in = new WITemp(params);
        }
        else if (type == "WIAnalog" || type == "WebAnalogIn")
        {
                in = new WIAnalog(params);
        }
        else if (type == "scenario")
        {
                in = new Scenario(params);
        }
        else if (type == "InPlageHoraire")
        {
                in = new InPlageHoraire(params);
        }
        else if (type == "internbool" || type == "internalbool" ||
                 type == "InternalBoolOutput" || type == "InternalBoolInput" ||
                 type == "InternalBool")
        {
                type = "InternalBool";
                params.Add("type", type);

                in = new Internal(params);
        }
        else if (type == "internint" || type == "internalint" ||
                 type == "InternalIntOutput" || type == "InternalIntInput" ||
                 type == "InternalInt")
        {
                type = "InternalInt";
                params.Add("type", type);

                in = new Internal(params);
        }
        else if (type == "internstring" || type == "internalstring" ||
                 type == "InternalStringOutput" || type == "InternalStringInput" ||
                 type == "InternalString")
        {
                type = "InternalString";
                params.Add("type", type);

                in = new Internal(params);
        }

        return in;
}
//-----------------------------------------------------------------------------
Output *IOFactory::CreateOutput(std::string type, Params &params)
{
        Output *out = NULL;

        if (type == "OutputFake")
        {
                out = new OutputFake(params);
        }
        else if (type == "OutTouchscreen")
        {
                out = new OutTouchscreen(params);
        }
        else if (type == "WODigital")
        {
                out = new WODigital(params);
        }
        else if (type == "WONeon")
        {
                out = new WONeon(params);
        }
        else if (type == "WOVolet")
        {
                out = new WOVolet(params);
        }
        else if (type == "WOVoletSmart")
        {
                out = new WOVoletSmart(params);
        }
        else if (type == "WODali")
        {
                out = new WODali(params);
        }
        else if (type == "WODaliRVB")
        {
                out = new WODaliRVB(params);
        }
        else if (type == "Audio")
        {
                out = new Audio(params);
        }
        else if (type == "Camera")
        {
                out = new Camera(params);
        }
        else if (type == "WOAnalog")
        {
                out = new WOAnalog(params);
        }
        else if (type == "GpioOutputSwitch")
        {
                out = new WODigital(params);
        }

        return out;
}
//-----------------------------------------------------------------------------
