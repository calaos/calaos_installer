#ifndef DEVICEEDIT_HOUSE_H
#define DEVICEEDIT_HOUSE_H

// In a header: moc reads the "//" of the namespace URL in a raw string as a comment.

namespace
{

// Two thermostats as the wizard writes them, the first with its owned IOs, rules on
// the device and on an owned IO, and IOs of each kind to link.
const char *kIo = R"(<?xml version="1.0"?>
<calaos:ioconfig xmlns:calaos="http://www.calaos.fr">
<calaos:home format="2">
<calaos:room name="Salon" type="salon" hits="0">
  <calaos:input id="temp_salon" type="MqttInputTemp" name="Salon temperature" host="h" port="1883" topic_sub="s/t"/>
  <calaos:input id="hum_salon" type="MqttInputAnalog" name="Salon humidity" host="h" port="1883" topic_sub="s/h"/>
  <calaos:input id="switch_salon" type="WIDigitalBP" name="Salon switch" host="h" port="502" var="1"/>
  <calaos:output id="relay_heat" type="WODigital" name="Heating relay" host="h" port="502" var="2"/>
  <calaos:output id="relay_cool" type="WODigital" name="Cooling relay" host="h" port="502" var="3"/>
  <calaos:output id="relay_spare" type="WODigital" name="Spare relay" host="h" port="502" var="4"/>
  <calaos:internal id="flag" type="InternalBool" name="Flag"/>
  <calaos:device id="th" type="climate" impl="thermostat" name="Chauffage salon">
    <calaos:internal id="th_target_temp" type="InternalInt" name="Chauffage salon setpoint" save="true" step="0.5" unit="°C" value="20" value_max="30" value_min="7" visible="false"/>
    <calaos:internal id="th_target_temp_low" type="InternalInt" name="Chauffage salon low setpoint" save="true" value="19" visible="false"/>
    <calaos:internal id="th_target_temp_high" type="InternalInt" name="Chauffage salon high setpoint" save="true" value="25" visible="false"/>
    <calaos:internal id="th_action" type="InternalString" name="Chauffage salon action" visible="false"/>
    <calaos:link name="current_temp" io="temp_salon"/>
    <calaos:link name="target_temp" io="th_target_temp"/>
    <calaos:link name="target_temp_low" io="th_target_temp_low"/>
    <calaos:link name="target_temp_high" io="th_target_temp_high"/>
    <calaos:link name="action" io="th_action"/>
    <calaos:link name="output_heat" io="relay_heat"/>
    <calaos:link name="output_cool" io="relay_cool"/>
    <calaos:param name="emitter" value="heat_pump"/>
    <calaos:param name="hysteresis" value="0.5"/>
    <calaos:param name="min_on_time" value="600"/>
    <calaos:param name="min_off_time" value="300"/>
    <calaos:param name="min_range" value="2"/>
    <calaos:param name="sensor_timeout" value="9000"/>
  </calaos:device>
</calaos:room>
<calaos:room name="Cave" type="cave" hits="0">
  <calaos:input id="temp_cave" type="WITemp" name="Cave temperature" host="h" port="502" var="5"/>
  <calaos:output id="relay_cave" type="WODigital" name="Cave relay" host="h" port="502" var="6"/>
  <calaos:input id="mode_state" type="MqttInputString" name="AC mode" host="h" port="1883" topic_sub="ac/mode"/>
  <calaos:output id="mode_cmd" type="MqttOutputString" name="AC mode command" host="h" port="1883" topic_pub="ac/set" topic_sub="ac/set"/>
  <calaos:output id="mode_other" type="MqttOutputString" name="Other mode" host="h" port="1883" topic_pub="o/set" topic_sub="o/mode"/>
  <calaos:device id="px" type="climate" impl="proxy" name="Cave AC">
    <calaos:link name="mode" io="mode_state" command_io="mode_cmd"/>
  </calaos:device>
  <calaos:device id="th2" type="climate" impl="thermostat" name="Cave thermostat">
    <calaos:internal id="th2_target_temp" type="InternalInt" name="Cave setpoint" save="true" value="12" visible="false"/>
    <calaos:internal id="th2_action" type="InternalString" name="Cave action" visible="false"/>
    <calaos:link name="current_temp" io="temp_cave"/>
    <calaos:link name="target_temp" io="th2_target_temp"/>
    <calaos:link name="action" io="th2_action"/>
    <calaos:link name="output_heat" io="relay_cave"/>
  </calaos:device>
</calaos:room>
</calaos:home>
</calaos:ioconfig>
)";

const char *kRules = R"(<?xml version="1.0"?>
<calaos:rules xmlns:calaos="http://www.calaos.fr">
  <calaos:rule name="setpoint" type="climate" spacialType="">
    <calaos:condition>
      <calaos:input id="flag" oper="==" val="true"/>
    </calaos:condition>
    <calaos:action>
      <calaos:output id="th_target_temp" val="21"/>
    </calaos:action>
  </calaos:rule>
  <calaos:rule name="mode" type="climate" spacialType="">
    <calaos:condition>
      <calaos:input id="th_action" oper="==" val="heating"/>
    </calaos:condition>
    <calaos:action>
      <calaos:output id="th" val="heat"/>
    </calaos:action>
  </calaos:rule>
</calaos:rules>
)";

}

#endif // DEVICEEDIT_HOUSE_H
