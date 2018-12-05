
#AVReceiver
AVReceiver object to control network amplifier

##Parameters of AVReceiver
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
model | string | yes | AVReceiver model. Supported: pioneer, denon, onkyo, marantz, yamaha
zone | int | no | Zone of the amplifier (if supported)
port | int | no | Port to use for connection
host | string | yes | IP address of the device
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

##Actions of AVReceiver
Name | Description
---- | -----------
custom XXXXXX | Send a custom command to receiver (if you know the protocol) 
 volume 50 | Set current volume 
 source X | Change current input source 
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
ptz | bool | no | Set to true if camera has PTZ support
model | string | yes | Camera model/chanel to use
log_history | bool | no | If enabled, write an entry in the history event log for this IO
zoom_step | string | no | 
name | string | yes | Name of Input/Output.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
pan_framesize | string | no | 
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"


#BlinkstickOutputLightRGB
RGB Light dimmer using a Blinkstick


RGB light. Choose a color to be set for this light.

##Parameters of BlinkstickOutputLightRGB
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
nb_leds | int | yes | Number of LEDs to control with the blinkstick
serial | string | yes | Blinkstick serial to control
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

##Conditions of BlinkstickOutputLightRGB
Name | Description
---- | -----------
value | Event when light is at this value 
 changed | Event on any change of value 
 
##Actions of BlinkstickOutputLightRGB
Name | Description
---- | -----------
down_blue 5 | Decrease intensity by X percent of blue channel 
 up_blue 5 | Increase intensity by X percent of blue channel 
 set_blue 50 | Set blue channel to X percent 
 set_green 50 | Set green channel to X percent 
 set #AA1294 | Set color. Color can be represented by using HTML notation: #AABBCC, rgb(50, 10, 30), hsl(11, 22, 33) 
 up_red 5 | Increase intensity by X percent of red channel 
 false | Switch the light off 
 up_green 5 | Increase intensity by X percent of green channel 
 true | Switch the light on 
 set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 down_red 5 | Decrease intensity by X percent of red channel 
 set_red 50 | Set red channel to X percent 
 toggle | Invert the light state (ON/OFF) 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 down_green 5 | Decrease intensity by X percent of green channel 
 set_state #AA1294 | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 
##More Infos
* OLA: http://www.blinkstick.com

