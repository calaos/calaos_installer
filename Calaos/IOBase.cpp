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
REGISTER_INPUT(InputTime)
REGISTER_INPUT(InputTimer)
REGISTER_INPUT(GpioInputSwitch)
REGISTER_INPUT(GpioInputSwitchLongPress)
REGISTER_INPUT(GpioInputSwitchTriple)
REGISTER_INPUT(InPlageHoraire)
REGISTER_INPUT_USERTYPE(TimeRange, InPlageHoraire)
REGISTER_INPUT_USERTYPE(InternalInt, Internal)
REGISTER_INPUT_USERTYPE(InternalBool, Internal)
REGISTER_INPUT_USERTYPE(InternalString, Internal)
REGISTER_INPUT(OWTemp)
REGISTER_INPUT(Scenario)
REGISTER_INPUT(WIAnalog)
REGISTER_INPUT_USERTYPE(WagoInputAnalog, WIAnalog)
REGISTER_INPUT(WIDigitalBP)
REGISTER_INPUT_USERTYPE(WIDigital, WIDigitalBP)
REGISTER_INPUT_USERTYPE(WagoInputSwitch, WIDigitalBP)
REGISTER_INPUT(WIDigitalLong)
REGISTER_INPUT_USERTYPE(WagoInputSwitchLongPress, WIDigitalLong)
REGISTER_INPUT(WIDigitalTriple)
REGISTER_INPUT_USERTYPE(WagoInputSwitchTriple, WIDigitalTriple)
REGISTER_INPUT(WITemp)
REGISTER_INPUT_USERTYPE(WagoInputTemp, WITemp)
REGISTER_INPUT(WebInputAnalog)
REGISTER_INPUT(WebInputTemp)
REGISTER_INPUT(ZibaseAnalogIn)
REGISTER_INPUT(ZibaseDigitalIn)

/* Declare output class/type */
REGISTER_OUTPUT(OutputFake)
REGISTER_OUTPUT(GpioOutputSwitch)
REGISTER_OUTPUT_USERTYPE(AVReceiver, IOAVReceiver)
REGISTER_OUTPUT(WOAnalog)
REGISTER_OUTPUT_USERTYPE(WagoOutputAnalog, WOAnalog)
REGISTER_OUTPUT(WODali)
REGISTER_OUTPUT_USERTYPE(WagoOutputDimmer, WODali)
REGISTER_OUTPUT(WODaliRVB)
REGISTER_OUTPUT_USERTYPE(WagoOutputDimmerRGB, WODaliRVB)
REGISTER_OUTPUT(WODigital)
REGISTER_OUTPUT_USERTYPE(WagoOutputLight, WODigital)
REGISTER_OUTPUT(WOVolet)
REGISTER_OUTPUT_USERTYPE(WagoOutputShutter, WOVolet)
REGISTER_OUTPUT(WOVoletSmart)
REGISTER_OUTPUT_USERTYPE(WagoOutputShutterSmart, WOVoletSmart)
REGISTER_OUTPUT(X10Output)
REGISTER_OUTPUT(WebOutputString)

/* register music zones as Audio class */
REGISTER_OUTPUT_USERTYPE(slim, Audio)
REGISTER_OUTPUT_USERTYPE(Squeezebox, Audio)

/* register cameras as Camera class */
REGISTER_OUTPUT_USERTYPE(Axis, Camera)
REGISTER_OUTPUT_USERTYPE(Gadspot, Camera)
REGISTER_OUTPUT_USERTYPE(Planet, Camera)
REGISTER_OUTPUT_USERTYPE(StandardMjpeg, Camera)
REGISTER_OUTPUT_USERTYPE(standard_mjpeg, Camera)
