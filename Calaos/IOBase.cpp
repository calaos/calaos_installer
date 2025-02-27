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
REGISTER_INPUT(InputTime, "time", TBOOL, "true")
REGISTER_INPUT(InPlageHoraire, "time_range", TBOOL, "true")
REGISTER_INPUT(TimeRange, "time_range", TBOOL, "true")
REGISTER_INPUT(GpioInputSwitch, "switch", TBOOL, "true")
REGISTER_INPUT(GpioInputSwitchLongPress, "switch_long", TINT, "2")
REGISTER_INPUT(GpioInputSwitchTriple, "switch3", TINT, "2")
REGISTER_INPUT(OWTemp, "temp", TINT, "21")
REGISTER_INPUT(WIAnalog, "analog_in", TINT, "42")
REGISTER_INPUT(WagoInputAnalog, "analog_in", TINT, "42")
REGISTER_INPUT(WIDigitalBP, "switch", TBOOL, "true")
REGISTER_INPUT(WIDigital, "switch", TBOOL, "true")
REGISTER_INPUT(WagoInputSwitch, "switch", TBOOL, "true")
REGISTER_INPUT(WIDigitalLong, "switch_long", TINT, "2")
REGISTER_INPUT(WagoInputSwitchLongPress, "switch_long", TINT, "2")
REGISTER_INPUT(WIDigitalTriple, "switch3", TINT, "2")
REGISTER_INPUT(WagoInputSwitchTriple, "switch3", TINT, "2")
REGISTER_INPUT(WITemp, "temp", TINT, "21")
REGISTER_INPUT(WagoInputTemp, "temp", TINT, "21")
REGISTER_INPUT(WebInputSwitch, "switch", TBOOL, "true")
REGISTER_INPUT(WebInputAnalog, "analog_in", TINT, "42")
REGISTER_INPUT(WebInputTemp, "temp", TINT, "21")
REGISTER_INPUT(WebInputString, "string_in", TSTRING, "Test")
REGISTER_INPUT(ZibaseTemp, "temp", TINT, "21")
REGISTER_INPUT(ZibaseAnalogIn, "analog_in", TINT, "42")
REGISTER_INPUT(ZibaseDigitalIn, "switch", TBOOL, "true")
REGISTER_INPUT(MySensorsInputAnalog, "analog_in", TINT, "42")
REGISTER_INPUT(MySensorsInputString, "string_in", TSTRING, "Test")
REGISTER_INPUT(MySensorsInputSwitch, "switch", TBOOL, "true")
REGISTER_INPUT(MySensorsInputSwitchLongPress, "switch_long", TINT, "2")
REGISTER_INPUT(MySensorsInputSwitchTriple, "switch3", TINT, "2")
REGISTER_INPUT(MySensorsInputTemp, "temp", TINT, "21")
REGISTER_INPUT(PingInputSwitch, "switch", TBOOL, "true")
REGISTER_INPUT(KNXInputSwitch, "switch", TBOOL, "true")
REGISTER_INPUT(KNXInputAnalog, "analog_in", TINT, "42")
REGISTER_INPUT(KNXInputSwitchLongPress, "switch_long", TINT, "2")
REGISTER_INPUT(KNXInputSwitchTriple, "switch3", TINT, "2")
REGISTER_INPUT(KNXInputTemp, "temp", TINT, "21")
REGISTER_INPUT(xPLInputTemp, "temp", TINT, "21")
REGISTER_INPUT(xPLInputSwitch, "switch", TBOOL, "true")
REGISTER_INPUT(xPLInputAnalog, "analog_in", TINT, "42")
REGISTER_INPUT(xPLInputString, "string_in", TSTRING, "Test")
REGISTER_INPUT(MqttInputTemp, "temp", TINT, "19")
REGISTER_INPUT(MqttInputAnalog, "analog_in", TINT, "19")
REGISTER_INPUT(MqttInputString, "string_in", TSTRING, "Test")
REGISTER_INPUT(MqttInputSwitch, "switch", TBOOL, "true")


