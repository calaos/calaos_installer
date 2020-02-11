
#AVReceiver
AVReceiver object to control network amplifier

##Parameters of AVReceiver
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
model | string | yes | AVReceiver model. Supported: pioneer, denon, onkyo, marantz, yamaha
name | string | yes | Name of Input/Output.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
io_type | string | yes | IO type, can be "input", "output", "inout"
port | int | no | Port to use for connection
id | string | yes | Unique ID identifying the Input/Output in calaos-server
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
host | string | yes | IP address of the device
zone | int | no | Zone of the amplifier (if supported)

##Actions of AVReceiver
Name | Description
---- | -----------
custom XXXXXX | Send a custom command to receiver (if you know the protocol) 
 source X | Change current input source 
 volume 50 | Set current volume 
 power on | Switch receiver on 
 power off | Switch receiver off 
 
#Axis - UNDOCUMENTED IO
SPANK SPANK SPANK : naughty programmer ! You did not add documentation for this IO, that's BAD :'(
Go document it in your code or you will burn in hell!



Axis IP Camera/Encoder. Camera can be viewed directly inside calaos and used in rules.

##Parameters of Axis
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
resolution | string | no | Resolution to use
tilt_framesize | string | no | 
name | string | yes | Name of Input/Output.
io_type | string | yes | IO type, can be "input", "output", "inout"
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
pan_framesize | string | no | 
id | string | yes | Unique ID identifying the Input/Output in calaos-server
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
zoom_step | string | no | 
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
ptz | bool | no | Set to true if camera has PTZ support
model | string | yes | Camera model/chanel to use


#BlinkstickOutputLightRGB
RGB Light dimmer using a Blinkstick


RGB light. Choose a color to be set for this light.

##Parameters of BlinkstickOutputLightRGB
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
serial | string | yes | Blinkstick serial to control
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
nb_leds | int | yes | Number of LEDs to control with the blinkstick
log_history | bool | no | If enabled, write an entry in the history event log for this IO
io_type | string | yes | IO type, can be "input", "output", "inout"
name | string | yes | Name of Input/Output.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.

##Conditions of BlinkstickOutputLightRGB
Name | Description
---- | -----------
value | Event when light is at this value 
 changed | Event on any change of value 
 
##Actions of BlinkstickOutputLightRGB
Name | Description
---- | -----------
down_green 5 | Decrease intensity by X percent of green channel 
 down_red 5 | Decrease intensity by X percent of red channel 
 up_blue 5 | Increase intensity by X percent of blue channel 
 up_green 5 | Increase intensity by X percent of green channel 
 false | Switch the light off 
 set #AA1294 | Set color. Color can be represented by using HTML notation: #AABBCC, rgb(50, 10, 30), hsl(11, 22, 33) 
 set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 toggle | Invert the light state (ON/OFF) 
 true | Switch the light on 
 set_blue 50 | Set blue channel to X percent 
 up_red 5 | Increase intensity by X percent of red channel 
 down_blue 5 | Decrease intensity by X percent of blue channel 
 set_state #AA1294 | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 set_red 50 | Set red channel to X percent 
 set_green 50 | Set green channel to X percent 
 
##More Infos
* OLA: http://www.blinkstick.com

#Gadspot - UNDOCUMENTED IO
SPANK SPANK SPANK : naughty programmer ! You did not add documentation for this IO, that's BAD :'(
Go document it in your code or you will burn in hell!



Gadspot IP Camera. Camera can be viewed directly inside calaos and used in rules.

##Parameters of Gadspot
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
log_history | bool | no | If enabled, write an entry in the history event log for this IO
io_type | string | yes | IO type, can be "input", "output", "inout"
name | string | yes | Name of Input/Output.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.


#GpioInputSwitch
Input switch with a GPIO


Basic switch with press/release states.

##Parameters of GpioInputSwitch
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
active_low | bool | no | Set this if your GPIO has an inverted level
gpio | int | yes | GPIO ID on your hardware
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | A switch can't be visible. Always false.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
io_type | string | yes | IO type, can be "input", "output", "inout"
name | string | yes | Name of Input/Output.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.

##Conditions of GpioInputSwitch
Name | Description
---- | -----------
true | Event triggered when switch is pressed 
 changed | Event on any change of state 
 false | Event triggered when switch is released 
 

#GpioInputSwitchLongPress
Input switch with a GPIO


Long press switch. This switch supports single press and long press. User has 500ms to perform the long press.

##Parameters of GpioInputSwitchLongPress
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
active_low | bool | no | Set this is your GPIO has an inverted level
gpio | int | yes | GPIO ID on your hardware
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | A switch can't be visible. Always false.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
io_type | string | yes | IO type, can be "input", "output", "inout"
name | string | yes | Name of Input/Output.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.

##Conditions of GpioInputSwitchLongPress
Name | Description
---- | -----------
changed | Event on any change of state 
 1 | Event triggered when switch is pressed quickly 
 2 | Event triggered when switch is pressed at least for 500ms (long press) 
 

#GpioInputSwitchTriple
Input switch with a GPIO


Triple click switch. This switch can start 3 kind of actions. User has 500ms to do a multiple click.

##Parameters of GpioInputSwitchTriple
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
active_low | bool | no | Set this is your GPIO has an inverted level
gpio | int | yes | GPIO ID on your hardware
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | A switch can't be visible. Always false.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
io_type | string | yes | IO type, can be "input", "output", "inout"
name | string | yes | Name of Input/Output.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.

##Conditions of GpioInputSwitchTriple
Name | Description
---- | -----------
3 | Event triggered when switch is triple clicked 
 changed | Event on any change of state 
 1 | Event triggered when switch is single clicked 
 2 | Event triggered when switch is double clicked 
 

#GpioOutputShutter
Shutter with 2 GPIOs


Simple shutter. This shutter supports open/close states, as well as impulse shutters.

##Parameters of GpioOutputShutter
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
gpio_down | int | yes | GPIO ID for closing on your hardware
name | string | yes | Name of Input/Output.
active_low_up | bool | no | Set this is your GPIO has an inverted level
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
gpio_up | int | yes | GPIO ID for opening on your hardware
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
active_low_down | bool | no | Set this is your GPIO has an inverted level
id | string | yes | Unique ID identifying the Input/Output in calaos-server
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
time | int | yes | Time in sec for shutter to open or close
io_type | string | yes | IO type, can be "input", "output", "inout"
impulse_time | int | no | Impulse time for shutter that needs impulse instead of holding up/down relays. If set to 0 impulse shutter is disabled. Time is in ms. Default to 0
stop_both | bool | no | If in impulse mode, some shutters needs to activate both up dans down relays when stopping the shutter

##Conditions of GpioOutputShutter
Name | Description
---- | -----------
false | Event when shutter is closed 
 changed | Event on any change of shutter state 
 true | Event when shutter is open 
 
##Actions of GpioOutputShutter
Name | Description
---- | -----------
set_state true | Update internal shutter state without starting real action. This is useful when having updating the shutter state from an external source. 
 set_state false | Update internal shutter state without starting real action. This is useful when having updating the shutter state from an external source. 
 impulse down 200 | Close shutter for X ms 
 down | Close the shutter 
 up | Open the shutter 
 toggle | Invert shutter state 
 stop | Stop the shutter 
 impulse up 200 | Open shutter for X ms 
 

#GpioOutputShutterSmart
Shutter with 2 GPIOs


Smart shutter. This shutter calculates the position of the shutter based on the time it takes to open and close. It then allows to set directly the shutter at a specified position.

##Parameters of GpioOutputShutterSmart
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
gpio_down | int | yes | GPIO ID for closing on your hardware
stop_both | bool | no | If in impulse mode, some shutters needs to activate both up dans down relays when stopping the shutter
name | string | yes | Name of Input/Output.
active_low_up | bool | no | Set this is your GPIO has an inverted level
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
gpio_up | int | yes | GPIO ID for opening on your hardware
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
active_low_down | bool | no | Set this is your GPIO has an inverted level
id | string | yes | Unique ID identifying the Input/Output in calaos-server
time_down | int | yes | Time in sec for shutter to fully closed. The more accurate, the better it will work
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
time_up | int | yes | Time in sec for shutter to be fully open. The more accurate, the better it will work
io_type | string | yes | IO type, can be "input", "output", "inout"
impulse_time | int | no | Impulse time for shutter that needs impulse instead of holding up/down relays. If set to 0 impulse shutter is disabled. Time is in ms. Default to 0

##Conditions of GpioOutputShutterSmart
Name | Description
---- | -----------
false | Event when shutter is closed 
 changed | Event on any change of shutter state 
 true | Event when shutter is open 
 
##Actions of GpioOutputShutterSmart
Name | Description
---- | -----------
impulse up 200 | Open shutter for X ms 
 set 50 | Set shutter at position X in percent 
 up | Open the shutter 
 down | Close the shutter 
 impulse down 200 | Close shutter for X ms 
 set_state false | Update internal shutter state without starting real action. This is useful when having updating the shutter state from an external source. 
 toggle | Invert shutter state 
 up 5 | Open the shutter by X percent 
 calibrate | Start calibration on shutter. This opens fully the shutter and resets all internal position values. Use this if shutter sync is lost. 
 stop | Stop the shutter 
 down 5 | Close the shutter by X percent 
 set_state true | Update internal shutter state without starting real action. This is useful when having updating the shutter state from an external source. 
 

#GpioOutputSwitch
Light with a GPIO


Basic light. This light have only 2 states, ON or OFF. Can also be used to control simple relays output

##Parameters of GpioOutputSwitch
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
active_low | bool | no | Set this is your GPIO has an inverted level
gpio | int | yes | GPIO ID on your hardware
io_style | list | yes | GUI style display. This will control the icon displayed on the UI
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
log_history | bool | no | If enabled, write an entry in the history event log for this IO
io_type | string | yes | IO type, can be "input", "output", "inout"
name | string | yes | Name of Input/Output.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.

##Conditions of GpioOutputSwitch
Name | Description
---- | -----------
false | Event when light is off 
 changed | Event on any change of value 
 true | Event when light is on 
 
##Actions of GpioOutputSwitch
Name | Description
---- | -----------
set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 false | Switch the light off 
 true | Switch the light on 
 toggle | Invert light state 
 impulse 500 200 500 200 | Do an impulse on light state with a pattern.<br>Ex: 500 200 500 200 means: TRUE for 500ms, FALSE for 200ms, TRUE for 500ms, FALSE for 200ms<br>Ex: 500 loop 200 300 means: TRUE for 500ms, then loop the next steps for infinite, FALSE for 200ms, TRUE for 300ms<br>Ex: 100 100 200 old means: blinks and then set to the old start state (before impulse starts) 
 impulse 200 | Do an impulse on light state. Set to true for X ms then reset to false 
 

#HueOutputLightRGB
RGB Light dimmer using a Philips Hue


RGB light. Choose a color to be set for this light.

##Parameters of HueOutputLightRGB
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
id_hue | string | yes | Unique ID describing the Hue Light. This value is returned by the Hue Wizard.
api | string | yes | API key return by Hue bridge when assciation has been made. Use Hue Wizard in calaos_installer to get this value automatically.
host | string | yes | Hue bridge IP address
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
log_history | bool | no | If enabled, write an entry in the history event log for this IO
io_type | string | yes | IO type, can be "input", "output", "inout"
name | string | yes | Name of Input/Output.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.

##Conditions of HueOutputLightRGB
Name | Description
---- | -----------
value | Event when light is at this value 
 changed | Event on any change of value 
 
##Actions of HueOutputLightRGB
Name | Description
---- | -----------
down_green 5 | Decrease intensity by X percent of green channel 
 down_red 5 | Decrease intensity by X percent of red channel 
 up_blue 5 | Increase intensity by X percent of blue channel 
 up_green 5 | Increase intensity by X percent of green channel 
 false | Switch the light off 
 set #AA1294 | Set color. Color can be represented by using HTML notation: #AABBCC, rgb(50, 10, 30), hsl(11, 22, 33) 
 set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 toggle | Invert the light state (ON/OFF) 
 true | Switch the light on 
 set_blue 50 | Set blue channel to X percent 
 up_red 5 | Increase intensity by X percent of red channel 
 down_blue 5 | Decrease intensity by X percent of blue channel 
 set_state #AA1294 | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 set_red 50 | Set red channel to X percent 
 set_green 50 | Set green channel to X percent 
 
##More Infos
* Meet Hue: http://www.meethue.com


#InputTime
Basic time input. An event is triggered when the current time equals the configured time. A specific date can also be set.

##Parameters of InputTime
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
day | int | no | Day for this time input
month | int | no | Month for this time input
name | string | yes | Name of Input/Output.
io_type | string | yes | IO type, can be "input", "output", "inout"
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
sec | int | yes | Seconds for this time input
visible | bool | no | A time object can't be visible. Always false.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
year | int | no | Year for this time input
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
min | int | yes | Minutes for this time input
hour | int | yes | Hour for this time input

##Conditions of InputTime
Name | Description
---- | -----------
true | Event triggered when current time equals 
 changed | Event on any change of time 
 false | Event triggered when current time is not equal 
 

#InputTimer
Timer object. trigger an event after the configured time has expired.

##Parameters of InputTimer
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
autorestart | bool | yes | Auto restart the timer when time expires
autostart | bool | yes | Auto start the timer when calaos starts
name | string | yes | Name of Input/Output.
io_type | string | yes | IO type, can be "input", "output", "inout"
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
sec | int | yes | Seconds for the timer interval
visible | bool | no | A timer object can't be visible. Always false.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
msec | int | yes | Miliseconds for the timer interval
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
min | int | yes | Minutes for the timer interval
hour | int | yes | Hour for the timer interval

##Conditions of InputTimer
Name | Description
---- | -----------
change | Event triggered on any change 
 true | Event triggered when timer expires 
 false | Event triggered when timer starts 
 
##Actions of InputTimer
Name | Description
---- | -----------
00:00:00:200 | Reset the configured time to a value. Format is h:m:s:ms 
 start | Start the timer 
 stop | Stop the timer 
 

#InternalBool
Internal boolean object. This object is useful for doing internal programing in rules, like keeping boolean states, or displaying boolean values

##Parameters of InternalBool
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
save | bool | no | Automatically save the value in cache. The value will be reloaded when restarting calaos is true. Default to false
rw | bool | no | Enable edit mode for this object. It allows user to modify the value on interfaces. Default to false
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
log_history | bool | no | If enabled, write an entry in the history event log for this IO
io_type | string | yes | IO type, can be "input", "output", "inout"
name | string | yes | Name of Input/Output.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.

##Conditions of InternalBool
Name | Description
---- | -----------
true | Event when value is true 
 changed | Event on any change of value 
 false | Event when value is false 
 
##Actions of InternalBool
Name | Description
---- | -----------
impulse 200 | Do an impulse on boolean value. Set to true for X ms then reset to false 
 impulse 500 200 500 200 | Do an impulse on boolean value with a pattern.<br>Ex: 500 200 500 200 means: TRUE for 500ms, FALSE for 200ms, TRUE for 500ms, FALSE for 200ms<br>Ex: 500 loop 200 300 means: TRUE for 500ms, then loop the next steps for infinite, FALSE for 200ms, TRUE for 300ms<br>Ex: 100 100 200 old means: blinks and then set to the old start state (before impulse starts) 
 toggle | Invert boolean value 
 true | Set a value to true 
 false | Set a value to false 
 

#InternalInt
Internal number object. This object is useful for doing internal programing in rules, like counters, of displaying values.

##Parameters of InternalInt
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
save | bool | no | Automatically save the value in cache. The value will be reloaded when restarting calaos is true. Default to false
name | string | yes | Name of Input/Output.
io_type | string | yes | IO type, can be "input", "output", "inout"
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
unit | string | no | Unit which will be displayed on the UI as a suffix.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
step | float | no | Set a step for increment/decrement value. Default is 1.0
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
rw | bool | no | Enable edit mode for this object. It allows user to modify the value on interfaces. Default to false

##Conditions of InternalInt
Name | Description
---- | -----------
changed | Event on any change of value 
 0 | Event on a specific number value 
 
##Actions of InternalInt
Name | Description
---- | -----------
dec 1 | Decrement value by value 
 inc 1 | Increment value by value 
 dec | Decrement value with configured step 
 0 | Set a specific number value 
 inc | Increment value with configured step 
 

#InternalString
Internal string object. This object is useful for doing internal programing in rules or displaying text values on user interfaces.

##Parameters of InternalString
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
save | bool | no | Automatically save the value in cache. The value will be reloaded when restarting calaos is true. Default to false
rw | bool | no | Enable edit mode for this object. It allows user to modify the value on interfaces. Default to false
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
log_history | bool | no | If enabled, write an entry in the history event log for this IO
io_type | string | yes | IO type, can be "input", "output", "inout"
name | string | yes | Name of Input/Output.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.

##Conditions of InternalString
Name | Description
---- | -----------
changed | Event on any change of value 
 value | Event on a specific string value 
 
##Actions of InternalString
Name | Description
---- | -----------
value | Set a specific string value 
 

#KNXInputAnalog
Input analog with KNX and eibnetmux


An analog input can be used to read analog values to display them and use them in rules.

##Parameters of KNXInputAnalog
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
host | string | yes | Hostname of knxd, default to localhost
io_style | list | yes | GUI style display. This will control the icon displayed on the UI
unit | string | no | Unit which will be displayed on the UI as a suffix.
precision | int | no | Precision of the returned value. The value represents the number of decimal after the dot. The value is rounded like this : value = 19.275 => returned value 19.28 when preicision = 2, 19.3 when precision = 1, 19 when precision = 0
knx_group | string | yes | KNX Group address, Ex: x/y/z
frequency | float | no | Sampling time in microsecond. The value is read at this frequency. If this value is not set, calaos tries to read the interval parameter
name | string | yes | Name of Input/Output.
read_at_start | bool | yes | Send a read request at start to get the current value. Default is false
eis | int | no | KNX EIS (Data type)
io_type | string | yes | IO type, can be "input", "output", "inout"
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
id | string | yes | Unique ID identifying the Input/Output in calaos-server
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
listen_knx_group | string | no | KNX Group address for listening status, Ex: x/y/z
interval | float | no | Sampling time in seconds. The value is read at this frequency. If this value is not set, the default value is 15s
coeff_a | float | no | use in conjunction of coeff_b to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 1.0.
coeff_b | float | no | use in conjunction of coeff_a to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 0.0
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
offset | float | no | same as coeff_b, can be used alone. Default value is 0.0

##Conditions of KNXInputAnalog
Name | Description
---- | -----------
changed | Event on any change of value 
 value | Event on a specific value 
 
##More Infos
* knxd: https://github.com/knxd/knxd/g


#KNXInputSwitch
Input switch with KNX and eibnetmux


Basic switch with press/release states.

##Parameters of KNXInputSwitch
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
host | string | yes | Hostname of knxd, default to localhost
name | string | yes | Name of Input/Output.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
visible | bool | no | A switch can't be visible. Always false.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
knx_group | string | yes | KNX Group address, Ex: x/y/z
listen_knx_group | string | no | KNX Group address for listening status, Ex: x/y/z
read_at_start | bool | yes | Send a read request at start to get the current value. Default is false
io_type | string | yes | IO type, can be "input", "output", "inout"
eis | int | no | KNX EIS (Data type)

##Conditions of KNXInputSwitch
Name | Description
---- | -----------
true | Event triggered when switch is pressed 
 changed | Event on any change of state 
 false | Event triggered when switch is released 
 
##More Infos
* knxd: https://github.com/knxd/knxd/g


#KNXInputSwitchLongPress
Input switch long press with KNX and eibnetmux


Long press switch. This switch supports single press and long press. User has 500ms to perform the long press.

##Parameters of KNXInputSwitchLongPress
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
host | string | yes | Hostname of knxd, default to localhost
name | string | yes | Name of Input/Output.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
visible | bool | no | A switch can't be visible. Always false.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
knx_group | string | yes | KNX Group address, Ex: x/y/z
listen_knx_group | string | no | KNX Group address for listening status, Ex: x/y/z
read_at_start | bool | yes | Send a read request at start to get the current value. Default is false
io_type | string | yes | IO type, can be "input", "output", "inout"
eis | int | no | KNX EIS (Data type)

##Conditions of KNXInputSwitchLongPress
Name | Description
---- | -----------
changed | Event on any change of state 
 1 | Event triggered when switch is pressed quickly 
 2 | Event triggered when switch is pressed at least for 500ms (long press) 
 
##More Infos
* knxd: https://github.com/knxd/knxd/g


#KNXInputSwitchTriple
Input switch triple with KNX and eibnetmux


Triple click switch. This switch can start 3 kind of actions. User has 500ms to do a multiple click.

##Parameters of KNXInputSwitchTriple
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
host | string | yes | Hostname of knxd, default to localhost
name | string | yes | Name of Input/Output.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
visible | bool | no | A switch can't be visible. Always false.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
knx_group | string | yes | KNX Group address, Ex: x/y/z
listen_knx_group | string | no | KNX Group address for listening status, Ex: x/y/z
read_at_start | bool | yes | Send a read request at start to get the current value. Default is false
io_type | string | yes | IO type, can be "input", "output", "inout"
eis | int | no | KNX EIS (Data type)

##Conditions of KNXInputSwitchTriple
Name | Description
---- | -----------
3 | Event triggered when switch is triple clicked 
 changed | Event on any change of state 
 1 | Event triggered when switch is single clicked 
 2 | Event triggered when switch is double clicked 
 
##More Infos
* knxd: https://github.com/knxd/knxd/g


#KNXInputTemp
Input temperature with KNX and eibnetmux


Temperature sensor input. Use for displaying temperature and to control heating devices with rules based on temperature value

##Parameters of KNXInputTemp
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
host | string | yes | Hostname of knxd, default to localhost
precision | int | no | Precision of the returned value. The value represents the number of decimal after the dot. The value is rounded like this : value = 19.275 => returned value 19.28 when preicision = 2, 19.3 when precision = 1, 19 when precision = 0
knx_group | string | yes | KNX Group address, Ex: x/y/z
frequency | float | no | Sampling time in microsecond. The value is read at this frequency. If this value is not set, calaos tries to read the interval parameter
name | string | yes | Name of Input/Output.
read_at_start | bool | yes | Send a read request at start to get the current value. Default is false
eis | int | no | KNX EIS (Data type)
io_type | string | yes | IO type, can be "input", "output", "inout"
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
id | string | yes | Unique ID identifying the Input/Output in calaos-server
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
listen_knx_group | string | no | KNX Group address for listening status, Ex: x/y/z
interval | float | no | Sampling time in seconds. The value is read at this frequency. If this value is not set, the default value is 15s
coeff_a | float | no | use in conjunction of coeff_b to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 1.0.
coeff_b | float | no | use in conjunction of coeff_a to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 0.0
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
offset | float | no | same as coeff_b, can be used alone. Default value is 0.0

##Conditions of KNXInputTemp
Name | Description
---- | -----------
changed | Event on any change of temperature value 
 value | Event on a temperature value in degree Celsius 
 
##More Infos
* knxd: https://github.com/knxd/knxd/g


#KNXOutputAnalog
Analog output with KNX and eibnetmux


Analog output. Useful to control analog output devices connected to calaos.

##Parameters of KNXOutputAnalog
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
host | string | yes | Hostname of knxd, default to localhost
knx_group | string | yes | KNX Group address, Ex: x/y/z
io_style | list | yes | GUI style display. This will control the icon displayed on the UI
unit | string | no | Unit which will be displayed on the UI as a suffix.
name | string | yes | Name of Input/Output.
read_at_start | bool | yes | Send a read request at start to get the current value. Default is false
eis | int | no | KNX EIS (Data type)
io_type | string | yes | IO type, can be "input", "output", "inout"
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
id | string | yes | Unique ID identifying the Input/Output in calaos-server
step | float | no | Set a step for increment/decrement value. Default is 1.0
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
listen_knx_group | string | no | KNX Group address for listening status, Ex: x/y/z
coeff_a | float | no | use in conjunction of coeff_b to apply equation of the form `value_sent = coeff_a * raw_value + coeff_b`. Default value is 1.0.
coeff_b | float | no | use in conjunction of coeff_a to apply equation of the form `value_sent = coeff_a * raw_value + coeff_b`. Default value is 0.0

##Conditions of KNXOutputAnalog
Name | Description
---- | -----------
0 | Event on a specific number value 
 value | Event on a specific value 
 changed | Event on any change of value 
 
##Actions of KNXOutputAnalog
Name | Description
---- | -----------
dec 1 | Decrement value by value 
 inc 1 | Increment value by value 
 dec | Decrement value with configured step 
 0 | Set a specific number value 
 inc | Increment value with configured step 
 
##More Infos
* knxd: https://github.com/knxd/knxd/g


#KNXOutputLight
Light output with KNX and eibnetmux


Basic light. This light have only 2 states, ON or OFF. Can also be used to control simple relays output

##Parameters of KNXOutputLight
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
host | string | yes | Hostname of knxd, default to localhost
name | string | yes | Name of Input/Output.
read_at_start | bool | yes | Send a read request at start to get the current value. Default is false
eis | int | no | KNX EIS (Data type)
io_type | string | yes | IO type, can be "input", "output", "inout"
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
id | string | yes | Unique ID identifying the Input/Output in calaos-server
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
io_style | list | yes | GUI style display. This will control the icon displayed on the UI
knx_group | string | yes | KNX Group address, Ex: x/y/z
listen_knx_group | string | no | KNX Group address for listening status, Ex: x/y/z

##Conditions of KNXOutputLight
Name | Description
---- | -----------
false | Event when light is off 
 changed | Event on any change of value 
 true | Event when light is on 
 
##Actions of KNXOutputLight
Name | Description
---- | -----------
set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 false | Switch the light off 
 true | Switch the light on 
 toggle | Invert light state 
 impulse 500 200 500 200 | Do an impulse on light state with a pattern.<br>Ex: 500 200 500 200 means: TRUE for 500ms, FALSE for 200ms, TRUE for 500ms, FALSE for 200ms<br>Ex: 500 loop 200 300 means: TRUE for 500ms, then loop the next steps for infinite, FALSE for 200ms, TRUE for 300ms<br>Ex: 100 100 200 old means: blinks and then set to the old start state (before impulse starts) 
 impulse 200 | Do an impulse on light state. Set to true for X ms then reset to false 
 
##More Infos
* knxd: https://github.com/knxd/knxd/g


#KNXOutputLightDimmer
Light dimmer with KNX and eibnetmux


Light with dimming control. Light intensity can be changed for this light.

##Parameters of KNXOutputLightDimmer
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
host | string | yes | Hostname of knxd, default to localhost
name | string | yes | Name of Input/Output.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
id | string | yes | Unique ID identifying the Input/Output in calaos-server
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
knx_group | string | yes | KNX Group address, Ex: x/y/z
listen_knx_group | string | no | KNX Group address for listening status, Ex: x/y/z
read_at_start | bool | yes | Send a read request at start to get the current value. Default is false
io_type | string | yes | IO type, can be "input", "output", "inout"
eis | int | no | KNX EIS (Data type)

##Conditions of KNXOutputLightDimmer
Name | Description
---- | -----------
value | Event when light is at this value 
 changed | Event on any change of value 
 
##Actions of KNXOutputLightDimmer
Name | Description
---- | -----------
hold stop | Dynamically change light intensity when holding a switch (stop action) 
 hold press | Dynamically change light intensity when holding a switch (press action) 
 down 5 | Decrease intensity by X percent 
 impulse 500 200 500 200 | Do an impulse on light state with a pattern.<br>Ex: 500 200 500 200 means: TRUE for 500ms, FALSE for 200ms, TRUE for 500ms, FALSE for 200ms<br>Ex: 500 loop 200 300 means: TRUE for 500ms, then loop the next steps for infinite, FALSE for 200ms, TRUE for 300ms<br>Ex: 100 100 200 old means: blinks and then set to the old start state (before impulse starts) 
 set off 50 | Set light value without switching on. This will be the light intensity for the next ON 
 true | Switch the light on 
 set_state 50 | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 false | Switch the light off 
 toggle | Invert the light state 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 up 5 | Increase intensity by X percent 
 set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 impulse 200 | Do an impulse on light state. Set to true for X ms then reset to false 
 set 50 | Set light intensity and swith on if light is off 
 
##More Infos
* knxd: https://github.com/knxd/knxd/g


#KNXOutputLightRGB
Light RGB with KNX and eibnetmux


RGB light. Choose a color to be set for this light.

##Parameters of KNXOutputLightRGB
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
host | string | yes | Hostname of knxd, default to localhost
listen_knx_group_blue | string | no | Blue Group address for listening status, Ex: x/y/z
listen_knx_group_green | string | no | Green Group address for listening status, Ex: x/y/z
listen_knx_group_red | string | no | Red Group address for listening status, Ex: x/y/z
name | string | yes | Name of Input/Output.
read_at_start | bool | yes | Send a read request at start to get the current value. Default is false
eis | int | no | KNX EIS (Data type)
io_type | string | yes | IO type, can be "input", "output", "inout"
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
knx_group_green | string | yes | Green channel KNX Group address, Ex: x/y/z
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
id | string | yes | Unique ID identifying the Input/Output in calaos-server
knx_group_blue | string | yes | Blue channel KNX Group address, Ex: x/y/z
knx_group_red | string | yes | Red channel KNX Group address, Ex: x/y/z

##Conditions of KNXOutputLightRGB
Name | Description
---- | -----------
value | Event when light is at this value 
 changed | Event on any change of value 
 
##Actions of KNXOutputLightRGB
Name | Description
---- | -----------
down_green 5 | Decrease intensity by X percent of green channel 
 down_red 5 | Decrease intensity by X percent of red channel 
 up_blue 5 | Increase intensity by X percent of blue channel 
 up_green 5 | Increase intensity by X percent of green channel 
 false | Switch the light off 
 set #AA1294 | Set color. Color can be represented by using HTML notation: #AABBCC, rgb(50, 10, 30), hsl(11, 22, 33) 
 set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 toggle | Invert the light state (ON/OFF) 
 true | Switch the light on 
 set_blue 50 | Set blue channel to X percent 
 up_red 5 | Increase intensity by X percent of red channel 
 down_blue 5 | Decrease intensity by X percent of blue channel 
 set_state #AA1294 | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 set_red 50 | Set red channel to X percent 
 set_green 50 | Set green channel to X percent 
 
##More Infos
* knxd: https://github.com/knxd/knxd/g


#KNXOutputShutter
Shutter with with KNX and eibnetmux


Simple shutter. This shutter supports open/close states, as well as impulse shutters.

##Parameters of KNXOutputShutter
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
host | string | yes | Hostname of knxd, default to localhost
knx_group_up | string | yes | Up KNX Group address, Ex: x/y/z
name | string | yes | Name of Input/Output.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
knx_group_down | string | yes | Down KNX Group address, Ex: x/y/z
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
id | string | yes | Unique ID identifying the Input/Output in calaos-server
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
time | int | yes | Time in sec for shutter to open or close
read_at_start | bool | yes | Send a read request at start to get the current value. Default is false
eis | int | no | KNX EIS (Data type)
io_type | string | yes | IO type, can be "input", "output", "inout"
impulse_time | int | no | Impulse time for shutter that needs impulse instead of holding up/down relays. If set to 0 impulse shutter is disabled. Time is in ms. Default to 0
stop_both | bool | no | If in impulse mode, some shutters needs to activate both up dans down relays when stopping the shutter

##Conditions of KNXOutputShutter
Name | Description
---- | -----------
false | Event when shutter is closed 
 changed | Event on any change of shutter state 
 true | Event when shutter is open 
 
##Actions of KNXOutputShutter
Name | Description
---- | -----------
set_state true | Update internal shutter state without starting real action. This is useful when having updating the shutter state from an external source. 
 set_state false | Update internal shutter state without starting real action. This is useful when having updating the shutter state from an external source. 
 impulse down 200 | Close shutter for X ms 
 down | Close the shutter 
 up | Open the shutter 
 toggle | Invert shutter state 
 stop | Stop the shutter 
 impulse up 200 | Open shutter for X ms 
 
##More Infos
* knxd: https://github.com/knxd/knxd/g


#KNXOutputShutterSmart
Shutter with with KNX and eibnetmux


Smart shutter. This shutter calculates the position of the shutter based on the time it takes to open and close. It then allows to set directly the shutter at a specified position.

##Parameters of KNXOutputShutterSmart
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
listen_knx_group | string | no | KNX Group address for listening status, Ex: x/y/z
knx_group | string | yes | KNX Group address, Ex: x/y/z
host | string | yes | Hostname of knxd, default to localhost
knx_group_up | string | yes | Up KNX Group address, Ex: x/y/z
stop_both | bool | no | If in impulse mode, some shutters needs to activate both up dans down relays when stopping the shutter
name | string | yes | Name of Input/Output.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
knx_group_down | string | yes | Down KNX Group address, Ex: x/y/z
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
id | string | yes | Unique ID identifying the Input/Output in calaos-server
time_down | int | yes | Time in sec for shutter to fully closed. The more accurate, the better it will work
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
time_up | int | yes | Time in sec for shutter to be fully open. The more accurate, the better it will work
read_at_start | bool | yes | Send a read request at start to get the current value. Default is false
eis | int | no | KNX EIS (Data type)
io_type | string | yes | IO type, can be "input", "output", "inout"
impulse_time | int | no | Impulse time for shutter that needs impulse instead of holding up/down relays. If set to 0 impulse shutter is disabled. Time is in ms. Default to 0

##Conditions of KNXOutputShutterSmart
Name | Description
---- | -----------
false | Event when shutter is closed 
 changed | Event on any change of shutter state 
 true | Event when shutter is open 
 
##Actions of KNXOutputShutterSmart
Name | Description
---- | -----------
impulse up 200 | Open shutter for X ms 
 set 50 | Set shutter at position X in percent 
 up | Open the shutter 
 down | Close the shutter 
 impulse down 200 | Close shutter for X ms 
 set_state false | Update internal shutter state without starting real action. This is useful when having updating the shutter state from an external source. 
 toggle | Invert shutter state 
 up 5 | Open the shutter by X percent 
 calibrate | Start calibration on shutter. This opens fully the shutter and resets all internal position values. Use this if shutter sync is lost. 
 stop | Stop the shutter 
 down 5 | Close the shutter by X percent 
 set_state true | Update internal shutter state without starting real action. This is useful when having updating the shutter state from an external source. 
 
##More Infos
* knxd: https://github.com/knxd/knxd/g


#MilightOutputLightRGB
RGB light support for Limitless/Milight RGB bulbs.


RGB light. Choose a color to be set for this light.

##Parameters of MilightOutputLightRGB
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
zone | int | yes | Zone to control. Each gateway supports 4 zones.
host | string | yes | Milight wifi gateway IP address
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
port | int | no | Gateway port, default to 8899
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
log_history | bool | no | If enabled, write an entry in the history event log for this IO
io_type | string | yes | IO type, can be "input", "output", "inout"
name | string | yes | Name of Input/Output.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.

##Conditions of MilightOutputLightRGB
Name | Description
---- | -----------
value | Event when light is at this value 
 changed | Event on any change of value 
 
##Actions of MilightOutputLightRGB
Name | Description
---- | -----------
down_green 5 | Decrease intensity by X percent of green channel 
 down_red 5 | Decrease intensity by X percent of red channel 
 up_blue 5 | Increase intensity by X percent of blue channel 
 up_green 5 | Increase intensity by X percent of green channel 
 false | Switch the light off 
 set #AA1294 | Set color. Color can be represented by using HTML notation: #AABBCC, rgb(50, 10, 30), hsl(11, 22, 33) 
 set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 toggle | Invert the light state (ON/OFF) 
 true | Switch the light on 
 set_blue 50 | Set blue channel to X percent 
 up_red 5 | Increase intensity by X percent of red channel 
 down_blue 5 | Decrease intensity by X percent of blue channel 
 set_state #AA1294 | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 set_red 50 | Set red channel to X percent 
 set_green 50 | Set green channel to X percent 
 
##More Infos
* LimitlessLED: http://www.limitlessled.com


#MqttInputAnalog
Temperature read from a mqtt broker


An analog input can be used to read analog values to display them and use them in rules.

##Parameters of MqttInputAnalog
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
offset | float | no | same as coeff_b, can be used alone. Default value is 0.0
log_history | bool | no | If enabled, write an entry in the history event log for this IO
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
coeff_b | float | no | use in conjunction of coeff_a to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 0.0
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
topic_pub | string | yes | Topic on witch to publish.
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
io_type | string | yes | IO type, can be "input", "output", "inout"
user | string | no | User to use for authentication with mqtt broker. Password must be defined in that case.
port | int | no | TCP port of the mqtt broker. Default value is 1883
topic_sub | string | yes | Topic on witch to subscribe.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
name | string | yes | Name of Input/Output.
path | string | yes | The path where to found the value in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. for example weather[0]/description, try to read the description value of the 1 element of the array of the weather object. if payload is somple json, just try to use the key of the value you want to read, for example : {"temperature":14.23} use "temperature" as path

frequency | float | no | Sampling time in microsecond. The value is read at this frequency. If this value is not set, calaos tries to read the interval parameter
unit | string | no | Unit which will be displayed on the UI as a suffix.
coeff_a | float | no | use in conjunction of coeff_b to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 1.0.
io_style | list | yes | GUI style display. This will control the icon displayed on the UI
precision | int | no | Precision of the returned value. The value represents the number of decimal after the dot. The value is rounded like this : value = 19.275 => returned value 19.28 when preicision = 2, 19.3 when precision = 1, 19 when precision = 0
host | string | no | IP address of the mqtt broker to connect to. Default value is 127.0.0.1.
keepalive | int | no | keepalive timeout in seconds. Time between two mqtt PING.
interval | float | no | Sampling time in seconds. The value is read at this frequency. If this value is not set, the default value is 15s
password | string | no | Password to use for authentication with mqtt broker. User must be defined in that case.

##Conditions of MqttInputAnalog
Name | Description
---- | -----------
changed | Event on any change of value 
 value | Event on a specific value 
 

#MqttInputString
Temperature read from a mqtt broker

##Parameters of MqttInputString
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
path | string | yes | The path where to found the value in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. for example weather[0]/description, try to read the description value of the 1 element of the array of the weather object. if payload is somple json, just try to use the key of the value you want to read, for example : {"temperature":14.23} use "temperature" as path

user | string | no | User to use for authentication with mqtt broker. Password must be defined in that case.
password | string | no | Password to use for authentication with mqtt broker. User must be defined in that case.
name | string | yes | Name of Input/Output.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
io_type | string | yes | IO type, can be "input", "output", "inout"
port | int | no | TCP port of the mqtt broker. Default value is 1883
keepalive | int | no | keepalive timeout in seconds. Time between two mqtt PING.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
topic_sub | string | yes | Topic on witch to subscribe.
topic_pub | string | yes | Topic on witch to publish.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
host | string | no | IP address of the mqtt broker to connect to. Default value is 127.0.0.1.


#MqttInputSwitch
Switch value readed from a mqtt broker


Basic switch with press/release states.

##Parameters of MqttInputSwitch
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
off_value | string | yes | Value to interpret as OFF value
on_value | string | yes | Value to interpret as ON value
path | string | yes | The path where to found the value in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. for example weather[0]/description, try to read the description value of the 1 element of the array of the weather object. if payload is somple json, just try to use the key of the value you want to read, for example : {"temperature":14.23} use "temperature" as path

user | string | no | User to use for authentication with mqtt broker. Password must be defined in that case.
password | string | no | Password to use for authentication with mqtt broker. User must be defined in that case.
name | string | yes | Name of Input/Output.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
visible | bool | no | A switch can't be visible. Always false.
io_type | string | yes | IO type, can be "input", "output", "inout"
port | int | no | TCP port of the mqtt broker. Default value is 1883
keepalive | int | no | keepalive timeout in seconds. Time between two mqtt PING.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
topic_sub | string | yes | Topic on witch to subscribe.
topic_pub | string | yes | Topic on witch to publish.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
host | string | no | IP address of the mqtt broker to connect to. Default value is 127.0.0.1.

##Conditions of MqttInputSwitch
Name | Description
---- | -----------
true | Event triggered when switch is pressed 
 changed | Event on any change of state 
 false | Event triggered when switch is released 
 

#MqttInputTemp
Temperature read from a mqtt broker


Temperature sensor input. Use for displaying temperature and to control heating devices with rules based on temperature value

##Parameters of MqttInputTemp
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
path | string | yes | The path where to found the value in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. for example weather[0]/description, try to read the description value of the 1 element of the array of the weather object. if payload is somple json, just try to use the key of the value you want to read, for example : {"temperature":14.23} use "temperature" as path

password | string | no | Password to use for authentication with mqtt broker. User must be defined in that case.
keepalive | int | no | keepalive timeout in seconds. Time between two mqtt PING.
host | string | no | IP address of the mqtt broker to connect to. Default value is 127.0.0.1.
precision | int | no | Precision of the returned value. The value represents the number of decimal after the dot. The value is rounded like this : value = 19.275 => returned value 19.28 when preicision = 2, 19.3 when precision = 1, 19 when precision = 0
frequency | float | no | Sampling time in microsecond. The value is read at this frequency. If this value is not set, calaos tries to read the interval parameter
name | string | yes | Name of Input/Output.
port | int | no | TCP port of the mqtt broker. Default value is 1883
io_type | string | yes | IO type, can be "input", "output", "inout"
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
id | string | yes | Unique ID identifying the Input/Output in calaos-server
topic_sub | string | yes | Topic on witch to subscribe.
topic_pub | string | yes | Topic on witch to publish.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
interval | float | no | Sampling time in seconds. The value is read at this frequency. If this value is not set, the default value is 15s
coeff_a | float | no | use in conjunction of coeff_b to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 1.0.
user | string | no | User to use for authentication with mqtt broker. Password must be defined in that case.
coeff_b | float | no | use in conjunction of coeff_a to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 0.0
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
offset | float | no | same as coeff_b, can be used alone. Default value is 0.0

##Conditions of MqttInputTemp
Name | Description
---- | -----------
changed | Event on any change of temperature value 
 value | Event on a temperature value in degree Celsius 
 

#MqttOutputAnalog
Control analog output through mqtt broker


Analog output. Useful to control analog output devices connected to calaos.

##Parameters of MqttOutputAnalog
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
path | string | yes | The path where to found the value in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. for example weather[0]/description, try to read the description value of the 1 element of the array of the weather object. if payload is somple json, just try to use the key of the value you want to read, for example : {"temperature":14.23} use "temperature" as path

password | string | no | Password to use for authentication with mqtt broker. User must be defined in that case.
keepalive | int | no | keepalive timeout in seconds. Time between two mqtt PING.
host | string | no | IP address of the mqtt broker to connect to. Default value is 127.0.0.1.
io_style | list | yes | GUI style display. This will control the icon displayed on the UI
unit | string | no | Unit which will be displayed on the UI as a suffix.
name | string | yes | Name of Input/Output.
port | int | no | TCP port of the mqtt broker. Default value is 1883
io_type | string | yes | IO type, can be "input", "output", "inout"
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
id | string | yes | Unique ID identifying the Input/Output in calaos-server
topic_sub | string | yes | Topic on witch to subscribe.
topic_pub | string | yes | Topic on witch to publish.
step | float | no | Set a step for increment/decrement value. Default is 1.0
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
coeff_a | float | no | use in conjunction of coeff_b to apply equation of the form `value_sent = coeff_a * raw_value + coeff_b`. Default value is 1.0.
user | string | no | User to use for authentication with mqtt broker. Password must be defined in that case.
coeff_b | float | no | use in conjunction of coeff_a to apply equation of the form `value_sent = coeff_a * raw_value + coeff_b`. Default value is 0.0

##Conditions of MqttOutputAnalog
Name | Description
---- | -----------
0 | Event on a specific number value 
 value | Event on a specific value 
 changed | Event on any change of value 
 
##Actions of MqttOutputAnalog
Name | Description
---- | -----------
dec 1 | Decrement value by value 
 inc 1 | Increment value by value 
 dec | Decrement value with configured step 
 0 | Set a specific number value 
 inc | Increment value with configured step 
 

#MqttOutputLight
Control lights through mqtt broker


Basic light. This light have only 2 states, ON or OFF. Can also be used to control simple relays output

##Parameters of MqttOutputLight
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
off_value | string | yes | Value to interpret as OFF value
on_value | string | yes | Value to interpret as ON value
path | string | yes | The path where to found the value in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. for example weather[0]/description, try to read the description value of the 1 element of the array of the weather object. if payload is somple json, just try to use the key of the value you want to read, for example : {"temperature":14.23} use "temperature" as path

user | string | no | User to use for authentication with mqtt broker. Password must be defined in that case.
password | string | no | Password to use for authentication with mqtt broker. User must be defined in that case.
keepalive | int | no | keepalive timeout in seconds. Time between two mqtt PING.
name | string | yes | Name of Input/Output.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
io_type | string | yes | IO type, can be "input", "output", "inout"
port | int | no | TCP port of the mqtt broker. Default value is 1883
id | string | yes | Unique ID identifying the Input/Output in calaos-server
topic_sub | string | yes | Topic on witch to subscribe.
topic_pub | string | yes | Topic on witch to publish.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
io_style | list | yes | GUI style display. This will control the icon displayed on the UI
host | string | no | IP address of the mqtt broker to connect to. Default value is 127.0.0.1.

##Conditions of MqttOutputLight
Name | Description
---- | -----------
false | Event when light is off 
 changed | Event on any change of value 
 true | Event when light is on 
 
##Actions of MqttOutputLight
Name | Description
---- | -----------
set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 false | Switch the light off 
 true | Switch the light on 
 toggle | Invert light state 
 impulse 500 200 500 200 | Do an impulse on light state with a pattern.<br>Ex: 500 200 500 200 means: TRUE for 500ms, FALSE for 200ms, TRUE for 500ms, FALSE for 200ms<br>Ex: 500 loop 200 300 means: TRUE for 500ms, then loop the next steps for infinite, FALSE for 200ms, TRUE for 300ms<br>Ex: 100 100 200 old means: blinks and then set to the old start state (before impulse starts) 
 impulse 200 | Do an impulse on light state. Set to true for X ms then reset to false 
 

#MqttOutputLightDimmer
Control lights through mqtt broker


Light with dimming control. Light intensity can be changed for this light.

##Parameters of MqttOutputLightDimmer
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
path | string | yes | The path where to found the value in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. for example weather[0]/description, try to read the description value of the 1 element of the array of the weather object. if payload is somple json, just try to use the key of the value you want to read, for example : {"temperature":14.23} use "temperature" as path

user | string | no | User to use for authentication with mqtt broker. Password must be defined in that case.
password | string | no | Password to use for authentication with mqtt broker. User must be defined in that case.
name | string | yes | Name of Input/Output.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
io_type | string | yes | IO type, can be "input", "output", "inout"
port | int | no | TCP port of the mqtt broker. Default value is 1883
keepalive | int | no | keepalive timeout in seconds. Time between two mqtt PING.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
topic_sub | string | yes | Topic on witch to subscribe.
topic_pub | string | yes | Topic on witch to publish.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
host | string | no | IP address of the mqtt broker to connect to. Default value is 127.0.0.1.

##Conditions of MqttOutputLightDimmer
Name | Description
---- | -----------
value | Event when light is at this value 
 changed | Event on any change of value 
 
##Actions of MqttOutputLightDimmer
Name | Description
---- | -----------
hold stop | Dynamically change light intensity when holding a switch (stop action) 
 hold press | Dynamically change light intensity when holding a switch (press action) 
 down 5 | Decrease intensity by X percent 
 impulse 500 200 500 200 | Do an impulse on light state with a pattern.<br>Ex: 500 200 500 200 means: TRUE for 500ms, FALSE for 200ms, TRUE for 500ms, FALSE for 200ms<br>Ex: 500 loop 200 300 means: TRUE for 500ms, then loop the next steps for infinite, FALSE for 200ms, TRUE for 300ms<br>Ex: 100 100 200 old means: blinks and then set to the old start state (before impulse starts) 
 set off 50 | Set light value without switching on. This will be the light intensity for the next ON 
 true | Switch the light on 
 set_state 50 | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 false | Switch the light off 
 toggle | Invert the light state 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 up 5 | Increase intensity by X percent 
 set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 impulse 200 | Do an impulse on light state. Set to true for X ms then reset to false 
 set 50 | Set light intensity and swith on if light is off 
 

#MySensorsInputAnalog
Analog measurement with MySensors node


An analog input can be used to read analog values to display them and use them in rules.

##Parameters of MySensorsInputAnalog
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
sensor_id | string | yes | Sensor ID, as set in your node
host | string | yes | IP address of the tcp gateway if relevant
io_style | list | yes | GUI style display. This will control the icon displayed on the UI
unit | string | no | Unit which will be displayed on the UI as a suffix.
precision | int | no | Precision of the returned value. The value represents the number of decimal after the dot. The value is rounded like this : value = 19.275 => returned value 19.28 when preicision = 2, 19.3 when precision = 1, 19 when precision = 0
frequency | float | no | Sampling time in microsecond. The value is read at this frequency. If this value is not set, calaos tries to read the interval parameter
gateway | list | yes | Gateway type used, tcp or serial are supported
name | string | yes | Name of Input/Output.
port | string | yes | If using serial gateway, port is the serial port (/dev/ttyUSB0 for ex.). If using tcp gateway port is TCP port of the gateway.
io_type | string | yes | IO type, can be "input", "output", "inout"
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
node_id | string | yes | Node ID as set in your network
id | string | yes | Unique ID identifying the Input/Output in calaos-server
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
interval | float | no | Sampling time in seconds. The value is read at this frequency. If this value is not set, the default value is 15s
coeff_a | float | no | use in conjunction of coeff_b to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 1.0.
coeff_b | float | no | use in conjunction of coeff_a to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 0.0
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
offset | float | no | same as coeff_b, can be used alone. Default value is 0.0

##Conditions of MySensorsInputAnalog
Name | Description
---- | -----------
changed | Event on any change of value 
 value | Event on a specific value 
 
##More Infos
* MySensors: http://mysensors.org


#MySensorsInputString
Display string from MySensors node

##Parameters of MySensorsInputString
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
sensor_id | string | yes | Sensor ID, as set in your node
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gateway | list | yes | Gateway type used, tcp or serial are supported
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
io_type | string | yes | IO type, can be "input", "output", "inout"
port | string | yes | If using serial gateway, port is the serial port (/dev/ttyUSB0 for ex.). If using tcp gateway port is TCP port of the gateway.
node_id | string | yes | Node ID as set in your network
id | string | yes | Unique ID identifying the Input/Output in calaos-server
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
host | string | yes | IP address of the tcp gateway if relevant

##More Infos
* MySensors: http://mysensors.org


#MySensorsInputSwitch
Input switch with MySensors node


Basic switch with press/release states.

##Parameters of MySensorsInputSwitch
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
sensor_id | string | yes | Sensor ID, as set in your node
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gateway | list | yes | Gateway type used, tcp or serial are supported
visible | bool | no | A switch can't be visible. Always false.
io_type | string | yes | IO type, can be "input", "output", "inout"
port | string | yes | If using serial gateway, port is the serial port (/dev/ttyUSB0 for ex.). If using tcp gateway port is TCP port of the gateway.
node_id | string | yes | Node ID as set in your network
id | string | yes | Unique ID identifying the Input/Output in calaos-server
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
host | string | yes | IP address of the tcp gateway if relevant

##Conditions of MySensorsInputSwitch
Name | Description
---- | -----------
true | Event triggered when switch is pressed 
 changed | Event on any change of state 
 false | Event triggered when switch is released 
 
##More Infos
* MySensors: http://mysensors.org


#MySensorsInputSwitchLongPress
Input long press switch with MySensors node


Long press switch. This switch supports single press and long press. User has 500ms to perform the long press.

##Parameters of MySensorsInputSwitchLongPress
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
sensor_id | string | yes | Sensor ID, as set in your node
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gateway | list | yes | Gateway type used, tcp or serial are supported
visible | bool | no | A switch can't be visible. Always false.
io_type | string | yes | IO type, can be "input", "output", "inout"
port | string | yes | If using serial gateway, port is the serial port (/dev/ttyUSB0 for ex.). If using tcp gateway port is TCP port of the gateway.
node_id | string | yes | Node ID as set in your network
id | string | yes | Unique ID identifying the Input/Output in calaos-server
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
host | string | yes | IP address of the tcp gateway if relevant

##Conditions of MySensorsInputSwitchLongPress
Name | Description
---- | -----------
changed | Event on any change of state 
 1 | Event triggered when switch is pressed quickly 
 2 | Event triggered when switch is pressed at least for 500ms (long press) 
 
##More Infos
* MySensors: http://mysensors.org


#MySensorsInputSwitchTriple
Input triple click switch with MySensors node


Triple click switch. This switch can start 3 kind of actions. User has 500ms to do a multiple click.

##Parameters of MySensorsInputSwitchTriple
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
sensor_id | string | yes | Sensor ID, as set in your node
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gateway | list | yes | Gateway type used, tcp or serial are supported
visible | bool | no | A switch can't be visible. Always false.
io_type | string | yes | IO type, can be "input", "output", "inout"
port | string | yes | If using serial gateway, port is the serial port (/dev/ttyUSB0 for ex.). If using tcp gateway port is TCP port of the gateway.
node_id | string | yes | Node ID as set in your network
id | string | yes | Unique ID identifying the Input/Output in calaos-server
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
host | string | yes | IP address of the tcp gateway if relevant

##Conditions of MySensorsInputSwitchTriple
Name | Description
---- | -----------
3 | Event triggered when switch is triple clicked 
 changed | Event on any change of state 
 1 | Event triggered when switch is single clicked 
 2 | Event triggered when switch is double clicked 
 
##More Infos
* MySensors: http://mysensors.org


#MySensorsInputTemp
Temperature sensor with MySensors node


Temperature sensor input. Use for displaying temperature and to control heating devices with rules based on temperature value

##Parameters of MySensorsInputTemp
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
sensor_id | string | yes | Sensor ID, as set in your node
host | string | yes | IP address of the tcp gateway if relevant
precision | int | no | Precision of the returned value. The value represents the number of decimal after the dot. The value is rounded like this : value = 19.275 => returned value 19.28 when preicision = 2, 19.3 when precision = 1, 19 when precision = 0
frequency | float | no | Sampling time in microsecond. The value is read at this frequency. If this value is not set, calaos tries to read the interval parameter
gateway | list | yes | Gateway type used, tcp or serial are supported
name | string | yes | Name of Input/Output.
port | string | yes | If using serial gateway, port is the serial port (/dev/ttyUSB0 for ex.). If using tcp gateway port is TCP port of the gateway.
io_type | string | yes | IO type, can be "input", "output", "inout"
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
node_id | string | yes | Node ID as set in your network
id | string | yes | Unique ID identifying the Input/Output in calaos-server
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
interval | float | no | Sampling time in seconds. The value is read at this frequency. If this value is not set, the default value is 15s
coeff_a | float | no | use in conjunction of coeff_b to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 1.0.
coeff_b | float | no | use in conjunction of coeff_a to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 0.0
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
offset | float | no | same as coeff_b, can be used alone. Default value is 0.0

##Conditions of MySensorsInputTemp
Name | Description
---- | -----------
changed | Event on any change of temperature value 
 value | Event on a temperature value in degree Celsius 
 
##More Infos
* MySensors: http://mysensors.org


#MySensorsOutputAnalog
Analog output with MySensors node


Analog output. Useful to control analog output devices connected to calaos.

##Parameters of MySensorsOutputAnalog
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
data_type | string | no | Data type sent to the node. Default: V_DIMMER, see MySensors.cpp for more values.
sensor_id | string | yes | Sensor ID, as set in your node
host | string | yes | IP address of the tcp gateway if relevant
io_style | list | yes | GUI style display. This will control the icon displayed on the UI
unit | string | no | Unit which will be displayed on the UI as a suffix.
gateway | list | yes | Gateway type used, tcp or serial are supported
name | string | yes | Name of Input/Output.
port | string | yes | If using serial gateway, port is the serial port (/dev/ttyUSB0 for ex.). If using tcp gateway port is TCP port of the gateway.
io_type | string | yes | IO type, can be "input", "output", "inout"
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
node_id | string | yes | Node ID as set in your network
id | string | yes | Unique ID identifying the Input/Output in calaos-server
step | float | no | Set a step for increment/decrement value. Default is 1.0
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
coeff_a | float | no | use in conjunction of coeff_b to apply equation of the form `value_sent = coeff_a * raw_value + coeff_b`. Default value is 1.0.
coeff_b | float | no | use in conjunction of coeff_a to apply equation of the form `value_sent = coeff_a * raw_value + coeff_b`. Default value is 0.0

##Conditions of MySensorsOutputAnalog
Name | Description
---- | -----------
0 | Event on a specific number value 
 value | Event on a specific value 
 changed | Event on any change of value 
 
##Actions of MySensorsOutputAnalog
Name | Description
---- | -----------
dec 1 | Decrement value by value 
 inc 1 | Increment value by value 
 dec | Decrement value with configured step 
 0 | Set a specific number value 
 inc | Increment value with configured step 
 
##More Infos
* MySensors: http://mysensors.org


#MySensorsOutputDimmer
Light dimmer with MySensors node


Light with dimming control. Light intensity can be changed for this light.

##Parameters of MySensorsOutputDimmer
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
data_type | string | no | Data type sent to the node. Default: V_DIMMER, see MySensors.cpp for more values.
sensor_id | string | yes | Sensor ID, as set in your node
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gateway | list | yes | Gateway type used, tcp or serial are supported
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
io_type | string | yes | IO type, can be "input", "output", "inout"
port | string | yes | If using serial gateway, port is the serial port (/dev/ttyUSB0 for ex.). If using tcp gateway port is TCP port of the gateway.
node_id | string | yes | Node ID as set in your network
id | string | yes | Unique ID identifying the Input/Output in calaos-server
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
host | string | yes | IP address of the tcp gateway if relevant

##Conditions of MySensorsOutputDimmer
Name | Description
---- | -----------
value | Event when light is at this value 
 changed | Event on any change of value 
 
##Actions of MySensorsOutputDimmer
Name | Description
---- | -----------
hold stop | Dynamically change light intensity when holding a switch (stop action) 
 hold press | Dynamically change light intensity when holding a switch (press action) 
 down 5 | Decrease intensity by X percent 
 impulse 500 200 500 200 | Do an impulse on light state with a pattern.<br>Ex: 500 200 500 200 means: TRUE for 500ms, FALSE for 200ms, TRUE for 500ms, FALSE for 200ms<br>Ex: 500 loop 200 300 means: TRUE for 500ms, then loop the next steps for infinite, FALSE for 200ms, TRUE for 300ms<br>Ex: 100 100 200 old means: blinks and then set to the old start state (before impulse starts) 
 set off 50 | Set light value without switching on. This will be the light intensity for the next ON 
 true | Switch the light on 
 set_state 50 | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 false | Switch the light off 
 toggle | Invert the light state 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 up 5 | Increase intensity by X percent 
 set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 impulse 200 | Do an impulse on light state. Set to true for X ms then reset to false 
 set 50 | Set light intensity and swith on if light is off 
 
##More Infos
* MySensors: http://mysensors.org


#MySensorsOutputLight
Light/relay with MySensors node


Basic light. This light have only 2 states, ON or OFF. Can also be used to control simple relays output

##Parameters of MySensorsOutputLight
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
data_type | string | no | Data type sent to the node. Default: V_LIGHT, see MySensors.cpp for more values.
sensor_id | string | yes | Sensor ID, as set in your node
host | string | yes | IP address of the tcp gateway if relevant
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gateway | list | yes | Gateway type used, tcp or serial are supported
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
io_type | string | yes | IO type, can be "input", "output", "inout"
port | string | yes | If using serial gateway, port is the serial port (/dev/ttyUSB0 for ex.). If using tcp gateway port is TCP port of the gateway.
node_id | string | yes | Node ID as set in your network
id | string | yes | Unique ID identifying the Input/Output in calaos-server
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
io_style | list | yes | GUI style display. This will control the icon displayed on the UI

##Conditions of MySensorsOutputLight
Name | Description
---- | -----------
false | Event when light is off 
 changed | Event on any change of value 
 true | Event when light is on 
 
##Actions of MySensorsOutputLight
Name | Description
---- | -----------
set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 false | Switch the light off 
 true | Switch the light on 
 toggle | Invert light state 
 impulse 500 200 500 200 | Do an impulse on light state with a pattern.<br>Ex: 500 200 500 200 means: TRUE for 500ms, FALSE for 200ms, TRUE for 500ms, FALSE for 200ms<br>Ex: 500 loop 200 300 means: TRUE for 500ms, then loop the next steps for infinite, FALSE for 200ms, TRUE for 300ms<br>Ex: 100 100 200 old means: blinks and then set to the old start state (before impulse starts) 
 impulse 200 | Do an impulse on light state. Set to true for X ms then reset to false 
 
##More Infos
* MySensors: http://mysensors.org


#MySensorsOutputLightRGB
RGB Light dimmer with MySensors node


RGB light. Choose a color to be set for this light.

##Parameters of MySensorsOutputLightRGB
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
data_type | string | no | Data type sent to the node. Default: V_DIMMER, see MySensors.cpp for more values.
sensor_id_blue | string | yes | Sensor ID blue red channel, as set in your node
sensor_id_green | string | yes | Sensor ID green red channel, as set in your node
node_id_green | string | yes | Node ID for green channel, as set in your network
sensor_id_red | string | yes | Sensor ID for red channel, as set in your node
node_id_red | string | yes | Node ID for red channel, as set in your network
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gateway | list | yes | Gateway type used, tcp or serial are supported
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
io_type | string | yes | IO type, can be "input", "output", "inout"
port | string | yes | If using serial gateway, port is the serial port (/dev/ttyUSB0 for ex.). If using tcp gateway port is TCP port of the gateway.
node_id_blue | string | yes | Node ID for blue channel, as set in your network
id | string | yes | Unique ID identifying the Input/Output in calaos-server
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
host | string | yes | IP address of the tcp gateway if relevant

##Conditions of MySensorsOutputLightRGB
Name | Description
---- | -----------
value | Event when light is at this value 
 changed | Event on any change of value 
 
##Actions of MySensorsOutputLightRGB
Name | Description
---- | -----------
down_green 5 | Decrease intensity by X percent of green channel 
 down_red 5 | Decrease intensity by X percent of red channel 
 up_blue 5 | Increase intensity by X percent of blue channel 
 up_green 5 | Increase intensity by X percent of green channel 
 false | Switch the light off 
 set #AA1294 | Set color. Color can be represented by using HTML notation: #AABBCC, rgb(50, 10, 30), hsl(11, 22, 33) 
 set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 toggle | Invert the light state (ON/OFF) 
 true | Switch the light on 
 set_blue 50 | Set blue channel to X percent 
 up_red 5 | Increase intensity by X percent of red channel 
 down_blue 5 | Decrease intensity by X percent of blue channel 
 set_state #AA1294 | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 set_red 50 | Set red channel to X percent 
 set_green 50 | Set green channel to X percent 
 
##More Infos
* MySensors: http://mysensors.org


#MySensorsOutputShutter
Shutter with MySensors node


Simple shutter. This shutter supports open/close states, as well as impulse shutters.

##Parameters of MySensorsOutputShutter
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
data_type | string | no | Data type sent to the node. Default: V_LIGHT, see MySensors.cpp for more values.
sensor_id_down | string | yes | Sensor ID for closing shutter, as set in your node
node_id_up | string | yes | Node ID for opening shutter, as set in your network
sensor_id_up | string | yes | Sensor ID for opening shutter, as set in your node
host | string | yes | IP address of the tcp gateway if relevant
gateway | list | yes | Gateway type used, tcp or serial are supported
name | string | yes | Name of Input/Output.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
id | string | yes | Unique ID identifying the Input/Output in calaos-server
node_id_down | string | yes | Node ID for closing shutter, as set in your network
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
time | int | yes | Time in sec for shutter to open or close
port | string | yes | If using serial gateway, port is the serial port (/dev/ttyUSB0 for ex.). If using tcp gateway port is TCP port of the gateway.
io_type | string | yes | IO type, can be "input", "output", "inout"
impulse_time | int | no | Impulse time for shutter that needs impulse instead of holding up/down relays. If set to 0 impulse shutter is disabled. Time is in ms. Default to 0
stop_both | bool | no | If in impulse mode, some shutters needs to activate both up dans down relays when stopping the shutter

##Conditions of MySensorsOutputShutter
Name | Description
---- | -----------
false | Event when shutter is closed 
 changed | Event on any change of shutter state 
 true | Event when shutter is open 
 
##Actions of MySensorsOutputShutter
Name | Description
---- | -----------
set_state true | Update internal shutter state without starting real action. This is useful when having updating the shutter state from an external source. 
 set_state false | Update internal shutter state without starting real action. This is useful when having updating the shutter state from an external source. 
 impulse down 200 | Close shutter for X ms 
 down | Close the shutter 
 up | Open the shutter 
 toggle | Invert shutter state 
 stop | Stop the shutter 
 impulse up 200 | Open shutter for X ms 
 
##More Infos
* MySensors: http://mysensors.org


#MySensorsOutputShutterSmart
Smart shutter with MySensors node


Smart shutter. This shutter calculates the position of the shutter based on the time it takes to open and close. It then allows to set directly the shutter at a specified position.

##Parameters of MySensorsOutputShutterSmart
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
data_type | string | no | Data type sent to the node. Default: V_LIGHT, see MySensors.cpp for more values.
sensor_id_down | string | yes | Sensor ID for closing shutter, as set in your node
node_id_up | string | yes | Node ID for opening shutter, as set in your network
sensor_id_up | string | yes | Sensor ID for opening shutter, as set in your node
host | string | yes | IP address of the tcp gateway if relevant
stop_both | bool | no | If in impulse mode, some shutters needs to activate both up dans down relays when stopping the shutter
gateway | list | yes | Gateway type used, tcp or serial are supported
name | string | yes | Name of Input/Output.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
id | string | yes | Unique ID identifying the Input/Output in calaos-server
time_down | int | yes | Time in sec for shutter to fully closed. The more accurate, the better it will work
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
node_id_down | string | yes | Node ID for closing shutter, as set in your network
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
time_up | int | yes | Time in sec for shutter to be fully open. The more accurate, the better it will work
port | string | yes | If using serial gateway, port is the serial port (/dev/ttyUSB0 for ex.). If using tcp gateway port is TCP port of the gateway.
io_type | string | yes | IO type, can be "input", "output", "inout"
impulse_time | int | no | Impulse time for shutter that needs impulse instead of holding up/down relays. If set to 0 impulse shutter is disabled. Time is in ms. Default to 0

##Conditions of MySensorsOutputShutterSmart
Name | Description
---- | -----------
false | Event when shutter is closed 
 changed | Event on any change of shutter state 
 true | Event when shutter is open 
 
##Actions of MySensorsOutputShutterSmart
Name | Description
---- | -----------
impulse up 200 | Open shutter for X ms 
 set 50 | Set shutter at position X in percent 
 up | Open the shutter 
 down | Close the shutter 
 impulse down 200 | Close shutter for X ms 
 set_state false | Update internal shutter state without starting real action. This is useful when having updating the shutter state from an external source. 
 toggle | Invert shutter state 
 up 5 | Open the shutter by X percent 
 calibrate | Start calibration on shutter. This opens fully the shutter and resets all internal position values. Use this if shutter sync is lost. 
 stop | Stop the shutter 
 down 5 | Close the shutter by X percent 
 set_state true | Update internal shutter state without starting real action. This is useful when having updating the shutter state from an external source. 
 
##More Infos
* MySensors: http://mysensors.org


#MySensorsOutputString
String output with MySensors node

##Parameters of MySensorsOutputString
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
data_type | string | no | Data type sent to the node. Default: V_VAR1, see MySensors.cpp for more values.
sensor_id | string | yes | Sensor ID, as set in your node
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gateway | list | yes | Gateway type used, tcp or serial are supported
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
io_type | string | yes | IO type, can be "input", "output", "inout"
port | string | yes | If using serial gateway, port is the serial port (/dev/ttyUSB0 for ex.). If using tcp gateway port is TCP port of the gateway.
node_id | string | yes | Node ID as set in your network
id | string | yes | Unique ID identifying the Input/Output in calaos-server
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
host | string | yes | IP address of the tcp gateway if relevant

##More Infos
* MySensors: http://mysensors.org


#OLAOutputLightDimmer
DMX Light dimmer using OLA (Open Lighting Architecture)


Light with dimming control. Light intensity can be changed for this light.

##Parameters of OLAOutputLightDimmer
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
universe | int | yes | OLA universe to control
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
channel | int | yes | DMX channel to control
log_history | bool | no | If enabled, write an entry in the history event log for this IO
io_type | string | yes | IO type, can be "input", "output", "inout"
name | string | yes | Name of Input/Output.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.

##Conditions of OLAOutputLightDimmer
Name | Description
---- | -----------
value | Event when light is at this value 
 changed | Event on any change of value 
 
##Actions of OLAOutputLightDimmer
Name | Description
---- | -----------
hold stop | Dynamically change light intensity when holding a switch (stop action) 
 hold press | Dynamically change light intensity when holding a switch (press action) 
 down 5 | Decrease intensity by X percent 
 impulse 500 200 500 200 | Do an impulse on light state with a pattern.<br>Ex: 500 200 500 200 means: TRUE for 500ms, FALSE for 200ms, TRUE for 500ms, FALSE for 200ms<br>Ex: 500 loop 200 300 means: TRUE for 500ms, then loop the next steps for infinite, FALSE for 200ms, TRUE for 300ms<br>Ex: 100 100 200 old means: blinks and then set to the old start state (before impulse starts) 
 set off 50 | Set light value without switching on. This will be the light intensity for the next ON 
 true | Switch the light on 
 set_state 50 | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 false | Switch the light off 
 toggle | Invert the light state 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 up 5 | Increase intensity by X percent 
 set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 impulse 200 | Do an impulse on light state. Set to true for X ms then reset to false 
 set 50 | Set light intensity and swith on if light is off 
 
##More Infos
* OLA: http://www.openlighting.org


#OLAOutputLightRGB
RGB Light dimmer using 3 DMX channels with OLA (Open Lighting Architecture)


RGB light. Choose a color to be set for this light.

##Parameters of OLAOutputLightRGB
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
name | string | yes | Name of Input/Output.
io_type | string | yes | IO type, can be "input", "output", "inout"
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
id | string | yes | Unique ID identifying the Input/Output in calaos-server
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
channel_blue | int | yes | DMX channel for blue to control
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
universe | int | yes | OLA universe to control
channel_green | int | yes | DMX channel for green to control
channel_red | int | yes | DMX channel for red to control

##Conditions of OLAOutputLightRGB
Name | Description
---- | -----------
value | Event when light is at this value 
 changed | Event on any change of value 
 
##Actions of OLAOutputLightRGB
Name | Description
---- | -----------
down_green 5 | Decrease intensity by X percent of green channel 
 down_red 5 | Decrease intensity by X percent of red channel 
 up_blue 5 | Increase intensity by X percent of blue channel 
 up_green 5 | Increase intensity by X percent of green channel 
 false | Switch the light off 
 set #AA1294 | Set color. Color can be represented by using HTML notation: #AABBCC, rgb(50, 10, 30), hsl(11, 22, 33) 
 set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 toggle | Invert the light state (ON/OFF) 
 true | Switch the light on 
 set_blue 50 | Set blue channel to X percent 
 up_red 5 | Increase intensity by X percent of red channel 
 down_blue 5 | Decrease intensity by X percent of blue channel 
 set_state #AA1294 | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 set_red 50 | Set red channel to X percent 
 set_green 50 | Set green channel to X percent 
 
##More Infos
* OLA: http://www.openlighting.org


#OutputFake
Fake test output. Do nothing. Do not use.

##Parameters of OutputFake
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
log_history | bool | no | If enabled, write an entry in the history event log for this IO
io_type | string | yes | IO type, can be "input", "output", "inout"
name | string | yes | Name of Input/Output.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.


#OWTemp
Temperature measurement with DS18B20 Onewire Sensor


Temperature sensor input. Use for displaying temperature and to control heating devices with rules based on temperature value

##Parameters of OWTemp
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
ow_args | string | yes | Additional parameter used for owfs initialization.For example you can use -u to use the USB owfs drivers
ow_id | string | yes | Unique ID of sensor on OneWire bus.
use_w1 | bool | no | Force the use of w1 kernel driver instead of OneWire driver
precision | int | no | Precision of the returned value. The value represents the number of decimal after the dot. The value is rounded like this : value = 19.275 => returned value 19.28 when preicision = 2, 19.3 when precision = 1, 19 when precision = 0
frequency | float | no | Sampling time in microsecond. The value is read at this frequency. If this value is not set, calaos tries to read the interval parameter
name | string | yes | Name of Input/Output.
io_type | string | yes | IO type, can be "input", "output", "inout"
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
id | string | yes | Unique ID identifying the Input/Output in calaos-server
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
interval | float | no | Sampling time in seconds. The value is read at this frequency. If this value is not set, the default value is 15s
coeff_a | float | no | use in conjunction of coeff_b to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 1.0.
coeff_b | float | no | use in conjunction of coeff_a to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 0.0
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
offset | float | no | same as coeff_b, can be used alone. Default value is 0.0

##Conditions of OWTemp
Name | Description
---- | -----------
changed | Event on any change of temperature value 
 value | Event on a temperature value in degree Celsius 
 
##More Infos
* Calaos Wiki: http://calaos.fr/wiki/OneWire


#PingInputSwitch
A switch input based on the result of a ping command. Useful to detect presence of a host on the network.


Basic switch with press/release states.

##Parameters of PingInputSwitch
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
interval | int | no | Interval between pings in ms. Default to 15 sec
timeout | int | no | Timeout of the ping request in ms
host | string | yes | IP address or host where to send the ping
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | A switch can't be visible. Always false.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
io_type | string | yes | IO type, can be "input", "output", "inout"
name | string | yes | Name of Input/Output.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.

##Conditions of PingInputSwitch
Name | Description
---- | -----------
true | The host is online and respond to the ping 
 changed | Event on any change of state 
 false | The host is offline and/or does not respond to the ping 
 
#Planet - UNDOCUMENTED IO
SPANK SPANK SPANK : naughty programmer ! You did not add documentation for this IO, that's BAD :'(
Go document it in your code or you will burn in hell!



Planet IP Camera/Encoder. Camera can be viewed directly inside calaos and used in rules.

##Parameters of Planet
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
password | string | no | Password for user
username | string | no | Username for accessing the camera
model | string | yes | Camera model (ICA-210, ICA-210W, ICA-300, ICA-302, ICA-500) to use
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
log_history | bool | no | If enabled, write an entry in the history event log for this IO
io_type | string | yes | IO type, can be "input", "output", "inout"
name | string | yes | Name of Input/Output.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.


#Scenario
A scenario variable. Use this like a virtual button to start a scenario (list of actions)

##Parameters of Scenario
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
auto_scenario | string | no | Internal use only for Auto Scenario. read only.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
log_history | bool | no | If enabled, write an entry in the history event log for this IO
io_type | string | yes | IO type, can be "input", "output", "inout"
name | string | yes | Name of Input/Output.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.

##Conditions of Scenario
Name | Description
---- | -----------
true | Event triggered when scenario is started 
 
##Actions of Scenario
Name | Description
---- | -----------
true | Start the scenario 
 changed | Event triggered on any change 
 false | Stop the scenario (only for special looping scenarios) 
 

#Squeezebox
#### Alias: slim
Squeezebox audio player allows control of a Squeezebox from Calaos

##Parameters of Squeezebox
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
port_web | int | no | Web interface port of LMS, default to 9000.
port_cli | int | no | CLI port of LMS, default to 9090
host | string | yes | Logitech media server IP address
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID of squeezebox in LMS
visible | bool | yes | Audio players are not displayed in rooms
log_history | bool | no | If enabled, write an entry in the history event log for this IO
io_type | string | yes | IO type, can be "input", "output", "inout"
name | string | yes | Name of Input/Output.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.

##Conditions of Squeezebox
Name | Description
---- | -----------
onvolumechange | Event when a change of volume happens 
 onpause | Event when pausing player 
 onstop | Event when stopping player 
 onplay | Event when play is started 
 onplaylistchange | Event when a change in the current playlist happens 
 onsongchange | Event when a new song is being played 
 
##Actions of Squeezebox
Name | Description
---- | -----------
add <argument> | Add tracks to playlist. <argument> can be any of album_id:XX artist_id:XX playlist_id:XX, ... 
 play <argument> | Clear playlist and play argument. <argument> can be any of album_id:XX artist_id:XX playlist_id:XX, ... 
 sync <playerid> | Sync this player with an other 
 previous | Play previous song in playlist 
 power off | Switch player off 
 volume down 1 | Decrease volume by a value 
 unsync <playerid> | Stop sync of this player with an other 
 play | Start playing 
 stop | Stop player 
 pause | Pause player 
 volume set 50 | Set current volume 
 next | Play next song in playlist 
 sleep 10 | Start sleep mode with X seconds 
 volume up 1 | Increase volume by a value 
 power on | Switch player on 
 
#standard_mjpeg - UNDOCUMENTED IO
SPANK SPANK SPANK : naughty programmer ! You did not add documentation for this IO, that's BAD :'(
Go document it in your code or you will burn in hell!



MJPEG/Jpeg IP Camera. Camera can be viewed directly inside calaos and used in rules.

##Parameters of standard_mjpeg
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
url_mjpeg | string | no | URL for mjpeg stream support
ptz | bool | no | Set to true if camera has PTZ support
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
url_jpeg | string | yes | URL for snapshot in jpeg format
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
log_history | bool | no | If enabled, write an entry in the history event log for this IO
io_type | string | yes | IO type, can be "input", "output", "inout"
name | string | yes | Name of Input/Output.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.

#StandardMjpeg - UNDOCUMENTED IO
SPANK SPANK SPANK : naughty programmer ! You did not add documentation for this IO, that's BAD :'(
Go document it in your code or you will burn in hell!



MJPEG/Jpeg IP Camera. Camera can be viewed directly inside calaos and used in rules.

##Parameters of StandardMjpeg
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
url_mjpeg | string | no | URL for mjpeg stream support
ptz | bool | no | Set to true if camera has PTZ support
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
url_jpeg | string | yes | URL for snapshot in jpeg format
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
log_history | bool | no | If enabled, write an entry in the history event log for this IO
io_type | string | yes | IO type, can be "input", "output", "inout"
name | string | yes | Name of Input/Output.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.

#SynoSurveillanceStation - UNDOCUMENTED IO
SPANK SPANK SPANK : naughty programmer ! You did not add documentation for this IO, that's BAD :'(
Go document it in your code or you will burn in hell!



Synology Surveillance Station IP Camera. Camera can be viewed directly inside calaos and used in rules.

##Parameters of SynoSurveillanceStation
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
name | string | yes | Name of Input/Output.
camera_profile | int | no | Profile to use for snapshot. 0- High quality, 1- Balanced, 2- Low bandwidth
io_type | string | yes | IO type, can be "input", "output", "inout"
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
id | string | yes | Unique ID identifying the Input/Output in calaos-server
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
camera_id | string | yes | ID of the camera
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
url | string | yes | Full url to Synology nas. Ex: https://192.168.0.22:5000
username | string | yes | Username which can access Surveillance Station
password | string | yes | Password for user


#TeleinfoInputAnalog
Analog measurement retrieved from Teleinfo informations.


An analog input can be used to read analog values to display them and use them in rules.

##Parameters of TeleinfoInputAnalog
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
value | list | yes | All theses values are reported by the Teleinfo equipment as double.
io_style | list | yes | GUI style display. This will control the icon displayed on the UI
unit | string | no | Unit which will be displayed on the UI as a suffix.
precision | int | no | Precision of the returned value. The value represents the number of decimal after the dot. The value is rounded like this : value = 19.275 => returned value 19.28 when preicision = 2, 19.3 when precision = 1, 19 when precision = 0
frequency | float | no | Sampling time in microsecond. The value is read at this frequency. If this value is not set, calaos tries to read the interval parameter
name | string | yes | Name of Input/Output.
port | string | yes | port on which to get Teleinfo information usually a serial port like /dev/ttyS0 or /dev/ttyAMA0
io_type | string | yes | IO type, can be "input", "output", "inout"
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
id | string | yes | Unique ID identifying the Input/Output in calaos-server
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
interval | float | no | Sampling time in seconds. The value is read at this frequency. If this value is not set, the default value is 15s
coeff_a | float | no | use in conjunction of coeff_b to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 1.0.
coeff_b | float | no | use in conjunction of coeff_a to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 0.0
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
offset | float | no | same as coeff_b, can be used alone. Default value is 0.0

##Conditions of TeleinfoInputAnalog
Name | Description
---- | -----------
changed | Event on any change of value 
 value | Event on a specific value 
 

#TimeRange
#### Alias: InPlageHoraire
Represent a time range object. A time range is true if current time is in one of the included range, false otherwise. The time range also support weekdays and months.

##Parameters of TimeRange
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | A time range can't be visible. Always false.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
io_type | string | yes | IO type, can be "input", "output", "inout"
name | string | yes | Name of Input/Output.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.

##Conditions of TimeRange
Name | Description
---- | -----------
true | Event triggered when entering the range 
 changed | Event on any change of range 
 false | Event triggered when exiting the range 
 

#WebInputAnalog
Analog input read from a web document


An analog input can be used to read analog values to display them and use them in rules.

##Parameters of WebInputAnalog
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
path | string | yes | The path where to found the value. This value can take multiple values depending on the file type. If file_type is JSON, the json file downloaded will be read, and the informations will be extracted from the path. for example weather[0]/description, try to read the description value of the 1 element of the array of the weather object.
If file_type is XML, the path is an xpath expression; Look here for syntax : http://www.w3schools.com/xsl/xpath_syntax.asp If file_type is TEXT, the downloaded file is returned as plain text file, and path must be in the form line/pos/separator Line is read, and is split using separator as delimiters The value returned is the value at pos in the split list. If the separator is not found, the whole line is returned. Example the file contains 
10.0,10.1,10.2,10.3
20.0,20.1,20.2,20.3
If the path is 2/4/, the value returne wil be 20.3

file_type | string | yes | File type of the document. Values can be xml, json or text.
url | string | yes | URL where to download the document from
If URL begins with / or with file:// the data is read from the local file
io_style | list | yes | GUI style display. This will control the icon displayed on the UI
unit | string | no | Unit which will be displayed on the UI as a suffix.
precision | int | no | Precision of the returned value. The value represents the number of decimal after the dot. The value is rounded like this : value = 19.275 => returned value 19.28 when preicision = 2, 19.3 when precision = 1, 19 when precision = 0
frequency | float | no | Sampling time in microsecond. The value is read at this frequency. If this value is not set, calaos tries to read the interval parameter
name | string | yes | Name of Input/Output.
io_type | string | yes | IO type, can be "input", "output", "inout"
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
id | string | yes | Unique ID identifying the Input/Output in calaos-server
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
interval | float | no | Sampling time in seconds. The value is read at this frequency. If this value is not set, the default value is 15s
coeff_a | float | no | use in conjunction of coeff_b to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 1.0.
coeff_b | float | no | use in conjunction of coeff_a to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 0.0
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
offset | float | no | same as coeff_b, can be used alone. Default value is 0.0

##Conditions of WebInputAnalog
Name | Description
---- | -----------
changed | Event on any change of value 
 value | Event on a specific value 
 

#WebInputString
String input providing from a web document

##Parameters of WebInputString
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
path | string | yes | The path where to found the value. This value can take multiple values depending on the file type. If file_type is JSON, the json file downloaded will be read, and the informations will be extracted from the path. for example weather[0]/description, try to read the description value of the 1 element of the array of the weather object.
If file_type is XML, the path is an xpath expression; Look here for syntax : http://www.w3schools.com/xsl/xpath_syntax.asp If file_type is TEXT, the downloaded file is returned as plain text file, and path must be in the form line/pos/separator Line is read, and is split using separator as delimiters The value returned is the value at pos in the split list. If the separator is not found, the whole line is returned. Example the file contains 
10.0,10.1,10.2,10.3
20.0,20.1,20.2,20.3
If the path is 2/4/, the value returne wil be 20.3

url | string | yes | URL where to download the document from
If URL begins with / or with file:// the data is read from the local file
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
file_type | string | yes | File type of the document. Values can be xml, json or text.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
io_type | string | yes | IO type, can be "input", "output", "inout"
name | string | yes | Name of Input/Output.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.


#WebInputTemp
Temperature input read from a web document


Temperature sensor input. Use for displaying temperature and to control heating devices with rules based on temperature value

##Parameters of WebInputTemp
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
path | string | yes | The path where to found the value. This value can take multiple values depending on the file type. If file_type is JSON, the json file downloaded will be read, and the informations will be extracted from the path. for example weather[0]/description, try to read the description value of the 1 element of the array of the weather object.
If file_type is XML, the path is an xpath expression; Look here for syntax : http://www.w3schools.com/xsl/xpath_syntax.asp If file_type is TEXT, the downloaded file is returned as plain text file, and path must be in the form line/pos/separator Line is read, and is split using separator as delimiters The value returned is the value at pos in the split list. If the separator is not found, the whole line is returned. Example the file contains 
10.0,10.1,10.2,10.3
20.0,20.1,20.2,20.3
If the path is 2/4/, the value returne wil be 20.3

file_type | string | yes | File type of the document. Values can be xml, json or text.
url | string | yes | URL where to download the document from
If URL begins with / or with file:// the data is read from the local file
precision | int | no | Precision of the returned value. The value represents the number of decimal after the dot. The value is rounded like this : value = 19.275 => returned value 19.28 when preicision = 2, 19.3 when precision = 1, 19 when precision = 0
frequency | float | no | Sampling time in microsecond. The value is read at this frequency. If this value is not set, calaos tries to read the interval parameter
name | string | yes | Name of Input/Output.
io_type | string | yes | IO type, can be "input", "output", "inout"
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
id | string | yes | Unique ID identifying the Input/Output in calaos-server
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
interval | float | no | Sampling time in seconds. The value is read at this frequency. If this value is not set, the default value is 15s
coeff_a | float | no | use in conjunction of coeff_b to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 1.0.
coeff_b | float | no | use in conjunction of coeff_a to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 0.0
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
offset | float | no | same as coeff_b, can be used alone. Default value is 0.0

##Conditions of WebInputTemp
Name | Description
---- | -----------
changed | Event on any change of temperature value 
 value | Event on a temperature value in degree Celsius 
 

#WebOutputAnalog
Analog output in a web request


Analog output. Useful to control analog output devices connected to calaos.

##Parameters of WebOutputAnalog
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
path | string | yes | The path where to found the value. This value can take multiple values depending on the file type. If file_type is JSON, the json file downloaded will be read, and the informations will be extracted from the path. for example weather[0]/description, try to read the description value of the 1 element of the array of the weather object.
If file_type is XML, the path is an xpath expression; Look here for syntax : http://www.w3schools.com/xsl/xpath_syntax.asp If file_type is TEXT, the downloaded file is returned as plain text file, and path must be in the form line/pos/separator Line is read, and is split using separator as delimiters The value returned is the value at pos in the split list. If the separator is not found, the whole line is returned. Example the file contains 
10.0,10.1,10.2,10.3
20.0,20.1,20.2,20.3
If the path is 2/4/, the value returne wil be 20.3

file_type | string | yes | File type of the document. Values can be xml, json or text.
url | string | yes | URL where to download the document from
If URL begins with / or with file:// the data is read from the local file
io_style | list | yes | GUI style display. This will control the icon displayed on the UI
unit | string | no | Unit which will be displayed on the UI as a suffix.
name | string | yes | Name of Input/Output.
io_type | string | yes | IO type, can be "input", "output", "inout"
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
id | string | yes | Unique ID identifying the Input/Output in calaos-server
step | float | no | Set a step for increment/decrement value. Default is 1.0
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
coeff_a | float | no | use in conjunction of coeff_b to apply equation of the form `value_sent = coeff_a * raw_value + coeff_b`. Default value is 1.0.
coeff_b | float | no | use in conjunction of coeff_a to apply equation of the form `value_sent = coeff_a * raw_value + coeff_b`. Default value is 0.0

##Conditions of WebOutputAnalog
Name | Description
---- | -----------
0 | Event on a specific number value 
 value | Event on a specific value 
 changed | Event on any change of value 
 
##Actions of WebOutputAnalog
Name | Description
---- | -----------
dec 1 | Decrement value by value 
 inc 1 | Increment value by value 
 dec | Decrement value with configured step 
 0 | Set a specific number value 
 inc | Increment value with configured step 
 

#WebOutputLight
Bool output written to a web document or URL


Basic light. This light have only 2 states, ON or OFF. Can also be used to control simple relays output

##Parameters of WebOutputLight
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
data_type | string | yes | The HTTP header Content-Type used when posting the document. It depends on the website, but you can use application/json application/xml as correct values.
name | string | yes | Name of Input/Output.
io_type | string | yes | IO type, can be "input", "output", "inout"
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
id | string | yes | Unique ID identifying the Input/Output in calaos-server
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
io_style | list | yes | GUI style display. This will control the icon displayed on the UI
url | string | yes | URL where to POST the document to. The POST request is associated with the data field if not null. When no data is provided, Calaos substitutes __##VALUE##__ string with the value to send. For example if the url is http://example.com/api?value=__##VALUE##__ the url post will be :
http://example.com/api?value=20.3
The url is encoded before being sent.
If the URL begins with / or file:// the data is written to a file.
data | string | yes | The document send when posting data. This value can be void, in, that case the value is substituted in the url, otherwise the __##VALUE##__ contained in data is substituted with with the value to be sent.

##Conditions of WebOutputLight
Name | Description
---- | -----------
false | Event when light is off 
 changed | Event on any change of value 
 true | Event when light is on 
 
##Actions of WebOutputLight
Name | Description
---- | -----------
set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 false | Switch the light off 
 true | Switch the light on 
 toggle | Invert light state 
 impulse 500 200 500 200 | Do an impulse on light state with a pattern.<br>Ex: 500 200 500 200 means: TRUE for 500ms, FALSE for 200ms, TRUE for 500ms, FALSE for 200ms<br>Ex: 500 loop 200 300 means: TRUE for 500ms, then loop the next steps for infinite, FALSE for 200ms, TRUE for 300ms<br>Ex: 100 100 200 old means: blinks and then set to the old start state (before impulse starts) 
 impulse 200 | Do an impulse on light state. Set to true for X ms then reset to false 
 

#WebOutputLightRGB
RGB value written to a web document or URL


RGB light. Choose a color to be set for this light.

##Parameters of WebOutputLightRGB
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
raw_value | bool | no | RGB value has #RRGGBB. Sometimes some web api take only RRGGBBformat. If raw_value is true, the # in front of the line isremoved. The default value for this parameter is false.
name | string | yes | Name of Input/Output.
io_type | string | yes | IO type, can be "input", "output", "inout"
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
id | string | yes | Unique ID identifying the Input/Output in calaos-server
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
url | string | yes | URL where to POST the document to. The POST request is associated with the data field if not null. When no data is provided, Calaos substitutes __##VALUE##__ string with the value to send. For example if the url is http://example.com/api?value=__##VALUE##__ the url post will be :
http://example.com/api?value=20.3
The url is encoded before being sent.
If the URL begins with / or file:// the data is written to a file.
data | string | yes | The document send when posting data. This value can be void, in, that case the value is substituted in the url, otherwise the __##VALUE##__ contained in data is substituted with with the value to be sent.
data_type | string | yes | The HTTP header Content-Type used when posting the document. It depends on the website, but you can use application/json application/xml as correct values.

##Conditions of WebOutputLightRGB
Name | Description
---- | -----------
value | Event when light is at this value 
 changed | Event on any change of value 
 
##Actions of WebOutputLightRGB
Name | Description
---- | -----------
down_green 5 | Decrease intensity by X percent of green channel 
 down_red 5 | Decrease intensity by X percent of red channel 
 up_blue 5 | Increase intensity by X percent of blue channel 
 up_green 5 | Increase intensity by X percent of green channel 
 false | Switch the light off 
 set #AA1294 | Set color. Color can be represented by using HTML notation: #AABBCC, rgb(50, 10, 30), hsl(11, 22, 33) 
 set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 toggle | Invert the light state (ON/OFF) 
 true | Switch the light on 
 set_blue 50 | Set blue channel to X percent 
 up_red 5 | Increase intensity by X percent of red channel 
 down_blue 5 | Decrease intensity by X percent of blue channel 
 set_state #AA1294 | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 set_red 50 | Set red channel to X percent 
 set_green 50 | Set green channel to X percent 
 

#WebOutputString
String output written to a web document or URL

##Parameters of WebOutputString
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
data_type | string | yes | The HTTP header Content-Type used when posting the document. It depends on the website, but you can use application/json application/xml as correct values.
data | string | yes | The document send when posting data. This value can be void, in, that case the value is substituted in the url, otherwise the __##VALUE##__ contained in data is substituted with with the value to be sent.
name | string | yes | Name of Input/Output.
io_type | string | yes | IO type, can be "input", "output", "inout"
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
file_type | string | yes | File type of the document. Values can be xml, json or text.
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
id | string | yes | Unique ID identifying the Input/Output in calaos-server
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
url | string | yes | URL where to POST the document to. The POST request is associated with the data field if not null. When no data is provided, Calaos substitutes __##VALUE##__ string with the value to send. For example if the url is http://example.com/api?value=__##VALUE##__ the url post will be :
http://example.com/api?value=20.3
The url is encoded before being sent.
If the URL begins with / or file:// the data is written to a file.
path | string | yes | The path where to found the value. This value can take multiple values depending on the file type. If file_type is JSON, the json file downloaded will be read, and the informations will be extracted from the path. for example weather[0]/description, try to read the description value of the 1 element of the array of the weather object.
If file_type is XML, the path is an xpath expression; Look here for syntax : http://www.w3schools.com/xsl/xpath_syntax.asp If file_type is TEXT, the downloaded file is returned as plain text file, and path must be in the form line/pos/separator Line is read, and is split using separator as delimiters The value returned is the value at pos in the split list. If the separator is not found, the whole line is returned. Example the file contains 
10.0,10.1,10.2,10.3
20.0,20.1,20.2,20.3
If the path is 2/4/, the value returne wil be 20.3



#WIAnalog
#### Alias: WagoInputAnalog
Analog measurement with Wago module (like 0-10V, 4-20mA, ...)


An analog input can be used to read analog values to display them and use them in rules.

##Parameters of WIAnalog
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
host | string | yes | Wago PLC IP address on the network
io_style | list | yes | GUI style display. This will control the icon displayed on the UI
var | int | yes | PLC address of the input sensor
unit | string | no | Unit which will be displayed on the UI as a suffix.
precision | int | no | Precision of the returned value. The value represents the number of decimal after the dot. The value is rounded like this : value = 19.275 => returned value 19.28 when preicision = 2, 19.3 when precision = 1, 19 when precision = 0
frequency | float | no | Sampling time in microsecond. The value is read at this frequency. If this value is not set, calaos tries to read the interval parameter
name | string | yes | Name of Input/Output.
port | int | no | Wago ethernet port, default to 502
io_type | string | yes | IO type, can be "input", "output", "inout"
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
id | string | yes | Unique ID identifying the Input/Output in calaos-server
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
interval | float | no | Sampling time in seconds. The value is read at this frequency. If this value is not set, the default value is 15s
coeff_a | float | no | use in conjunction of coeff_b to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 1.0.
coeff_b | float | no | use in conjunction of coeff_a to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 0.0
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
offset | float | no | same as coeff_b, can be used alone. Default value is 0.0

##Conditions of WIAnalog
Name | Description
---- | -----------
changed | Event on any change of value 
 value | Event on a specific value 
 
##More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/entree_analog


#WIDigitalBP
#### Alias: WIDigital, WagoInputSwitch
Switch with digital input Wago modules (like 750-1405, ...)


Basic switch with press/release states.

##Parameters of WIDigitalBP
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
host | string | yes | Wago PLC IP address on the network
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
var | int | yes | PLC address of the digital input
id | string | yes | Unique ID identifying the Input/Output in calaos-server
port | int | no | Wago ethernet port, default to 502
visible | bool | no | A switch can't be visible. Always false.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
io_type | string | yes | IO type, can be "input", "output", "inout"
name | string | yes | Name of Input/Output.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.

##Conditions of WIDigitalBP
Name | Description
---- | -----------
true | Event triggered when switch is pressed 
 changed | Event on any change of state 
 false | Event triggered when switch is released 
 
##More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/750-1045


#WIDigitalLong
#### Alias: WagoInputSwitchLongPress
Switch long press with digital input Wago modules (like 750-1405, ...)


Long press switch. This switch supports single press and long press. User has 500ms to perform the long press.

##Parameters of WIDigitalLong
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
host | string | yes | Wago PLC IP address on the network
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
var | int | yes | PLC address of the digital input
id | string | yes | Unique ID identifying the Input/Output in calaos-server
port | int | no | Wago ethernet port, default to 502
visible | bool | no | A switch can't be visible. Always false.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
io_type | string | yes | IO type, can be "input", "output", "inout"
name | string | yes | Name of Input/Output.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.

##Conditions of WIDigitalLong
Name | Description
---- | -----------
changed | Event on any change of state 
 1 | Event triggered when switch is pressed quickly 
 2 | Event triggered when switch is pressed at least for 500ms (long press) 
 
##More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/750-1045


#WIDigitalTriple
#### Alias: WagoInputSwitchTriple
Switch triple click with digital input Wago modules (like 750-1405, ...)


Triple click switch. This switch can start 3 kind of actions. User has 500ms to do a multiple click.

##Parameters of WIDigitalTriple
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
host | string | yes | Wago PLC IP address on the network
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
var | int | yes | PLC address of the digital input
id | string | yes | Unique ID identifying the Input/Output in calaos-server
port | int | no | Wago ethernet port, default to 502
visible | bool | no | A switch can't be visible. Always false.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
io_type | string | yes | IO type, can be "input", "output", "inout"
name | string | yes | Name of Input/Output.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.

##Conditions of WIDigitalTriple
Name | Description
---- | -----------
3 | Event triggered when switch is triple clicked 
 changed | Event on any change of state 
 1 | Event triggered when switch is single clicked 
 2 | Event triggered when switch is double clicked 
 
##More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/750-1045


#WITemp
#### Alias: WagoInputTemp
Temperature measurement with Wago temperature module (like 750-460)


Temperature sensor input. Use for displaying temperature and to control heating devices with rules based on temperature value

##Parameters of WITemp
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
host | string | yes | Wago PLC IP address on the network
var | int | yes | PLC address of the input sensor
precision | int | no | Precision of the returned value. The value represents the number of decimal after the dot. The value is rounded like this : value = 19.275 => returned value 19.28 when preicision = 2, 19.3 when precision = 1, 19 when precision = 0
frequency | float | no | Sampling time in microsecond. The value is read at this frequency. If this value is not set, calaos tries to read the interval parameter
name | string | yes | Name of Input/Output.
port | int | no | Wago ethernet port, default to 502
io_type | string | yes | IO type, can be "input", "output", "inout"
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
id | string | yes | Unique ID identifying the Input/Output in calaos-server
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
interval | float | no | Sampling time in seconds. The value is read at this frequency. If this value is not set, the default value is 15s
coeff_a | float | no | use in conjunction of coeff_b to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 1.0.
coeff_b | float | no | use in conjunction of coeff_a to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 0.0
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
offset | float | no | same as coeff_b, can be used alone. Default value is 0.0

##Conditions of WITemp
Name | Description
---- | -----------
changed | Event on any change of temperature value 
 value | Event on a temperature value in degree Celsius 
 
##More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/750-460


#WOAnalog
#### Alias: WagoOutputAnalog
Analog output with Wago module (like 0-10V, 4-20mA, ...)


Analog output. Useful to control analog output devices connected to calaos.

##Parameters of WOAnalog
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
host | string | yes | Wago PLC IP address on the network
io_style | list | yes | GUI style display. This will control the icon displayed on the UI
var | int | yes | PLC address of the output
unit | string | no | Unit which will be displayed on the UI as a suffix.
name | string | yes | Name of Input/Output.
port | int | no | Wago ethernet port, default to 502
io_type | string | yes | IO type, can be "input", "output", "inout"
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
id | string | yes | Unique ID identifying the Input/Output in calaos-server
step | float | no | Set a step for increment/decrement value. Default is 1.0
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
coeff_a | float | no | use in conjunction of coeff_b to apply equation of the form `value_sent = coeff_a * raw_value + coeff_b`. Default value is 1.0.
coeff_b | float | no | use in conjunction of coeff_a to apply equation of the form `value_sent = coeff_a * raw_value + coeff_b`. Default value is 0.0

##Conditions of WOAnalog
Name | Description
---- | -----------
0 | Event on a specific number value 
 value | Event on a specific value 
 changed | Event on any change of value 
 
##Actions of WOAnalog
Name | Description
---- | -----------
dec 1 | Decrement value by value 
 inc 1 | Increment value by value 
 dec | Decrement value with configured step 
 0 | Set a specific number value 
 inc | Increment value with configured step 
 
##More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/sortie_analog


#WODali
#### Alias: WagoOutputDimmer
Light using DALI or DMX. For DALI you need a 750-641 wago module. For DMX, a DMX4ALL-LAN device connected to the Wago PLC.


Light with dimming control. Light intensity can be changed for this light.

##Parameters of WODali
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
fade_time | int | no | DALI fade time. value is between 1-10
group | int | no | Set to 1 if address is a DALI group address, set to 0 otherwise.
name | string | yes | Name of Input/Output.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
id | string | yes | Unique ID identifying the Input/Output in calaos-server
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
address | int | yes | Device address. For DALI address is between 1-64. For DMX, the address starts at 100. So for DMX device 5, address should be 105
host | string | yes | Wago PLC IP address on the network
io_type | string | yes | IO type, can be "input", "output", "inout"
port | int | no | Wago ethernet port, default to 502
line | int | no | DALI bus line, usually 1

##Conditions of WODali
Name | Description
---- | -----------
value | Event when light is at this value 
 changed | Event on any change of value 
 
##Actions of WODali
Name | Description
---- | -----------
hold stop | Dynamically change light intensity when holding a switch (stop action) 
 hold press | Dynamically change light intensity when holding a switch (press action) 
 down 5 | Decrease intensity by X percent 
 impulse 500 200 500 200 | Do an impulse on light state with a pattern.<br>Ex: 500 200 500 200 means: TRUE for 500ms, FALSE for 200ms, TRUE for 500ms, FALSE for 200ms<br>Ex: 500 loop 200 300 means: TRUE for 500ms, then loop the next steps for infinite, FALSE for 200ms, TRUE for 300ms<br>Ex: 100 100 200 old means: blinks and then set to the old start state (before impulse starts) 
 set off 50 | Set light value without switching on. This will be the light intensity for the next ON 
 true | Switch the light on 
 set_state 50 | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 false | Switch the light off 
 toggle | Invert the light state 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 up 5 | Increase intensity by X percent 
 set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 impulse 200 | Do an impulse on light state. Set to true for X ms then reset to false 
 set 50 | Set light intensity and swith on if light is off 
 
##More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/750-641
* Calaos Wiki: http://calaos.fr/wiki/fr/dmx-lan


#WODaliRVB
#### Alias: WagoOutputDimmerRGB
RGB Light using DALI or DMX. To work you need 3 DALI/DMX channels. For DALI you need a 750-641 wago module. For DMX, a DMX4ALL-LAN device connected to the Wago PLC.


RGB light. Choose a color to be set for this light.

##Parameters of WODaliRVB
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
gfade_time | int | no | DALI fade time for green channel. value is between 1-10
gline | int | no | DALI bus line for green channel, usually 1
bgroup | int | no | Set to 1 if address for blue channel is a DALI group address, set to 0 otherwise.
rgroup | int | no | Set to 1 if address for red channel is a DALI group address, set to 0 otherwise.
raddress | int | yes | Device address for red channel. For DALI address is between 1-64. For DMX, the address starts at 100. So for DMX device 5, address should be 105
name | string | yes | Name of Input/Output.
bfade_time | int | no | DALI fade time for blue channel. value is between 1-10
bline | int | no | DALI bus line for blue channel, usually 1
gaddress | int | yes | Device address for green channel. For DALI address is between 1-64. For DMX, the address starts at 100. So for DMX device 5, address should be 105
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
ggroup | int | no | Set to 1 if address for green channel is a DALI group address, set to 0 otherwise.
io_type | string | yes | IO type, can be "input", "output", "inout"
port | int | no | Wago ethernet port, default to 502
baddress | int | yes | Device address for blue channel. For DALI address is between 1-64. For DMX, the address starts at 100. So for DMX device 5, address should be 105
id | string | yes | Unique ID identifying the Input/Output in calaos-server
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
rfade_time | int | no | DALI fade time for red channel. value is between 1-10
rline | int | no | DALI bus line for red channel, usually 1
host | string | yes | Wago PLC IP address on the network

##Conditions of WODaliRVB
Name | Description
---- | -----------
value | Event when light is at this value 
 changed | Event on any change of value 
 
##Actions of WODaliRVB
Name | Description
---- | -----------
down_green 5 | Decrease intensity by X percent of green channel 
 down_red 5 | Decrease intensity by X percent of red channel 
 up_blue 5 | Increase intensity by X percent of blue channel 
 up_green 5 | Increase intensity by X percent of green channel 
 false | Switch the light off 
 set #AA1294 | Set color. Color can be represented by using HTML notation: #AABBCC, rgb(50, 10, 30), hsl(11, 22, 33) 
 set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 toggle | Invert the light state (ON/OFF) 
 true | Switch the light on 
 set_blue 50 | Set blue channel to X percent 
 up_red 5 | Increase intensity by X percent of red channel 
 down_blue 5 | Decrease intensity by X percent of blue channel 
 set_state #AA1294 | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 set_red 50 | Set red channel to X percent 
 set_green 50 | Set green channel to X percent 
 
##More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/750-641
* Calaos Wiki: http://calaos.fr/wiki/fr/dmx-lan


#WODigital
#### Alias: WagoOutputLight
Simple light or relay control using wago digital output modules (like 750-1504, ...)


Basic light. This light have only 2 states, ON or OFF. Can also be used to control simple relays output

##Parameters of WODigital
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
wago_841 | bool | yes | Should be false if PLC is 750-842, true otherwise
var | int | yes | PLC address of the output
name | string | yes | Name of Input/Output.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
io_type | string | yes | IO type, can be "input", "output", "inout"
port | int | no | Wago ethernet port, default to 502
id | string | yes | Unique ID identifying the Input/Output in calaos-server
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
io_style | list | yes | GUI style display. This will control the icon displayed on the UI
knx | bool | no | Set to true if output is a KNX device (only for 750-849 with KNX/TP1 module)
host | string | yes | Wago PLC IP address on the network

##Conditions of WODigital
Name | Description
---- | -----------
false | Event when light is off 
 changed | Event on any change of value 
 true | Event when light is on 
 
##Actions of WODigital
Name | Description
---- | -----------
set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 false | Switch the light off 
 true | Switch the light on 
 toggle | Invert light state 
 impulse 500 200 500 200 | Do an impulse on light state with a pattern.<br>Ex: 500 200 500 200 means: TRUE for 500ms, FALSE for 200ms, TRUE for 500ms, FALSE for 200ms<br>Ex: 500 loop 200 300 means: TRUE for 500ms, then loop the next steps for infinite, FALSE for 200ms, TRUE for 300ms<br>Ex: 100 100 200 old means: blinks and then set to the old start state (before impulse starts) 
 impulse 200 | Do an impulse on light state. Set to true for X ms then reset to false 
 
##More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/750-1504


#WOLOutputBool
Wake On Lan output object. Send wake-on-lan packet to a device on the network.

##Parameters of WOLOutputBool
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
interval | int | no | Interval between pings in ms. Default to 15 sec
address | string | yes | Ethernet MAC address of the host to wake up
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
log_history | bool | no | If enabled, write an entry in the history event log for this IO
io_type | string | yes | IO type, can be "input", "output", "inout"
name | string | yes | Name of Input/Output.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.

##Actions of WOLOutputBool
Name | Description
---- | -----------
true | Send wake on lan packet to the configured device 
 

#WOVolet
#### Alias: WagoOutputShutter
Simple shutter using wago digital output modules (like 750-1504, ...)


Simple shutter. This shutter supports open/close states, as well as impulse shutters.

##Parameters of WOVolet
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
var_down | int | yes | Digital output address on the PLC for closing the shutter
knx | bool | no | Set to true if output is a KNX device (only for 750-849 with KNX/TP1 module)
host | string | yes | Wago PLC IP address on the network
var_up | int | yes | Digital output address on the PLC for opening the shutter
name | string | yes | Name of Input/Output.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
id | string | yes | Unique ID identifying the Input/Output in calaos-server
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
wago_841 | bool | yes | Should be false if PLC is 750-842, true otherwise
time | int | yes | Time in sec for shutter to open or close
port | int | no | Wago ethernet port, default to 502
io_type | string | yes | IO type, can be "input", "output", "inout"
impulse_time | int | no | Impulse time for shutter that needs impulse instead of holding up/down relays. If set to 0 impulse shutter is disabled. Time is in ms. Default to 0
stop_both | bool | no | If in impulse mode, some shutters needs to activate both up dans down relays when stopping the shutter

##Conditions of WOVolet
Name | Description
---- | -----------
false | Event when shutter is closed 
 changed | Event on any change of shutter state 
 true | Event when shutter is open 
 
##Actions of WOVolet
Name | Description
---- | -----------
set_state true | Update internal shutter state without starting real action. This is useful when having updating the shutter state from an external source. 
 set_state false | Update internal shutter state without starting real action. This is useful when having updating the shutter state from an external source. 
 impulse down 200 | Close shutter for X ms 
 down | Close the shutter 
 up | Open the shutter 
 toggle | Invert shutter state 
 stop | Stop the shutter 
 impulse up 200 | Open shutter for X ms 
 
##More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/750-1504


#WOVoletSmart
#### Alias: WagoOutputShutterSmart
Smart shutter using wago digital output modules (like 750-1504, ...)


Smart shutter. This shutter calculates the position of the shutter based on the time it takes to open and close. It then allows to set directly the shutter at a specified position.

##Parameters of WOVoletSmart
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
wago_841 | bool | yes | Should be false if PLC is 750-842, true otherwise
var_down | int | yes | Digital output address on the PLC for closing the shutter
knx | bool | no | Set to true if output is a KNX device (only for 750-849 with KNX/TP1 module)
host | string | yes | Wago PLC IP address on the network
stop_both | bool | no | If in impulse mode, some shutters needs to activate both up dans down relays when stopping the shutter
var_up | int | yes | Digital output address on the PLC for opening the shutter
name | string | yes | Name of Input/Output.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
id | string | yes | Unique ID identifying the Input/Output in calaos-server
time_down | int | yes | Time in sec for shutter to fully closed. The more accurate, the better it will work
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
time_up | int | yes | Time in sec for shutter to be fully open. The more accurate, the better it will work
port | int | no | Wago ethernet port, default to 502
io_type | string | yes | IO type, can be "input", "output", "inout"
impulse_time | int | no | Impulse time for shutter that needs impulse instead of holding up/down relays. If set to 0 impulse shutter is disabled. Time is in ms. Default to 0

##Conditions of WOVoletSmart
Name | Description
---- | -----------
false | Event when shutter is closed 
 changed | Event on any change of shutter state 
 true | Event when shutter is open 
 
##Actions of WOVoletSmart
Name | Description
---- | -----------
impulse up 200 | Open shutter for X ms 
 set 50 | Set shutter at position X in percent 
 up | Open the shutter 
 down | Close the shutter 
 impulse down 200 | Close shutter for X ms 
 set_state false | Update internal shutter state without starting real action. This is useful when having updating the shutter state from an external source. 
 toggle | Invert shutter state 
 up 5 | Open the shutter by X percent 
 calibrate | Start calibration on shutter. This opens fully the shutter and resets all internal position values. Use this if shutter sync is lost. 
 stop | Stop the shutter 
 down 5 | Close the shutter by X percent 
 set_state true | Update internal shutter state without starting real action. This is useful when having updating the shutter state from an external source. 
 
##More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/750-1504


#X10Output
Light dimmer using X10 and heyu.


Light with dimming control. Light intensity can be changed for this light.

##Parameters of X10Output
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
code | string | yes | House code of the X10 light device
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
log_history | bool | no | If enabled, write an entry in the history event log for this IO
io_type | string | yes | IO type, can be "input", "output", "inout"
name | string | yes | Name of Input/Output.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.

##Conditions of X10Output
Name | Description
---- | -----------
value | Event when light is at this value 
 changed | Event on any change of value 
 
##Actions of X10Output
Name | Description
---- | -----------
hold stop | Dynamically change light intensity when holding a switch (stop action) 
 hold press | Dynamically change light intensity when holding a switch (press action) 
 down 5 | Decrease intensity by X percent 
 impulse 500 200 500 200 | Do an impulse on light state with a pattern.<br>Ex: 500 200 500 200 means: TRUE for 500ms, FALSE for 200ms, TRUE for 500ms, FALSE for 200ms<br>Ex: 500 loop 200 300 means: TRUE for 500ms, then loop the next steps for infinite, FALSE for 200ms, TRUE for 300ms<br>Ex: 100 100 200 old means: blinks and then set to the old start state (before impulse starts) 
 set off 50 | Set light value without switching on. This will be the light intensity for the next ON 
 true | Switch the light on 
 set_state 50 | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 false | Switch the light off 
 toggle | Invert the light state 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 up 5 | Increase intensity by X percent 
 set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 impulse 200 | Do an impulse on light state. Set to true for X ms then reset to false 
 set 50 | Set light intensity and swith on if light is off 
 

#xPLInputAnalog
xPL analog sensor


An analog input can be used to read analog values to display them and use them in rules.

##Parameters of xPLInputAnalog
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
io_style | list | yes | GUI style display. This will control the icon displayed on the UI
unit | string | no | Unit which will be displayed on the UI as a suffix.
precision | int | no | Precision of the returned value. The value represents the number of decimal after the dot. The value is rounded like this : value = 19.275 => returned value 19.28 when preicision = 2, 19.3 when precision = 1, 19 when precision = 0
frequency | float | no | Sampling time in microsecond. The value is read at this frequency. If this value is not set, calaos tries to read the interval parameter
name | string | yes | Name of Input/Output.
io_type | string | yes | IO type, can be "input", "output", "inout"
sensor | string | yes | Sensor ID, as set in your xPL network
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
id | string | yes | Unique ID identifying the Input/Output in calaos-server
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
interval | float | no | Sampling time in seconds. The value is read at this frequency. If this value is not set, the default value is 15s
coeff_a | float | no | use in conjunction of coeff_b to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 1.0.
coeff_b | float | no | use in conjunction of coeff_a to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 0.0
source | string | yes | Source name, as set in your xPL network (Format VendorId-DeviceId.Instance)
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
offset | float | no | same as coeff_b, can be used alone. Default value is 0.0

##Conditions of xPLInputAnalog
Name | Description
---- | -----------
changed | Event on any change of value 
 value | Event on a specific value 
 

#xPLInputAnalog
xPL string sensor

##Parameters of xPLInputAnalog
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
source | string | yes | Source name, as set in your xPL network (Format VendorId-DeviceId.Instance)
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
sensor | string | yes | Sensor ID, as set in your xPL network
log_history | bool | no | If enabled, write an entry in the history event log for this IO
io_type | string | yes | IO type, can be "input", "output", "inout"
name | string | yes | Name of Input/Output.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.


#xPLInputSwitch
xPL input switch


Basic switch with press/release states.

##Parameters of xPLInputSwitch
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
source | string | yes | Source name, as set in your xPL network (Format VendorId-DeviceId.Instance)
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | A switch can't be visible. Always false.
sensor | string | yes | Sensor ID, as set in your xPL network
log_history | bool | no | If enabled, write an entry in the history event log for this IO
io_type | string | yes | IO type, can be "input", "output", "inout"
name | string | yes | Name of Input/Output.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.

##Conditions of xPLInputSwitch
Name | Description
---- | -----------
true | Event triggered when switch is pressed 
 changed | Event on any change of state 
 false | Event triggered when switch is released 
 

#xPLInputTemp
xPL temperature sensor


Temperature sensor input. Use for displaying temperature and to control heating devices with rules based on temperature value

##Parameters of xPLInputTemp
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
precision | int | no | Precision of the returned value. The value represents the number of decimal after the dot. The value is rounded like this : value = 19.275 => returned value 19.28 when preicision = 2, 19.3 when precision = 1, 19 when precision = 0
frequency | float | no | Sampling time in microsecond. The value is read at this frequency. If this value is not set, calaos tries to read the interval parameter
name | string | yes | Name of Input/Output.
io_type | string | yes | IO type, can be "input", "output", "inout"
sensor | string | yes | Sensor ID, as set in your xPL network
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
id | string | yes | Unique ID identifying the Input/Output in calaos-server
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
interval | float | no | Sampling time in seconds. The value is read at this frequency. If this value is not set, the default value is 15s
coeff_a | float | no | use in conjunction of coeff_b to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 1.0.
coeff_b | float | no | use in conjunction of coeff_a to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 0.0
source | string | yes | Source name, as set in your xPL network (Format VendorId-DeviceId.Instance)
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
offset | float | no | same as coeff_b, can be used alone. Default value is 0.0

##Conditions of xPLInputTemp
Name | Description
---- | -----------
changed | Event on any change of temperature value 
 value | Event on a temperature value in degree Celsius 
 

#xPLOutputAnalog
Analog output controlled by xPL Protocol


Analog output. Useful to control analog output devices connected to calaos.

##Parameters of xPLOutputAnalog
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
io_style | list | yes | GUI style display. This will control the icon displayed on the UI
unit | string | no | Unit which will be displayed on the UI as a suffix.
name | string | yes | Name of Input/Output.
io_type | string | yes | IO type, can be "input", "output", "inout"
source | string | yes | Source name, as set in your xPL network (Format VendorId-DeviceId.Instance)
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
id | string | yes | Unique ID identifying the Input/Output in calaos-server
actuator | string | yes | Actuator ID, as set in your xPL network
step | float | no | Set a step for increment/decrement value. Default is 1.0
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
coeff_a | float | no | use in conjunction of coeff_b to apply equation of the form `value_sent = coeff_a * raw_value + coeff_b`. Default value is 1.0.
coeff_b | float | no | use in conjunction of coeff_a to apply equation of the form `value_sent = coeff_a * raw_value + coeff_b`. Default value is 0.0

##Conditions of xPLOutputAnalog
Name | Description
---- | -----------
0 | Event on a specific number value 
 value | Event on a specific value 
 changed | Event on any change of value 
 
##Actions of xPLOutputAnalog
Name | Description
---- | -----------
dec 1 | Decrement value by value 
 inc 1 | Increment value by value 
 dec | Decrement value with configured step 
 0 | Set a specific number value 
 inc | Increment value with configured step 
 

#xPLOutputAnalog
Analog output controlled by xPL Protocol

##Parameters of xPLOutputAnalog
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
actuator | string | yes | Actuator ID, as set in your xPL network
source | string | yes | Source name, as set in your xPL network (Format VendorId-DeviceId.Instance)
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
log_history | bool | no | If enabled, write an entry in the history event log for this IO
io_type | string | yes | IO type, can be "input", "output", "inout"
name | string | yes | Name of Input/Output.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.


#xPLOutputSwitch
Light/relay controlled by xPL Protocol


Basic light. This light have only 2 states, ON or OFF. Can also be used to control simple relays output

##Parameters of xPLOutputSwitch
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
actuator | string | yes | Actuator ID, as set in your xPL network
source | string | yes | Source name, as set in your xPL network (Format VendorId-DeviceId.Instance)
io_style | list | yes | GUI style display. This will control the icon displayed on the UI
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
log_history | bool | no | If enabled, write an entry in the history event log for this IO
io_type | string | yes | IO type, can be "input", "output", "inout"
name | string | yes | Name of Input/Output.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.

##Conditions of xPLOutputSwitch
Name | Description
---- | -----------
false | Event when light is off 
 changed | Event on any change of value 
 true | Event when light is on 
 
##Actions of xPLOutputSwitch
Name | Description
---- | -----------
set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 false | Switch the light off 
 true | Switch the light on 
 toggle | Invert light state 
 impulse 500 200 500 200 | Do an impulse on light state with a pattern.<br>Ex: 500 200 500 200 means: TRUE for 500ms, FALSE for 200ms, TRUE for 500ms, FALSE for 200ms<br>Ex: 500 loop 200 300 means: TRUE for 500ms, then loop the next steps for infinite, FALSE for 200ms, TRUE for 300ms<br>Ex: 100 100 200 old means: blinks and then set to the old start state (before impulse starts) 
 impulse 200 | Do an impulse on light state. Set to true for X ms then reset to false 
 

#ZibaseAnalogIn
Zibase analog input. This object can read value from devices like Energy monitor sensors, Lux sensors, ...


An analog input can be used to read analog values to display them and use them in rules.

##Parameters of ZibaseAnalogIn
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
zibase_sensor | list | yes | Type of sensor
host | string | yes | Zibase IP address on the network
io_style | list | yes | GUI style display. This will control the icon displayed on the UI
unit | string | no | Unit which will be displayed on the UI as a suffix.
precision | int | no | Precision of the returned value. The value represents the number of decimal after the dot. The value is rounded like this : value = 19.275 => returned value 19.28 when preicision = 2, 19.3 when precision = 1, 19 when precision = 0
zibase_id | string | yes | Zibase device ID (ABC)
frequency | float | no | Sampling time in microsecond. The value is read at this frequency. If this value is not set, calaos tries to read the interval parameter
name | string | yes | Name of Input/Output.
port | int | no | Zibase ethernet port, default to 17100
io_type | string | yes | IO type, can be "input", "output", "inout"
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
id | string | yes | Unique ID identifying the Input/Output in calaos-server
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
interval | float | no | Sampling time in seconds. The value is read at this frequency. If this value is not set, the default value is 15s
coeff_a | float | no | use in conjunction of coeff_b to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 1.0.
coeff_b | float | no | use in conjunction of coeff_a to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 0.0
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
offset | float | no | same as coeff_b, can be used alone. Default value is 0.0

##Conditions of ZibaseAnalogIn
Name | Description
---- | -----------
changed | Event on any change of value 
 value | Event on a specific value 
 

#ZibaseDigitalIn
Zibase digital input. This object acts as a switch


Basic switch with press/release states.

##Parameters of ZibaseDigitalIn
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
zibase_sensor | list | yes | Type of sensor
name | string | yes | Name of Input/Output.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
visible | bool | no | A switch can't be visible. Always false.
io_type | string | yes | IO type, can be "input", "output", "inout"
port | int | no | Zibase ethernet port, default to 17100
id | string | yes | Unique ID identifying the Input/Output in calaos-server
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
zibase_id2 | string | yes | Second Zibase device ID (ABC)
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
zibase_id | string | yes | First Zibase device ID (ABC)
host | string | yes | Zibase IP address on the network

##Conditions of ZibaseDigitalIn
Name | Description
---- | -----------
true | Event triggered when switch is pressed 
 changed | Event on any change of state 
 false | Event triggered when switch is released 
 

#ZibaseDigitalOut
Zibase digital output. This object controls Zibase devices


Basic light. This light have only 2 states, ON or OFF. Can also be used to control simple relays output

##Parameters of ZibaseDigitalOut
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
nbburst | int | no | Number of burst to send to the device
protocol | list | yes | Protocol to use
zibase_id | string | yes | Zibase device ID (ABC)
name | string | yes | Name of Input/Output.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
io_type | string | yes | IO type, can be "input", "output", "inout"
port | int | no | Zibase ethernet port, default to 17100
id | string | yes | Unique ID identifying the Input/Output in calaos-server
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
io_style | list | yes | GUI style display. This will control the icon displayed on the UI
host | string | yes | Zibase IP address on the network

##Conditions of ZibaseDigitalOut
Name | Description
---- | -----------
false | Event when light is off 
 changed | Event on any change of value 
 true | Event when light is on 
 
##Actions of ZibaseDigitalOut
Name | Description
---- | -----------
set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 false | Switch the light off 
 true | Switch the light on 
 toggle | Invert light state 
 impulse 500 200 500 200 | Do an impulse on light state with a pattern.<br>Ex: 500 200 500 200 means: TRUE for 500ms, FALSE for 200ms, TRUE for 500ms, FALSE for 200ms<br>Ex: 500 loop 200 300 means: TRUE for 500ms, then loop the next steps for infinite, FALSE for 200ms, TRUE for 300ms<br>Ex: 100 100 200 old means: blinks and then set to the old start state (before impulse starts) 
 impulse 200 | Do an impulse on light state. Set to true for X ms then reset to false 
 

#ZibaseTemp
Zibase temperature sensor


Temperature sensor input. Use for displaying temperature and to control heating devices with rules based on temperature value

##Parameters of ZibaseTemp
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
zibase_sensor | list | yes | Type of sensor
host | string | yes | Zibase IP address on the network
precision | int | no | Precision of the returned value. The value represents the number of decimal after the dot. The value is rounded like this : value = 19.275 => returned value 19.28 when preicision = 2, 19.3 when precision = 1, 19 when precision = 0
zibase_id | string | yes | Zibase device ID (ABC)
frequency | float | no | Sampling time in microsecond. The value is read at this frequency. If this value is not set, calaos tries to read the interval parameter
name | string | yes | Name of Input/Output.
port | int | no | Zibase ethernet port, default to 17100
io_type | string | yes | IO type, can be "input", "output", "inout"
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
id | string | yes | Unique ID identifying the Input/Output in calaos-server
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
logged | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
interval | float | no | Sampling time in seconds. The value is read at this frequency. If this value is not set, the default value is 15s
coeff_a | float | no | use in conjunction of coeff_b to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 1.0.
coeff_b | float | no | use in conjunction of coeff_a to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 0.0
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
offset | float | no | same as coeff_b, can be used alone. Default value is 0.0

##Conditions of ZibaseTemp
Name | Description
---- | -----------
changed | Event on any change of temperature value 
 value | Event on a temperature value in degree Celsius 
 
