#include "MySensors.h"
#include <string>

using namespace std;

string MySensors::getSensorString(int s)
{
    switch (s) {
        case S_DOOR: return "Door";
        case S_MOTION: return "Motion";
        case S_SMOKE: return "Smoke";
        case S_LIGHT: return "Light";
        case S_DIMMER: return "Dimmer";
        case S_COVER: return "Cover";
        case S_TEMP: return "Temp";
        case S_HUM: return "Hum";
        case S_BARO: return "Baro";
        case S_WIND: return "Wind";
        case S_RAIN: return "Rain";
        case S_UV: return "UV";
        case S_WEIGHT: return "Weight";
        case S_POWER: return "Power";
        case S_HEATER: return "Heater";
        case S_DISTANCE: return "Distance";
        case S_LIGHT_LEVEL: return "Light_level";
        case S_ARDUINO_NODE: return "Arduino_node";
        case S_ARDUINO_REPEATER_NODE: return "Arduino repeater node";
        case S_LOCK: return "Lock";
        case S_IR: return "Ir";
        case S_WATER: return "Water";
        case S_AIR_QUALITY: return "Air quality";
        case S_CUSTOM: return "Custom";
        case S_DUST: return "Dust";
        case S_SCENE_CONTROLLER: return "Scene controller";
        case S_RGB_LIGHT: return "Rgb light";
        case S_RGBW_LIGHT: return "Rgbw light";
        case S_COLOR_SENSOR: return "Color sensor";
        case S_HVAC: return "Hvac";
        case S_MULTIMETER: return "Multimeter";
        case S_SPRINKLER: return "Sprinkler";
        case S_WATER_LEAK: return "Water leak";
        case S_SOUND: return "Sound";
        case S_VIBRATION: return "Vibration";
        case S_MOISTURE: return "Moisture";
        case S_INFO: return "Info";
        case S_GAS: return "Gas";
        case S_GPS: return "GPS";
        case S_WATER_QUALITY: return "Water quality";
        default: return "Unknown";
    }
}