/* Declare output class/type */
REGISTER_OUTPUT(OutputFake, "light", TBOOL, "true")
REGISTER_OUTPUT(GpioOutputSwitch, "light", TBOOL, "true")
REGISTER_OUTPUT(GpioOutputShutter, "shutter", TSTRING, "true")
REGISTER_OUTPUT(GpioOutputShutterSmart, "shutter_smart", TSTRING, "up")
REGISTER_OUTPUT(WOAnalog, "analog_out", TINT, "42")
REGISTER_OUTPUT(WagoOutputAnalog, "analog_out", TINT, "42")
REGISTER_OUTPUT(WebOutputAnalog, "analog_out", TINT, "42")
REGISTER_OUTPUT(WODali, "light_dimmer", TSTRING, "42")
REGISTER_OUTPUT(WagoOutputDimmer, "light_dimmer", TSTRING, "42")
REGISTER_OUTPUT(WODaliRVB, "light_rgb", TSTRING, "#00FF11")
REGISTER_OUTPUT(WagoOutputDimmerRGB, "light_rgb", TSTRING, "#00FF11")
REGISTER_OUTPUT(WODigital, "light", TBOOL, "true")
REGISTER_OUTPUT(WagoOutputLight, "light", TBOOL, "true")
REGISTER_OUTPUT(WOVolet, "shutter", TSTRING, "true")
REGISTER_OUTPUT(WagoOutputShutter, "shutter", TSTRING, "true")
REGISTER_OUTPUT(WOVoletSmart, "shutter_smart", TSTRING, "up")
REGISTER_OUTPUT(WagoOutputShutterSmart, "shutter_smart", TSTRING, "up")
REGISTER_OUTPUT(X10Output, "light", TBOOL, "true")
REGISTER_OUTPUT(WebOutputString, "string_out", TSTRING, "Test")
REGISTER_OUTPUT(WebOutputLight, "light", TBOOL, "true")
REGISTER_OUTPUT(WebOutputLightRGB, "light_rgb", TSTRING, "#00FF11")
REGISTER_OUTPUT(ZibaseDigitalOut, "light", TBOOL, "true")
REGISTER_OUTPUT(MySensorsOutputAnalog, "analog_out", TINT, "42")
REGISTER_OUTPUT(MySensorsOutputDimmer, "light_dimmer", TSTRING, "42")
REGISTER_OUTPUT(MySensorsOutputLight, "light", TBOOL, "true")
REGISTER_OUTPUT(MySensorsOutputLightRGB, "light_rgb", TSTRING, "#00FF11")
REGISTER_OUTPUT(MySensorsOutputShutter, "shutter", TSTRING, "true")
REGISTER_OUTPUT(MySensorsOutputShutterSmart, "shutter_smart", TSTRING, "#00FF11")
REGISTER_OUTPUT(MySensorsOutputString, "string_out", TSTRING, "Test")
REGISTER_OUTPUT(OLAOutputLightDimmer, "light_dimmer", TSTRING, "42")
REGISTER_OUTPUT(OLAOutputLightRGB, "light_rgb", TSTRING, "#00FF11")
REGISTER_OUTPUT(WOLOutputBool, "var_bool", TBOOL, "true")
REGISTER_OUTPUT(KNXOutputLight, "light", TBOOL, "true")
REGISTER_OUTPUT(KNXOutputAnalog, "analog_out", TINT, "42")
REGISTER_OUTPUT(KNXOutputLightDimmer, "light_dimmer", TSTRING, "42")
REGISTER_OUTPUT(KNXOutputLightRGB, "light_rgb", TSTRING, "#00FF11")
REGISTER_OUTPUT(KNXOutputShutter, "shutter", TSTRING, "true")
REGISTER_OUTPUT(KNXOutputShutterSmart, "shutter_smart", TSTRING, "up")
REGISTER_OUTPUT(HueOutputLightRGB, "light_rgb", TSTRING, "#00FF11")
REGISTER_OUTPUT(xPLOutputSwitch, "light", TBOOL, "true")
REGISTER_OUTPUT(xPLOutputAnalog, "analog_out", TINT, "42")
REGISTER_OUTPUT(xPLOutputString, "string_out", TSTRING, "Test")
REGISTER_OUTPUT(MqttOutputLight, "light", TBOOL, "true")
REGISTER_OUTPUT(MqttOutputLightDimmer, "light_dimmer", TSTRING, "42")
REGISTER_OUTPUT(MqttOutputLightRGB, "light_rgb", TSTRING, "#FF00FF")
REGISTER_OUTPUT(MqttOutputAnalog, "analog_out", TINT, "42")