#Gadspot - UNDOCUMENTED IO
SPANK SPANK SPANK : naughty programmer ! You did not add documentation for this IO, that's BAD :'(
Go document it in your code or you will burn in hell!



Gadspot IP Camera. Camera can be viewed directly inside calaos and used in rules.

##Parameters of Gadspot
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"


#GpioInputSwitch
Input switch with a GPIO


Basic switch with press/release states.

##Parameters of GpioInputSwitch
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
active_low | bool | no | Set this is your GPIO has an inverted level
gpio | int | yes | GPIO ID on your hardware
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | A switch can't be visible. Always false.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

##Conditions of GpioInputSwitch
Name | Description
---- | -----------
changed | Event on any change of state 
 true | Event triggered when switch is pressed 
 false | Event triggered when switch is released 
 

#GpioInputSwitchLongPress
Input switch with a GPIO


Long press switch. This switch supports single press and long press. User has 500ms to perform the long press.

##Parameters of GpioInputSwitchLongPress
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
active_low | bool | no | Set this is your GPIO has an inverted level
gpio | int | yes | GPIO ID on your hardware
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | A switch can't be visible. Always false.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

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
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | A switch can't be visible. Always false.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

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
active_low_up | bool | no | Set this is your GPIO has an inverted level
gpio_down | int | yes | GPIO ID for closing on your hardware
gpio_up | int | yes | GPIO ID for opening on your hardware
active_low_down | bool | no | Set this is your GPIO has an inverted level
stop_both | bool | no | If in impulse mode, some shutters needs to activate both up dans down relays when stopping the shutter
impulse_time | int | no | Impulse time for shutter that needs impulse instead of holding up/down relays. If set to 0 impulse shutter is disabled. Time is in ms. Default to 0
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
time | int | yes | Time in sec for shutter to open or close
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

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
 down | Close the shutter 
 stop | Stop the shutter 
 set_state false | Update internal shutter state without starting real action. This is useful when having updating the shutter state from an external source. 
 toggle | Invert shutter state 
 impulse up 200 | Open shutter for X ms 
 up | Open the shutter 
 impulse down 200 | Close shutter for X ms 
 

#GpioOutputShutterSmart
Shutter with 2 GPIOs


Smart shutter. This shutter calculates the position of the shutter based on the time it takes to open and close. It then allows to set directly the shutter at a specified position.

##Parameters of GpioOutputShutterSmart
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
active_low_up | bool | no | Set this is your GPIO has an inverted level
gpio_down | int | yes | GPIO ID for closing on your hardware
gpio_up | int | yes | GPIO ID for opening on your hardware
time_down | int | yes | Time in sec for shutter to fully closed. The more accurate, the better it will work
active_low_down | bool | no | Set this is your GPIO has an inverted level
stop_both | bool | no | If in impulse mode, some shutters needs to activate both up dans down relays when stopping the shutter
time_up | int | yes | Time in sec for shutter to be fully open. The more accurate, the better it will work
impulse_time | int | no | Impulse time for shutter that needs impulse instead of holding up/down relays. If set to 0 impulse shutter is disabled. Time is in ms. Default to 0
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

##Conditions of GpioOutputShutterSmart
Name | Description
---- | -----------
false | Event when shutter is closed 
 changed | Event on any change of shutter state 
 true | Event when shutter is open 
 
##Actions of GpioOutputShutterSmart
Name | Description
---- | -----------
set_state true | Update internal shutter state without starting real action. This is useful when having updating the shutter state from an external source. 
 calibrate | Start calibration on shutter. This opens fully the shutter and resets all internal position values. Use this if shutter sync is lost. 
 set 50 | Set shutter at position X in percent 
 down | Close the shutter 
 stop | Stop the shutter 
 set_state false | Update internal shutter state without starting real action. This is useful when having updating the shutter state from an external source. 
 up 5 | Open the shutter by X percent 
 toggle | Invert shutter state 
 down 5 | Close the shutter by X percent 
 impulse up 200 | Open shutter for X ms 
 up | Open the shutter 
 impulse down 200 | Close shutter for X ms 
 

#GpioOutputSwitch
Light with a GPIO


Basic light. This light have only 2 states, ON or OFF. Can also be used to control simple relays output

##Parameters of GpioOutputSwitch
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
active_low | bool | no | Set this is your GPIO has an inverted level
gpio | int | yes | GPIO ID on your hardware
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

##Conditions of GpioOutputSwitch
Name | Description
---- | -----------
false | Event when light is off 
 changed | Event on any change of value 
 true | Event when light is on 
 
##Actions of GpioOutputSwitch
Name | Description
---- | -----------
false | Switch the light off 
 true | Switch the light on 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 toggle | Invert light state 
 impulse 200 | Do an impulse on light state. Set to true for X ms then reset to false 
 impulse 500 200 500 200 | Do an impulse on light state with a pattern.<br>Ex: 500 200 500 200 means: TRUE for 500ms, FALSE for 200ms, TRUE for 500ms, FALSE for 200ms<br>Ex: 500 loop 200 300 means: TRUE for 500ms, then loop the next steps for infinite, FALSE for 200ms, TRUE for 300ms<br>Ex: 100 100 200 old means: blinks and then set to the old start state (before impulse starts) 
 set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 

#HueOutputLightRGB
RGB Light dimmer using a Philips Hue


RGB light. Choose a color to be set for this light.

##Parameters of HueOutputLightRGB
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
id_hue | string | yes | Unique ID describing the Hue Light. This value is returned by the Hue Wizard.
host | string | yes | Hue bridge IP address
log_history | bool | no | If enabled, write an entry in the history event log for this IO
api | string | yes | API key return by Hue bridge when assciation has been made. Use Hue Wizard in calaos_installer to get this value automatically.
name | string | yes | Name of Input/Output.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

##Conditions of HueOutputLightRGB
Name | Description
---- | -----------
value | Event when light is at this value 
 changed | Event on any change of value 
 
##Actions of HueOutputLightRGB
Name | Description
---- | -----------
down_blue 5 | Decrease intensity by X percent of blue channel 
 up_blue 5 | Increase intensity by X percent of blue channel 
 set_blue 50 | Set blue channel to X percent 
 set_green 50 | Set green channel to X percent 
 set #AA1294 | Set color. Color can be represented by using HTML notation: #AABBCC, rgb(50, 10, 30), hsl(11, 22, 33) 
 up_red 5 | Increase intensity by X percent of red channel 
 false | Switch the light off 
 up_green 5 | Increase intensity by X percent of green channel 
 true | Switch the light on 
 set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 down_red 5 | Decrease intensity by X percent of red channel 
 set_red 50 | Set red channel to X percent 
 toggle | Invert the light state (ON/OFF) 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 down_green 5 | Decrease intensity by X percent of green channel 
 set_state #AA1294 | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 
##More Infos
* Meet Hue: http://www.meethue.com


#InputTime
Basic time input. An event is triggered when the current time equals the configured time. A specific date can also be set.

##Parameters of InputTime
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
day | int | no | Day for this time input
month | int | no | Month for this time input
year | int | no | Year for this time input
min | int | yes | Minutes for this time input
hour | int | yes | Hour for this time input
sec | int | yes | Seconds for this time input
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | A time object can't be visible. Always false.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

##Conditions of InputTime
Name | Description
---- | -----------
changed | Event on any change of time 
 true | Event triggered when current time equals 
 false | Event triggered when current time is not equal 
 

#InputTimer
Timer object. trigger an event after the configured time has expired.

##Parameters of InputTimer
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
autorestart | bool | yes | Auto restart the timer when time expires
min | int | yes | Minutes for the timer interval
hour | int | yes | Hour for the timer interval
autostart | bool | yes | Auto start the timer when calaos starts
sec | int | yes | Seconds for the timer interval
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
msec | int | yes | Miliseconds for the timer interval
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | A timer object can't be visible. Always false.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

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
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
rw | bool | no | Enable edit mode for this object. It allows user to modify the value on interfaces. Default to false
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

##Conditions of InternalBool
Name | Description
---- | -----------
changed | Event on any change of value 
 true | Event when value is true 
 false | Event when value is false 
 
##Actions of InternalBool
Name | Description
---- | -----------
impulse 500 200 500 200 | Do an impulse on boolean value with a pattern.<br>Ex: 500 200 500 200 means: TRUE for 500ms, FALSE for 200ms, TRUE for 500ms, FALSE for 200ms<br>Ex: 500 loop 200 300 means: TRUE for 500ms, then loop the next steps for infinite, FALSE for 200ms, TRUE for 300ms<br>Ex: 100 100 200 old means: blinks and then set to the old start state (before impulse starts) 
 impulse 200 | Do an impulse on boolean value. Set to true for X ms then reset to false 
 toggle | Invert boolean value 
 true | Set a value to true 
 false | Set a value to false 
 

#InternalInt
Internal number object. This object is useful for doing internal programing in rules, like counters, of displaying values.

##Parameters of InternalInt
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
save | bool | no | Automatically save the value in cache. The value will be reloaded when restarting calaos is true. Default to false
step | float | no | Set a step for increment/decrement value. Default is 1.0
log_history | bool | no | If enabled, write an entry in the history event log for this IO
unit | string | no | Unit which will be displayed on the UI as a suffix.
name | string | yes | Name of Input/Output.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
rw | bool | no | Enable edit mode for this object. It allows user to modify the value on interfaces. Default to false
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

##Conditions of InternalInt
Name | Description
---- | -----------
changed | Event on any change of value 
 0 | Event on a specific number value 
 
##Actions of InternalInt
Name | Description
---- | -----------
dec 1 | Decrement value by value 
 dec | Decrement value with configured step 
 inc 1 | Increment value by value 
 0 | Set a specific number value 
 inc | Increment value with configured step 
 

#InternalString
Internal string object. This object is useful for doing internal programing in rules or displaying text values on user interfaces.

##Parameters of InternalString
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
save | bool | no | Automatically save the value in cache. The value will be reloaded when restarting calaos is true. Default to false
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
rw | bool | no | Enable edit mode for this object. It allows user to modify the value on interfaces. Default to false
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

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
listen_knx_group | string | no | KNX Group address for listening status, Ex: x/y/z
read_at_start | bool | yes | Send a read request at start to get the current value. Default is false
io_type | string | yes | IO type, can be "input", "output", "inout"
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
coeff_b | float | no | use in conjunction of coeff_a to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 0.0
name | string | yes | Name of Input/Output.
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
frequency | float | no | Sampling time in microsecond. The value is read at this frequency. If this value is not set, calaos tries to read the interval parameter
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
unit | string | no | Unit which will be displayed on the UI as a suffix.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
coeff_a | float | no | use in conjunction of coeff_b to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 1.0.
offset | float | no | same as coeff_b, can be used alone. Default value is 0.0
precision | int | no | Precision of the returned value. The value represents the number of decimal after the dot. The value is rounded like this : value = 19.275 => returned value 19.28 when preicision = 2, 19.3 when precision = 1, 19 when precision = 0
eis | int | no | KNX EIS (Data type)
interval | float | no | Sampling time in seconds. The value is read at this frequency. If this value is not set, the default value is 15s
gui_style | list | yes | GUI style display. This will control the icon displayed on the UI
knx_group | string | yes | KNX Group address, Ex: x/y/z

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
read_at_start | bool | yes | Send a read request at start to get the current value. Default is false
listen_knx_group | string | no | KNX Group address for listening status, Ex: x/y/z
eis | int | no | KNX EIS (Data type)
knx_group | string | yes | KNX Group address, Ex: x/y/z
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | A switch can't be visible. Always false.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

##Conditions of KNXInputSwitch
Name | Description
---- | -----------
changed | Event on any change of state 
 true | Event triggered when switch is pressed 
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
read_at_start | bool | yes | Send a read request at start to get the current value. Default is false
listen_knx_group | string | no | KNX Group address for listening status, Ex: x/y/z
eis | int | no | KNX EIS (Data type)
knx_group | string | yes | KNX Group address, Ex: x/y/z
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | A switch can't be visible. Always false.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

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
read_at_start | bool | yes | Send a read request at start to get the current value. Default is false
listen_knx_group | string | no | KNX Group address for listening status, Ex: x/y/z
eis | int | no | KNX EIS (Data type)
knx_group | string | yes | KNX Group address, Ex: x/y/z
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | A switch can't be visible. Always false.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

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
read_at_start | bool | yes | Send a read request at start to get the current value. Default is false
io_type | string | yes | IO type, can be "input", "output", "inout"
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
coeff_b | float | no | use in conjunction of coeff_a to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 0.0
name | string | yes | Name of Input/Output.
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
frequency | float | no | Sampling time in microsecond. The value is read at this frequency. If this value is not set, calaos tries to read the interval parameter
log_history | bool | no | If enabled, write an entry in the history event log for this IO
coeff_a | float | no | use in conjunction of coeff_b to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 1.0.
offset | float | no | same as coeff_b, can be used alone. Default value is 0.0
precision | int | no | Precision of the returned value. The value represents the number of decimal after the dot. The value is rounded like this : value = 19.275 => returned value 19.28 when preicision = 2, 19.3 when precision = 1, 19 when precision = 0
knx_group | string | yes | KNX Group address, Ex: x/y/z
interval | float | no | Sampling time in seconds. The value is read at this frequency. If this value is not set, the default value is 15s
eis | int | no | KNX EIS (Data type)
listen_knx_group | string | no | KNX Group address for listening status, Ex: x/y/z

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
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
id | string | yes | Unique ID identifying the Input/Output in calaos-server
coeff_b | float | no | use in conjunction of coeff_a to apply equation of the form `value_sent = coeff_a * raw_value + coeff_b`. Default value is 0.0
name | string | yes | Name of Input/Output.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
unit | string | no | Unit which will be displayed on the UI as a suffix.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
coeff_a | float | no | use in conjunction of coeff_b to apply equation of the form `value_sent = coeff_a * raw_value + coeff_b`. Default value is 1.0.
step | float | no | Set a step for increment/decrement value. Default is 1.0
gui_style | list | yes | GUI style display. This will control the icon displayed on the UI
knx_group | string | yes | KNX Group address, Ex: x/y/z
eis | int | no | KNX EIS (Data type)
listen_knx_group | string | no | KNX Group address for listening status, Ex: x/y/z
io_type | string | yes | IO type, can be "input", "output", "inout"
read_at_start | bool | yes | Send a read request at start to get the current value. Default is false

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
 dec | Decrement value with configured step 
 inc 1 | Increment value by value 
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
read_at_start | bool | yes | Send a read request at start to get the current value. Default is false
listen_knx_group | string | no | KNX Group address for listening status, Ex: x/y/z
eis | int | no | KNX EIS (Data type)
knx_group | string | yes | KNX Group address, Ex: x/y/z
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

##Conditions of KNXOutputLight
Name | Description
---- | -----------
false | Event when light is off 
 changed | Event on any change of value 
 true | Event when light is on 
 
##Actions of KNXOutputLight
Name | Description
---- | -----------
false | Switch the light off 
 true | Switch the light on 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 toggle | Invert light state 
 impulse 200 | Do an impulse on light state. Set to true for X ms then reset to false 
 impulse 500 200 500 200 | Do an impulse on light state with a pattern.<br>Ex: 500 200 500 200 means: TRUE for 500ms, FALSE for 200ms, TRUE for 500ms, FALSE for 200ms<br>Ex: 500 loop 200 300 means: TRUE for 500ms, then loop the next steps for infinite, FALSE for 200ms, TRUE for 300ms<br>Ex: 100 100 200 old means: blinks and then set to the old start state (before impulse starts) 
 set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 
##More Infos
* knxd: https://github.com/knxd/knxd/g


#KNXOutputLightDimmer
Light dimmer with KNX and eibnetmux


Light with dimming control. Light intensity can be changed for this light.

##Parameters of KNXOutputLightDimmer
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
host | string | yes | Hostname of knxd, default to localhost
read_at_start | bool | yes | Send a read request at start to get the current value. Default is false
listen_knx_group | string | no | KNX Group address for listening status, Ex: x/y/z
eis | int | no | KNX EIS (Data type)
knx_group | string | yes | KNX Group address, Ex: x/y/z
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

##Conditions of KNXOutputLightDimmer
Name | Description
---- | -----------
value | Event when light is at this value 
 changed | Event on any change of value 
 
##Actions of KNXOutputLightDimmer
Name | Description
---- | -----------
hold stop | Dynamically change light intensity when holding a switch (stop action) 
 down 5 | Decrease intensity by X percent 
 set 50 | Set light intensity and swith on if light is off 
 set off 50 | Set light value without switching on. This will be the light intensity for the next ON 
 set_state 50 | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 false | Switch the light off 
 hold press | Dynamically change light intensity when holding a switch (press action) 
 true | Switch the light on 
 up 5 | Increase intensity by X percent 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 toggle | Invert the light state 
 impulse 200 | Do an impulse on light state. Set to true for X ms then reset to false 
 impulse 500 200 500 200 | Do an impulse on light state with a pattern.<br>Ex: 500 200 500 200 means: TRUE for 500ms, FALSE for 200ms, TRUE for 500ms, FALSE for 200ms<br>Ex: 500 loop 200 300 means: TRUE for 500ms, then loop the next steps for infinite, FALSE for 200ms, TRUE for 300ms<br>Ex: 100 100 200 old means: blinks and then set to the old start state (before impulse starts) 
 set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 
##More Infos
* knxd: https://github.com/knxd/knxd/g


#KNXOutputLightRGB
Light RGB with KNX and eibnetmux


RGB light. Choose a color to be set for this light.

##Parameters of KNXOutputLightRGB
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
eis | int | no | KNX EIS (Data type)
host | string | yes | Hostname of knxd, default to localhost
read_at_start | bool | yes | Send a read request at start to get the current value. Default is false
listen_knx_group_red | string | no | Red Group address for listening status, Ex: x/y/z
knx_group_green | string | yes | Green channel KNX Group address, Ex: x/y/z
knx_group_red | string | yes | Red channel KNX Group address, Ex: x/y/z
listen_knx_group_blue | string | no | Blue Group address for listening status, Ex: x/y/z
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
knx_group_blue | string | yes | Blue channel KNX Group address, Ex: x/y/z
id | string | yes | Unique ID identifying the Input/Output in calaos-server
listen_knx_group_green | string | no | Green Group address for listening status, Ex: x/y/z
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

##Conditions of KNXOutputLightRGB
Name | Description
---- | -----------
value | Event when light is at this value 
 changed | Event on any change of value 
 
##Actions of KNXOutputLightRGB
Name | Description
---- | -----------
down_blue 5 | Decrease intensity by X percent of blue channel 
 up_blue 5 | Increase intensity by X percent of blue channel 
 set_blue 50 | Set blue channel to X percent 
 set_green 50 | Set green channel to X percent 
 set #AA1294 | Set color. Color can be represented by using HTML notation: #AABBCC, rgb(50, 10, 30), hsl(11, 22, 33) 
 up_red 5 | Increase intensity by X percent of red channel 
 false | Switch the light off 
 up_green 5 | Increase intensity by X percent of green channel 
 true | Switch the light on 
 set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 down_red 5 | Decrease intensity by X percent of red channel 
 set_red 50 | Set red channel to X percent 
 toggle | Invert the light state (ON/OFF) 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 down_green 5 | Decrease intensity by X percent of green channel 
 set_state #AA1294 | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 
##More Infos
* knxd: https://github.com/knxd/knxd/g


#KNXOutputShutter
Shutter with with KNX and eibnetmux


Simple shutter. This shutter supports open/close states, as well as impulse shutters.

##Parameters of KNXOutputShutter
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
host | string | yes | Hostname of knxd, default to localhost
read_at_start | bool | yes | Send a read request at start to get the current value. Default is false
eis | int | no | KNX EIS (Data type)
knx_group_down | string | yes | Down KNX Group address, Ex: x/y/z
knx_group_up | string | yes | Up KNX Group address, Ex: x/y/z
stop_both | bool | no | If in impulse mode, some shutters needs to activate both up dans down relays when stopping the shutter
impulse_time | int | no | Impulse time for shutter that needs impulse instead of holding up/down relays. If set to 0 impulse shutter is disabled. Time is in ms. Default to 0
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
time | int | yes | Time in sec for shutter to open or close
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

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
 down | Close the shutter 
 stop | Stop the shutter 
 set_state false | Update internal shutter state without starting real action. This is useful when having updating the shutter state from an external source. 
 toggle | Invert shutter state 
 impulse up 200 | Open shutter for X ms 
 up | Open the shutter 
 impulse down 200 | Close shutter for X ms 
 
##More Infos
* knxd: https://github.com/knxd/knxd/g


#KNXOutputShutterSmart
Shutter with with KNX and eibnetmux


Smart shutter. This shutter calculates the position of the shutter based on the time it takes to open and close. It then allows to set directly the shutter at a specified position.

##Parameters of KNXOutputShutterSmart
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
read_at_start | bool | yes | Send a read request at start to get the current value. Default is false
io_type | string | yes | IO type, can be "input", "output", "inout"
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
id | string | yes | Unique ID identifying the Input/Output in calaos-server
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
name | string | yes | Name of Input/Output.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
time_up | int | yes | Time in sec for shutter to be fully open. The more accurate, the better it will work
stop_both | bool | no | If in impulse mode, some shutters needs to activate both up dans down relays when stopping the shutter
time_down | int | yes | Time in sec for shutter to fully closed. The more accurate, the better it will work
host | string | yes | Hostname of knxd, default to localhost
knx_group_up | string | yes | Up KNX Group address, Ex: x/y/z
knx_group_down | string | yes | Down KNX Group address, Ex: x/y/z
knx_group | string | yes | KNX Group address, Ex: x/y/z
eis | int | no | KNX EIS (Data type)
impulse_time | int | no | Impulse time for shutter that needs impulse instead of holding up/down relays. If set to 0 impulse shutter is disabled. Time is in ms. Default to 0
listen_knx_group | string | no | KNX Group address for listening status, Ex: x/y/z

##Conditions of KNXOutputShutterSmart
Name | Description
---- | -----------
false | Event when shutter is closed 
 changed | Event on any change of shutter state 
 true | Event when shutter is open 
 
##Actions of KNXOutputShutterSmart
Name | Description
---- | -----------
set_state true | Update internal shutter state without starting real action. This is useful when having updating the shutter state from an external source. 
 calibrate | Start calibration on shutter. This opens fully the shutter and resets all internal position values. Use this if shutter sync is lost. 
 set 50 | Set shutter at position X in percent 
 down | Close the shutter 
 stop | Stop the shutter 
 set_state false | Update internal shutter state without starting real action. This is useful when having updating the shutter state from an external source. 
 up 5 | Open the shutter by X percent 
 toggle | Invert shutter state 
 down 5 | Close the shutter by X percent 
 impulse up 200 | Open shutter for X ms 
 up | Open the shutter 
 impulse down 200 | Close shutter for X ms 
 
##More Infos
* knxd: https://github.com/knxd/knxd/g


#MilightOutputLightRGB
RGB light support for Limitless/Milight RGB bulbs.


RGB light. Choose a color to be set for this light.

##Parameters of MilightOutputLightRGB
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
zone | int | yes | Zone to control. Each gateway supports 4 zones.
port | int | no | Gateway port, default to 8899
host | string | yes | Milight wifi gateway IP address
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

##Conditions of MilightOutputLightRGB
Name | Description
---- | -----------
value | Event when light is at this value 
 changed | Event on any change of value 
 
##Actions of MilightOutputLightRGB
Name | Description
---- | -----------
down_blue 5 | Decrease intensity by X percent of blue channel 
 up_blue 5 | Increase intensity by X percent of blue channel 
 set_blue 50 | Set blue channel to X percent 
 set_green 50 | Set green channel to X percent 
 set #AA1294 | Set color. Color can be represented by using HTML notation: #AABBCC, rgb(50, 10, 30), hsl(11, 22, 33) 
 up_red 5 | Increase intensity by X percent of red channel 
 false | Switch the light off 
 up_green 5 | Increase intensity by X percent of green channel 
 true | Switch the light on 
 set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 down_red 5 | Decrease intensity by X percent of red channel 
 set_red 50 | Set red channel to X percent 
 toggle | Invert the light state (ON/OFF) 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 down_green 5 | Decrease intensity by X percent of green channel 
 set_state #AA1294 | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 
##More Infos
* LimitlessLED: http://www.limitlessled.com


#MySensorsInputAnalog
Analog measurement with MySensors node


An analog input can be used to read analog values to display them and use them in rules.

##Parameters of MySensorsInputAnalog
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
node_id | string | yes | Node ID as set in your network
host | string | yes | IP address of the tcp gateway if relevant
port | string | yes | If using serial gateway, port is the serial port (/dev/ttyUSB0 for ex.). If using tcp gateway port is TCP port of the gateway.
sensor_id | string | yes | Sensor ID, as set in your node
io_type | string | yes | IO type, can be "input", "output", "inout"
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
coeff_b | float | no | use in conjunction of coeff_a to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 0.0
name | string | yes | Name of Input/Output.
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
frequency | float | no | Sampling time in microsecond. The value is read at this frequency. If this value is not set, calaos tries to read the interval parameter
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
unit | string | no | Unit which will be displayed on the UI as a suffix.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
coeff_a | float | no | use in conjunction of coeff_b to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 1.0.
offset | float | no | same as coeff_b, can be used alone. Default value is 0.0
precision | int | no | Precision of the returned value. The value represents the number of decimal after the dot. The value is rounded like this : value = 19.275 => returned value 19.28 when preicision = 2, 19.3 when precision = 1, 19 when precision = 0
interval | float | no | Sampling time in seconds. The value is read at this frequency. If this value is not set, the default value is 15s
gui_style | list | yes | GUI style display. This will control the icon displayed on the UI
gateway | list | yes | Gateway type used, tcp or serial are supported

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
node_id | string | yes | Node ID as set in your network
host | string | yes | IP address of the tcp gateway if relevant
sensor_id | string | yes | Sensor ID, as set in your node
port | string | yes | If using serial gateway, port is the serial port (/dev/ttyUSB0 for ex.). If using tcp gateway port is TCP port of the gateway.
gateway | list | yes | Gateway type used, tcp or serial are supported
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

##More Infos
* MySensors: http://mysensors.org


#MySensorsInputSwitch
Input switch with MySensors node


Basic switch with press/release states.

##Parameters of MySensorsInputSwitch
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
node_id | string | yes | Node ID as set in your network
host | string | yes | IP address of the tcp gateway if relevant
sensor_id | string | yes | Sensor ID, as set in your node
port | string | yes | If using serial gateway, port is the serial port (/dev/ttyUSB0 for ex.). If using tcp gateway port is TCP port of the gateway.
gateway | list | yes | Gateway type used, tcp or serial are supported
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | A switch can't be visible. Always false.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

##Conditions of MySensorsInputSwitch
Name | Description
---- | -----------
changed | Event on any change of state 
 true | Event triggered when switch is pressed 
 false | Event triggered when switch is released 
 
##More Infos
* MySensors: http://mysensors.org


#MySensorsInputSwitchLongPress
Input long press switch with MySensors node


Long press switch. This switch supports single press and long press. User has 500ms to perform the long press.

##Parameters of MySensorsInputSwitchLongPress
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
node_id | string | yes | Node ID as set in your network
host | string | yes | IP address of the tcp gateway if relevant
sensor_id | string | yes | Sensor ID, as set in your node
port | string | yes | If using serial gateway, port is the serial port (/dev/ttyUSB0 for ex.). If using tcp gateway port is TCP port of the gateway.
gateway | list | yes | Gateway type used, tcp or serial are supported
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | A switch can't be visible. Always false.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

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
node_id | string | yes | Node ID as set in your network
host | string | yes | IP address of the tcp gateway if relevant
sensor_id | string | yes | Sensor ID, as set in your node
port | string | yes | If using serial gateway, port is the serial port (/dev/ttyUSB0 for ex.). If using tcp gateway port is TCP port of the gateway.
gateway | list | yes | Gateway type used, tcp or serial are supported
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | A switch can't be visible. Always false.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

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
node_id | string | yes | Node ID as set in your network
sensor_id | string | yes | Sensor ID, as set in your node
io_type | string | yes | IO type, can be "input", "output", "inout"
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
coeff_b | float | no | use in conjunction of coeff_a to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 0.0
name | string | yes | Name of Input/Output.
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
frequency | float | no | Sampling time in microsecond. The value is read at this frequency. If this value is not set, calaos tries to read the interval parameter
log_history | bool | no | If enabled, write an entry in the history event log for this IO
coeff_a | float | no | use in conjunction of coeff_b to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 1.0.
offset | float | no | same as coeff_b, can be used alone. Default value is 0.0
precision | int | no | Precision of the returned value. The value represents the number of decimal after the dot. The value is rounded like this : value = 19.275 => returned value 19.28 when preicision = 2, 19.3 when precision = 1, 19 when precision = 0
interval | float | no | Sampling time in seconds. The value is read at this frequency. If this value is not set, the default value is 15s
gateway | list | yes | Gateway type used, tcp or serial are supported
port | string | yes | If using serial gateway, port is the serial port (/dev/ttyUSB0 for ex.). If using tcp gateway port is TCP port of the gateway.
host | string | yes | IP address of the tcp gateway if relevant

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
io_type | string | yes | IO type, can be "input", "output", "inout"
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
id | string | yes | Unique ID identifying the Input/Output in calaos-server
coeff_b | float | no | use in conjunction of coeff_a to apply equation of the form `value_sent = coeff_a * raw_value + coeff_b`. Default value is 0.0
name | string | yes | Name of Input/Output.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
unit | string | no | Unit which will be displayed on the UI as a suffix.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
coeff_a | float | no | use in conjunction of coeff_b to apply equation of the form `value_sent = coeff_a * raw_value + coeff_b`. Default value is 1.0.
step | float | no | Set a step for increment/decrement value. Default is 1.0
gui_style | list | yes | GUI style display. This will control the icon displayed on the UI
gateway | list | yes | Gateway type used, tcp or serial are supported
port | string | yes | If using serial gateway, port is the serial port (/dev/ttyUSB0 for ex.). If using tcp gateway port is TCP port of the gateway.
host | string | yes | IP address of the tcp gateway if relevant
node_id | string | yes | Node ID as set in your network

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
 dec | Decrement value with configured step 
 inc 1 | Increment value by value 
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
node_id | string | yes | Node ID as set in your network
host | string | yes | IP address of the tcp gateway if relevant
sensor_id | string | yes | Sensor ID, as set in your node
port | string | yes | If using serial gateway, port is the serial port (/dev/ttyUSB0 for ex.). If using tcp gateway port is TCP port of the gateway.
gateway | list | yes | Gateway type used, tcp or serial are supported
data_type | string | no | Data type sent to the node. Default: V_DIMMER, see MySensors.cpp for more values.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

##Conditions of MySensorsOutputDimmer
Name | Description
---- | -----------
value | Event when light is at this value 
 changed | Event on any change of value 
 
##Actions of MySensorsOutputDimmer
Name | Description
---- | -----------
hold stop | Dynamically change light intensity when holding a switch (stop action) 
 down 5 | Decrease intensity by X percent 
 set 50 | Set light intensity and swith on if light is off 
 set off 50 | Set light value without switching on. This will be the light intensity for the next ON 
 set_state 50 | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 false | Switch the light off 
 hold press | Dynamically change light intensity when holding a switch (press action) 
 true | Switch the light on 
 up 5 | Increase intensity by X percent 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 toggle | Invert the light state 
 impulse 200 | Do an impulse on light state. Set to true for X ms then reset to false 
 impulse 500 200 500 200 | Do an impulse on light state with a pattern.<br>Ex: 500 200 500 200 means: TRUE for 500ms, FALSE for 200ms, TRUE for 500ms, FALSE for 200ms<br>Ex: 500 loop 200 300 means: TRUE for 500ms, then loop the next steps for infinite, FALSE for 200ms, TRUE for 300ms<br>Ex: 100 100 200 old means: blinks and then set to the old start state (before impulse starts) 
 set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 
##More Infos
* MySensors: http://mysensors.org


#MySensorsOutputLight
Light/relay with MySensors node


Basic light. This light have only 2 states, ON or OFF. Can also be used to control simple relays output

##Parameters of MySensorsOutputLight
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
node_id | string | yes | Node ID as set in your network
host | string | yes | IP address of the tcp gateway if relevant
sensor_id | string | yes | Sensor ID, as set in your node
port | string | yes | If using serial gateway, port is the serial port (/dev/ttyUSB0 for ex.). If using tcp gateway port is TCP port of the gateway.
gateway | list | yes | Gateway type used, tcp or serial are supported
data_type | string | no | Data type sent to the node. Default: V_LIGHT, see MySensors.cpp for more values.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

##Conditions of MySensorsOutputLight
Name | Description
---- | -----------
false | Event when light is off 
 changed | Event on any change of value 
 true | Event when light is on 
 
##Actions of MySensorsOutputLight
Name | Description
---- | -----------
false | Switch the light off 
 true | Switch the light on 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 toggle | Invert light state 
 impulse 200 | Do an impulse on light state. Set to true for X ms then reset to false 
 impulse 500 200 500 200 | Do an impulse on light state with a pattern.<br>Ex: 500 200 500 200 means: TRUE for 500ms, FALSE for 200ms, TRUE for 500ms, FALSE for 200ms<br>Ex: 500 loop 200 300 means: TRUE for 500ms, then loop the next steps for infinite, FALSE for 200ms, TRUE for 300ms<br>Ex: 100 100 200 old means: blinks and then set to the old start state (before impulse starts) 
 set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 
##More Infos
* MySensors: http://mysensors.org


#MySensorsOutputLightRGB
RGB Light dimmer with MySensors node


RGB light. Choose a color to be set for this light.

##Parameters of MySensorsOutputLightRGB
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
data_type | string | no | Data type sent to the node. Default: V_DIMMER, see MySensors.cpp for more values.
io_type | string | yes | IO type, can be "input", "output", "inout"
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
id | string | yes | Unique ID identifying the Input/Output in calaos-server
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
node_id_blue | string | yes | Node ID for blue channel, as set in your network
name | string | yes | Name of Input/Output.
sensor_id_red | string | yes | Sensor ID for red channel, as set in your node
log_history | bool | no | If enabled, write an entry in the history event log for this IO
sensor_id_green | string | yes | Sensor ID green red channel, as set in your node
gateway | list | yes | Gateway type used, tcp or serial are supported
port | string | yes | If using serial gateway, port is the serial port (/dev/ttyUSB0 for ex.). If using tcp gateway port is TCP port of the gateway.
host | string | yes | IP address of the tcp gateway if relevant
node_id_red | string | yes | Node ID for red channel, as set in your network
node_id_green | string | yes | Node ID for green channel, as set in your network
sensor_id_blue | string | yes | Sensor ID blue red channel, as set in your node

##Conditions of MySensorsOutputLightRGB
Name | Description
---- | -----------
value | Event when light is at this value 
 changed | Event on any change of value 
 
##Actions of MySensorsOutputLightRGB
Name | Description
---- | -----------
down_blue 5 | Decrease intensity by X percent of blue channel 
 up_blue 5 | Increase intensity by X percent of blue channel 
 set_blue 50 | Set blue channel to X percent 
 set_green 50 | Set green channel to X percent 
 set #AA1294 | Set color. Color can be represented by using HTML notation: #AABBCC, rgb(50, 10, 30), hsl(11, 22, 33) 
 up_red 5 | Increase intensity by X percent of red channel 
 false | Switch the light off 
 up_green 5 | Increase intensity by X percent of green channel 
 true | Switch the light on 
 set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 down_red 5 | Decrease intensity by X percent of red channel 
 set_red 50 | Set red channel to X percent 
 toggle | Invert the light state (ON/OFF) 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 down_green 5 | Decrease intensity by X percent of green channel 
 set_state #AA1294 | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 
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
io_type | string | yes | IO type, can be "input", "output", "inout"
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
id | string | yes | Unique ID identifying the Input/Output in calaos-server
time | int | yes | Time in sec for shutter to open or close
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
name | string | yes | Name of Input/Output.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
impulse_time | int | no | Impulse time for shutter that needs impulse instead of holding up/down relays. If set to 0 impulse shutter is disabled. Time is in ms. Default to 0
stop_both | bool | no | If in impulse mode, some shutters needs to activate both up dans down relays when stopping the shutter
gateway | list | yes | Gateway type used, tcp or serial are supported
node_id_up | string | yes | Node ID for opening shutter, as set in your network
node_id_down | string | yes | Node ID for closing shutter, as set in your network
port | string | yes | If using serial gateway, port is the serial port (/dev/ttyUSB0 for ex.). If using tcp gateway port is TCP port of the gateway.
host | string | yes | IP address of the tcp gateway if relevant
sensor_id_up | string | yes | Sensor ID for opening shutter, as set in your node

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
 down | Close the shutter 
 stop | Stop the shutter 
 set_state false | Update internal shutter state without starting real action. This is useful when having updating the shutter state from an external source. 
 toggle | Invert shutter state 
 impulse up 200 | Open shutter for X ms 
 up | Open the shutter 
 impulse down 200 | Close shutter for X ms 
 
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
node_id_down | string | yes | Node ID for closing shutter, as set in your network
io_type | string | yes | IO type, can be "input", "output", "inout"
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
id | string | yes | Unique ID identifying the Input/Output in calaos-server
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
name | string | yes | Name of Input/Output.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
impulse_time | int | no | Impulse time for shutter that needs impulse instead of holding up/down relays. If set to 0 impulse shutter is disabled. Time is in ms. Default to 0
time_up | int | yes | Time in sec for shutter to be fully open. The more accurate, the better it will work
stop_both | bool | no | If in impulse mode, some shutters needs to activate both up dans down relays when stopping the shutter
time_down | int | yes | Time in sec for shutter to fully closed. The more accurate, the better it will work
gateway | list | yes | Gateway type used, tcp or serial are supported
node_id_up | string | yes | Node ID for opening shutter, as set in your network
port | string | yes | If using serial gateway, port is the serial port (/dev/ttyUSB0 for ex.). If using tcp gateway port is TCP port of the gateway.
host | string | yes | IP address of the tcp gateway if relevant
sensor_id_up | string | yes | Sensor ID for opening shutter, as set in your node

##Conditions of MySensorsOutputShutterSmart
Name | Description
---- | -----------
false | Event when shutter is closed 
 changed | Event on any change of shutter state 
 true | Event when shutter is open 
 
##Actions of MySensorsOutputShutterSmart
Name | Description
---- | -----------
set_state true | Update internal shutter state without starting real action. This is useful when having updating the shutter state from an external source. 
 calibrate | Start calibration on shutter. This opens fully the shutter and resets all internal position values. Use this if shutter sync is lost. 
 set 50 | Set shutter at position X in percent 
 down | Close the shutter 
 stop | Stop the shutter 
 set_state false | Update internal shutter state without starting real action. This is useful when having updating the shutter state from an external source. 
 up 5 | Open the shutter by X percent 
 toggle | Invert shutter state 
 down 5 | Close the shutter by X percent 
 impulse up 200 | Open shutter for X ms 
 up | Open the shutter 
 impulse down 200 | Close shutter for X ms 
 
##More Infos
* MySensors: http://mysensors.org


#MySensorsOutputString
String output with MySensors node

##Parameters of MySensorsOutputString
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
node_id | string | yes | Node ID as set in your network
host | string | yes | IP address of the tcp gateway if relevant
sensor_id | string | yes | Sensor ID, as set in your node
port | string | yes | If using serial gateway, port is the serial port (/dev/ttyUSB0 for ex.). If using tcp gateway port is TCP port of the gateway.
gateway | list | yes | Gateway type used, tcp or serial are supported
data_type | string | no | Data type sent to the node. Default: V_VAR1, see MySensors.cpp for more values.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

##More Infos
* MySensors: http://mysensors.org


#OLAOutputLightDimmer
DMX Light dimmer using OLA (Open Lighting Architecture)


Light with dimming control. Light intensity can be changed for this light.

##Parameters of OLAOutputLightDimmer
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
universe | int | yes | OLA universe to control
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
channel | int | yes | DMX channel to control
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

##Conditions of OLAOutputLightDimmer
Name | Description
---- | -----------
value | Event when light is at this value 
 changed | Event on any change of value 
 
##Actions of OLAOutputLightDimmer
Name | Description
---- | -----------
hold stop | Dynamically change light intensity when holding a switch (stop action) 
 down 5 | Decrease intensity by X percent 
 set 50 | Set light intensity and swith on if light is off 
 set off 50 | Set light value without switching on. This will be the light intensity for the next ON 
 set_state 50 | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 false | Switch the light off 
 hold press | Dynamically change light intensity when holding a switch (press action) 
 true | Switch the light on 
 up 5 | Increase intensity by X percent 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 toggle | Invert the light state 
 impulse 200 | Do an impulse on light state. Set to true for X ms then reset to false 
 impulse 500 200 500 200 | Do an impulse on light state with a pattern.<br>Ex: 500 200 500 200 means: TRUE for 500ms, FALSE for 200ms, TRUE for 500ms, FALSE for 200ms<br>Ex: 500 loop 200 300 means: TRUE for 500ms, then loop the next steps for infinite, FALSE for 200ms, TRUE for 300ms<br>Ex: 100 100 200 old means: blinks and then set to the old start state (before impulse starts) 
 set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 
##More Infos
* OLA: http://www.openlighting.org


#OLAOutputLightRGB
RGB Light dimmer using 3 DMX channels with OLA (Open Lighting Architecture)


RGB light. Choose a color to be set for this light.

##Parameters of OLAOutputLightRGB
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
channel_blue | int | yes | DMX channel for blue to control
channel_green | int | yes | DMX channel for green to control
channel_red | int | yes | DMX channel for red to control
universe | int | yes | OLA universe to control
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

##Conditions of OLAOutputLightRGB
Name | Description
---- | -----------
value | Event when light is at this value 
 changed | Event on any change of value 
 
##Actions of OLAOutputLightRGB
Name | Description
---- | -----------
down_blue 5 | Decrease intensity by X percent of blue channel 
 up_blue 5 | Increase intensity by X percent of blue channel 
 set_blue 50 | Set blue channel to X percent 
 set_green 50 | Set green channel to X percent 
 set #AA1294 | Set color. Color can be represented by using HTML notation: #AABBCC, rgb(50, 10, 30), hsl(11, 22, 33) 
 up_red 5 | Increase intensity by X percent of red channel 
 false | Switch the light off 
 up_green 5 | Increase intensity by X percent of green channel 
 true | Switch the light on 
 set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 down_red 5 | Decrease intensity by X percent of red channel 
 set_red 50 | Set red channel to X percent 
 toggle | Invert the light state (ON/OFF) 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 down_green 5 | Decrease intensity by X percent of green channel 
 set_state #AA1294 | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 
##More Infos
* OLA: http://www.openlighting.org


#OutputFake
Fake test output. Do nothing. Do not use.

##Parameters of OutputFake
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"


#OWTemp
Temperature measurement with DS18B20 Onewire Sensor


Temperature sensor input. Use for displaying temperature and to control heating devices with rules based on temperature value

##Parameters of OWTemp
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
ow_id | string | yes | Unique ID of sensor on OneWire bus.
interval | float | no | Sampling time in seconds. The value is read at this frequency. If this value is not set, the default value is 15s
precision | int | no | Precision of the returned value. The value represents the number of decimal after the dot. The value is rounded like this : value = 19.275 => returned value 19.28 when preicision = 2, 19.3 when precision = 1, 19 when precision = 0
offset | float | no | same as coeff_b, can be used alone. Default value is 0.0
coeff_a | float | no | use in conjunction of coeff_b to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 1.0.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
use_w1 | bool | no | Force the use of w1 kernel driver instead of OneWire driver
ow_args | string | yes | Additional parameter used for owfs initialization.For example you can use -u to use the USB owfs drivers
frequency | float | no | Sampling time in microsecond. The value is read at this frequency. If this value is not set, calaos tries to read the interval parameter
name | string | yes | Name of Input/Output.
coeff_b | float | no | use in conjunction of coeff_a to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 0.0
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

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
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | A switch can't be visible. Always false.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

##Conditions of PingInputSwitch
Name | Description
---- | -----------
changed | Event on any change of state 
 true | The host is online and respond to the ping 
 false | The host is offline and/or does not respond to the ping 
 
#Planet - UNDOCUMENTED IO
SPANK SPANK SPANK : naughty programmer ! You did not add documentation for this IO, that's BAD :'(
Go document it in your code or you will burn in hell!



Planet IP Camera/Encoder. Camera can be viewed directly inside calaos and used in rules.

##Parameters of Planet
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
password | string | no | Password for user
model | string | yes | Camera model (ICA-210, ICA-210W, ICA-300, ICA-302, ICA-500) to use
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
username | string | no | Username for accessing the camera
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"


#Scenario
A scenario variable. Use this like a virtual button to start a scenario (list of actions)

##Parameters of Scenario
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
auto_scenario | string | no | Internal use only for Auto Scenario. read only.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

##Conditions of Scenario
Name | Description
---- | -----------
true | Event triggered when scenario is started 
 
##Actions of Scenario
Name | Description
---- | -----------
changed | Event triggered on any change 
 true | Start the scenario 
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
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID of squeezebox in LMS
visible | bool | yes | Audio players are not displayed in rooms
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

##Conditions of Squeezebox
Name | Description
---- | -----------
onplaylistchange | Event when a change in the current playlist happens 
 onsongchange | Event when a new song is being played 
 onstop | Event when stopping player 
 onplay | Event when play is started 
 onvolumechange | Event when a change of volume happens 
 onpause | Event when pausing player 
 
##Actions of Squeezebox
Name | Description
---- | -----------
unsync <playerid> | Stop sync of this player with an other 
 sync <playerid> | Sync this player with an other 
 power off | Switch player off 
 volume down 1 | Decrease volume by a value 
 power on | Switch player on 
 volume up 1 | Increase volume by a value 
 play <argument> | Clear playlist and play argument. <argument> can be any of album_id:XX artist_id:XX playlist_id:XX, ... 
 sleep 10 | Start sleep mode with X seconds 
 pause | Pause player 
 add <argument> | Add tracks to playlist. <argument> can be any of album_id:XX artist_id:XX playlist_id:XX, ... 
 play | Start playing 
 previous | Play previous song in playlist 
 volume set 50 | Set current volume 
 stop | Stop player 
 next | Play next song in playlist 
 
#standard_mjpeg - UNDOCUMENTED IO
SPANK SPANK SPANK : naughty programmer ! You did not add documentation for this IO, that's BAD :'(
Go document it in your code or you will burn in hell!



MJPEG/Jpeg IP Camera. Camera can be viewed directly inside calaos and used in rules.

##Parameters of standard_mjpeg
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
ptz | bool | no | Set to true if camera has PTZ support
url_jpeg | string | yes | URL for snapshot in jpeg format
log_history | bool | no | If enabled, write an entry in the history event log for this IO
url_mjpeg | string | no | URL for mjpeg stream support
name | string | yes | Name of Input/Output.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

#StandardMjpeg - UNDOCUMENTED IO
SPANK SPANK SPANK : naughty programmer ! You did not add documentation for this IO, that's BAD :'(
Go document it in your code or you will burn in hell!



MJPEG/Jpeg IP Camera. Camera can be viewed directly inside calaos and used in rules.

##Parameters of StandardMjpeg
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
ptz | bool | no | Set to true if camera has PTZ support
url_jpeg | string | yes | URL for snapshot in jpeg format
log_history | bool | no | If enabled, write an entry in the history event log for this IO
url_mjpeg | string | no | URL for mjpeg stream support
name | string | yes | Name of Input/Output.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

#SynoSurveillanceStation - UNDOCUMENTED IO
SPANK SPANK SPANK : naughty programmer ! You did not add documentation for this IO, that's BAD :'(
Go document it in your code or you will burn in hell!



Synology Surveillance Station IP Camera. Camera can be viewed directly inside calaos and used in rules.

##Parameters of SynoSurveillanceStation
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
camera_profile | int | no | Profile to use for snapshot. 0- High quality, 1- Balanced, 2- Low bandwidth
camera_id | string | yes | ID of the camera
password | string | yes | Password for user
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
username | string | yes | Username which can access Surveillance Station
url | string | yes | Full url to Synology nas. Ex: https://192.168.0.22:5000
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"


#TeleinfoInputAnalog
Analog measurement retrieved from Teleinfo informations.


An analog input can be used to read analog values to display them and use them in rules.

##Parameters of TeleinfoInputAnalog
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
io_type | string | yes | IO type, can be "input", "output", "inout"
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
value | list | yes | All theses values are reported by the Teleinfo equipment as double.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
coeff_b | float | no | use in conjunction of coeff_a to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 0.0
name | string | yes | Name of Input/Output.
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
frequency | float | no | Sampling time in microsecond. The value is read at this frequency. If this value is not set, calaos tries to read the interval parameter
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
unit | string | no | Unit which will be displayed on the UI as a suffix.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
coeff_a | float | no | use in conjunction of coeff_b to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 1.0.
offset | float | no | same as coeff_b, can be used alone. Default value is 0.0
precision | int | no | Precision of the returned value. The value represents the number of decimal after the dot. The value is rounded like this : value = 19.275 => returned value 19.28 when preicision = 2, 19.3 when precision = 1, 19 when precision = 0
port | string | yes | port on which to get Teleinfo information usually a serial port like /dev/ttyS0 or /dev/ttyAMA0
interval | float | no | Sampling time in seconds. The value is read at this frequency. If this value is not set, the default value is 15s
gui_style | list | yes | GUI style display. This will control the icon displayed on the UI

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
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | A time range can't be visible. Always false.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

##Conditions of TimeRange
Name | Description
---- | -----------
changed | Event on any change of range 
 true | Event triggered when entering the range 
 false | Event triggered when exiting the range 
 

#WebInputAnalog
Analog input read from a web document


An analog input can be used to read analog values to display them and use them in rules.

##Parameters of WebInputAnalog
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
file_type | string | yes | File type of the document. Values can be xml, json or text.
io_type | string | yes | IO type, can be "input", "output", "inout"
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
coeff_b | float | no | use in conjunction of coeff_a to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 0.0
path | string | yes | The path where to found the value. This value can take multiple values depending on the file type. If file_type is JSON, the json file downloaded will be read, and the informations will be extracted from the path. for example weather[0]/description, try to read the description value of the 1 element of the array of the weather object.
If file_type is XML, the path is an xpath expression; Look here for syntax : http://www.w3schools.com/xsl/xpath_syntax.asp If file_type is TEXT, the downloaded file is returned as plain text file, and path must be in the form line/pos/separator Line is read, and is split using separator as delimiters The value returned is the value at pos in the split list. If the separator is not found, the whole line is returned. Example the file contains 
10.0,10.1,10.2,10.3
20.0,20.1,20.2,20.3
If the path is 2/4/, the value returne wil be 20.3

url | string | yes | URL where to download the document from
If URL begins with / or with file:// the data is read from the local file
name | string | yes | Name of Input/Output.
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
frequency | float | no | Sampling time in microsecond. The value is read at this frequency. If this value is not set, calaos tries to read the interval parameter
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
unit | string | no | Unit which will be displayed on the UI as a suffix.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
coeff_a | float | no | use in conjunction of coeff_b to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 1.0.
offset | float | no | same as coeff_b, can be used alone. Default value is 0.0
precision | int | no | Precision of the returned value. The value represents the number of decimal after the dot. The value is rounded like this : value = 19.275 => returned value 19.28 when preicision = 2, 19.3 when precision = 1, 19 when precision = 0
interval | float | no | Sampling time in seconds. The value is read at this frequency. If this value is not set, the default value is 15s
gui_style | list | yes | GUI style display. This will control the icon displayed on the UI

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

log_history | bool | no | If enabled, write an entry in the history event log for this IO
file_type | string | yes | File type of the document. Values can be xml, json or text.
name | string | yes | Name of Input/Output.
url | string | yes | URL where to download the document from
If URL begins with / or with file:// the data is read from the local file
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"


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

interval | float | no | Sampling time in seconds. The value is read at this frequency. If this value is not set, the default value is 15s
precision | int | no | Precision of the returned value. The value represents the number of decimal after the dot. The value is rounded like this : value = 19.275 => returned value 19.28 when preicision = 2, 19.3 when precision = 1, 19 when precision = 0
offset | float | no | same as coeff_b, can be used alone. Default value is 0.0
coeff_a | float | no | use in conjunction of coeff_b to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 1.0.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
file_type | string | yes | File type of the document. Values can be xml, json or text.
frequency | float | no | Sampling time in microsecond. The value is read at this frequency. If this value is not set, calaos tries to read the interval parameter
name | string | yes | Name of Input/Output.
url | string | yes | URL where to download the document from
If URL begins with / or with file:// the data is read from the local file
coeff_b | float | no | use in conjunction of coeff_a to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 0.0
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

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

gui_style | list | yes | GUI style display. This will control the icon displayed on the UI
step | float | no | Set a step for increment/decrement value. Default is 1.0
coeff_a | float | no | use in conjunction of coeff_b to apply equation of the form `value_sent = coeff_a * raw_value + coeff_b`. Default value is 1.0.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
file_type | string | yes | File type of the document. Values can be xml, json or text.
unit | string | no | Unit which will be displayed on the UI as a suffix.
name | string | yes | Name of Input/Output.
url | string | yes | URL where to download the document from
If URL begins with / or with file:// the data is read from the local file
coeff_b | float | no | use in conjunction of coeff_a to apply equation of the form `value_sent = coeff_a * raw_value + coeff_b`. Default value is 0.0
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

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
 dec | Decrement value with configured step 
 inc 1 | Increment value by value 
 0 | Set a specific number value 
 inc | Increment value with configured step 
 

#WebOutputLight
Bool output written to a web document or URL


Basic light. This light have only 2 states, ON or OFF. Can also be used to control simple relays output

##Parameters of WebOutputLight
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
data_type | string | yes | The HTTP header Content-Type used when posting the document. It depends on the website, but you can use application/json application/xml as correct values.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
url | string | yes | URL where to POST the document to. The POST request is associated with the data field if not null. When no data is provided, Calaos substitutes __##VALUE##__ string with the value to send. For example if the url is http://example.com/api?value=__##VALUE##__ the url post will be :
http://example.com/api?value=20.3
The url is encoded before being sent.
If the URL begins with / or file:// the data is written to a file.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
data | string | yes | The document send when posting data. This value can be void, in, that case the value is substituted in the url, otherwise the __##VALUE##__ contained in data is substituted with with the value to be sent.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

##Conditions of WebOutputLight
Name | Description
---- | -----------
false | Event when light is off 
 changed | Event on any change of value 
 true | Event when light is on 
 
##Actions of WebOutputLight
Name | Description
---- | -----------
false | Switch the light off 
 true | Switch the light on 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 toggle | Invert light state 
 impulse 200 | Do an impulse on light state. Set to true for X ms then reset to false 
 impulse 500 200 500 200 | Do an impulse on light state with a pattern.<br>Ex: 500 200 500 200 means: TRUE for 500ms, FALSE for 200ms, TRUE for 500ms, FALSE for 200ms<br>Ex: 500 loop 200 300 means: TRUE for 500ms, then loop the next steps for infinite, FALSE for 200ms, TRUE for 300ms<br>Ex: 100 100 200 old means: blinks and then set to the old start state (before impulse starts) 
 set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 

#WebOutputLightRGB
RGB value written to a web document or URL


RGB light. Choose a color to be set for this light.

##Parameters of WebOutputLightRGB
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
raw_value | bool | no | RGB value has #RRGGBB. Sometimes some web api take only RRGGBBformat. If raw_value is true, the # in front of the line isremoved. The default value for this parameter is false.
data_type | string | yes | The HTTP header Content-Type used when posting the document. It depends on the website, but you can use application/json application/xml as correct values.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
url | string | yes | URL where to POST the document to. The POST request is associated with the data field if not null. When no data is provided, Calaos substitutes __##VALUE##__ string with the value to send. For example if the url is http://example.com/api?value=__##VALUE##__ the url post will be :
http://example.com/api?value=20.3
The url is encoded before being sent.
If the URL begins with / or file:// the data is written to a file.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
data | string | yes | The document send when posting data. This value can be void, in, that case the value is substituted in the url, otherwise the __##VALUE##__ contained in data is substituted with with the value to be sent.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

##Conditions of WebOutputLightRGB
Name | Description
---- | -----------
value | Event when light is at this value 
 changed | Event on any change of value 
 
##Actions of WebOutputLightRGB
Name | Description
---- | -----------
down_blue 5 | Decrease intensity by X percent of blue channel 
 up_blue 5 | Increase intensity by X percent of blue channel 
 set_blue 50 | Set blue channel to X percent 
 set_green 50 | Set green channel to X percent 
 set #AA1294 | Set color. Color can be represented by using HTML notation: #AABBCC, rgb(50, 10, 30), hsl(11, 22, 33) 
 up_red 5 | Increase intensity by X percent of red channel 
 false | Switch the light off 
 up_green 5 | Increase intensity by X percent of green channel 
 true | Switch the light on 
 set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 down_red 5 | Decrease intensity by X percent of red channel 
 set_red 50 | Set red channel to X percent 
 toggle | Invert the light state (ON/OFF) 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 down_green 5 | Decrease intensity by X percent of green channel 
 set_state #AA1294 | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 

#WebOutputString
String output written to a web document or URL

##Parameters of WebOutputString
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
path | string | yes | The path where to found the value. This value can take multiple values depending on the file type. If file_type is JSON, the json file downloaded will be read, and the informations will be extracted from the path. for example weather[0]/description, try to read the description value of the 1 element of the array of the weather object.
If file_type is XML, the path is an xpath expression; Look here for syntax : http://www.w3schools.com/xsl/xpath_syntax.asp If file_type is TEXT, the downloaded file is returned as plain text file, and path must be in the form line/pos/separator Line is read, and is split using separator as delimiters The value returned is the value at pos in the split list. If the separator is not found, the whole line is returned. Example the file contains 
10.0,10.1,10.2,10.3
20.0,20.1,20.2,20.3
If the path is 2/4/, the value returne wil be 20.3

data_type | string | yes | The HTTP header Content-Type used when posting the document. It depends on the website, but you can use application/json application/xml as correct values.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
file_type | string | yes | File type of the document. Values can be xml, json or text.
name | string | yes | Name of Input/Output.
url | string | yes | URL where to POST the document to. The POST request is associated with the data field if not null. When no data is provided, Calaos substitutes __##VALUE##__ string with the value to send. For example if the url is http://example.com/api?value=__##VALUE##__ the url post will be :
http://example.com/api?value=20.3
The url is encoded before being sent.
If the URL begins with / or file:// the data is written to a file.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
data | string | yes | The document send when posting data. This value can be void, in, that case the value is substituted in the url, otherwise the __##VALUE##__ contained in data is substituted with with the value to be sent.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"


#WIAnalog
#### Alias: WagoInputAnalog
Analog measurement with Wago module (like 0-10V, 4-20mA, ...)


An analog input can be used to read analog values to display them and use them in rules.

##Parameters of WIAnalog
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
io_type | string | yes | IO type, can be "input", "output", "inout"
var | int | yes | PLC address of the input sensor
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
coeff_b | float | no | use in conjunction of coeff_a to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 0.0
name | string | yes | Name of Input/Output.
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
frequency | float | no | Sampling time in microsecond. The value is read at this frequency. If this value is not set, calaos tries to read the interval parameter
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
unit | string | no | Unit which will be displayed on the UI as a suffix.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
coeff_a | float | no | use in conjunction of coeff_b to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 1.0.
offset | float | no | same as coeff_b, can be used alone. Default value is 0.0
precision | int | no | Precision of the returned value. The value represents the number of decimal after the dot. The value is rounded like this : value = 19.275 => returned value 19.28 when preicision = 2, 19.3 when precision = 1, 19 when precision = 0
interval | float | no | Sampling time in seconds. The value is read at this frequency. If this value is not set, the default value is 15s
gui_style | list | yes | GUI style display. This will control the icon displayed on the UI
port | int | no | Wago ethernet port, default to 502
host | string | yes | Wago PLC IP address on the network

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
var | int | yes | PLC address of the digital input
port | int | no | Wago ethernet port, default to 502
host | string | yes | Wago PLC IP address on the network
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | A switch can't be visible. Always false.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

##Conditions of WIDigitalBP
Name | Description
---- | -----------
changed | Event on any change of state 
 true | Event triggered when switch is pressed 
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
var | int | yes | PLC address of the digital input
port | int | no | Wago ethernet port, default to 502
host | string | yes | Wago PLC IP address on the network
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | A switch can't be visible. Always false.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

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
var | int | yes | PLC address of the digital input
port | int | no | Wago ethernet port, default to 502
host | string | yes | Wago PLC IP address on the network
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | A switch can't be visible. Always false.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

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
var | int | yes | PLC address of the input sensor
host | string | yes | Wago PLC IP address on the network
interval | float | no | Sampling time in seconds. The value is read at this frequency. If this value is not set, the default value is 15s
port | int | no | Wago ethernet port, default to 502
precision | int | no | Precision of the returned value. The value represents the number of decimal after the dot. The value is rounded like this : value = 19.275 => returned value 19.28 when preicision = 2, 19.3 when precision = 1, 19 when precision = 0
offset | float | no | same as coeff_b, can be used alone. Default value is 0.0
coeff_a | float | no | use in conjunction of coeff_b to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 1.0.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
frequency | float | no | Sampling time in microsecond. The value is read at this frequency. If this value is not set, calaos tries to read the interval parameter
name | string | yes | Name of Input/Output.
coeff_b | float | no | use in conjunction of coeff_a to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 0.0
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

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
var | int | yes | PLC address of the output
port | int | no | Wago ethernet port, default to 502
host | string | yes | Wago PLC IP address on the network
gui_style | list | yes | GUI style display. This will control the icon displayed on the UI
step | float | no | Set a step for increment/decrement value. Default is 1.0
coeff_a | float | no | use in conjunction of coeff_b to apply equation of the form `value_sent = coeff_a * raw_value + coeff_b`. Default value is 1.0.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
unit | string | no | Unit which will be displayed on the UI as a suffix.
name | string | yes | Name of Input/Output.
coeff_b | float | no | use in conjunction of coeff_a to apply equation of the form `value_sent = coeff_a * raw_value + coeff_b`. Default value is 0.0
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

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
 dec | Decrement value with configured step 
 inc 1 | Increment value by value 
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
address | int | yes | Device address. For DALI address is between 1-64. For DMX, the address starts at 100. So for DMX device 5, address should be 105
fade_time | int | no | DALI fade time. value is between 1-10
line | int | no | DALI bus line, usually 1
port | int | no | Wago ethernet port, default to 502
host | string | yes | Wago PLC IP address on the network
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
group | int | no | Set to 1 if address is a DALI group address, set to 0 otherwise.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

##Conditions of WODali
Name | Description
---- | -----------
value | Event when light is at this value 
 changed | Event on any change of value 
 
##Actions of WODali
Name | Description
---- | -----------
hold stop | Dynamically change light intensity when holding a switch (stop action) 
 down 5 | Decrease intensity by X percent 
 set 50 | Set light intensity and swith on if light is off 
 set off 50 | Set light value without switching on. This will be the light intensity for the next ON 
 set_state 50 | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 false | Switch the light off 
 hold press | Dynamically change light intensity when holding a switch (press action) 
 true | Switch the light on 
 up 5 | Increase intensity by X percent 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 toggle | Invert the light state 
 impulse 200 | Do an impulse on light state. Set to true for X ms then reset to false 
 impulse 500 200 500 200 | Do an impulse on light state with a pattern.<br>Ex: 500 200 500 200 means: TRUE for 500ms, FALSE for 200ms, TRUE for 500ms, FALSE for 200ms<br>Ex: 500 loop 200 300 means: TRUE for 500ms, then loop the next steps for infinite, FALSE for 200ms, TRUE for 300ms<br>Ex: 100 100 200 old means: blinks and then set to the old start state (before impulse starts) 
 set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 
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
bfade_time | int | no | DALI fade time for blue channel. value is between 1-10
bgroup | int | no | Set to 1 if address for blue channel is a DALI group address, set to 0 otherwise.
baddress | int | yes | Device address for blue channel. For DALI address is between 1-64. For DMX, the address starts at 100. So for DMX device 5, address should be 105
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
bline | int | no | DALI bus line for blue channel, usually 1
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
id | string | yes | Unique ID identifying the Input/Output in calaos-server
gfade_time | int | no | DALI fade time for green channel. value is between 1-10
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
name | string | yes | Name of Input/Output.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
rline | int | no | DALI bus line for red channel, usually 1
io_type | string | yes | IO type, can be "input", "output", "inout"
raddress | int | yes | Device address for red channel. For DALI address is between 1-64. For DMX, the address starts at 100. So for DMX device 5, address should be 105
rgroup | int | no | Set to 1 if address for red channel is a DALI group address, set to 0 otherwise.
host | string | yes | Wago PLC IP address on the network
port | int | no | Wago ethernet port, default to 502
rfade_time | int | no | DALI fade time for red channel. value is between 1-10
gline | int | no | DALI bus line for green channel, usually 1
ggroup | int | no | Set to 1 if address for green channel is a DALI group address, set to 0 otherwise.
gaddress | int | yes | Device address for green channel. For DALI address is between 1-64. For DMX, the address starts at 100. So for DMX device 5, address should be 105

##Conditions of WODaliRVB
Name | Description
---- | -----------
value | Event when light is at this value 
 changed | Event on any change of value 
 
##Actions of WODaliRVB
Name | Description
---- | -----------
down_blue 5 | Decrease intensity by X percent of blue channel 
 up_blue 5 | Increase intensity by X percent of blue channel 
 set_blue 50 | Set blue channel to X percent 
 set_green 50 | Set green channel to X percent 
 set #AA1294 | Set color. Color can be represented by using HTML notation: #AABBCC, rgb(50, 10, 30), hsl(11, 22, 33) 
 up_red 5 | Increase intensity by X percent of red channel 
 false | Switch the light off 
 up_green 5 | Increase intensity by X percent of green channel 
 true | Switch the light on 
 set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 down_red 5 | Decrease intensity by X percent of red channel 
 set_red 50 | Set red channel to X percent 
 toggle | Invert the light state (ON/OFF) 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 down_green 5 | Decrease intensity by X percent of green channel 
 set_state #AA1294 | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 
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
var | int | yes | PLC address of the output
port | int | no | Wago ethernet port, default to 502
wago_841 | bool | yes | Should be false if PLC is 750-842, true otherwise
host | string | yes | Wago PLC IP address on the network
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
knx | bool | no | Set to true if output is a KNX device (only for 750-849 with KNX/TP1 module)
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

##Conditions of WODigital
Name | Description
---- | -----------
false | Event when light is off 
 changed | Event on any change of value 
 true | Event when light is on 
 
##Actions of WODigital
Name | Description
---- | -----------
false | Switch the light off 
 true | Switch the light on 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 toggle | Invert light state 
 impulse 200 | Do an impulse on light state. Set to true for X ms then reset to false 
 impulse 500 200 500 200 | Do an impulse on light state with a pattern.<br>Ex: 500 200 500 200 means: TRUE for 500ms, FALSE for 200ms, TRUE for 500ms, FALSE for 200ms<br>Ex: 500 loop 200 300 means: TRUE for 500ms, then loop the next steps for infinite, FALSE for 200ms, TRUE for 300ms<br>Ex: 100 100 200 old means: blinks and then set to the old start state (before impulse starts) 
 set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 
##More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/750-1504


#WOLOutputBool
Wake On Lan output object. Send wake-on-lan packet to a device on the network.

##Parameters of WOLOutputBool
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
interval | int | no | Interval between pings in ms. Default to 15 sec
address | string | yes | Ethernet MAC address of the host to wake up
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

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
port | int | no | Wago ethernet port, default to 502
wago_841 | bool | yes | Should be false if PLC is 750-842, true otherwise
host | string | yes | Wago PLC IP address on the network
stop_both | bool | no | If in impulse mode, some shutters needs to activate both up dans down relays when stopping the shutter
impulse_time | int | no | Impulse time for shutter that needs impulse instead of holding up/down relays. If set to 0 impulse shutter is disabled. Time is in ms. Default to 0
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
knx | bool | no | Set to true if output is a KNX device (only for 750-849 with KNX/TP1 module)
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
time | int | yes | Time in sec for shutter to open or close
id | string | yes | Unique ID identifying the Input/Output in calaos-server
var_up | int | yes | Digital output address on the PLC for opening the shutter
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

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
 down | Close the shutter 
 stop | Stop the shutter 
 set_state false | Update internal shutter state without starting real action. This is useful when having updating the shutter state from an external source. 
 toggle | Invert shutter state 
 impulse up 200 | Open shutter for X ms 
 up | Open the shutter 
 impulse down 200 | Close shutter for X ms 
 
##More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/750-1504


#WOVoletSmart
#### Alias: WagoOutputShutterSmart
Smart shutter using wago digital output modules (like 750-1504, ...)


Smart shutter. This shutter calculates the position of the shutter based on the time it takes to open and close. It then allows to set directly the shutter at a specified position.

##Parameters of WOVoletSmart
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
knx | bool | no | Set to true if output is a KNX device (only for 750-849 with KNX/TP1 module)
io_type | string | yes | IO type, can be "input", "output", "inout"
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
id | string | yes | Unique ID identifying the Input/Output in calaos-server
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
name | string | yes | Name of Input/Output.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
impulse_time | int | no | Impulse time for shutter that needs impulse instead of holding up/down relays. If set to 0 impulse shutter is disabled. Time is in ms. Default to 0
time_up | int | yes | Time in sec for shutter to be fully open. The more accurate, the better it will work
stop_both | bool | no | If in impulse mode, some shutters needs to activate both up dans down relays when stopping the shutter
time_down | int | yes | Time in sec for shutter to fully closed. The more accurate, the better it will work
wago_841 | bool | yes | Should be false if PLC is 750-842, true otherwise
var_up | int | yes | Digital output address on the PLC for opening the shutter
host | string | yes | Wago PLC IP address on the network
port | int | no | Wago ethernet port, default to 502
var_down | int | yes | Digital output address on the PLC for closing the shutter

##Conditions of WOVoletSmart
Name | Description
---- | -----------
false | Event when shutter is closed 
 changed | Event on any change of shutter state 
 true | Event when shutter is open 
 
##Actions of WOVoletSmart
Name | Description
---- | -----------
set_state true | Update internal shutter state without starting real action. This is useful when having updating the shutter state from an external source. 
 calibrate | Start calibration on shutter. This opens fully the shutter and resets all internal position values. Use this if shutter sync is lost. 
 set 50 | Set shutter at position X in percent 
 down | Close the shutter 
 stop | Stop the shutter 
 set_state false | Update internal shutter state without starting real action. This is useful when having updating the shutter state from an external source. 
 up 5 | Open the shutter by X percent 
 toggle | Invert shutter state 
 down 5 | Close the shutter by X percent 
 impulse up 200 | Open shutter for X ms 
 up | Open the shutter 
 impulse down 200 | Close shutter for X ms 
 
##More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/750-1504


#X10Output
Light dimmer using X10 and heyu.


Light with dimming control. Light intensity can be changed for this light.

##Parameters of X10Output
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
code | string | yes | House code of the X10 light device
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

##Conditions of X10Output
Name | Description
---- | -----------
value | Event when light is at this value 
 changed | Event on any change of value 
 
##Actions of X10Output
Name | Description
---- | -----------
hold stop | Dynamically change light intensity when holding a switch (stop action) 
 down 5 | Decrease intensity by X percent 
 set 50 | Set light intensity and swith on if light is off 
 set off 50 | Set light value without switching on. This will be the light intensity for the next ON 
 set_state 50 | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 false | Switch the light off 
 hold press | Dynamically change light intensity when holding a switch (press action) 
 true | Switch the light on 
 up 5 | Increase intensity by X percent 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 toggle | Invert the light state 
 impulse 200 | Do an impulse on light state. Set to true for X ms then reset to false 
 impulse 500 200 500 200 | Do an impulse on light state with a pattern.<br>Ex: 500 200 500 200 means: TRUE for 500ms, FALSE for 200ms, TRUE for 500ms, FALSE for 200ms<br>Ex: 500 loop 200 300 means: TRUE for 500ms, then loop the next steps for infinite, FALSE for 200ms, TRUE for 300ms<br>Ex: 100 100 200 old means: blinks and then set to the old start state (before impulse starts) 
 set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 

#xPLInputAnalog
xPL analog sensor


An analog input can be used to read analog values to display them and use them in rules.

##Parameters of xPLInputAnalog
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
sensor | string | yes | Sensor ID, as set in your xPL network
io_type | string | yes | IO type, can be "input", "output", "inout"
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
coeff_b | float | no | use in conjunction of coeff_a to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 0.0
name | string | yes | Name of Input/Output.
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
frequency | float | no | Sampling time in microsecond. The value is read at this frequency. If this value is not set, calaos tries to read the interval parameter
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
unit | string | no | Unit which will be displayed on the UI as a suffix.
coeff_a | float | no | use in conjunction of coeff_b to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 1.0.
offset | float | no | same as coeff_b, can be used alone. Default value is 0.0
precision | int | no | Precision of the returned value. The value represents the number of decimal after the dot. The value is rounded like this : value = 19.275 => returned value 19.28 when preicision = 2, 19.3 when precision = 1, 19 when precision = 0
interval | float | no | Sampling time in seconds. The value is read at this frequency. If this value is not set, the default value is 15s
gui_style | list | yes | GUI style display. This will control the icon displayed on the UI
log_history | bool | no | If enabled, write an entry in the history event log for this IO
source | string | yes | Source name, as set in your xPL network (Format VendorId-DeviceId.Instance)

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
sensor | string | yes | Sensor ID, as set in your xPL network
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"


#xPLInputSwitch
xPL input switch


Basic switch with press/release states.

##Parameters of xPLInputSwitch
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
source | string | yes | Source name, as set in your xPL network (Format VendorId-DeviceId.Instance)
sensor | string | yes | Sensor ID, as set in your xPL network
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | A switch can't be visible. Always false.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

##Conditions of xPLInputSwitch
Name | Description
---- | -----------
changed | Event on any change of state 
 true | Event triggered when switch is pressed 
 false | Event triggered when switch is released 
 

#xPLInputTemp
xPL temperature sensor


Temperature sensor input. Use for displaying temperature and to control heating devices with rules based on temperature value

##Parameters of xPLInputTemp
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
source | string | yes | Source name, as set in your xPL network (Format VendorId-DeviceId.Instance)
interval | float | no | Sampling time in seconds. The value is read at this frequency. If this value is not set, the default value is 15s
precision | int | no | Precision of the returned value. The value represents the number of decimal after the dot. The value is rounded like this : value = 19.275 => returned value 19.28 when preicision = 2, 19.3 when precision = 1, 19 when precision = 0
offset | float | no | same as coeff_b, can be used alone. Default value is 0.0
coeff_a | float | no | use in conjunction of coeff_b to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 1.0.
sensor | string | yes | Sensor ID, as set in your xPL network
log_history | bool | no | If enabled, write an entry in the history event log for this IO
frequency | float | no | Sampling time in microsecond. The value is read at this frequency. If this value is not set, calaos tries to read the interval parameter
name | string | yes | Name of Input/Output.
coeff_b | float | no | use in conjunction of coeff_a to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 0.0
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

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
actuator | string | yes | Actuator ID, as set in your xPL network
source | string | yes | Source name, as set in your xPL network (Format VendorId-DeviceId.Instance)
gui_style | list | yes | GUI style display. This will control the icon displayed on the UI
step | float | no | Set a step for increment/decrement value. Default is 1.0
coeff_a | float | no | use in conjunction of coeff_b to apply equation of the form `value_sent = coeff_a * raw_value + coeff_b`. Default value is 1.0.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
unit | string | no | Unit which will be displayed on the UI as a suffix.
name | string | yes | Name of Input/Output.
coeff_b | float | no | use in conjunction of coeff_a to apply equation of the form `value_sent = coeff_a * raw_value + coeff_b`. Default value is 0.0
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

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
 dec | Decrement value with configured step 
 inc 1 | Increment value by value 
 0 | Set a specific number value 
 inc | Increment value with configured step 
 

#xPLOutputAnalog
Analog output controlled by xPL Protocol

##Parameters of xPLOutputAnalog
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
actuator | string | yes | Actuator ID, as set in your xPL network
source | string | yes | Source name, as set in your xPL network (Format VendorId-DeviceId.Instance)
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"


#xPLOutputSwitch
Light/relay controlled by xPL Protocol


Basic light. This light have only 2 states, ON or OFF. Can also be used to control simple relays output

##Parameters of xPLOutputSwitch
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
actuator | string | yes | Actuator ID, as set in your xPL network
source | string | yes | Source name, as set in your xPL network (Format VendorId-DeviceId.Instance)
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

##Conditions of xPLOutputSwitch
Name | Description
---- | -----------
false | Event when light is off 
 changed | Event on any change of value 
 true | Event when light is on 
 
##Actions of xPLOutputSwitch
Name | Description
---- | -----------
false | Switch the light off 
 true | Switch the light on 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 toggle | Invert light state 
 impulse 200 | Do an impulse on light state. Set to true for X ms then reset to false 
 impulse 500 200 500 200 | Do an impulse on light state with a pattern.<br>Ex: 500 200 500 200 means: TRUE for 500ms, FALSE for 200ms, TRUE for 500ms, FALSE for 200ms<br>Ex: 500 loop 200 300 means: TRUE for 500ms, then loop the next steps for infinite, FALSE for 200ms, TRUE for 300ms<br>Ex: 100 100 200 old means: blinks and then set to the old start state (before impulse starts) 
 set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 

#ZibaseAnalogIn
Zibase analog input. This object can read value from devices like Energy monitor sensors, Lux sensors, ...


An analog input can be used to read analog values to display them and use them in rules.

##Parameters of ZibaseAnalogIn
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
zibase_sensor | list | yes | Type of sensor
zibase_id | string | yes | Zibase device ID (ABC)
io_type | string | yes | IO type, can be "input", "output", "inout"
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
coeff_b | float | no | use in conjunction of coeff_a to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 0.0
name | string | yes | Name of Input/Output.
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
frequency | float | no | Sampling time in microsecond. The value is read at this frequency. If this value is not set, calaos tries to read the interval parameter
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
unit | string | no | Unit which will be displayed on the UI as a suffix.
log_history | bool | no | If enabled, write an entry in the history event log for this IO
coeff_a | float | no | use in conjunction of coeff_b to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 1.0.
offset | float | no | same as coeff_b, can be used alone. Default value is 0.0
precision | int | no | Precision of the returned value. The value represents the number of decimal after the dot. The value is rounded like this : value = 19.275 => returned value 19.28 when preicision = 2, 19.3 when precision = 1, 19 when precision = 0
interval | float | no | Sampling time in seconds. The value is read at this frequency. If this value is not set, the default value is 15s
gui_style | list | yes | GUI style display. This will control the icon displayed on the UI
port | int | no | Zibase ethernet port, default to 17100
host | string | yes | Zibase IP address on the network

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
zibase_id | string | yes | First Zibase device ID (ABC)
port | int | no | Zibase ethernet port, default to 17100
host | string | yes | Zibase IP address on the network
zibase_id2 | string | yes | Second Zibase device ID (ABC)
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | A switch can't be visible. Always false.
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

##Conditions of ZibaseDigitalIn
Name | Description
---- | -----------
changed | Event on any change of state 
 true | Event triggered when switch is pressed 
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
port | int | no | Zibase ethernet port, default to 17100
host | string | yes | Zibase IP address on the network
log_history | bool | no | If enabled, write an entry in the history event log for this IO
name | string | yes | Name of Input/Output.
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
io_type | string | yes | IO type, can be "input", "output", "inout"

##Conditions of ZibaseDigitalOut
Name | Description
---- | -----------
false | Event when light is off 
 changed | Event on any change of value 
 true | Event when light is on 
 
##Actions of ZibaseDigitalOut
Name | Description
---- | -----------
false | Switch the light off 
 true | Switch the light on 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 toggle | Invert light state 
 impulse 200 | Do an impulse on light state. Set to true for X ms then reset to false 
 impulse 500 200 500 200 | Do an impulse on light state with a pattern.<br>Ex: 500 200 500 200 means: TRUE for 500ms, FALSE for 200ms, TRUE for 500ms, FALSE for 200ms<br>Ex: 500 loop 200 300 means: TRUE for 500ms, then loop the next steps for infinite, FALSE for 200ms, TRUE for 300ms<br>Ex: 100 100 200 old means: blinks and then set to the old start state (before impulse starts) 
 set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 

#ZibaseTemp
Zibase temperature sensor


Temperature sensor input. Use for displaying temperature and to control heating devices with rules based on temperature value

##Parameters of ZibaseTemp
Name | Type | Mandatory | Description
---- | ---- | --------- | -----------
zibase_sensor | list | yes | Type of sensor
io_type | string | yes | IO type, can be "input", "output", "inout"
enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
id | string | yes | Unique ID identifying the Input/Output in calaos-server
gui_type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
coeff_b | float | no | use in conjunction of coeff_a to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 0.0
name | string | yes | Name of Input/Output.
visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
frequency | float | no | Sampling time in microsecond. The value is read at this frequency. If this value is not set, calaos tries to read the interval parameter
log_history | bool | no | If enabled, write an entry in the history event log for this IO
coeff_a | float | no | use in conjunction of coeff_b to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 1.0.
offset | float | no | same as coeff_b, can be used alone. Default value is 0.0
precision | int | no | Precision of the returned value. The value represents the number of decimal after the dot. The value is rounded like this : value = 19.275 => returned value 19.28 when preicision = 2, 19.3 when precision = 1, 19 when precision = 0
zibase_id | string | yes | Zibase device ID (ABC)
interval | float | no | Sampling time in seconds. The value is read at this frequency. If this value is not set, the default value is 15s
port | int | no | Zibase ethernet port, default to 17100
host | string | yes | Zibase IP address on the network

##Conditions of ZibaseTemp
Name | Description
---- | -----------
changed | Event on any change of temperature value 
 value | Event on a temperature value in degree Celsius 
 
