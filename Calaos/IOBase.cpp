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
#include <IOBase.h>
#include <IOFactory.h>

using namespace Calaos;

/* Declare input class/type */
REGISTER_INPUT(InputTime, "time", TBOOL)
REGISTER_INPUT(InPlageHoraire, "time_range", TBOOL)
REGISTER_INPUT(TimeRange, "time_range", TBOOL)
REGISTER_INPUT(GpioInputSwitch, "switch", TBOOL)
REGISTER_INPUT(GpioInputSwitchLongPress, "switch_long", TINT)
REGISTER_INPUT(GpioInputSwitchTriple, "switch3", TINT)
REGISTER_INPUT(OWTemp, "temp", TINT)
REGISTER_INPUT(WIAnalog, "analog_in", TINT)
REGISTER_INPUT(WagoInputAnalog, "analog_in", TINT)
REGISTER_INPUT(WIDigitalBP, "switch", TBOOL)
REGISTER_INPUT(WIDigital, "switch", TBOOL)
REGISTER_INPUT(WagoInputSwitch, "switch", TBOOL)
REGISTER_INPUT(WIDigitalLong, "switch_long", TINT)
REGISTER_INPUT(WagoInputSwitchLongPress, "switch_long", TINT)
REGISTER_INPUT(WIDigitalTriple, "switch3", TINT)
REGISTER_INPUT(WagoInputSwitchTriple, "switch3", TINT)
REGISTER_INPUT(WITemp, "temp", TINT)
REGISTER_INPUT(WagoInputTemp, "temp", TINT)
REGISTER_INPUT(WebInputAnalog, "analog_in", TINT)
REGISTER_INPUT(WebInputTemp, "temp", TINT)
REGISTER_INPUT(WebInputString, "string_in", TSTRING)
REGISTER_INPUT(ZibaseAnalogIn, "analog_in", TINT)
REGISTER_INPUT(ZibaseDigitalIn, "switch", TBOOL)

/* Declare output class/type */
REGISTER_OUTPUT(OutputFake, "light", TBOOL)
REGISTER_OUTPUT(GpioOutputSwitch, "light", TBOOL)
REGISTER_OUTPUT(WOAnalog, "analog_out", TINT)
REGISTER_OUTPUT(WagoOutputAnalog, "analog_out", TINT)
REGISTER_OUTPUT(WODali, "light_dimmer", TSTRING)
REGISTER_OUTPUT(WagoOutputDimmer, "light_dimmer", TSTRING)
REGISTER_OUTPUT(WODaliRVB, "light_rgb", TSTRING)
REGISTER_OUTPUT(WagoOutputDimmerRGB, "light_rgb", TSTRING)
REGISTER_OUTPUT(WODigital, "light", TBOOL)
REGISTER_OUTPUT(WagoOutputLight, "light", TBOOL)
REGISTER_OUTPUT(WOVolet, "shutter", TBOOL)
REGISTER_OUTPUT(WagoOutputShutter, "shutter", TBOOL)
REGISTER_OUTPUT(WOVoletSmart, "shutter_smart", TSTRING)
REGISTER_OUTPUT(WagoOutputShutterSmart, "shutter_smart", TSTRING)
REGISTER_OUTPUT(X10Output, "light", TBOOL)
REGISTER_OUTPUT(WebOutputString, "string_out", TSTRING)
REGISTER_OUTPUT(WebOutputLightRGB, "light_rgb", TSTRING)

/* Declare InOut class/type */
REGISTER_INOUT(InputTimer, "timer", TBOOL)
REGISTER_INOUT(Scenario, "scenario", TBOOL)
REGISTER_INOUT(InternalInt, "var_int", TINT)
REGISTER_INOUT(InternalBool, "var_bool", TBOOL)
REGISTER_INOUT(InternalString, "var_string", TSTRING)
REGISTER_INOUT(AVReceiver, "avreceiver", TSTRING)

/* register music zones as Audio class */
REGISTER_INOUT(slim, "audio", TSTRING)
REGISTER_INOUT(Squeezebox, "audio", TSTRING)

/* register cameras as Camera class */
REGISTER_INOUT(Axis, "camera", TSTRING)
REGISTER_INOUT(Gadspot, "camera", TSTRING)
REGISTER_INOUT(Planet, "camera", TSTRING)
REGISTER_INOUT(StandardMjpeg, "camera", TSTRING)
REGISTER_INOUT(standard_mjpeg, "camera", TSTRING)