/* Declare InOut class/type */
REGISTER_INOUT(InputTimer, "timer", TSTRING, "true")
REGISTER_INOUT(Scenario, "scenario", TBOOL, "true")
REGISTER_INOUT(InternalInt, "var_int", TINT, "42")
REGISTER_INOUT(InternalBool, "var_bool", TBOOL, "true")
REGISTER_INOUT(InternalString, "var_string", TSTRING, "Test")
REGISTER_INOUT(AVReceiver, "avreceiver", TSTRING, "Test")

/* register music zones as Audio class */
REGISTER_INOUT(slim, "audio", TSTRING, "Test")
REGISTER_INOUT(Squeezebox, "audio", TSTRING, "Test")
REGISTER_INOUT(roon, "audio", TSTRING, "Test")

/* register cameras as Camera class */
REGISTER_INOUT(Axis, "camera", TSTRING, "0")
REGISTER_INOUT(Gadspot, "camera", TSTRING, "0")
REGISTER_INOUT(Planet, "camera", TSTRING, "0")
REGISTER_INOUT(StandardMjpeg, "camera", TSTRING, "0")
REGISTER_INOUT(standard_mjpeg, "camera", TSTRING, "0")
REGISTER_INOUT(SynoSurveillanceStation, "camera", TSTRING, "0")
REGISTER_INOUT(Foscam, "camera", TSTRING, "0")


