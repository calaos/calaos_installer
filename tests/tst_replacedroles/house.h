#ifndef REPLACEDROLES_HOUSE_H
#define REPLACEDROLES_HOUSE_H

// In a header: moc reads the "//" of the namespace URL in a raw string as a comment.

namespace
{

// A floor zone on its valve, a thermostat on two outputs, spare relays.
const char *kIo = R"(<?xml version="1.0"?>
<calaos:ioconfig xmlns:calaos="http://www.calaos.fr">
<calaos:home format="2">
<calaos:room name="Salon" type="salon" hits="0">
  <calaos:input id="temp_salon" type="MqttInputTemp" name="Salon temperature" host="h" port="1883" topic_sub="s/t"/>
  <calaos:output id="valve_salon" type="WODigital" name="Salon valve" host="h" port="502" var="1"/>
  <calaos:output id="relay_heat" type="WODigital" name="Heating relay" host="h" port="502" var="2"/>
  <calaos:output id="relay_cool" type="WODigital" name="Cooling relay" host="h" port="502" var="3"/>
  <calaos:output id="relay_spare" type="WODigital" name="Spare relay" host="h" port="502" var="4"/>
  <calaos:device id="zone" type="climate" impl="thermostat" name="Salon floor">
    <calaos:internal id="zone_target_temp" type="InternalInt" name="Salon floor setpoint" save="true" value="20" visible="false"/>
    <calaos:internal id="zone_action" type="InternalString" name="Salon floor action" visible="false"/>
    <calaos:link name="current_temp" io="temp_salon"/>
    <calaos:link name="target_temp" io="zone_target_temp"/>
    <calaos:link name="action" io="zone_action"/>
    <calaos:link name="output" io="valve_salon"/>
    <calaos:param name="emitter" value="floor"/>
    <calaos:param name="modes" value="off,heat,cool"/>
  </calaos:device>
  <calaos:device id="th" type="climate" impl="thermostat" name="Salon heat pump">
    <calaos:internal id="th_target_temp" type="InternalInt" name="Salon heat pump setpoint" save="true" value="20" visible="false"/>
    <calaos:link name="current_temp" io="temp_salon"/>
    <calaos:link name="target_temp" io="th_target_temp"/>
    <calaos:link name="output_heat" io="relay_heat"/>
    <calaos:link name="output_cool" io="relay_cool"/>
  </calaos:device>
</calaos:room>
</calaos:home>
</calaos:ioconfig>
)";

}

#endif // REPLACEDROLES_HOUSE_H