void IOBase::buildInputSourcesList()
{
    if (gui_type != "avreceiver") return;

    if (get_param("model") == "pioneer")
    {
        source_names[IOBase::AVR_INPUT_BD] = "Blu-ray Disc";
        source_names[IOBase::AVR_INPUT_DVD] = "DVD";
        source_names[IOBase::AVR_INPUT_TVSAT] = "TV/Sat";
        source_names[IOBase::AVR_INPUT_DVRBDR] = "DVR/BDR";
        source_names[IOBase::AVR_INPUT_VIDEO_1] = "Video 1";
        source_names[IOBase::AVR_INPUT_VIDEO_2] = "Video 2";
        source_names[IOBase::AVR_INPUT_HDMI_1] = "HDMI 1";
        source_names[IOBase::AVR_INPUT_HDMI_2] = "HDMI 2";
        source_names[IOBase::AVR_INPUT_HDMI_3] = "HDMI 3";
        source_names[IOBase::AVR_INPUT_HDMI_4] = "HDMI 4";
        source_names[IOBase::AVR_INPUT_HDMI_5] = "HDMI 5";
        source_names[IOBase::AVR_INPUT_HDMI_6] = "HDMI 6";
        source_names[IOBase::AVR_INPUT_NETRADIO] = "Home Media Gallery";
        source_names[IOBase::AVR_INPUT_IPOD] = "iPod/USB";
        source_names[IOBase::AVR_INPUT_CD] = "CD";
        source_names[IOBase::AVR_INPUT_CDRTAPE] = "CD-R/Tape";
        source_names[IOBase::AVR_INPUT_TUNER] = "Tuner";
        source_names[IOBase::AVR_INPUT_PHONO] = "Phono";
        source_names[IOBase::AVR_INPUT_MULTIIN] = "Multi Channel In";
        source_names[IOBase::AVR_INPUT_APORT] = "Adapter Port";
        source_names[IOBase::AVR_INPUT_SIRIUS] = "Sirius";
    }
    else if (get_param("model") == "denon")
    {
        source_names[IOBase::AVR_INPUT_DVD] = "DVD";
        source_names[IOBase::AVR_INPUT_BD] = "Blu-ray Disc";
        source_names[IOBase::AVR_INPUT_CD] = "CD";
        source_names[IOBase::AVR_INPUT_PHONO] = "Phono";
        source_names[IOBase::AVR_INPUT_TUNER] = "Tuner";
        source_names[IOBase::AVR_INPUT_TV] = "TV";
        source_names[IOBase::AVR_INPUT_SAT] = "Sat/CBL";
        source_names[IOBase::AVR_INPUT_DVRBDR] = "DVR";
        source_names[IOBase::AVR_INPUT_GAME_1] = "Game";
        source_names[IOBase::AVR_INPUT_GAME_2] = "Game 2";
        source_names[IOBase::AVR_INPUT_AUX] = "V. Aux";
        source_names[IOBase::AVR_INPUT_DOCK] = "Dock";
        source_names[IOBase::AVR_INPUT_HDRADIO] = "HD Radio";
        source_names[IOBase::AVR_INPUT_IPOD] = "iPod";
        source_names[IOBase::AVR_INPUT_USB] = "Net/USB";
        source_names[IOBase::AVR_INPUT_RHAPSODY] = "Rhapsody";
        source_names[IOBase::AVR_INPUT_NAPSTER] = "Napster";
        source_names[IOBase::AVR_INPUT_PANDORA] = "Pandora";
        source_names[IOBase::AVR_INPUT_LASTFM] = "LastFM";
        source_names[IOBase::AVR_INPUT_FLICKR] = "Flickr";
        source_names[IOBase::AVR_INPUT_FAV] = "Favorites";
        source_names[IOBase::AVR_INPUT_NETRADIO] = "IRadio";
        source_names[IOBase::AVR_INPUT_SERVER] = "Server";
    }
    else if (get_param("model") == "onkyo")
    {
        source_names[IOBase::AVR_INPUT_DVD] = "DVD / Bluray";
        source_names[IOBase::AVR_INPUT_CD] = "CD";
        source_names[IOBase::AVR_INPUT_PHONO] = "Phono";
        source_names[IOBase::AVR_INPUT_TUNER] = "Tuner";
        source_names[IOBase::AVR_INPUT_VIDEO_1] = "VCR/DVR";
        source_names[IOBase::AVR_INPUT_VIDEO_2] = "Sat/CBL";
        source_names[IOBase::AVR_INPUT_VIDEO_3] = "Game/TV";
        source_names[IOBase::AVR_INPUT_VIDEO_4] = "Aux. 1";
        source_names[IOBase::AVR_INPUT_VIDEO_5] = "Aux. 2";
        source_names[IOBase::AVR_INPUT_VIDEO_6] = "PC";
        source_names[IOBase::AVR_INPUT_VIDEO_7] = "Video 7";
        source_names[IOBase::AVR_INPUT_TV] = "TV/Tape";
        source_names[IOBase::AVR_INPUT_SERVER] = "Music Server DLNA";
        source_names[IOBase::AVR_INPUT_NETRADIO] = "Internet Radio";
        source_names[IOBase::AVR_INPUT_USB] = "USB Front";
        source_names[IOBase::AVR_INPUT_USB2] = "USB Rear";
        source_names[IOBase::AVR_INPUT_NETWORK] = "Network";
        source_names[IOBase::AVR_INPUT_APORT] = "Universal Port";
        source_names[IOBase::AVR_INPUT_MULTIIN] = "Multi Ch. In";
    }
    else if (get_param("model") == "marantz")
    {
        source_names[IOBase::AVR_INPUT_PHONO] = "Phono";
        source_names[IOBase::AVR_INPUT_CD] = "CD";
        source_names[IOBase::AVR_INPUT_DVD] = "DVD";
        source_names[IOBase::AVR_INPUT_BD] = "Bluray";
        source_names[IOBase::AVR_INPUT_TV] = "TV";
        source_names[IOBase::AVR_INPUT_SAT] = "Sat/CBL";
        source_names[IOBase::AVR_INPUT_TVSAT] = "Sat";
        source_names[IOBase::AVR_INPUT_VCR] = "VCR";
        source_names[IOBase::AVR_INPUT_GAME_1] = "Game";
        source_names[IOBase::AVR_INPUT_AUX] = "V. Aux";
        source_names[IOBase::AVR_INPUT_TUNER] = "Tuner";
        source_names[IOBase::AVR_INPUT_HDRADIO] = "HD Radio";
        source_names[IOBase::AVR_INPUT_RHAPSODY] = "Rhapsody";
        source_names[IOBase::AVR_INPUT_NAPSTER] = "Napster";
        source_names[IOBase::AVR_INPUT_PANDORA] = "Pandora";
        source_names[IOBase::AVR_INPUT_LASTFM] = "LastFM";
        source_names[IOBase::AVR_INPUT_FLICKR] = "Flickr";
        source_names[IOBase::AVR_INPUT_FAV] = "Favorites";
        source_names[IOBase::AVR_INPUT_NETRADIO] = "IRadio";
        source_names[IOBase::AVR_INPUT_SERVER] = "Server";
        source_names[IOBase::AVR_INPUT_CDRTAPE] = "CDR";
        source_names[IOBase::AVR_INPUT_AUX1] = "Aux 1";
        source_names[IOBase::AVR_INPUT_AUX2] = "Aux 2";
        source_names[IOBase::AVR_INPUT_USB] = "Net/USB";
        source_names[IOBase::AVR_INPUT_IPOD] = "USB/iPod";
    }
    else if (get_param("model") == "yamaha")
    {
        source_names[IOBase::AVR_INPUT_HDMI_1] = "HDMI 1";
        source_names[IOBase::AVR_INPUT_HDMI_2] = "HDMI 2";
        source_names[IOBase::AVR_INPUT_HDMI_3] = "HDMI 3";
        source_names[IOBase::AVR_INPUT_HDMI_4] = "HDMI 4";
        source_names[IOBase::AVR_INPUT_HDMI_5] = "HDMI 5";
        source_names[IOBase::AVR_INPUT_TUNER] = "Tuner";
        source_names[IOBase::AVR_INPUT_PHONO] = "Phono";
        source_names[IOBase::AVR_INPUT_VIDEO_1] = "Video 1";
        source_names[IOBase::AVR_INPUT_VIDEO_2] = "Video 2";
        source_names[IOBase::AVR_INPUT_VIDEO_3] = "Video 3";
        source_names[IOBase::AVR_INPUT_VIDEO_4] = "Video 4";
        source_names[IOBase::AVR_INPUT_VIDEO_5] = "Video 5";
        source_names[IOBase::AVR_INPUT_VIDEO_6] = "Video 6";
        source_names[IOBase::AVR_INPUT_AUX] = "V-Aux";
        source_names[IOBase::AVR_INPUT_AUX1] = "Audio 1";
        source_names[IOBase::AVR_INPUT_AUX2] = "Audio 2";
        source_names[IOBase::AVR_INPUT_DOCK] = "Dock";
        source_names[IOBase::AVR_INPUT_IPOD] = "iPod";
        source_names[IOBase::AVR_INPUT_BLUETOOTH] = "Bluetooth";
        source_names[IOBase::AVR_INPUT_NETWORK] = "Network";
        source_names[IOBase::AVR_INPUT_NAPSTER] = "Napster";
        source_names[IOBase::AVR_INPUT_NETRADIO] = "Net Radio";
        source_names[IOBase::AVR_INPUT_USB] = "USB";
        source_names[IOBase::AVR_INPUT_IPODUSB] = "iPod (USB)";
        source_names[IOBase::AVR_INPUT_PC] = "PC";
        source_names[IOBase::AVR_INPUT_UAW] = "UAW";
    }
}

bool IOBase::getDemoValueBool()
{
    if (scriptDemoValue == "true")
        return true;
    return false;
}

double IOBase::getDemoValueDouble()
{
    double v = 0;
    Utils::from_string(scriptDemoValue, v);
    return v;
}

string IOBase::getDemoValueString()
{
    return scriptDemoValue;
}
