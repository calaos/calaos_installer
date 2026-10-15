
# AVReceiver
AVReceiver object to control network amplifier

## Parameters of AVReceiver
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
host |  | string | yes | IP address of the device
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
model |  | string | yes | AVReceiver model. Supported: pioneer, denon, onkyo, marantz, yamaha
name | Name | string | yes | Name of Input/Output.
port |  | int | no | Port to use for connection
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
zone |  | int | no | Zone of the amplifier (if supported)

## Actions of AVReceiver
Name | Description
---- | -----------
custom XXXXXX | Send a custom command to receiver (if you know the protocol) 
 power off | Switch receiver off 
 power on | Switch receiver on 
 source X | Change current input source 
 volume 50 | Set current volume 
 
# Axis - UNDOCUMENTED IO
SPANK SPANK SPANK : naughty programmer ! You did not add documentation for this IO, that's BAD :'(
Go document it in your code or you will burn in hell!



Axis IP Camera/Encoder. Camera can be viewed directly inside calaos and used in rules.

## Parameters of Axis
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
insecure |  | bool | no | Skip TLS certificate verification when connecting to the camera. Default to true (most cameras use self-signed certificates). Set to false to only allow a verified HTTPS connection.
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
model |  | string | yes | Camera model/chanel to use
name | Name | string | yes | Name of Input/Output.
pan_framesize |  | string | no | 
ptz |  | bool | no | Set to true if camera has PTZ support
resolution |  | string | no | Resolution to use
rotate |  | int | no | Rotate the image. Set a value between. The value is in degrees. Example : -90 for  Counter Clock Wise rotation, 90 for Clock Wise rotationCW.
tilt_framesize |  | string | no | 
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
width |  | int | no | Width of the image, if this parameter is set, video will be resized to fit the given width. Let parameter empty to keep the original size.
zoom_step |  | string | no | 

# Foscam - UNDOCUMENTED IO
SPANK SPANK SPANK : naughty programmer ! You did not add documentation for this IO, that's BAD :'(
Go document it in your code or you will burn in hell!



Foscam IP Camera/Encoder. Camera can be viewed directly inside calaos and used in rules.

## Parameters of Foscam
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
host |  | string | yes | IP Address
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
insecure |  | bool | no | Skip TLS certificate verification when connecting to the camera. Default to true (most cameras use self-signed certificates). Set to false to only allow a verified HTTPS connection.
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
password |  | string | yes | Password for user
port |  | string | yes | Port number
ptz |  | bool | no | Set to true if camera has PTZ support
rotate |  | int | no | Rotate the image. Set a value between. The value is in degrees. Example : -90 for  Counter Clock Wise rotation, 90 for Clock Wise rotationCW.
username |  | string | yes | Username for accessing the camera
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
width |  | int | no | Width of the image, if this parameter is set, video will be resized to fit the given width. Let parameter empty to keep the original size.
zoom_step |  | string | no | 


# GpioInputSwitch
Input switch with a GPIO


Basic switch with press/release states.

## Parameters of GpioInputSwitch
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
active_low |  | bool | no | Set this if your GPIO has an inverted level
debounce |  | float | no | Debounce time in seconds. Values changing faster than this are filtered out
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gpio |  | int | yes | GPIO ID on your hardware
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
io_style |  | list | yes | GUI style display. This will control the icon displayed on the UI
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
visible |  | bool | no | Display the Input/Output on all user interfaces if set. Default to false for switches

## Conditions of GpioInputSwitch
Name | Description
---- | -----------
changed | Event on any change of state 
 false | Event triggered when switch is released 
 true | Event triggered when switch is pressed 
 

# GpioInputSwitchLongPress
Input switch with a GPIO


Long press switch. This switch supports single press and long press. User has 500ms to perform the long press.

## Parameters of GpioInputSwitchLongPress
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
active_low |  | bool | no | Set this if your GPIO has an inverted level
debounce |  | float | no | Debounce time in seconds. Values changing faster than this are filtered out
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gpio |  | int | yes | GPIO ID on your hardware
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
visible |  | bool | no | A switch can't be visible. Always false.

## Conditions of GpioInputSwitchLongPress
Name | Description
---- | -----------
1 | Event triggered when switch is pressed quickly 
 2 | Event triggered when switch is pressed at least for 500ms (long press) 
 changed | Event on any change of state 
 

# GpioInputSwitchTriple
Input switch with a GPIO


Triple click switch. This switch can start 3 kind of actions. User has 500ms to do a multiple click.

## Parameters of GpioInputSwitchTriple
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
active_low |  | bool | no | Set this if your GPIO has an inverted level
debounce |  | float | no | Debounce time in seconds. Values changing faster than this are filtered out
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gpio |  | int | yes | GPIO ID on your hardware
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
visible |  | bool | no | A switch can't be visible. Always false.

## Conditions of GpioInputSwitchTriple
Name | Description
---- | -----------
1 | Event triggered when switch is single clicked 
 2 | Event triggered when switch is double clicked 
 3 | Event triggered when switch is triple clicked 
 changed | Event on any change of state 
 

# GpioOutputShutter
Shutter with 2 GPIOs


Simple shutter. This shutter supports open/close states, as well as impulse shutters.

## Parameters of GpioOutputShutter
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
active_low_down |  | bool | no | Set this if your GPIO has an inverted level
active_low_up |  | bool | no | Set this if your GPIO has an inverted level
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gpio_down |  | int | yes | GPIO ID for closing on your hardware
gpio_up |  | int | yes | GPIO ID for opening on your hardware
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
impulse_time |  | int | no | Impulse time for shutter that needs impulse instead of holding up/down relays. If set to 0 impulse shutter is disabled. Time is in ms. Default to 0
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
stop_both |  | bool | no | If in impulse mode, some shutters needs to activate both up dans down relays when stopping the shutter
time |  | int | yes | Time in sec for shutter to open or close
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true

## Conditions of GpioOutputShutter
Name | Description
---- | -----------
changed | Event on any change of shutter state 
 false | Event when shutter is closed 
 true | Event when shutter is open 
 
## Actions of GpioOutputShutter
Name | Description
---- | -----------
down | Close the shutter 
 impulse down 200 | Close shutter for X ms 
 impulse up 200 | Open shutter for X ms 
 set_state false | Update internal shutter state without starting real action. This is useful when having updating the shutter state from an external source. 
 set_state true | Update internal shutter state without starting real action. This is useful when having updating the shutter state from an external source. 
 stop | Stop the shutter 
 toggle | Invert shutter state 
 up | Open the shutter 
 

# GpioOutputShutterSmart
Shutter with 2 GPIOs


Smart shutter. This shutter calculates the position of the shutter based on the time it takes to open and close. It then allows to set directly the shutter at a specified position.

## Parameters of GpioOutputShutterSmart
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
active_low_down |  | bool | no | Set this if your GPIO has an inverted level
active_low_up |  | bool | no | Set this if your GPIO has an inverted level
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gpio_down |  | int | yes | GPIO ID for closing on your hardware
gpio_up |  | int | yes | GPIO ID for opening on your hardware
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
impulse_time |  | int | no | Impulse time for shutter that needs impulse instead of holding up/down relays. If set to 0 impulse shutter is disabled. Time is in ms. Default to 0
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
stop_both |  | bool | no | If in impulse mode, some shutters needs to activate both up dans down relays when stopping the shutter
time_down |  | int | yes | Time in sec for shutter to fully closed. The more accurate, the better it will work
time_up |  | int | yes | Time in sec for shutter to be fully open. The more accurate, the better it will work
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true

## Conditions of GpioOutputShutterSmart
Name | Description
---- | -----------
changed | Event on any change of shutter state 
 false | Event when shutter is closed 
 true | Event when shutter is open 
 
## Actions of GpioOutputShutterSmart
Name | Description
---- | -----------
calibrate | Start calibration on shutter. This opens fully the shutter and resets all internal position values. Use this if shutter sync is lost. 
 down | Close the shutter 
 down 5 | Close the shutter by X percent 
 impulse down 200 | Close shutter for X ms 
 impulse up 200 | Open shutter for X ms 
 set 50 | Set shutter at position X in percent 
 set_state 50 | Update internal shutter position, in percent, without starting real action. This is useful when having updating the shutter state from an external source. 
 stop | Stop the shutter 
 toggle | Invert shutter state 
 up | Open the shutter 
 up 5 | Open the shutter by X percent 
 

# GpioOutputSwitch
Light with a GPIO


Basic light. This light have only 2 states, ON or OFF. Can also be used to control simple relays output

## Parameters of GpioOutputSwitch
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
active_low |  | bool | no | Set this if your GPIO has an inverted level
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gpio |  | int | yes | GPIO ID on your hardware
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
io_style |  | list | yes | GUI style display. This will control the icon displayed on the UI
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true

## Conditions of GpioOutputSwitch
Name | Description
---- | -----------
changed | Event on any change of value 
 false | Event when light is off 
 true | Event when light is on 
 
## Actions of GpioOutputSwitch
Name | Description
---- | -----------
false | Switch the light off 
 impulse 200 | Do an impulse on light state. Set to true for X ms then reset to false 
 impulse 500 200 500 200 | Do an impulse on light state with a pattern.<br>Ex: 500 200 500 200 means: TRUE for 500ms, FALSE for 200ms, TRUE for 500ms, FALSE for 200ms<br>Ex: 500 loop 200 300 means: TRUE for 500ms, then loop the next steps for infinite, FALSE for 200ms, TRUE for 300ms<br>Ex: 100 100 200 old means: blinks and then set to the old start state (before impulse starts) 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 toggle | Invert light state 
 true | Switch the light on 
 

# HueOutputLightRGB
RGB Light dimmer using a Philips Hue


RGB light. Choose a color to be set for this light.

## Parameters of HueOutputLightRGB
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
api |  | string | yes | API key return by Hue bridge when assciation has been made. Use Hue Wizard in calaos_installer to get this value automatically.
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
host |  | string | yes | Hue bridge IP address
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
id_hue |  | string | yes | Unique ID describing the Hue Light. This value is returned by the Hue Wizard.
insecure |  | bool | no | Skip TLS certificate verification when connecting to the Hue bridge. Default to true (the bridge uses a self-signed certificate). Set to false to only allow a verified HTTPS connection.
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true

## Conditions of HueOutputLightRGB
Name | Description
---- | -----------
changed | Event on any change of value 
 value | Event when light is at this value 
 
## Actions of HueOutputLightRGB
Name | Description
---- | -----------
down_blue 5 | Decrease intensity by X percent of blue channel 
 down_green 5 | Decrease intensity by X percent of green channel 
 down_red 5 | Decrease intensity by X percent of red channel 
 false | Switch the light off 
 set #AA1294 | Set color. Color can be represented by using HTML notation: #AABBCC, rgb(50, 10, 30), hsl(11, 22, 33) 
 set_blue 50 | Set blue channel to X percent 
 set_green 50 | Set green channel to X percent 
 set_red 50 | Set red channel to X percent 
 set_state #AA1294 | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 toggle | Invert the light state (ON/OFF) 
 true | Switch the light on 
 up_blue 5 | Increase intensity by X percent of blue channel 
 up_green 5 | Increase intensity by X percent of green channel 
 up_red 5 | Increase intensity by X percent of red channel 
 
## More Infos
* Meet Hue: http://www.meethue.com


# InputTime
Basic time input. An event is triggered when the current time equals the configured time. A specific date can also be set.

## Parameters of InputTime
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
day |  | int | no | Day for this time input
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
hour |  | int | yes | Hour for this time input
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
min |  | int | yes | Minutes for this time input
month |  | int | no | Month for this time input
name | Name | string | yes | Name of Input/Output.
sec |  | int | yes | Seconds for this time input
visible |  | bool | no | A time object can't be visible. Always false.
year |  | int | no | Year for this time input

## Conditions of InputTime
Name | Description
---- | -----------
changed | Event on any change of time 
 false | Event triggered when current time is not equal 
 true | Event triggered when current time equals 
 

# InputTimer
Timer object. trigger an event after the configured time has expired.

## Parameters of InputTimer
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
autorestart |  | bool | yes | Auto restart the timer when time expires
autostart |  | bool | yes | Auto start the timer when calaos starts
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
hour |  | int | yes | Hour for the timer interval
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
min |  | int | yes | Minutes for the timer interval
msec |  | int | yes | Miliseconds for the timer interval
name | Name | string | yes | Name of Input/Output.
sec |  | int | yes | Seconds for the timer interval
visible |  | bool | no | A timer object can't be visible. Always false.

## Conditions of InputTimer
Name | Description
---- | -----------
change | Event triggered on any change 
 false | Event triggered when timer starts 
 true | Event triggered when timer expires 
 
## Actions of InputTimer
Name | Description
---- | -----------
00:00:00:200 | Reset the configured time to a value. Format is h:m:s:ms 
 start | Start the timer 
 stop | Stop the timer 
 

# InternalBool
Internal boolean object. This object is useful for doing internal programing in rules, like keeping boolean states, or displaying boolean values

## Parameters of InternalBool
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
rw |  | bool | no | Enable edit mode for this object. It allows user to modify the value on interfaces. Default to false
save |  | bool | no | Automatically save the value in cache. The value will be reloaded when restarting calaos is true. Default to false
value |  | string | no | Initial value, used when save is true and the cache holds no value yet.
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true

## Conditions of InternalBool
Name | Description
---- | -----------
changed | Event on any change of value 
 false | Event when value is false 
 true | Event when value is true 
 
## Actions of InternalBool
Name | Description
---- | -----------
false | Set a value to false 
 impulse 200 | Do an impulse on boolean value. Set to true for X ms then reset to false 
 impulse 500 200 500 200 | Do an impulse on boolean value with a pattern.<br>Ex: 500 200 500 200 means: TRUE for 500ms, FALSE for 200ms, TRUE for 500ms, FALSE for 200ms<br>Ex: 500 loop 200 300 means: TRUE for 500ms, then loop the next steps for infinite, FALSE for 200ms, TRUE for 300ms<br>Ex: 100 100 200 old means: blinks and then set to the old start state (before impulse starts) 
 toggle | Invert boolean value 
 true | Set a value to true 
 

# InternalInt
Internal number object. This object is useful for doing internal programing in rules, like counters, of displaying values.

## Parameters of InternalInt
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
rw |  | bool | no | Enable edit mode for this object. It allows user to modify the value on interfaces. Default to false
save |  | bool | no | Automatically save the value in cache. The value will be reloaded when restarting calaos is true. Default to false
step |  | float | no | Set a step for increment/decrement value. Default is 1.0
unit |  | string | no | Unit which will be displayed on the UI as a suffix.
value |  | string | no | Initial value, used when save is true and the cache holds no value yet.
value_max |  | float | no | Highest value accepted when writing. A higher value is lowered to `value_max`. The bound applies to the written value, before coeff_a/coeff_b/calc_expr, and also to the result of inc/dec. No upper bound if not set. Ignored, with an error in the log, if lower than `value_min`.
value_min |  | float | no | Lowest value accepted when writing. A lower value is raised to `value_min`. The bound applies to the written value, before coeff_a/coeff_b/calc_expr, and also to the result of inc/dec. No lower bound if not set. Ignored, with an error in the log, if greater than `value_max`.
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true

## Conditions of InternalInt
Name | Description
---- | -----------
0 | Event on a specific number value 
 changed | Event on any change of value 
 
## Actions of InternalInt
Name | Description
---- | -----------
0 | Set a specific number value 
 dec | Decrement value with configured step 
 dec 1 | Decrement value by value 
 inc | Increment value with configured step 
 inc 1 | Increment value by value 
 

# InternalString
Internal string object. This object is useful for doing internal programing in rules or displaying text values on user interfaces.

## Parameters of InternalString
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
rw |  | bool | no | Enable edit mode for this object. It allows user to modify the value on interfaces. Default to false
save |  | bool | no | Automatically save the value in cache. The value will be reloaded when restarting calaos is true. Default to false
value |  | string | no | Initial value, used when save is true and the cache holds no value yet.
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true

## Conditions of InternalString
Name | Description
---- | -----------
changed | Event on any change of value 
 value | Event on a specific string value 
 
## Actions of InternalString
Name | Description
---- | -----------
value | Set a specific string value 
 

# KNXInputAnalog
Input analog with KNX and eibnetmux


An analog input can be used to read analog values to display them and use them in rules.

## Parameters of KNXInputAnalog
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
calc_expr |  | string | no | Use a mathematical expression to calculate the value from the raw value. The variable `x` is replaced with the raw value. For example, if you want to convert a raw value of 0-1000 to a temperature in Celsius, you can use `x / 10.0 - 50.0`. If this expression is set, coeff_a, coeff_b and offset parameters are ignored.
coeff_a |  | float | no | use in conjunction of coeff_b to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 1.0.
coeff_b |  | float | no | use in conjunction of coeff_a to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 0.0
display_warning |  | bool | no | Display a warning if value has not been updated for a long time. Default to true
eis |  | int | no | KNX EIS (Data type)
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
host |  | string | yes | Hostname of knxd, default to localhost
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
interval |  | float | no | Sampling time in seconds. The value is read at this frequency. If this value is not set, the default value is 15s
io_style |  | list | yes | GUI style display. This will control the icon displayed on the UI
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
knx_group |  | string | yes | KNX Group address, Ex: x/y/z
listen_knx_group |  | string | no | KNX Group address for listening status, Ex: x/y/z
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
offset |  | float | no | same as coeff_b, can be used alone. Default value is 0.0
period |  | float | no | Sampling time in millisecond. The value is read at this frequency. If this value is not set, calaos tries to read the interval parameter
precision |  | int | no | Precision of the returned value. The value represents the number of decimal after the dot. The value is rounded like this : value = 19.275 => returned value 19.28 when preicision = 2, 19.3 when precision = 1, 19 when precision = 0
read_at_start |  | bool | yes | Send a read request at start to get the current value. Default is false
unit |  | string | no | Unit which will be displayed on the UI as a suffix.
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true

## Conditions of KNXInputAnalog
Name | Description
---- | -----------
changed | Event on any change of value 
 value | Event on a specific value 
 
## More Infos
* knxd: https://github.com/knxd/knxd/g


# KNXInputSwitch
Input switch with KNX and eibnetmux


Basic switch with press/release states.

## Parameters of KNXInputSwitch
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
eis |  | int | no | KNX EIS (Data type)
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
host |  | string | yes | Hostname of knxd, default to localhost
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
io_style |  | list | yes | GUI style display. This will control the icon displayed on the UI
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
knx_group |  | string | yes | KNX Group address, Ex: x/y/z
listen_knx_group |  | string | no | KNX Group address for listening status, Ex: x/y/z
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
read_at_start |  | bool | yes | Send a read request at start to get the current value. Default is false
visible |  | bool | no | Display the Input/Output on all user interfaces if set. Default to false for switches

## Conditions of KNXInputSwitch
Name | Description
---- | -----------
changed | Event on any change of state 
 false | Event triggered when switch is released 
 true | Event triggered when switch is pressed 
 
## More Infos
* knxd: https://github.com/knxd/knxd/g


# KNXInputSwitchLongPress
Input switch long press with KNX and eibnetmux


Long press switch. This switch supports single press and long press. User has 500ms to perform the long press.

## Parameters of KNXInputSwitchLongPress
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
eis |  | int | no | KNX EIS (Data type)
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
host |  | string | yes | Hostname of knxd, default to localhost
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
knx_group |  | string | yes | KNX Group address, Ex: x/y/z
listen_knx_group |  | string | no | KNX Group address for listening status, Ex: x/y/z
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
read_at_start |  | bool | yes | Send a read request at start to get the current value. Default is false
visible |  | bool | no | A switch can't be visible. Always false.

## Conditions of KNXInputSwitchLongPress
Name | Description
---- | -----------
1 | Event triggered when switch is pressed quickly 
 2 | Event triggered when switch is pressed at least for 500ms (long press) 
 changed | Event on any change of state 
 
## More Infos
* knxd: https://github.com/knxd/knxd/g


# KNXInputSwitchTriple
Input switch triple with KNX and eibnetmux


Triple click switch. This switch can start 3 kind of actions. User has 500ms to do a multiple click.

## Parameters of KNXInputSwitchTriple
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
eis |  | int | no | KNX EIS (Data type)
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
host |  | string | yes | Hostname of knxd, default to localhost
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
knx_group |  | string | yes | KNX Group address, Ex: x/y/z
listen_knx_group |  | string | no | KNX Group address for listening status, Ex: x/y/z
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
read_at_start |  | bool | yes | Send a read request at start to get the current value. Default is false
visible |  | bool | no | A switch can't be visible. Always false.

## Conditions of KNXInputSwitchTriple
Name | Description
---- | -----------
1 | Event triggered when switch is single clicked 
 2 | Event triggered when switch is double clicked 
 3 | Event triggered when switch is triple clicked 
 changed | Event on any change of state 
 
## More Infos
* knxd: https://github.com/knxd/knxd/g


# KNXInputTemp
Input temperature with KNX and eibnetmux


Temperature sensor input. Used for displaying temperature and to control heating devices with rules based on temperature value

## Parameters of KNXInputTemp
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
calc_expr |  | string | no | Use a mathematical expression to calculate the value from the raw value. The variable `x` is replaced with the raw value. For example, if you want to convert a raw value of 0-1000 to a temperature in Celsius, you can use `x / 10.0 - 50.0`. If this expression is set, coeff_a, coeff_b and offset parameters are ignored.
coeff_a |  | float | no | use in conjunction of coeff_b to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 1.0.
coeff_b |  | float | no | use in conjunction of coeff_a to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 0.0
display_warning |  | bool | no | Display a warning if value has not been updated for a long time. Default to true
eis |  | int | no | KNX EIS (Data type)
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
host |  | string | yes | Hostname of knxd, default to localhost
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
interval |  | float | no | Sampling time in seconds. The value is read at this frequency. If this value is not set, the default value is 15s
io_style |  | list | yes | GUI style display. This will control the icon displayed on the UI
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
knx_group |  | string | yes | KNX Group address, Ex: x/y/z
listen_knx_group |  | string | no | KNX Group address for listening status, Ex: x/y/z
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
offset |  | float | no | same as coeff_b, can be used alone. Default value is 0.0
period |  | float | no | Sampling time in millisecond. The value is read at this frequency. If this value is not set, calaos tries to read the interval parameter
precision |  | int | no | Precision of the returned value. The value represents the number of decimal after the dot. The value is rounded like this : value = 19.275 => returned value 19.28 when preicision = 2, 19.3 when precision = 1, 19 when precision = 0
read_at_start |  | bool | yes | Send a read request at start to get the current value. Default is false
unit |  | string | no | Unit which will be displayed on the UI as a suffix.
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true

## Conditions of KNXInputTemp
Name | Description
---- | -----------
changed | Event on any change of value 
 value | Event on a specific value 
 
## More Infos
* knxd: https://github.com/knxd/knxd/g


# KNXOutputAnalog
Analog output with KNX and eibnetmux


Analog output. Useful to control analog output devices connected to calaos.

## Parameters of KNXOutputAnalog
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
calc_expr |  | string | no | Use a mathematical expression to calculate the value from the raw value. The variable `x` is replaced with the raw value. For example, if you want to convert a raw value of 0-1000 to a temperature in Celsius, you can use `x / 10.0 - 50.0`. If this expression is set, coeff_a, coeff_b and offset parameters are ignored.
coeff_a |  | float | no | use in conjunction of coeff_b to apply equation of the form `value_sent = coeff_a * raw_value + coeff_b`. Default value is 1.0.
coeff_b |  | float | no | use in conjunction of coeff_a to apply equation of the form `value_sent = coeff_a * raw_value + coeff_b`. Default value is 0.0
eis |  | int | no | KNX EIS (Data type)
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
host |  | string | yes | Hostname of knxd, default to localhost
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
io_style |  | list | yes | GUI style display. This will control the icon displayed on the UI
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
knx_group |  | string | yes | KNX Group address, Ex: x/y/z
listen_knx_group |  | string | no | KNX Group address for listening status, Ex: x/y/z
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
read_at_start |  | bool | yes | Send a read request at start to get the current value. Default is false
step |  | float | no | Set a step for increment/decrement value. Default is 1.0
unit |  | string | no | Unit which will be displayed on the UI as a suffix.
value_max |  | float | no | Highest value accepted when writing. A higher value is lowered to `value_max`. The bound applies to the written value, before coeff_a/coeff_b/calc_expr, and also to the result of inc/dec. No upper bound if not set. Ignored, with an error in the log, if lower than `value_min`.
value_min |  | float | no | Lowest value accepted when writing. A lower value is raised to `value_min`. The bound applies to the written value, before coeff_a/coeff_b/calc_expr, and also to the result of inc/dec. No lower bound if not set. Ignored, with an error in the log, if greater than `value_max`.
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true

## Conditions of KNXOutputAnalog
Name | Description
---- | -----------
0 | Event on a specific number value 
 changed | Event on any change of value 
 value | Event on a specific value 
 
## Actions of KNXOutputAnalog
Name | Description
---- | -----------
0 | Set a specific number value 
 dec | Decrement value with configured step 
 dec 1 | Decrement value by value 
 inc | Increment value with configured step 
 inc 1 | Increment value by value 
 
## More Infos
* knxd: https://github.com/knxd/knxd/g


# KNXOutputLight
Light output with KNX and eibnetmux


Basic light. This light have only 2 states, ON or OFF. Can also be used to control simple relays output

## Parameters of KNXOutputLight
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
eis |  | int | no | KNX EIS (Data type)
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
host |  | string | yes | Hostname of knxd, default to localhost
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
io_style |  | list | yes | GUI style display. This will control the icon displayed on the UI
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
knx_group |  | string | yes | KNX Group address, Ex: x/y/z
listen_knx_group |  | string | no | KNX Group address for listening status, Ex: x/y/z
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
read_at_start |  | bool | yes | Send a read request at start to get the current value. Default is false
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true

## Conditions of KNXOutputLight
Name | Description
---- | -----------
changed | Event on any change of value 
 false | Event when light is off 
 true | Event when light is on 
 
## Actions of KNXOutputLight
Name | Description
---- | -----------
false | Switch the light off 
 impulse 200 | Do an impulse on light state. Set to true for X ms then reset to false 
 impulse 500 200 500 200 | Do an impulse on light state with a pattern.<br>Ex: 500 200 500 200 means: TRUE for 500ms, FALSE for 200ms, TRUE for 500ms, FALSE for 200ms<br>Ex: 500 loop 200 300 means: TRUE for 500ms, then loop the next steps for infinite, FALSE for 200ms, TRUE for 300ms<br>Ex: 100 100 200 old means: blinks and then set to the old start state (before impulse starts) 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 toggle | Invert light state 
 true | Switch the light on 
 
## More Infos
* knxd: https://github.com/knxd/knxd/g


# KNXOutputLightDimmer
Light dimmer with KNX and eibnetmux


Light with dimming control. Light intensity can be changed for this light.

## Parameters of KNXOutputLightDimmer
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
eis |  | int | no | KNX EIS (Data type)
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
host |  | string | yes | Hostname of knxd, default to localhost
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
knx_group |  | string | yes | KNX Group address, Ex: x/y/z
listen_knx_group |  | string | no | KNX Group address for listening status, Ex: x/y/z
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
read_at_start |  | bool | yes | Send a read request at start to get the current value. Default is false
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true

## Conditions of KNXOutputLightDimmer
Name | Description
---- | -----------
changed | Event on any change of value 
 value | Event when light is at this value 
 
## Actions of KNXOutputLightDimmer
Name | Description
---- | -----------
down 5 | Decrease intensity by X percent 
 false | Switch the light off 
 hold press | Dynamically change light intensity when holding a switch (press action) 
 hold stop | Dynamically change light intensity when holding a switch (stop action) 
 impulse 200 | Do an impulse on light state. Set to true for X ms then reset to false 
 impulse 500 200 500 200 | Do an impulse on light state with a pattern.<br>Ex: 500 200 500 200 means: TRUE for 500ms, FALSE for 200ms, TRUE for 500ms, FALSE for 200ms<br>Ex: 500 loop 200 300 means: TRUE for 500ms, then loop the next steps for infinite, FALSE for 200ms, TRUE for 300ms<br>Ex: 100 100 200 old means: blinks and then set to the old start state (before impulse starts) 
 set 50 | Set light intensity and swith on if light is off 
 set off 50 | Set light value without switching on. This will be the light intensity for the next ON 
 set_state 50 | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 toggle | Invert the light state 
 true | Switch the light on 
 up 5 | Increase intensity by X percent 
 
## More Infos
* knxd: https://github.com/knxd/knxd/g


# KNXOutputLightRGB
Light RGB with KNX and eibnetmux


RGB light. Choose a color to be set for this light.

## Parameters of KNXOutputLightRGB
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
eis |  | int | no | KNX EIS (Data type)
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
host |  | string | yes | Hostname of knxd, default to localhost
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
knx_group_blue |  | string | yes | Blue channel KNX Group address, Ex: x/y/z
knx_group_green |  | string | yes | Green channel KNX Group address, Ex: x/y/z
knx_group_red |  | string | yes | Red channel KNX Group address, Ex: x/y/z
listen_knx_group_blue |  | string | no | Blue Group address for listening status, Ex: x/y/z
listen_knx_group_green |  | string | no | Green Group address for listening status, Ex: x/y/z
listen_knx_group_red |  | string | no | Red Group address for listening status, Ex: x/y/z
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
read_at_start |  | bool | yes | Send a read request at start to get the current value. Default is false
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true

## Conditions of KNXOutputLightRGB
Name | Description
---- | -----------
changed | Event on any change of value 
 value | Event when light is at this value 
 
## Actions of KNXOutputLightRGB
Name | Description
---- | -----------
down_blue 5 | Decrease intensity by X percent of blue channel 
 down_green 5 | Decrease intensity by X percent of green channel 
 down_red 5 | Decrease intensity by X percent of red channel 
 false | Switch the light off 
 set #AA1294 | Set color. Color can be represented by using HTML notation: #AABBCC, rgb(50, 10, 30), hsl(11, 22, 33) 
 set_blue 50 | Set blue channel to X percent 
 set_green 50 | Set green channel to X percent 
 set_red 50 | Set red channel to X percent 
 set_state #AA1294 | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 toggle | Invert the light state (ON/OFF) 
 true | Switch the light on 
 up_blue 5 | Increase intensity by X percent of blue channel 
 up_green 5 | Increase intensity by X percent of green channel 
 up_red 5 | Increase intensity by X percent of red channel 
 
## More Infos
* knxd: https://github.com/knxd/knxd/g


# KNXOutputShutter
Shutter with with KNX and eibnetmux


Simple shutter. This shutter supports open/close states, as well as impulse shutters.

## Parameters of KNXOutputShutter
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
eis |  | int | no | KNX EIS (Data type)
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
host |  | string | yes | Hostname of knxd, default to localhost
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
impulse_time |  | int | no | Impulse time for shutter that needs impulse instead of holding up/down relays. If set to 0 impulse shutter is disabled. Time is in ms. Default to 0
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
knx_group_down |  | string | yes | Down KNX Group address, Ex: x/y/z
knx_group_up |  | string | yes | Up KNX Group address, Ex: x/y/z
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
read_at_start |  | bool | yes | Send a read request at start to get the current value. Default is false
stop_both |  | bool | no | If in impulse mode, some shutters needs to activate both up dans down relays when stopping the shutter
time |  | int | yes | Time in sec for shutter to open or close
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true

## Conditions of KNXOutputShutter
Name | Description
---- | -----------
changed | Event on any change of shutter state 
 false | Event when shutter is closed 
 true | Event when shutter is open 
 
## Actions of KNXOutputShutter
Name | Description
---- | -----------
down | Close the shutter 
 impulse down 200 | Close shutter for X ms 
 impulse up 200 | Open shutter for X ms 
 set_state false | Update internal shutter state without starting real action. This is useful when having updating the shutter state from an external source. 
 set_state true | Update internal shutter state without starting real action. This is useful when having updating the shutter state from an external source. 
 stop | Stop the shutter 
 toggle | Invert shutter state 
 up | Open the shutter 
 
## More Infos
* knxd: https://github.com/knxd/knxd/g


# KNXOutputShutterSmart
Shutter with with KNX and eibnetmux


Smart shutter. This shutter calculates the position of the shutter based on the time it takes to open and close. It then allows to set directly the shutter at a specified position.

## Parameters of KNXOutputShutterSmart
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
eis |  | int | no | KNX EIS (Data type)
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
host |  | string | yes | Hostname of knxd, default to localhost
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
impulse_time |  | int | no | Impulse time for shutter that needs impulse instead of holding up/down relays. If set to 0 impulse shutter is disabled. Time is in ms. Default to 0
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
knx_group |  | string | yes | KNX Group address, Ex: x/y/z
knx_group_down |  | string | yes | Down KNX Group address, Ex: x/y/z
knx_group_up |  | string | yes | Up KNX Group address, Ex: x/y/z
listen_knx_group |  | string | no | KNX Group address for listening status, Ex: x/y/z
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
read_at_start |  | bool | yes | Send a read request at start to get the current value. Default is false
stop_both |  | bool | no | If in impulse mode, some shutters needs to activate both up dans down relays when stopping the shutter
time_down |  | int | yes | Time in sec for shutter to fully closed. The more accurate, the better it will work
time_up |  | int | yes | Time in sec for shutter to be fully open. The more accurate, the better it will work
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true

## Conditions of KNXOutputShutterSmart
Name | Description
---- | -----------
changed | Event on any change of shutter state 
 false | Event when shutter is closed 
 true | Event when shutter is open 
 
## Actions of KNXOutputShutterSmart
Name | Description
---- | -----------
calibrate | Start calibration on shutter. This opens fully the shutter and resets all internal position values. Use this if shutter sync is lost. 
 down | Close the shutter 
 down 5 | Close the shutter by X percent 
 impulse down 200 | Close shutter for X ms 
 impulse up 200 | Open shutter for X ms 
 set 50 | Set shutter at position X in percent 
 set_state 50 | Update internal shutter position, in percent, without starting real action. This is useful when having updating the shutter state from an external source. 
 stop | Stop the shutter 
 toggle | Invert shutter state 
 up | Open the shutter 
 up 5 | Open the shutter by X percent 
 
## More Infos
* knxd: https://github.com/knxd/knxd/g


# MqttInputAnalog
Analog value read from a mqtt broker


An analog input can be used to read analog values to display them and use them in rules.

## Parameters of MqttInputAnalog
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
battery_expr |  | string | no | If the battery value is not directly available in the payload, you can use this parameter to calculate the battery value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the battery value will be read directly from the path. Example: "x * 100 / 255"
battery_path |  | string | no | The path where to find the battery status in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json object, just try to use the key of the value you want to read, for example : {"battery":90} use "battery" as path. When this path is set, and the level drops below 30%. The battery reported should be in percent. Use `battery_expr` to adjust if required.
battery_topic |  | string | no | The topic on witch to publish the battery status of the sensor. If not set, no battery status will be reported.
calc_expr |  | string | no | Use a mathematical expression to calculate the value from the raw value. The variable `x` is replaced with the raw value. For example, if you want to convert a raw value of 0-1000 to a temperature in Celsius, you can use `x / 10.0 - 50.0`. If this expression is set, coeff_a, coeff_b and offset parameters are ignored.
coeff_a |  | float | no | use in conjunction of coeff_b to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 1.0.
coeff_b |  | float | no | use in conjunction of coeff_a to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 0.0
connected_status_expr |  | string | no | If the connected status value is not directly available in the payload, you can use this parameter to convert the value from the path to a boolean. The value will be calculated as any valid mathematic expression. In the expression, the variable `value` is replaced with the raw value from path. If not set, the connected status will be read directly from the path. Example: "value == 'connected'" or "value > 30 and value < 150"
connected_status_path |  | string | no | The path where to find the connected status in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"connected":true} use "connected" as path. The value should be a boolean. Use connected_status_expr to convert to a boolean
connected_status_topic |  | string | no | The topic on witch to publish the connected status of the sensor. If not set, no connected status will be reported.
display_warning |  | bool | no | Display a warning if value has not been updated for a long time. Default to true
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
host |  | string | no | IP address of the mqtt broker to connect to. Default value is 127.0.0.1.
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
interval |  | float | no | Sampling time in seconds. The value is read at this frequency. If this value is not set, the default value is 15s
io_style |  | list | yes | GUI style display. This will control the icon displayed on the UI
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
ip_address_path |  | string | no | The path where to find the IP address of the sensor in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"ip_address": "192.168.1.156"} use "ip_address" as path. The value should be a string with the IP address.
ip_address_topic |  | string | no | The topic on witch to publish the IP address of the sensor. If not set, no IP address will be reported.
keepalive |  | int | no | keepalive timeout in seconds. Time between two mqtt PING.
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
notif_battery |  | bool | no | If set, a notification will be sent when the battery level drops below 30%. This is only used if the battery_topic is set.
notif_connected |  | bool | no | If set, a notification will be sent when the connected status changes. This is only used if the connected_status_topic is set.
offset |  | float | no | same as coeff_b, can be used alone. Default value is 0.0
password |  | string | no | Password to use for authentication with mqtt broker. User must be defined in that case.
path |  | string | yes | The path where to find the value in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. if payload is simple json, just try to use the key of the value you want to read, for example : {"temperature":14.23} use "temperature" as path
period |  | float | no | Sampling time in millisecond. The value is read at this frequency. If this value is not set, calaos tries to read the interval parameter
port |  | int | no | TCP port of the mqtt broker. Default value is 1883
precision |  | int | no | Precision of the returned value. The value represents the number of decimal after the dot. The value is rounded like this : value = 19.275 => returned value 19.28 when preicision = 2, 19.3 when precision = 1, 19 when precision = 0
topic_pub |  | string | yes | Topic on witch to publish.
topic_sub |  | string | yes | Topic on witch to subscribe.
unit |  | string | no | Unit which will be displayed on the UI as a suffix.
uptime_expr |  | string | no | If the uptime value is not directly available in the payload, you can use this parameter to calculate the uptime value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the uptime value will be read directly from the path. Example: "x * 100 / 255"
uptime_path |  | string | no | The path where to find the uptime of the sensor in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"uptime": 3600} use "uptime" as path. The value should be a number in seconds. Use `uptime_expr` to adjust if required.
uptime_topic |  | string | no | The topic on witch to publish the uptime of the sensor. If not set, no uptime will be reported.
user |  | string | no | User to use for authentication with mqtt broker. Password must be defined in that case.
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
wifi_ssid_path |  | string | no | The path where to find the WiFi SSID where the sensor is connected in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"wifi_ssid": "MyWifi"} use "wifi_ssid" as path. The value should be a string with the WiFi SSID.
wifi_ssid_topic |  | string | no | The topic on witch to publish the WiFi SSID of the sensor. If not set, no WiFi SSID will be reported.
wireless_signal_expr |  | string | no | If the wireless signal value is not directly available in the payload, you can use this parameter to calculate the wireless signal value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the wireless signal value will be read directly from the path. Example: "x * 100 / 255"
wireless_signal_path |  | string | no | The path where to find the wireless signal strength in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"signal": 70} use "signal" as path. The value should be a number in percent. Use `wireless_signal_expr` to adjust if required.
wireless_signal_topic |  | string | no | The topic on witch to publish the wireless signal strength of the sensor. If not set, no wireless signal strength will be reported.

## Conditions of MqttInputAnalog
Name | Description
---- | -----------
changed | Event on any change of value 
 value | Event on a specific value 
 

# MqttInputString
String value read from a mqtt broker

## Parameters of MqttInputString
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
battery_expr |  | string | no | If the battery value is not directly available in the payload, you can use this parameter to calculate the battery value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the battery value will be read directly from the path. Example: "x * 100 / 255"
battery_path |  | string | no | The path where to find the battery status in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json object, just try to use the key of the value you want to read, for example : {"battery":90} use "battery" as path. When this path is set, and the level drops below 30%. The battery reported should be in percent. Use `battery_expr` to adjust if required.
battery_topic |  | string | no | The topic on witch to publish the battery status of the sensor. If not set, no battery status will be reported.
connected_status_expr |  | string | no | If the connected status value is not directly available in the payload, you can use this parameter to convert the value from the path to a boolean. The value will be calculated as any valid mathematic expression. In the expression, the variable `value` is replaced with the raw value from path. If not set, the connected status will be read directly from the path. Example: "value == 'connected'" or "value > 30 and value < 150"
connected_status_path |  | string | no | The path where to find the connected status in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"connected":true} use "connected" as path. The value should be a boolean. Use connected_status_expr to convert to a boolean
connected_status_topic |  | string | no | The topic on witch to publish the connected status of the sensor. If not set, no connected status will be reported.
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
host |  | string | no | IP address of the mqtt broker to connect to. Default value is 127.0.0.1.
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
ip_address_path |  | string | no | The path where to find the IP address of the sensor in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"ip_address": "192.168.1.156"} use "ip_address" as path. The value should be a string with the IP address.
ip_address_topic |  | string | no | The topic on witch to publish the IP address of the sensor. If not set, no IP address will be reported.
keepalive |  | int | no | keepalive timeout in seconds. Time between two mqtt PING.
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
notif_battery |  | bool | no | If set, a notification will be sent when the battery level drops below 30%. This is only used if the battery_topic is set.
notif_connected |  | bool | no | If set, a notification will be sent when the connected status changes. This is only used if the connected_status_topic is set.
password |  | string | no | Password to use for authentication with mqtt broker. User must be defined in that case.
path |  | string | yes | The path where to find the value in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. if payload is simple json, just try to use the key of the value you want to read, for example : {"temperature":14.23} use "temperature" as path
port |  | int | no | TCP port of the mqtt broker. Default value is 1883
topic_pub |  | string | yes | Topic on witch to publish.
topic_sub |  | string | yes | Topic on witch to subscribe.
uptime_expr |  | string | no | If the uptime value is not directly available in the payload, you can use this parameter to calculate the uptime value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the uptime value will be read directly from the path. Example: "x * 100 / 255"
uptime_path |  | string | no | The path where to find the uptime of the sensor in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"uptime": 3600} use "uptime" as path. The value should be a number in seconds. Use `uptime_expr` to adjust if required.
uptime_topic |  | string | no | The topic on witch to publish the uptime of the sensor. If not set, no uptime will be reported.
user |  | string | no | User to use for authentication with mqtt broker. Password must be defined in that case.
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
wifi_ssid_path |  | string | no | The path where to find the WiFi SSID where the sensor is connected in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"wifi_ssid": "MyWifi"} use "wifi_ssid" as path. The value should be a string with the WiFi SSID.
wifi_ssid_topic |  | string | no | The topic on witch to publish the WiFi SSID of the sensor. If not set, no WiFi SSID will be reported.
wireless_signal_expr |  | string | no | If the wireless signal value is not directly available in the payload, you can use this parameter to calculate the wireless signal value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the wireless signal value will be read directly from the path. Example: "x * 100 / 255"
wireless_signal_path |  | string | no | The path where to find the wireless signal strength in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"signal": 70} use "signal" as path. The value should be a number in percent. Use `wireless_signal_expr` to adjust if required.
wireless_signal_topic |  | string | no | The topic on witch to publish the wireless signal strength of the sensor. If not set, no wireless signal strength will be reported.


# MqttInputSwitch
Switch value read from a mqtt broker


Basic switch with press/release states.

## Parameters of MqttInputSwitch
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
battery_expr |  | string | no | If the battery value is not directly available in the payload, you can use this parameter to calculate the battery value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the battery value will be read directly from the path. Example: "x * 100 / 255"
battery_path |  | string | no | The path where to find the battery status in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json object, just try to use the key of the value you want to read, for example : {"battery":90} use "battery" as path. When this path is set, and the level drops below 30%. The battery reported should be in percent. Use `battery_expr` to adjust if required.
battery_topic |  | string | no | The topic on witch to publish the battery status of the sensor. If not set, no battery status will be reported.
connected_status_expr |  | string | no | If the connected status value is not directly available in the payload, you can use this parameter to convert the value from the path to a boolean. The value will be calculated as any valid mathematic expression. In the expression, the variable `value` is replaced with the raw value from path. If not set, the connected status will be read directly from the path. Example: "value == 'connected'" or "value > 30 and value < 150"
connected_status_path |  | string | no | The path where to find the connected status in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"connected":true} use "connected" as path. The value should be a boolean. Use connected_status_expr to convert to a boolean
connected_status_topic |  | string | no | The topic on witch to publish the connected status of the sensor. If not set, no connected status will be reported.
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
host |  | string | no | IP address of the mqtt broker to connect to. Default value is 127.0.0.1.
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
io_style |  | list | yes | GUI style display. This will control the icon displayed on the UI
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
ip_address_path |  | string | no | The path where to find the IP address of the sensor in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"ip_address": "192.168.1.156"} use "ip_address" as path. The value should be a string with the IP address.
ip_address_topic |  | string | no | The topic on witch to publish the IP address of the sensor. If not set, no IP address will be reported.
keepalive |  | int | no | keepalive timeout in seconds. Time between two mqtt PING.
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
notif_battery |  | bool | no | If set, a notification will be sent when the battery level drops below 30%. This is only used if the battery_topic is set.
notif_connected |  | bool | no | If set, a notification will be sent when the connected status changes. This is only used if the connected_status_topic is set.
off_value |  | string | yes | Value to interpret as OFF value
on_value |  | string | yes | Value to interpret as ON value
password |  | string | no | Password to use for authentication with mqtt broker. User must be defined in that case.
path |  | string | yes | The path where to find the value in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. if payload is simple json, just try to use the key of the value you want to read, for example : {"temperature":14.23} use "temperature" as path
port |  | int | no | TCP port of the mqtt broker. Default value is 1883
topic_pub |  | string | yes | Topic on witch to publish.
topic_sub |  | string | yes | Topic on witch to subscribe.
uptime_expr |  | string | no | If the uptime value is not directly available in the payload, you can use this parameter to calculate the uptime value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the uptime value will be read directly from the path. Example: "x * 100 / 255"
uptime_path |  | string | no | The path where to find the uptime of the sensor in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"uptime": 3600} use "uptime" as path. The value should be a number in seconds. Use `uptime_expr` to adjust if required.
uptime_topic |  | string | no | The topic on witch to publish the uptime of the sensor. If not set, no uptime will be reported.
user |  | string | no | User to use for authentication with mqtt broker. Password must be defined in that case.
visible |  | bool | no | Display the Input/Output on all user interfaces if set. Default to false for switches
wifi_ssid_path |  | string | no | The path where to find the WiFi SSID where the sensor is connected in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"wifi_ssid": "MyWifi"} use "wifi_ssid" as path. The value should be a string with the WiFi SSID.
wifi_ssid_topic |  | string | no | The topic on witch to publish the WiFi SSID of the sensor. If not set, no WiFi SSID will be reported.
wireless_signal_expr |  | string | no | If the wireless signal value is not directly available in the payload, you can use this parameter to calculate the wireless signal value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the wireless signal value will be read directly from the path. Example: "x * 100 / 255"
wireless_signal_path |  | string | no | The path where to find the wireless signal strength in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"signal": 70} use "signal" as path. The value should be a number in percent. Use `wireless_signal_expr` to adjust if required.
wireless_signal_topic |  | string | no | The topic on witch to publish the wireless signal strength of the sensor. If not set, no wireless signal strength will be reported.

## Conditions of MqttInputSwitch
Name | Description
---- | -----------
changed | Event on any change of state 
 false | Event triggered when switch is released 
 true | Event triggered when switch is pressed 
 

# MqttInputTemp
Temperature read from a mqtt broker


Temperature sensor input. Used for displaying temperature and to control heating devices with rules based on temperature value

## Parameters of MqttInputTemp
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
battery_expr |  | string | no | If the battery value is not directly available in the payload, you can use this parameter to calculate the battery value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the battery value will be read directly from the path. Example: "x * 100 / 255"
battery_path |  | string | no | The path where to find the battery status in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json object, just try to use the key of the value you want to read, for example : {"battery":90} use "battery" as path. When this path is set, and the level drops below 30%. The battery reported should be in percent. Use `battery_expr` to adjust if required.
battery_topic |  | string | no | The topic on witch to publish the battery status of the sensor. If not set, no battery status will be reported.
calc_expr |  | string | no | Use a mathematical expression to calculate the value from the raw value. The variable `x` is replaced with the raw value. For example, if you want to convert a raw value of 0-1000 to a temperature in Celsius, you can use `x / 10.0 - 50.0`. If this expression is set, coeff_a, coeff_b and offset parameters are ignored.
coeff_a |  | float | no | use in conjunction of coeff_b to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 1.0.
coeff_b |  | float | no | use in conjunction of coeff_a to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 0.0
connected_status_expr |  | string | no | If the connected status value is not directly available in the payload, you can use this parameter to convert the value from the path to a boolean. The value will be calculated as any valid mathematic expression. In the expression, the variable `value` is replaced with the raw value from path. If not set, the connected status will be read directly from the path. Example: "value == 'connected'" or "value > 30 and value < 150"
connected_status_path |  | string | no | The path where to find the connected status in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"connected":true} use "connected" as path. The value should be a boolean. Use connected_status_expr to convert to a boolean
connected_status_topic |  | string | no | The topic on witch to publish the connected status of the sensor. If not set, no connected status will be reported.
display_warning |  | bool | no | Display a warning if value has not been updated for a long time. Default to true
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
host |  | string | no | IP address of the mqtt broker to connect to. Default value is 127.0.0.1.
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
interval |  | float | no | Sampling time in seconds. The value is read at this frequency. If this value is not set, the default value is 15s
io_style |  | list | yes | GUI style display. This will control the icon displayed on the UI
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
ip_address_path |  | string | no | The path where to find the IP address of the sensor in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"ip_address": "192.168.1.156"} use "ip_address" as path. The value should be a string with the IP address.
ip_address_topic |  | string | no | The topic on witch to publish the IP address of the sensor. If not set, no IP address will be reported.
keepalive |  | int | no | keepalive timeout in seconds. Time between two mqtt PING.
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
notif_battery |  | bool | no | If set, a notification will be sent when the battery level drops below 30%. This is only used if the battery_topic is set.
notif_connected |  | bool | no | If set, a notification will be sent when the connected status changes. This is only used if the connected_status_topic is set.
offset |  | float | no | same as coeff_b, can be used alone. Default value is 0.0
password |  | string | no | Password to use for authentication with mqtt broker. User must be defined in that case.
path |  | string | yes | The path where to find the value in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. if payload is simple json, just try to use the key of the value you want to read, for example : {"temperature":14.23} use "temperature" as path
period |  | float | no | Sampling time in millisecond. The value is read at this frequency. If this value is not set, calaos tries to read the interval parameter
port |  | int | no | TCP port of the mqtt broker. Default value is 1883
precision |  | int | no | Precision of the returned value. The value represents the number of decimal after the dot. The value is rounded like this : value = 19.275 => returned value 19.28 when preicision = 2, 19.3 when precision = 1, 19 when precision = 0
topic_pub |  | string | yes | Topic on witch to publish.
topic_sub |  | string | yes | Topic on witch to subscribe.
unit |  | string | no | Unit which will be displayed on the UI as a suffix.
uptime_expr |  | string | no | If the uptime value is not directly available in the payload, you can use this parameter to calculate the uptime value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the uptime value will be read directly from the path. Example: "x * 100 / 255"
uptime_path |  | string | no | The path where to find the uptime of the sensor in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"uptime": 3600} use "uptime" as path. The value should be a number in seconds. Use `uptime_expr` to adjust if required.
uptime_topic |  | string | no | The topic on witch to publish the uptime of the sensor. If not set, no uptime will be reported.
user |  | string | no | User to use for authentication with mqtt broker. Password must be defined in that case.
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
wifi_ssid_path |  | string | no | The path where to find the WiFi SSID where the sensor is connected in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"wifi_ssid": "MyWifi"} use "wifi_ssid" as path. The value should be a string with the WiFi SSID.
wifi_ssid_topic |  | string | no | The topic on witch to publish the WiFi SSID of the sensor. If not set, no WiFi SSID will be reported.
wireless_signal_expr |  | string | no | If the wireless signal value is not directly available in the payload, you can use this parameter to calculate the wireless signal value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the wireless signal value will be read directly from the path. Example: "x * 100 / 255"
wireless_signal_path |  | string | no | The path where to find the wireless signal strength in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"signal": 70} use "signal" as path. The value should be a number in percent. Use `wireless_signal_expr` to adjust if required.
wireless_signal_topic |  | string | no | The topic on witch to publish the wireless signal strength of the sensor. If not set, no wireless signal strength will be reported.

## Conditions of MqttInputTemp
Name | Description
---- | -----------
changed | Event on any change of value 
 value | Event on a specific value 
 

# MqttOutputAnalog
Control analog output through mqtt broker


Analog output. Useful to control analog output devices connected to calaos.

## Parameters of MqttOutputAnalog
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
battery_expr |  | string | no | If the battery value is not directly available in the payload, you can use this parameter to calculate the battery value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the battery value will be read directly from the path. Example: "x * 100 / 255"
battery_path |  | string | no | The path where to find the battery status in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json object, just try to use the key of the value you want to read, for example : {"battery":90} use "battery" as path. When this path is set, and the level drops below 30%. The battery reported should be in percent. Use `battery_expr` to adjust if required.
battery_topic |  | string | no | The topic on witch to publish the battery status of the sensor. If not set, no battery status will be reported.
calc_expr |  | string | no | Use a mathematical expression to calculate the value from the raw value. The variable `x` is replaced with the raw value. For example, if you want to convert a raw value of 0-1000 to a temperature in Celsius, you can use `x / 10.0 - 50.0`. If this expression is set, coeff_a, coeff_b and offset parameters are ignored.
coeff_a |  | float | no | use in conjunction of coeff_b to apply equation of the form `value_sent = coeff_a * raw_value + coeff_b`. Default value is 1.0.
coeff_b |  | float | no | use in conjunction of coeff_a to apply equation of the form `value_sent = coeff_a * raw_value + coeff_b`. Default value is 0.0
connected_status_expr |  | string | no | If the connected status value is not directly available in the payload, you can use this parameter to convert the value from the path to a boolean. The value will be calculated as any valid mathematic expression. In the expression, the variable `value` is replaced with the raw value from path. If not set, the connected status will be read directly from the path. Example: "value == 'connected'" or "value > 30 and value < 150"
connected_status_path |  | string | no | The path where to find the connected status in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"connected":true} use "connected" as path. The value should be a boolean. Use connected_status_expr to convert to a boolean
connected_status_topic |  | string | no | The topic on witch to publish the connected status of the sensor. If not set, no connected status will be reported.
data |  | string | yes | The data sent when publishing to topic. The __##VALUE##__ contained in data is substituted with the state (float value) to be sent.
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
host |  | string | no | IP address of the mqtt broker to connect to. Default value is 127.0.0.1.
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
io_style |  | list | yes | GUI style display. This will control the icon displayed on the UI
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
ip_address_path |  | string | no | The path where to find the IP address of the sensor in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"ip_address": "192.168.1.156"} use "ip_address" as path. The value should be a string with the IP address.
ip_address_topic |  | string | no | The topic on witch to publish the IP address of the sensor. If not set, no IP address will be reported.
keepalive |  | int | no | keepalive timeout in seconds. Time between two mqtt PING.
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
notif_battery |  | bool | no | If set, a notification will be sent when the battery level drops below 30%. This is only used if the battery_topic is set.
notif_connected |  | bool | no | If set, a notification will be sent when the connected status changes. This is only used if the connected_status_topic is set.
password |  | string | no | Password to use for authentication with mqtt broker. User must be defined in that case.
path |  | string | yes | The path where to find the value in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. if payload is simple json, just try to use the key of the value you want to read, for example : {"temperature":14.23} use "temperature" as path
port |  | int | no | TCP port of the mqtt broker. Default value is 1883
step |  | float | no | Set a step for increment/decrement value. Default is 1.0
topic_pub |  | string | yes | Topic on witch to publish.
topic_sub |  | string | yes | Topic on witch to subscribe.
unit |  | string | no | Unit which will be displayed on the UI as a suffix.
uptime_expr |  | string | no | If the uptime value is not directly available in the payload, you can use this parameter to calculate the uptime value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the uptime value will be read directly from the path. Example: "x * 100 / 255"
uptime_path |  | string | no | The path where to find the uptime of the sensor in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"uptime": 3600} use "uptime" as path. The value should be a number in seconds. Use `uptime_expr` to adjust if required.
uptime_topic |  | string | no | The topic on witch to publish the uptime of the sensor. If not set, no uptime will be reported.
user |  | string | no | User to use for authentication with mqtt broker. Password must be defined in that case.
value_max |  | float | no | Highest value accepted when writing. A higher value is lowered to `value_max`. The bound applies to the written value, before coeff_a/coeff_b/calc_expr, and also to the result of inc/dec. No upper bound if not set. Ignored, with an error in the log, if lower than `value_min`.
value_min |  | float | no | Lowest value accepted when writing. A lower value is raised to `value_min`. The bound applies to the written value, before coeff_a/coeff_b/calc_expr, and also to the result of inc/dec. No lower bound if not set. Ignored, with an error in the log, if greater than `value_max`.
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
wifi_ssid_path |  | string | no | The path where to find the WiFi SSID where the sensor is connected in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"wifi_ssid": "MyWifi"} use "wifi_ssid" as path. The value should be a string with the WiFi SSID.
wifi_ssid_topic |  | string | no | The topic on witch to publish the WiFi SSID of the sensor. If not set, no WiFi SSID will be reported.
wireless_signal_expr |  | string | no | If the wireless signal value is not directly available in the payload, you can use this parameter to calculate the wireless signal value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the wireless signal value will be read directly from the path. Example: "x * 100 / 255"
wireless_signal_path |  | string | no | The path where to find the wireless signal strength in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"signal": 70} use "signal" as path. The value should be a number in percent. Use `wireless_signal_expr` to adjust if required.
wireless_signal_topic |  | string | no | The topic on witch to publish the wireless signal strength of the sensor. If not set, no wireless signal strength will be reported.

## Conditions of MqttOutputAnalog
Name | Description
---- | -----------
0 | Event on a specific number value 
 changed | Event on any change of value 
 value | Event on a specific value 
 
## Actions of MqttOutputAnalog
Name | Description
---- | -----------
0 | Set a specific number value 
 dec | Decrement value with configured step 
 dec 1 | Decrement value by value 
 inc | Increment value with configured step 
 inc 1 | Increment value by value 
 

# MqttOutputLight
Control lights through mqtt broker


Basic light. This light have only 2 states, ON or OFF. Can also be used to control simple relays output

## Parameters of MqttOutputLight
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
battery_expr |  | string | no | If the battery value is not directly available in the payload, you can use this parameter to calculate the battery value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the battery value will be read directly from the path. Example: "x * 100 / 255"
battery_path |  | string | no | The path where to find the battery status in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json object, just try to use the key of the value you want to read, for example : {"battery":90} use "battery" as path. When this path is set, and the level drops below 30%. The battery reported should be in percent. Use `battery_expr` to adjust if required.
battery_topic |  | string | no | The topic on witch to publish the battery status of the sensor. If not set, no battery status will be reported.
connected_status_expr |  | string | no | If the connected status value is not directly available in the payload, you can use this parameter to convert the value from the path to a boolean. The value will be calculated as any valid mathematic expression. In the expression, the variable `value` is replaced with the raw value from path. If not set, the connected status will be read directly from the path. Example: "value == 'connected'" or "value > 30 and value < 150"
connected_status_path |  | string | no | The path where to find the connected status in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"connected":true} use "connected" as path. The value should be a boolean. Use connected_status_expr to convert to a boolean
connected_status_topic |  | string | no | The topic on witch to publish the connected status of the sensor. If not set, no connected status will be reported.
data |  | string | yes | The data sent when publishing to topic. The __##VALUE##__ contained in data is substituted with with the state (on_value, off_value) to be sent.
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
host |  | string | no | IP address of the mqtt broker to connect to. Default value is 127.0.0.1.
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
io_style |  | list | yes | GUI style display. This will control the icon displayed on the UI
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
ip_address_path |  | string | no | The path where to find the IP address of the sensor in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"ip_address": "192.168.1.156"} use "ip_address" as path. The value should be a string with the IP address.
ip_address_topic |  | string | no | The topic on witch to publish the IP address of the sensor. If not set, no IP address will be reported.
keepalive |  | int | no | keepalive timeout in seconds. Time between two mqtt PING.
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
notif_battery |  | bool | no | If set, a notification will be sent when the battery level drops below 30%. This is only used if the battery_topic is set.
notif_connected |  | bool | no | If set, a notification will be sent when the connected status changes. This is only used if the connected_status_topic is set.
off_value |  | string | yes | Value to interpret as OFF value
on_value |  | string | yes | Value to interpret as ON value
password |  | string | no | Password to use for authentication with mqtt broker. User must be defined in that case.
path |  | string | yes | The path where to find the value in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. if payload is simple json, just try to use the key of the value you want to read, for example : {"temperature":14.23} use "temperature" as path
port |  | int | no | TCP port of the mqtt broker. Default value is 1883
topic_pub |  | string | yes | Topic on witch to publish.
topic_sub |  | string | yes | Topic on witch to subscribe.
uptime_expr |  | string | no | If the uptime value is not directly available in the payload, you can use this parameter to calculate the uptime value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the uptime value will be read directly from the path. Example: "x * 100 / 255"
uptime_path |  | string | no | The path where to find the uptime of the sensor in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"uptime": 3600} use "uptime" as path. The value should be a number in seconds. Use `uptime_expr` to adjust if required.
uptime_topic |  | string | no | The topic on witch to publish the uptime of the sensor. If not set, no uptime will be reported.
user |  | string | no | User to use for authentication with mqtt broker. Password must be defined in that case.
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
wifi_ssid_path |  | string | no | The path where to find the WiFi SSID where the sensor is connected in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"wifi_ssid": "MyWifi"} use "wifi_ssid" as path. The value should be a string with the WiFi SSID.
wifi_ssid_topic |  | string | no | The topic on witch to publish the WiFi SSID of the sensor. If not set, no WiFi SSID will be reported.
wireless_signal_expr |  | string | no | If the wireless signal value is not directly available in the payload, you can use this parameter to calculate the wireless signal value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the wireless signal value will be read directly from the path. Example: "x * 100 / 255"
wireless_signal_path |  | string | no | The path where to find the wireless signal strength in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"signal": 70} use "signal" as path. The value should be a number in percent. Use `wireless_signal_expr` to adjust if required.
wireless_signal_topic |  | string | no | The topic on witch to publish the wireless signal strength of the sensor. If not set, no wireless signal strength will be reported.

## Conditions of MqttOutputLight
Name | Description
---- | -----------
changed | Event on any change of value 
 false | Event when light is off 
 true | Event when light is on 
 
## Actions of MqttOutputLight
Name | Description
---- | -----------
false | Switch the light off 
 impulse 200 | Do an impulse on light state. Set to true for X ms then reset to false 
 impulse 500 200 500 200 | Do an impulse on light state with a pattern.<br>Ex: 500 200 500 200 means: TRUE for 500ms, FALSE for 200ms, TRUE for 500ms, FALSE for 200ms<br>Ex: 500 loop 200 300 means: TRUE for 500ms, then loop the next steps for infinite, FALSE for 200ms, TRUE for 300ms<br>Ex: 100 100 200 old means: blinks and then set to the old start state (before impulse starts) 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 toggle | Invert light state 
 true | Switch the light on 
 

# MqttOutputLightDimmer
Control lights through mqtt broker


Light with dimming control. Light intensity can be changed for this light.

## Parameters of MqttOutputLightDimmer
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
battery_expr |  | string | no | If the battery value is not directly available in the payload, you can use this parameter to calculate the battery value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the battery value will be read directly from the path. Example: "x * 100 / 255"
battery_path |  | string | no | The path where to find the battery status in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json object, just try to use the key of the value you want to read, for example : {"battery":90} use "battery" as path. When this path is set, and the level drops below 30%. The battery reported should be in percent. Use `battery_expr` to adjust if required.
battery_topic |  | string | no | The topic on witch to publish the battery status of the sensor. If not set, no battery status will be reported.
connected_status_expr |  | string | no | If the connected status value is not directly available in the payload, you can use this parameter to convert the value from the path to a boolean. The value will be calculated as any valid mathematic expression. In the expression, the variable `value` is replaced with the raw value from path. If not set, the connected status will be read directly from the path. Example: "value == 'connected'" or "value > 30 and value < 150"
connected_status_path |  | string | no | The path where to find the connected status in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"connected":true} use "connected" as path. The value should be a boolean. Use connected_status_expr to convert to a boolean
connected_status_topic |  | string | no | The topic on witch to publish the connected status of the sensor. If not set, no connected status will be reported.
data |  | string | yes | The data sent when publishing to topic. The __##VALUE##__ contained in data is substituted with the state (integer value) to be sent.
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
host |  | string | no | IP address of the mqtt broker to connect to. Default value is 127.0.0.1.
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
in_expr |  | string | no | Use a mathematical expression to convert the raw value into a percent between 0 and 100. The variable `x` is replaced with the raw value. For example, if you want to convert a brightness of 0-254 to a percentage, you can use `x / 2.54`.
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
ip_address_path |  | string | no | The path where to find the IP address of the sensor in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"ip_address": "192.168.1.156"} use "ip_address" as path. The value should be a string with the IP address.
ip_address_topic |  | string | no | The topic on witch to publish the IP address of the sensor. If not set, no IP address will be reported.
keepalive |  | int | no | keepalive timeout in seconds. Time between two mqtt PING.
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
notif_battery |  | bool | no | If set, a notification will be sent when the battery level drops below 30%. This is only used if the battery_topic is set.
notif_connected |  | bool | no | If set, a notification will be sent when the connected status changes. This is only used if the connected_status_topic is set.
out_expr |  | string | no | Use a mathematical expression to convert the value of the percentage into a raw value. The variable `x` is replaced with the percent value. For example, if you want to convert a percent value of 0-100 to a brightness of 0-254, you can use `x * 2.54`.
password |  | string | no | Password to use for authentication with mqtt broker. User must be defined in that case.
path |  | string | yes | The path where to find the value in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. if payload is simple json, just try to use the key of the value you want to read, for example : {"temperature":14.23} use "temperature" as path
port |  | int | no | TCP port of the mqtt broker. Default value is 1883
topic_pub |  | string | yes | Topic on witch to publish.
topic_sub |  | string | yes | Topic on witch to subscribe.
uptime_expr |  | string | no | If the uptime value is not directly available in the payload, you can use this parameter to calculate the uptime value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the uptime value will be read directly from the path. Example: "x * 100 / 255"
uptime_path |  | string | no | The path where to find the uptime of the sensor in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"uptime": 3600} use "uptime" as path. The value should be a number in seconds. Use `uptime_expr` to adjust if required.
uptime_topic |  | string | no | The topic on witch to publish the uptime of the sensor. If not set, no uptime will be reported.
user |  | string | no | User to use for authentication with mqtt broker. Password must be defined in that case.
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
wifi_ssid_path |  | string | no | The path where to find the WiFi SSID where the sensor is connected in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"wifi_ssid": "MyWifi"} use "wifi_ssid" as path. The value should be a string with the WiFi SSID.
wifi_ssid_topic |  | string | no | The topic on witch to publish the WiFi SSID of the sensor. If not set, no WiFi SSID will be reported.
wireless_signal_expr |  | string | no | If the wireless signal value is not directly available in the payload, you can use this parameter to calculate the wireless signal value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the wireless signal value will be read directly from the path. Example: "x * 100 / 255"
wireless_signal_path |  | string | no | The path where to find the wireless signal strength in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"signal": 70} use "signal" as path. The value should be a number in percent. Use `wireless_signal_expr` to adjust if required.
wireless_signal_topic |  | string | no | The topic on witch to publish the wireless signal strength of the sensor. If not set, no wireless signal strength will be reported.

## Conditions of MqttOutputLightDimmer
Name | Description
---- | -----------
changed | Event on any change of value 
 value | Event when light is at this value 
 
## Actions of MqttOutputLightDimmer
Name | Description
---- | -----------
down 5 | Decrease intensity by X percent 
 false | Switch the light off 
 hold press | Dynamically change light intensity when holding a switch (press action) 
 hold stop | Dynamically change light intensity when holding a switch (stop action) 
 impulse 200 | Do an impulse on light state. Set to true for X ms then reset to false 
 impulse 500 200 500 200 | Do an impulse on light state with a pattern.<br>Ex: 500 200 500 200 means: TRUE for 500ms, FALSE for 200ms, TRUE for 500ms, FALSE for 200ms<br>Ex: 500 loop 200 300 means: TRUE for 500ms, then loop the next steps for infinite, FALSE for 200ms, TRUE for 300ms<br>Ex: 100 100 200 old means: blinks and then set to the old start state (before impulse starts) 
 set 50 | Set light intensity and swith on if light is off 
 set off 50 | Set light value without switching on. This will be the light intensity for the next ON 
 set_state 50 | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 toggle | Invert the light state 
 true | Switch the light on 
 up 5 | Increase intensity by X percent 
 

# MqttOutputLightRGB
Control RGB lights through mqtt broker


RGB light. Choose a color to be set for this light.

## Parameters of MqttOutputLightRGB
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
battery_expr |  | string | no | If the battery value is not directly available in the payload, you can use this parameter to calculate the battery value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the battery value will be read directly from the path. Example: "x * 100 / 255"
battery_path |  | string | no | The path where to find the battery status in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json object, just try to use the key of the value you want to read, for example : {"battery":90} use "battery" as path. When this path is set, and the level drops below 30%. The battery reported should be in percent. Use `battery_expr` to adjust if required.
battery_topic |  | string | no | The topic on witch to publish the battery status of the sensor. If not set, no battery status will be reported.
connected_status_expr |  | string | no | If the connected status value is not directly available in the payload, you can use this parameter to convert the value from the path to a boolean. The value will be calculated as any valid mathematic expression. In the expression, the variable `value` is replaced with the raw value from path. If not set, the connected status will be read directly from the path. Example: "value == 'connected'" or "value > 30 and value < 150"
connected_status_path |  | string | no | The path where to find the connected status in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"connected":true} use "connected" as path. The value should be a boolean. Use connected_status_expr to convert to a boolean
connected_status_topic |  | string | no | The topic on witch to publish the connected status of the sensor. If not set, no connected status will be reported.
data |  | string | yes | The data sent when publishing color to topic. The __##VALUE_R##__  __##VALUE_G##__  __##VALUE_B##__ or __##VALUE_HEX##__ or __##VALUE_X##__ __##VALUE_Y##__ __##VALUE_BRIGHTNESS##__ contained in data is substituted with the color (integer value or #RRGGBB string value) to be sent.
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
host |  | string | no | IP address of the mqtt broker to connect to. Default value is 127.0.0.1.
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
ip_address_path |  | string | no | The path where to find the IP address of the sensor in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"ip_address": "192.168.1.156"} use "ip_address" as path. The value should be a string with the IP address.
ip_address_topic |  | string | no | The topic on witch to publish the IP address of the sensor. If not set, no IP address will be reported.
keepalive |  | int | no | keepalive timeout in seconds. Time between two mqtt PING.
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
notif_battery |  | bool | no | If set, a notification will be sent when the battery level drops below 30%. This is only used if the battery_topic is set.
notif_connected |  | bool | no | If set, a notification will be sent when the connected status changes. This is only used if the connected_status_topic is set.
password |  | string | no | Password to use for authentication with mqtt broker. User must be defined in that case.
path |  | string | yes | The path where to find the value in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. if payload is simple json, just try to use the key of the value you want to read, for example : {"temperature":14.23} use "temperature" as path
path_brightness |  | string | yes | The path where to found the brightness value in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. for example 'brightness'
path_x |  | string | yes | The path where to found the X (X/Y Color space) value in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. for example color/x, try to read the x value from the color object.
path_y |  | string | yes | The path where to found the Y (X/Y Color space) value in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. for example color/y, try to read the y value from the color object.
port |  | int | no | TCP port of the mqtt broker. Default value is 1883
topic_pub |  | string | yes | Topic on witch to publish.
topic_sub |  | string | yes | Topic on witch to subscribe.
uptime_expr |  | string | no | If the uptime value is not directly available in the payload, you can use this parameter to calculate the uptime value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the uptime value will be read directly from the path. Example: "x * 100 / 255"
uptime_path |  | string | no | The path where to find the uptime of the sensor in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"uptime": 3600} use "uptime" as path. The value should be a number in seconds. Use `uptime_expr` to adjust if required.
uptime_topic |  | string | no | The topic on witch to publish the uptime of the sensor. If not set, no uptime will be reported.
user |  | string | no | User to use for authentication with mqtt broker. Password must be defined in that case.
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
wifi_ssid_path |  | string | no | The path where to find the WiFi SSID where the sensor is connected in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"wifi_ssid": "MyWifi"} use "wifi_ssid" as path. The value should be a string with the WiFi SSID.
wifi_ssid_topic |  | string | no | The topic on witch to publish the WiFi SSID of the sensor. If not set, no WiFi SSID will be reported.
wireless_signal_expr |  | string | no | If the wireless signal value is not directly available in the payload, you can use this parameter to calculate the wireless signal value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the wireless signal value will be read directly from the path. Example: "x * 100 / 255"
wireless_signal_path |  | string | no | The path where to find the wireless signal strength in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"signal": 70} use "signal" as path. The value should be a number in percent. Use `wireless_signal_expr` to adjust if required.
wireless_signal_topic |  | string | no | The topic on witch to publish the wireless signal strength of the sensor. If not set, no wireless signal strength will be reported.

## Conditions of MqttOutputLightRGB
Name | Description
---- | -----------
changed | Event on any change of value 
 value | Event when light is at this value 
 
## Actions of MqttOutputLightRGB
Name | Description
---- | -----------
down_blue 5 | Decrease intensity by X percent of blue channel 
 down_green 5 | Decrease intensity by X percent of green channel 
 down_red 5 | Decrease intensity by X percent of red channel 
 false | Switch the light off 
 set #AA1294 | Set color. Color can be represented by using HTML notation: #AABBCC, rgb(50, 10, 30), hsl(11, 22, 33) 
 set_blue 50 | Set blue channel to X percent 
 set_green 50 | Set green channel to X percent 
 set_red 50 | Set red channel to X percent 
 set_state #AA1294 | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 toggle | Invert the light state (ON/OFF) 
 true | Switch the light on 
 up_blue 5 | Increase intensity by X percent of blue channel 
 up_green 5 | Increase intensity by X percent of green channel 
 up_red 5 | Increase intensity by X percent of red channel 
 

# MqttOutputShutter
Control shutters through mqtt broker


Simple shutter. This shutter supports open/close states, as well as impulse shutters.

## Parameters of MqttOutputShutter
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
battery_expr |  | string | no | If the battery value is not directly available in the payload, you can use this parameter to calculate the battery value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the battery value will be read directly from the path. Example: "x * 100 / 255"
battery_path |  | string | no | The path where to find the battery status in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json object, just try to use the key of the value you want to read, for example : {"battery":90} use "battery" as path. When this path is set, and the level drops below 30%. The battery reported should be in percent. Use `battery_expr` to adjust if required.
battery_topic |  | string | no | The topic on witch to publish the battery status of the sensor. If not set, no battery status will be reported.
connected_status_expr |  | string | no | If the connected status value is not directly available in the payload, you can use this parameter to convert the value from the path to a boolean. The value will be calculated as any valid mathematic expression. In the expression, the variable `value` is replaced with the raw value from path. If not set, the connected status will be read directly from the path. Example: "value == 'connected'" or "value > 30 and value < 150"
connected_status_path |  | string | no | The path where to find the connected status in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"connected":true} use "connected" as path. The value should be a boolean. Use connected_status_expr to convert to a boolean
connected_status_topic |  | string | no | The topic on witch to publish the connected status of the sensor. If not set, no connected status will be reported.
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
host |  | string | no | IP address of the mqtt broker to connect to. Default value is 127.0.0.1.
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
impulse_time |  | int | no | Impulse time for shutter that needs impulse instead of holding up/down relays. If set to 0 impulse shutter is disabled. Time is in ms. Default to 0
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
ip_address_path |  | string | no | The path where to find the IP address of the sensor in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"ip_address": "192.168.1.156"} use "ip_address" as path. The value should be a string with the IP address.
ip_address_topic |  | string | no | The topic on witch to publish the IP address of the sensor. If not set, no IP address will be reported.
keepalive |  | int | no | keepalive timeout in seconds. Time between two mqtt PING.
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
notif_battery |  | bool | no | If set, a notification will be sent when the battery level drops below 30%. This is only used if the battery_topic is set.
notif_connected |  | bool | no | If set, a notification will be sent when the connected status changes. This is only used if the connected_status_topic is set.
password |  | string | no | Password to use for authentication with mqtt broker. User must be defined in that case.
path |  | string | yes | The path where to find the value in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. if payload is simple json, just try to use the key of the value you want to read, for example : {"temperature":14.23} use "temperature" as path
payload_close |  | string | yes | Payload to send when closing
payload_open |  | string | yes | Payload to send when opening
payload_stop |  | string | yes | Payload to send when stopping
port |  | int | no | TCP port of the mqtt broker. Default value is 1883
state_close |  | string | no | Value received for closed state (when topic_sub is set)
state_open |  | string | no | Value received for open state (when topic_sub is set)
stop_both |  | bool | no | If in impulse mode, some shutters needs to activate both up dans down relays when stopping the shutter
time |  | int | yes | Time in sec for shutter to open or close
topic_pub |  | string | yes | Topic to publish commands (open/close/stop)
topic_sub |  | string | no | Topic to subscribe to get shutter status (optional). If not set, state is managed by Calaos timing logic.
uptime_expr |  | string | no | If the uptime value is not directly available in the payload, you can use this parameter to calculate the uptime value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the uptime value will be read directly from the path. Example: "x * 100 / 255"
uptime_path |  | string | no | The path where to find the uptime of the sensor in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"uptime": 3600} use "uptime" as path. The value should be a number in seconds. Use `uptime_expr` to adjust if required.
uptime_topic |  | string | no | The topic on witch to publish the uptime of the sensor. If not set, no uptime will be reported.
user |  | string | no | User to use for authentication with mqtt broker. Password must be defined in that case.
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
wifi_ssid_path |  | string | no | The path where to find the WiFi SSID where the sensor is connected in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"wifi_ssid": "MyWifi"} use "wifi_ssid" as path. The value should be a string with the WiFi SSID.
wifi_ssid_topic |  | string | no | The topic on witch to publish the WiFi SSID of the sensor. If not set, no WiFi SSID will be reported.
wireless_signal_expr |  | string | no | If the wireless signal value is not directly available in the payload, you can use this parameter to calculate the wireless signal value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the wireless signal value will be read directly from the path. Example: "x * 100 / 255"
wireless_signal_path |  | string | no | The path where to find the wireless signal strength in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"signal": 70} use "signal" as path. The value should be a number in percent. Use `wireless_signal_expr` to adjust if required.
wireless_signal_topic |  | string | no | The topic on witch to publish the wireless signal strength of the sensor. If not set, no wireless signal strength will be reported.

## Conditions of MqttOutputShutter
Name | Description
---- | -----------
changed | Event on any change of shutter state 
 false | Event when shutter is closed 
 true | Event when shutter is open 
 
## Actions of MqttOutputShutter
Name | Description
---- | -----------
down | Close the shutter 
 impulse down 200 | Close shutter for X ms 
 impulse up 200 | Open shutter for X ms 
 set_state false | Update internal shutter state without starting real action. This is useful when having updating the shutter state from an external source. 
 set_state true | Update internal shutter state without starting real action. This is useful when having updating the shutter state from an external source. 
 stop | Stop the shutter 
 toggle | Invert shutter state 
 up | Open the shutter 
 

# MqttOutputString
Control a string output through mqtt broker

## Parameters of MqttOutputString
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
battery_expr |  | string | no | If the battery value is not directly available in the payload, you can use this parameter to calculate the battery value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the battery value will be read directly from the path. Example: "x * 100 / 255"
battery_path |  | string | no | The path where to find the battery status in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json object, just try to use the key of the value you want to read, for example : {"battery":90} use "battery" as path. When this path is set, and the level drops below 30%. The battery reported should be in percent. Use `battery_expr` to adjust if required.
battery_topic |  | string | no | The topic on witch to publish the battery status of the sensor. If not set, no battery status will be reported.
connected_status_expr |  | string | no | If the connected status value is not directly available in the payload, you can use this parameter to convert the value from the path to a boolean. The value will be calculated as any valid mathematic expression. In the expression, the variable `value` is replaced with the raw value from path. If not set, the connected status will be read directly from the path. Example: "value == 'connected'" or "value > 30 and value < 150"
connected_status_path |  | string | no | The path where to find the connected status in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"connected":true} use "connected" as path. The value should be a boolean. Use connected_status_expr to convert to a boolean
connected_status_topic |  | string | no | The topic on witch to publish the connected status of the sensor. If not set, no connected status will be reported.
data |  | string | yes | The data sent when publishing to topic. The __##VALUE##__ contained in data is substituted with the string to be sent. If data is a JSON document (starts with `{`, `[` or `"`), the string is JSON-escaped, so write the placeholder inside the quotes: `{"mode":"__##VALUE##__"}`.
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
host |  | string | no | IP address of the mqtt broker to connect to. Default value is 127.0.0.1.
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
ip_address_path |  | string | no | The path where to find the IP address of the sensor in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"ip_address": "192.168.1.156"} use "ip_address" as path. The value should be a string with the IP address.
ip_address_topic |  | string | no | The topic on witch to publish the IP address of the sensor. If not set, no IP address will be reported.
keepalive |  | int | no | keepalive timeout in seconds. Time between two mqtt PING.
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
notif_battery |  | bool | no | If set, a notification will be sent when the battery level drops below 30%. This is only used if the battery_topic is set.
notif_connected |  | bool | no | If set, a notification will be sent when the connected status changes. This is only used if the connected_status_topic is set.
password |  | string | no | Password to use for authentication with mqtt broker. User must be defined in that case.
path |  | string | yes | The path where to find the value in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. if payload is simple json, just try to use the key of the value you want to read, for example : {"temperature":14.23} use "temperature" as path
port |  | int | no | TCP port of the mqtt broker. Default value is 1883
topic_pub |  | string | yes | Topic on witch to publish.
topic_sub |  | string | yes | Topic on witch to subscribe.
uptime_expr |  | string | no | If the uptime value is not directly available in the payload, you can use this parameter to calculate the uptime value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the uptime value will be read directly from the path. Example: "x * 100 / 255"
uptime_path |  | string | no | The path where to find the uptime of the sensor in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"uptime": 3600} use "uptime" as path. The value should be a number in seconds. Use `uptime_expr` to adjust if required.
uptime_topic |  | string | no | The topic on witch to publish the uptime of the sensor. If not set, no uptime will be reported.
user |  | string | no | User to use for authentication with mqtt broker. Password must be defined in that case.
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
wifi_ssid_path |  | string | no | The path where to find the WiFi SSID where the sensor is connected in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"wifi_ssid": "MyWifi"} use "wifi_ssid" as path. The value should be a string with the WiFi SSID.
wifi_ssid_topic |  | string | no | The topic on witch to publish the WiFi SSID of the sensor. If not set, no WiFi SSID will be reported.
wireless_signal_expr |  | string | no | If the wireless signal value is not directly available in the payload, you can use this parameter to calculate the wireless signal value from the payload. The value will be calculated as any valid mathematic expression. In the expression, the variable x is replaced with the raw value from path. If not set, the wireless signal value will be read directly from the path. Example: "x * 100 / 255"
wireless_signal_path |  | string | no | The path where to find the wireless signal strength in the mqtt payload. If payload if JSON, informations will be extracted depending on the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array. If payload is simple json, just try to use the key of the value you want to read, for example : {"signal": 70} use "signal" as path. The value should be a number in percent. Use `wireless_signal_expr` to adjust if required.
wireless_signal_topic |  | string | no | The topic on witch to publish the wireless signal strength of the sensor. If not set, no wireless signal strength will be reported.

## Actions of MqttOutputString
Name | Description
---- | -----------
value | Publish a specific string value 
 

# OLAOutputLightDimmer
DMX Light dimmer using OLA (Open Lighting Architecture)


Light with dimming control. Light intensity can be changed for this light.

## Parameters of OLAOutputLightDimmer
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
channel |  | int | yes | DMX channel to control
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
universe |  | int | yes | OLA universe to control
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true

## Conditions of OLAOutputLightDimmer
Name | Description
---- | -----------
changed | Event on any change of value 
 value | Event when light is at this value 
 
## Actions of OLAOutputLightDimmer
Name | Description
---- | -----------
down 5 | Decrease intensity by X percent 
 false | Switch the light off 
 hold press | Dynamically change light intensity when holding a switch (press action) 
 hold stop | Dynamically change light intensity when holding a switch (stop action) 
 impulse 200 | Do an impulse on light state. Set to true for X ms then reset to false 
 impulse 500 200 500 200 | Do an impulse on light state with a pattern.<br>Ex: 500 200 500 200 means: TRUE for 500ms, FALSE for 200ms, TRUE for 500ms, FALSE for 200ms<br>Ex: 500 loop 200 300 means: TRUE for 500ms, then loop the next steps for infinite, FALSE for 200ms, TRUE for 300ms<br>Ex: 100 100 200 old means: blinks and then set to the old start state (before impulse starts) 
 set 50 | Set light intensity and swith on if light is off 
 set off 50 | Set light value without switching on. This will be the light intensity for the next ON 
 set_state 50 | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 toggle | Invert the light state 
 true | Switch the light on 
 up 5 | Increase intensity by X percent 
 
## More Infos
* OLA: http://www.openlighting.org


# OLAOutputLightRGB
RGB Light dimmer using 3 DMX channels with OLA (Open Lighting Architecture)


RGB light. Choose a color to be set for this light.

## Parameters of OLAOutputLightRGB
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
channel_blue |  | int | yes | DMX channel for blue to control
channel_green |  | int | yes | DMX channel for green to control
channel_red |  | int | yes | DMX channel for red to control
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
universe |  | int | yes | OLA universe to control
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true

## Conditions of OLAOutputLightRGB
Name | Description
---- | -----------
changed | Event on any change of value 
 value | Event when light is at this value 
 
## Actions of OLAOutputLightRGB
Name | Description
---- | -----------
down_blue 5 | Decrease intensity by X percent of blue channel 
 down_green 5 | Decrease intensity by X percent of green channel 
 down_red 5 | Decrease intensity by X percent of red channel 
 false | Switch the light off 
 set #AA1294 | Set color. Color can be represented by using HTML notation: #AABBCC, rgb(50, 10, 30), hsl(11, 22, 33) 
 set_blue 50 | Set blue channel to X percent 
 set_green 50 | Set green channel to X percent 
 set_red 50 | Set red channel to X percent 
 set_state #AA1294 | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 toggle | Invert the light state (ON/OFF) 
 true | Switch the light on 
 up_blue 5 | Increase intensity by X percent of blue channel 
 up_green 5 | Increase intensity by X percent of green channel 
 up_red 5 | Increase intensity by X percent of red channel 
 
## More Infos
* OLA: http://www.openlighting.org


# OutputFake
Fake test output. Do nothing. Do not use.

## Parameters of OutputFake
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true


# OWTemp
Temperature measurement with DS18B20 Onewire Sensor


Temperature sensor input. Used for displaying temperature and to control heating devices with rules based on temperature value

## Parameters of OWTemp
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
calc_expr |  | string | no | Use a mathematical expression to calculate the value from the raw value. The variable `x` is replaced with the raw value. For example, if you want to convert a raw value of 0-1000 to a temperature in Celsius, you can use `x / 10.0 - 50.0`. If this expression is set, coeff_a, coeff_b and offset parameters are ignored.
coeff_a |  | float | no | use in conjunction of coeff_b to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 1.0.
coeff_b |  | float | no | use in conjunction of coeff_a to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 0.0
display_warning |  | bool | no | Display a warning if value has not been updated for a long time. Default to true
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
interval |  | float | no | Sampling time in seconds. The value is read at this frequency. If this value is not set, the default value is 15s
io_style |  | list | yes | GUI style display. This will control the icon displayed on the UI
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
offset |  | float | no | same as coeff_b, can be used alone. Default value is 0.0
ow_args |  | string | yes | Additional parameter used for owfs initialization.For example you can use -u to use the USB owfs drivers
ow_id |  | string | yes | Unique ID of sensor on OneWire bus.
period |  | float | no | Sampling time in millisecond. The value is read at this frequency. If this value is not set, calaos tries to read the interval parameter
precision |  | int | no | Precision of the returned value. The value represents the number of decimal after the dot. The value is rounded like this : value = 19.275 => returned value 19.28 when preicision = 2, 19.3 when precision = 1, 19 when precision = 0
unit |  | string | no | Unit which will be displayed on the UI as a suffix.
use_w1 |  | bool | no | Force the use of w1 kernel driver instead of OneWire driver
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true

## Conditions of OWTemp
Name | Description
---- | -----------
changed | Event on any change of value 
 value | Event on a specific value 
 
## More Infos
* Calaos Wiki: http://calaos.fr/wiki/OneWire


# PingInputSwitch
A switch input based on the result of a ping command. Useful to detect presence of a host on the network.


Basic switch with press/release states.

## Parameters of PingInputSwitch
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
host |  | string | yes | IP address or host where to send the ping
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
interval |  | int | no | Interval between pings in ms. Default to 15 sec
io_style |  | list | yes | GUI style display. This will control the icon displayed on the UI
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
timeout |  | int | no | Timeout of the ping request in ms
visible |  | bool | no | Display the Input/Output on all user interfaces if set. Default to false for switches

## Conditions of PingInputSwitch
Name | Description
---- | -----------
changed | Event on any change of state 
 false | The host is offline and/or does not respond to the ping 
 true | The host is online and respond to the ping 
 
# Planet - UNDOCUMENTED IO
SPANK SPANK SPANK : naughty programmer ! You did not add documentation for this IO, that's BAD :'(
Go document it in your code or you will burn in hell!



Planet IP Camera/Encoder. Camera can be viewed directly inside calaos and used in rules.

## Parameters of Planet
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
insecure |  | bool | no | Skip TLS certificate verification when connecting to the camera. Default to true (most cameras use self-signed certificates). Set to false to only allow a verified HTTPS connection.
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
model |  | string | yes | Camera model (ICA-210, ICA-210W, ICA-300, ICA-302, ICA-500) to use
name | Name | string | yes | Name of Input/Output.
password |  | string | no | Password for user
rotate |  | int | no | Rotate the image. Set a value between. The value is in degrees. Example : -90 for  Counter Clock Wise rotation, 90 for Clock Wise rotationCW.
username |  | string | no | Username for accessing the camera
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
width |  | int | no | Width of the image, if this parameter is set, video will be resized to fit the given width. Let parameter empty to keep the original size.


# RemoteUI


Remote UI device. Represents a remote user interface device. Some actions are available to control the device from rules.

## Parameters of RemoteUI
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
auth_token |  | string | no | Authentication token
device_manufacturer |  | string | no | Device manufacturer
device_platform |  | string | no | Device platform
device_secret |  | string | no | Device secret for HMAC validation
device_type |  | list | yes | Device model
device_version |  | string | no | Device version
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
grid_h |  | int | yes | Grid horizontal size
grid_w |  | int | yes | Grid vertical size
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
mac_address |  | string | no | Device MAC address
name | Name | string | yes | Name of Input/Output.
provisioning_code |  | string | yes | Provisioning code for first time setup
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true

## Actions of RemoteUI
Name | Description
---- | -----------
set_brightness X | Set screen brightness (0-100) 
 set_page page_id | Navigate to a specific page by id 
 show_notif message | Show a notification on screen 
 

# RemoteUIOutputRelay


Physical relay on a RemoteUI panel device. Controllable as a standard light output.

## Parameters of RemoteUIOutputRelay
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
io_style |  | list | yes | GUI style display. This will control the icon displayed on the UI
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
relay_num |  | int | yes | Relay number on the device (1, 2, ...)
remote_ui_id |  | string | yes | ID of the parent RemoteUI device
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true

## Conditions of RemoteUIOutputRelay
Name | Description
---- | -----------
changed | Event on any change of value 
 false | Event when light is off 
 true | Event when light is on 
 
## Actions of RemoteUIOutputRelay
Name | Description
---- | -----------
false | Switch the light off 
 impulse 200 | Do an impulse on light state. Set to true for X ms then reset to false 
 impulse 500 200 500 200 | Do an impulse on light state with a pattern.<br>Ex: 500 200 500 200 means: TRUE for 500ms, FALSE for 200ms, TRUE for 500ms, FALSE for 200ms<br>Ex: 500 loop 200 300 means: TRUE for 500ms, then loop the next steps for infinite, FALSE for 200ms, TRUE for 300ms<br>Ex: 100 100 200 old means: blinks and then set to the old start state (before impulse starts) 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 toggle | Invert light state 
 true | Switch the light on 
 

# ReolinkInputSwitch
Switch activated by events from a Reolink camera


Basic switch with press/release states.

## Parameters of ReolinkInputSwitch
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
event_type |  | list | yes | Type of event to listen for from the Reolink camera
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
hostname |  | string | yes | IP address or hostname of the Reolink camera
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
io_style |  | list | yes | GUI style display. This will control the icon displayed on the UI
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
password |  | string | yes | Password for authentication with Reolink camera
username |  | string | yes | Username for authentication with Reolink camera
visible |  | bool | no | Display the Input/Output on all user interfaces if set. Default to false for switches

## Conditions of ReolinkInputSwitch
Name | Description
---- | -----------
changed | Event on any change of state 
 false | Event triggered when switch is released 
 true | Event triggered when switch is pressed 
 

# Roon
Roon audio player allows control of a Roon player from Calaos

## Parameters of Roon
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
host |  | string | no | Static Roon server IP address, empty to autodetect on network
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
port |  | int | no | Static Roon server port, empty to autodetect on network
visible |  | bool | yes | Audio players are not displayed in rooms
zone_id |  | string | yes | Roon zone ID

## Conditions of Roon
Name | Description
---- | -----------
onpause | Event when pausing player 
 onplay | Event when play is started 
 onplaylistchange | Event when a change in the current playlist happens 
 onsongchange | Event when a new song is being played 
 onstop | Event when stopping player 
 onvolumechange | Event when a change of volume happens 
 
## Actions of Roon
Name | Description
---- | -----------
next | Play next song in playlist 
 pause | Pause player 
 play | Start playing 
 previous | Play previous song in playlist 
 stop | Stop player 
 volume down 1 | Decrease volume by a value 
 volume set 50 | Set current volume 
 volume up 1 | Increase volume by a value 
 

# Scenario
A scenario variable. Use this like a virtual button to start a scenario (list of actions)

## Parameters of Scenario
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
autoscenario_uid |  | string | no | Internal use only for Auto Scenario. read only.
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true

## Conditions of Scenario
Name | Description
---- | -----------
true | Event triggered when scenario is started 
 
## Actions of Scenario
Name | Description
---- | -----------
changed | Event triggered on any change 
 false | Stop the scenario (only for special looping scenarios) 
 true | Start the scenario 
 

# Squeezebox
#### Alias: slim
Squeezebox audio player allows control of a Squeezebox from Calaos

## Parameters of Squeezebox
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
host |  | string | yes | Logitech media server IP address
id |  | string | yes | Unique ID of squeezebox in LMS
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
port_cli |  | int | no | CLI port of LMS, default to 9090
port_web |  | int | no | Web interface port of LMS, default to 9000.
visible |  | bool | yes | Audio players are not displayed in rooms

## Conditions of Squeezebox
Name | Description
---- | -----------
onpause | Event when pausing player 
 onplay | Event when play is started 
 onplaylistchange | Event when a change in the current playlist happens 
 onsongchange | Event when a new song is being played 
 onstop | Event when stopping player 
 onvolumechange | Event when a change of volume happens 
 
## Actions of Squeezebox
Name | Description
---- | -----------
add <argument> | Add tracks to playlist. <argument> can be any of album_id:XX artist_id:XX playlist_id:XX, ... 
 next | Play next song in playlist 
 pause | Pause player 
 play | Start playing 
 play <argument> | Clear playlist and play argument. <argument> can be any of album_id:XX artist_id:XX playlist_id:XX, ... 
 power off | Switch player off 
 power on | Switch player on 
 previous | Play previous song in playlist 
 sleep 10 | Start sleep mode with X seconds 
 stop | Stop player 
 sync <playerid> | Sync this player with an other 
 unsync <playerid> | Stop sync of this player with an other 
 volume down 1 | Decrease volume by a value 
 volume set 50 | Set current volume 
 volume up 1 | Increase volume by a value 
 
# standard_mjpeg - UNDOCUMENTED IO
SPANK SPANK SPANK : naughty programmer ! You did not add documentation for this IO, that's BAD :'(
Go document it in your code or you will burn in hell!



MJPEG/Jpeg IP Camera. Camera can be viewed directly inside calaos and used in rules.

## Parameters of standard_mjpeg
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
insecure |  | bool | no | Skip TLS certificate verification when connecting to the camera. Default to true (most cameras use self-signed certificates). Set to false to only allow a verified HTTPS connection.
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
ptz |  | bool | no | Set to true if camera has PTZ support
rotate |  | int | no | Rotate the image. Set a value between. The value is in degrees. Example : -90 for  Counter Clock Wise rotation, 90 for Clock Wise rotationCW.
url_jpeg |  | string | yes | URL for snapshot in jpeg format
url_mjpeg |  | string | no | URL for mjpeg stream support
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
width |  | int | no | Width of the image, if this parameter is set, video will be resized to fit the given width. Let parameter empty to keep the original size.

# StandardMjpeg - UNDOCUMENTED IO
SPANK SPANK SPANK : naughty programmer ! You did not add documentation for this IO, that's BAD :'(
Go document it in your code or you will burn in hell!



MJPEG/Jpeg IP Camera. Camera can be viewed directly inside calaos and used in rules.

## Parameters of StandardMjpeg
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
insecure |  | bool | no | Skip TLS certificate verification when connecting to the camera. Default to true (most cameras use self-signed certificates). Set to false to only allow a verified HTTPS connection.
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
ptz |  | bool | no | Set to true if camera has PTZ support
rotate |  | int | no | Rotate the image. Set a value between. The value is in degrees. Example : -90 for  Counter Clock Wise rotation, 90 for Clock Wise rotationCW.
url_jpeg |  | string | yes | URL for snapshot in jpeg format
url_mjpeg |  | string | no | URL for mjpeg stream support
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
width |  | int | no | Width of the image, if this parameter is set, video will be resized to fit the given width. Let parameter empty to keep the original size.

# SynoSurveillanceStation - UNDOCUMENTED IO
SPANK SPANK SPANK : naughty programmer ! You did not add documentation for this IO, that's BAD :'(
Go document it in your code or you will burn in hell!



Synology Surveillance Station IP Camera. Camera can be viewed directly inside calaos and used in rules.

## Parameters of SynoSurveillanceStation
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
camera_id |  | string | yes | ID of the camera
camera_profile |  | int | no | Profile to use for snapshot. 0- High quality, 1- Balanced, 2- Low bandwidth
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
insecure |  | bool | no | Skip TLS certificate verification when connecting to the camera. Default to true (most cameras use self-signed certificates). Set to false to only allow a verified HTTPS connection.
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
password |  | string | yes | Password for user
rotate |  | int | no | Rotate the image. Set a value between. The value is in degrees. Example : -90 for  Counter Clock Wise rotation, 90 for Clock Wise rotationCW.
url |  | string | yes | Full url to Synology nas. Ex: https://192.168.0.22:5000
username |  | string | yes | Username which can access Surveillance Station
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
width |  | int | no | Width of the image, if this parameter is set, video will be resized to fit the given width. Let parameter empty to keep the original size.


# TimeRange
#### Alias: InPlageHoraire
Represent a time range object. A time range is true if current time is in one of the included range, false otherwise. The time range also support weekdays and months. A range whose end is before its start wraps over midnight: it starts on the weekday it is attached to and ends the next morning. For example a range 23:00 -> 01:00 set on monday is true from monday 23:00 to tuesday 01:00, and never on monday between 00:00 and 01:00. Beware that bounds relative to sunrise/sunset move with the season: a range like sunset -> 23:00 is a normal range most of the year but starts to wrap over midnight when sunset gets later than 23:00. Such a range is reported in the log the first time it is evaluated as wrapping.

## Parameters of TimeRange
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
visible |  | bool | no | A time range can't be visible. Always false.

## Conditions of TimeRange
Name | Description
---- | -----------
changed | Event on any change of range 
 false | Event triggered when exiting the range 
 true | Event triggered when entering the range 
 

# WebInputAnalog
Analog input read from a web document


An analog input can be used to read analog values to display them and use them in rules.

## Parameters of WebInputAnalog
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
calc_expr |  | string | no | Use a mathematical expression to calculate the value from the raw value. The variable `x` is replaced with the raw value. For example, if you want to convert a raw value of 0-1000 to a temperature in Celsius, you can use `x / 10.0 - 50.0`. If this expression is set, coeff_a, coeff_b and offset parameters are ignored.
coeff_a |  | float | no | use in conjunction of coeff_b to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 1.0.
coeff_b |  | float | no | use in conjunction of coeff_a to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 0.0
display_warning |  | bool | no | Display a warning if value has not been updated for a long time. Default to true
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
file_type |  | string | yes | File type of the document. Values can be xml, json or text.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
insecure |  | bool | no | Skip TLS certificate verification when connecting to the url. Default to true (LAN devices commonly use self-signed certificates). Set to false to only allow a verified HTTPS connection.
interval |  | float | no | Sampling time in seconds. The value is read at this frequency. If this value is not set, the default value is 15s
io_style |  | list | yes | GUI style display. This will control the icon displayed on the UI
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
offset |  | float | no | same as coeff_b, can be used alone. Default value is 0.0
path |  | string | yes | The path where to found the value. This value can take multiple values depending on the file type. If file_type is JSON, the json file downloaded will be read, and the informations will be extracted from the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array.
If file_type is XML, the path is an xpath expression; Look here for syntax : http://www.w3schools.com/xsl/xpath_syntax.asp If file_type is TEXT, the downloaded file is returned as plain text file, and path must be in the form line/pos/separator Line is read, and is split using separator as delimiters The value returned is the value at pos in the split list. If the separator is not found, the whole line is returned. Example the file contains 
10.0,10.1,10.2,10.3
20.0,20.1,20.2,20.3
If the path is 2/4/, the value returne wil be 20.3

period |  | float | no | Sampling time in millisecond. The value is read at this frequency. If this value is not set, calaos tries to read the interval parameter
precision |  | int | no | Precision of the returned value. The value represents the number of decimal after the dot. The value is rounded like this : value = 19.275 => returned value 19.28 when preicision = 2, 19.3 when precision = 1, 19 when precision = 0
unit |  | string | no | Unit which will be displayed on the UI as a suffix.
url |  | string | yes | URL where to download the document from
If URL begins with / or with file:// the data is read from the local file
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true

## Conditions of WebInputAnalog
Name | Description
---- | -----------
changed | Event on any change of value 
 value | Event on a specific value 
 

# WebInputString
String input providing from a web document

## Parameters of WebInputString
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
file_type |  | string | yes | File type of the document. Values can be xml, json or text.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
insecure |  | bool | no | Skip TLS certificate verification when connecting to the url. Default to true (LAN devices commonly use self-signed certificates). Set to false to only allow a verified HTTPS connection.
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
path |  | string | yes | The path where to found the value. This value can take multiple values depending on the file type. If file_type is JSON, the json file downloaded will be read, and the informations will be extracted from the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array.
If file_type is XML, the path is an xpath expression; Look here for syntax : http://www.w3schools.com/xsl/xpath_syntax.asp If file_type is TEXT, the downloaded file is returned as plain text file, and path must be in the form line/pos/separator Line is read, and is split using separator as delimiters The value returned is the value at pos in the split list. If the separator is not found, the whole line is returned. Example the file contains 
10.0,10.1,10.2,10.3
20.0,20.1,20.2,20.3
If the path is 2/4/, the value returne wil be 20.3

url |  | string | yes | URL where to download the document from
If URL begins with / or with file:// the data is read from the local file
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true


# WebInputTemp
Temperature input read from a web document


Temperature sensor input. Used for displaying temperature and to control heating devices with rules based on temperature value

## Parameters of WebInputTemp
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
calc_expr |  | string | no | Use a mathematical expression to calculate the value from the raw value. The variable `x` is replaced with the raw value. For example, if you want to convert a raw value of 0-1000 to a temperature in Celsius, you can use `x / 10.0 - 50.0`. If this expression is set, coeff_a, coeff_b and offset parameters are ignored.
coeff_a |  | float | no | use in conjunction of coeff_b to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 1.0.
coeff_b |  | float | no | use in conjunction of coeff_a to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 0.0
display_warning |  | bool | no | Display a warning if value has not been updated for a long time. Default to true
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
file_type |  | string | yes | File type of the document. Values can be xml, json or text.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
insecure |  | bool | no | Skip TLS certificate verification when connecting to the url. Default to true (LAN devices commonly use self-signed certificates). Set to false to only allow a verified HTTPS connection.
interval |  | float | no | Sampling time in seconds. The value is read at this frequency. If this value is not set, the default value is 15s
io_style |  | list | yes | GUI style display. This will control the icon displayed on the UI
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
offset |  | float | no | same as coeff_b, can be used alone. Default value is 0.0
path |  | string | yes | The path where to found the value. This value can take multiple values depending on the file type. If file_type is JSON, the json file downloaded will be read, and the informations will be extracted from the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array.
If file_type is XML, the path is an xpath expression; Look here for syntax : http://www.w3schools.com/xsl/xpath_syntax.asp If file_type is TEXT, the downloaded file is returned as plain text file, and path must be in the form line/pos/separator Line is read, and is split using separator as delimiters The value returned is the value at pos in the split list. If the separator is not found, the whole line is returned. Example the file contains 
10.0,10.1,10.2,10.3
20.0,20.1,20.2,20.3
If the path is 2/4/, the value returne wil be 20.3

period |  | float | no | Sampling time in millisecond. The value is read at this frequency. If this value is not set, calaos tries to read the interval parameter
precision |  | int | no | Precision of the returned value. The value represents the number of decimal after the dot. The value is rounded like this : value = 19.275 => returned value 19.28 when preicision = 2, 19.3 when precision = 1, 19 when precision = 0
unit |  | string | no | Unit which will be displayed on the UI as a suffix.
url |  | string | yes | URL where to download the document from
If URL begins with / or with file:// the data is read from the local file
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true

## Conditions of WebInputTemp
Name | Description
---- | -----------
changed | Event on any change of value 
 value | Event on a specific value 
 

# WebOutputAnalog
Analog output in a web request


Analog output. Useful to control analog output devices connected to calaos.

## Parameters of WebOutputAnalog
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
calc_expr |  | string | no | Use a mathematical expression to calculate the value from the raw value. The variable `x` is replaced with the raw value. For example, if you want to convert a raw value of 0-1000 to a temperature in Celsius, you can use `x / 10.0 - 50.0`. If this expression is set, coeff_a, coeff_b and offset parameters are ignored.
coeff_a |  | float | no | use in conjunction of coeff_b to apply equation of the form `value_sent = coeff_a * raw_value + coeff_b`. Default value is 1.0.
coeff_b |  | float | no | use in conjunction of coeff_a to apply equation of the form `value_sent = coeff_a * raw_value + coeff_b`. Default value is 0.0
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
file_type |  | string | yes | File type of the document. Values can be xml, json or text.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
insecure |  | bool | no | Skip TLS certificate verification when connecting to the url. Default to true (LAN devices commonly use self-signed certificates). Set to false to only allow a verified HTTPS connection.
io_style |  | list | yes | GUI style display. This will control the icon displayed on the UI
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
path |  | string | yes | The path where to found the value. This value can take multiple values depending on the file type. If file_type is JSON, the json file downloaded will be read, and the informations will be extracted from the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array.
If file_type is XML, the path is an xpath expression; Look here for syntax : http://www.w3schools.com/xsl/xpath_syntax.asp If file_type is TEXT, the downloaded file is returned as plain text file, and path must be in the form line/pos/separator Line is read, and is split using separator as delimiters The value returned is the value at pos in the split list. If the separator is not found, the whole line is returned. Example the file contains 
10.0,10.1,10.2,10.3
20.0,20.1,20.2,20.3
If the path is 2/4/, the value returne wil be 20.3

step |  | float | no | Set a step for increment/decrement value. Default is 1.0
unit |  | string | no | Unit which will be displayed on the UI as a suffix.
url |  | string | yes | URL where to download the document from
If URL begins with / or with file:// the data is read from the local file
value_max |  | float | no | Highest value accepted when writing. A higher value is lowered to `value_max`. The bound applies to the written value, before coeff_a/coeff_b/calc_expr, and also to the result of inc/dec. No upper bound if not set. Ignored, with an error in the log, if lower than `value_min`.
value_min |  | float | no | Lowest value accepted when writing. A lower value is raised to `value_min`. The bound applies to the written value, before coeff_a/coeff_b/calc_expr, and also to the result of inc/dec. No lower bound if not set. Ignored, with an error in the log, if greater than `value_max`.
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true

## Conditions of WebOutputAnalog
Name | Description
---- | -----------
0 | Event on a specific number value 
 changed | Event on any change of value 
 value | Event on a specific value 
 
## Actions of WebOutputAnalog
Name | Description
---- | -----------
0 | Set a specific number value 
 dec | Decrement value with configured step 
 dec 1 | Decrement value by value 
 inc | Increment value with configured step 
 inc 1 | Increment value by value 
 

# WebOutputLight
Bool output written to a web document or URL


Basic light. This light have only 2 states, ON or OFF. Can also be used to control simple relays output

## Parameters of WebOutputLight
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
data |  | string | yes | The document send when posting data. This value can be void, in, that case the value is substituted in the url, otherwise the __##VALUE##__ contained in data is substituted with with the value to be sent.
data_type |  | string | yes | The HTTP header Content-Type used when posting the document. It depends on the website, but you can use application/json application/xml as correct values.
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
insecure |  | bool | no | Skip TLS certificate verification when connecting to the url. Default to true (LAN devices commonly use self-signed certificates). Set to false to only allow a verified HTTPS connection.
io_style |  | list | yes | GUI style display. This will control the icon displayed on the UI
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
url |  | string | yes | URL where to POST the document to. The POST request is associated with the data field if not null. When no data is provided, Calaos substitutes __##VALUE##__ string with the value to send. For example if the url is http://example.com/api?value=__##VALUE##__ the url post will be :
http://example.com/api?value=20.3
The url is encoded before being sent.
If the URL begins with / or file:// the data is written to a file.
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true

## Conditions of WebOutputLight
Name | Description
---- | -----------
changed | Event on any change of value 
 false | Event when light is off 
 true | Event when light is on 
 
## Actions of WebOutputLight
Name | Description
---- | -----------
false | Switch the light off 
 impulse 200 | Do an impulse on light state. Set to true for X ms then reset to false 
 impulse 500 200 500 200 | Do an impulse on light state with a pattern.<br>Ex: 500 200 500 200 means: TRUE for 500ms, FALSE for 200ms, TRUE for 500ms, FALSE for 200ms<br>Ex: 500 loop 200 300 means: TRUE for 500ms, then loop the next steps for infinite, FALSE for 200ms, TRUE for 300ms<br>Ex: 100 100 200 old means: blinks and then set to the old start state (before impulse starts) 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 toggle | Invert light state 
 true | Switch the light on 
 

# WebOutputLightRGB
RGB value written to a web document or URL


RGB light. Choose a color to be set for this light.

## Parameters of WebOutputLightRGB
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
data |  | string | yes | The document send when posting data. This value can be void, in, that case the value is substituted in the url, otherwise the __##VALUE##__ contained in data is substituted with with the value to be sent.
data_type |  | string | yes | The HTTP header Content-Type used when posting the document. It depends on the website, but you can use application/json application/xml as correct values.
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
insecure |  | bool | no | Skip TLS certificate verification when connecting to the url. Default to true (LAN devices commonly use self-signed certificates). Set to false to only allow a verified HTTPS connection.
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
raw_value |  | bool | no | RGB value has #RRGGBB. Sometimes some web api take only RRGGBBformat. If raw_value is true, the # in front of the line isremoved. The default value for this parameter is false.
url |  | string | yes | URL where to POST the document to. The POST request is associated with the data field if not null. When no data is provided, Calaos substitutes __##VALUE##__ string with the value to send. For example if the url is http://example.com/api?value=__##VALUE##__ the url post will be :
http://example.com/api?value=20.3
The url is encoded before being sent.
If the URL begins with / or file:// the data is written to a file.
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true

## Conditions of WebOutputLightRGB
Name | Description
---- | -----------
changed | Event on any change of value 
 value | Event when light is at this value 
 
## Actions of WebOutputLightRGB
Name | Description
---- | -----------
down_blue 5 | Decrease intensity by X percent of blue channel 
 down_green 5 | Decrease intensity by X percent of green channel 
 down_red 5 | Decrease intensity by X percent of red channel 
 false | Switch the light off 
 set #AA1294 | Set color. Color can be represented by using HTML notation: #AABBCC, rgb(50, 10, 30), hsl(11, 22, 33) 
 set_blue 50 | Set blue channel to X percent 
 set_green 50 | Set green channel to X percent 
 set_red 50 | Set red channel to X percent 
 set_state #AA1294 | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 toggle | Invert the light state (ON/OFF) 
 true | Switch the light on 
 up_blue 5 | Increase intensity by X percent of blue channel 
 up_green 5 | Increase intensity by X percent of green channel 
 up_red 5 | Increase intensity by X percent of red channel 
 

# WebOutputString
String output written to a web document or URL

## Parameters of WebOutputString
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
data |  | string | yes | The document send when posting data. This value can be void, in, that case the value is substituted in the url, otherwise the __##VALUE##__ contained in data is substituted with with the value to be sent.
data_type |  | string | yes | The HTTP header Content-Type used when posting the document. It depends on the website, but you can use application/json application/xml as correct values.
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
file_type |  | string | yes | File type of the document. Values can be xml, json or text.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
insecure |  | bool | no | Skip TLS certificate verification when connecting to the url. Default to true (LAN devices commonly use self-signed certificates). Set to false to only allow a verified HTTPS connection.
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
path |  | string | yes | The path where to found the value. This value can take multiple values depending on the file type. If file_type is JSON, the json file downloaded will be read, and the informations will be extracted from the path. Array indices are their own path segment, written between square brackets: for example weather/[0]/description reads the description value of the first element of the weather array.
If file_type is XML, the path is an xpath expression; Look here for syntax : http://www.w3schools.com/xsl/xpath_syntax.asp If file_type is TEXT, the downloaded file is returned as plain text file, and path must be in the form line/pos/separator Line is read, and is split using separator as delimiters The value returned is the value at pos in the split list. If the separator is not found, the whole line is returned. Example the file contains 
10.0,10.1,10.2,10.3
20.0,20.1,20.2,20.3
If the path is 2/4/, the value returne wil be 20.3

url |  | string | yes | URL where to POST the document to. The POST request is associated with the data field if not null. When no data is provided, Calaos substitutes __##VALUE##__ string with the value to send. For example if the url is http://example.com/api?value=__##VALUE##__ the url post will be :
http://example.com/api?value=20.3
The url is encoded before being sent.
If the URL begins with / or file:// the data is written to a file.
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true


# WIAnalog
#### Alias: WagoInputAnalog
Analog measurement with Wago module (like 0-10V, 4-20mA, ...)


An analog input can be used to read analog values to display them and use them in rules.

## Parameters of WIAnalog
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
calc_expr |  | string | no | Use a mathematical expression to calculate the value from the raw value. The variable `x` is replaced with the raw value. For example, if you want to convert a raw value of 0-1000 to a temperature in Celsius, you can use `x / 10.0 - 50.0`. If this expression is set, coeff_a, coeff_b and offset parameters are ignored.
coeff_a |  | float | no | use in conjunction of coeff_b to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 1.0.
coeff_b |  | float | no | use in conjunction of coeff_a to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 0.0
display_warning |  | bool | no | Display a warning if value has not been updated for a long time. Default to true
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
host |  | string | yes | Wago PLC IP address on the network
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
interval |  | float | no | Sampling time in seconds. The value is read at this frequency. If this value is not set, the default value is 15s
io_style |  | list | yes | GUI style display. This will control the icon displayed on the UI
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
offset |  | float | no | same as coeff_b, can be used alone. Default value is 0.0
period |  | float | no | Sampling time in millisecond. The value is read at this frequency. If this value is not set, calaos tries to read the interval parameter
port |  | int | no | Wago ethernet port, default to 502
precision |  | int | no | Precision of the returned value. The value represents the number of decimal after the dot. The value is rounded like this : value = 19.275 => returned value 19.28 when preicision = 2, 19.3 when precision = 1, 19 when precision = 0
unit |  | string | no | Unit which will be displayed on the UI as a suffix.
var |  | int | yes | PLC address of the input sensor
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true

## Conditions of WIAnalog
Name | Description
---- | -----------
changed | Event on any change of value 
 value | Event on a specific value 
 
## More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/entree_analog


# WIDigitalBP
#### Alias: WIDigital, WagoInputSwitch
Switch with digital input Wago modules (like 750-1405, ...)


Basic switch with press/release states.

## Parameters of WIDigitalBP
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
host |  | string | yes | Wago PLC IP address on the network
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
io_style |  | list | yes | GUI style display. This will control the icon displayed on the UI
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
port |  | int | no | Wago ethernet port, default to 502
var |  | int | yes | PLC address of the digital input
visible |  | bool | no | Display the Input/Output on all user interfaces if set. Default to false for switches

## Conditions of WIDigitalBP
Name | Description
---- | -----------
changed | Event on any change of state 
 false | Event triggered when switch is released 
 true | Event triggered when switch is pressed 
 
## More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/750-1045


# WIDigitalLong
#### Alias: WagoInputSwitchLongPress
Switch long press with digital input Wago modules (like 750-1405, ...)


Long press switch. This switch supports single press and long press. User has 500ms to perform the long press.

## Parameters of WIDigitalLong
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
host |  | string | yes | Wago PLC IP address on the network
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
port |  | int | no | Wago ethernet port, default to 502
var |  | int | yes | PLC address of the digital input
visible |  | bool | no | A switch can't be visible. Always false.

## Conditions of WIDigitalLong
Name | Description
---- | -----------
1 | Event triggered when switch is pressed quickly 
 2 | Event triggered when switch is pressed at least for 500ms (long press) 
 changed | Event on any change of state 
 
## More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/750-1045


# WIDigitalTriple
#### Alias: WagoInputSwitchTriple
Switch triple click with digital input Wago modules (like 750-1405, ...)


Triple click switch. This switch can start 3 kind of actions. User has 500ms to do a multiple click.

## Parameters of WIDigitalTriple
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
host |  | string | yes | Wago PLC IP address on the network
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
port |  | int | no | Wago ethernet port, default to 502
var |  | int | yes | PLC address of the digital input
visible |  | bool | no | A switch can't be visible. Always false.

## Conditions of WIDigitalTriple
Name | Description
---- | -----------
1 | Event triggered when switch is single clicked 
 2 | Event triggered when switch is double clicked 
 3 | Event triggered when switch is triple clicked 
 changed | Event on any change of state 
 
## More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/750-1045


# WITemp
#### Alias: WagoInputTemp
Temperature measurement with Wago temperature module (like 750-460)


Temperature sensor input. Used for displaying temperature and to control heating devices with rules based on temperature value

## Parameters of WITemp
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
calc_expr |  | string | no | Use a mathematical expression to calculate the value from the raw value. The variable `x` is replaced with the raw value. For example, if you want to convert a raw value of 0-1000 to a temperature in Celsius, you can use `x / 10.0 - 50.0`. If this expression is set, coeff_a, coeff_b and offset parameters are ignored.
coeff_a |  | float | no | use in conjunction of coeff_b to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 1.0.
coeff_b |  | float | no | use in conjunction of coeff_a to apply equation of the form `value_displayed = coeff_a * raw_value + coeff_b`. Default value is 0.0
display_warning |  | bool | no | Display a warning if value has not been updated for a long time. Default to true
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
host |  | string | yes | Wago PLC IP address on the network
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
interval |  | float | no | Sampling time in seconds. The value is read at this frequency. If this value is not set, the default value is 15s
io_style |  | list | yes | GUI style display. This will control the icon displayed on the UI
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
offset |  | float | no | same as coeff_b, can be used alone. Default value is 0.0
period |  | float | no | Sampling time in millisecond. The value is read at this frequency. If this value is not set, calaos tries to read the interval parameter
port |  | int | no | Wago ethernet port, default to 502
precision |  | int | no | Precision of the returned value. The value represents the number of decimal after the dot. The value is rounded like this : value = 19.275 => returned value 19.28 when preicision = 2, 19.3 when precision = 1, 19 when precision = 0
unit |  | string | no | Unit which will be displayed on the UI as a suffix.
var |  | int | yes | PLC address of the input sensor
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true

## Conditions of WITemp
Name | Description
---- | -----------
changed | Event on any change of value 
 value | Event on a specific value 
 
## More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/750-460


# WOAnalog
#### Alias: WagoOutputAnalog
Analog output with Wago module (like 0-10V, 4-20mA, ...)


Analog output. Useful to control analog output devices connected to calaos.

## Parameters of WOAnalog
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
calc_expr |  | string | no | Use a mathematical expression to calculate the value from the raw value. The variable `x` is replaced with the raw value. For example, if you want to convert a raw value of 0-1000 to a temperature in Celsius, you can use `x / 10.0 - 50.0`. If this expression is set, coeff_a, coeff_b and offset parameters are ignored.
coeff_a |  | float | no | use in conjunction of coeff_b to apply equation of the form `value_sent = coeff_a * raw_value + coeff_b`. Default value is 1.0.
coeff_b |  | float | no | use in conjunction of coeff_a to apply equation of the form `value_sent = coeff_a * raw_value + coeff_b`. Default value is 0.0
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
host |  | string | yes | Wago PLC IP address on the network
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
io_style |  | list | yes | GUI style display. This will control the icon displayed on the UI
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
port |  | int | no | Wago ethernet port, default to 502
step |  | float | no | Set a step for increment/decrement value. Default is 1.0
unit |  | string | no | Unit which will be displayed on the UI as a suffix.
value_max |  | float | no | Highest value accepted when writing. A higher value is lowered to `value_max`. The bound applies to the written value, before coeff_a/coeff_b/calc_expr, and also to the result of inc/dec. No upper bound if not set. Ignored, with an error in the log, if lower than `value_min`.
value_min |  | float | no | Lowest value accepted when writing. A lower value is raised to `value_min`. The bound applies to the written value, before coeff_a/coeff_b/calc_expr, and also to the result of inc/dec. No lower bound if not set. Ignored, with an error in the log, if greater than `value_max`.
var |  | int | yes | PLC address of the output
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true

## Conditions of WOAnalog
Name | Description
---- | -----------
0 | Event on a specific number value 
 changed | Event on any change of value 
 value | Event on a specific value 
 
## Actions of WOAnalog
Name | Description
---- | -----------
0 | Set a specific number value 
 dec | Decrement value with configured step 
 dec 1 | Decrement value by value 
 inc | Increment value with configured step 
 inc 1 | Increment value by value 
 
## More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/sortie_analog


# WODali
#### Alias: WagoOutputDimmer
Light using DALI or DMX. For DALI you need a 750-641 wago module. For DMX, a DMX4ALL-LAN device connected to the Wago PLC.


Light with dimming control. Light intensity can be changed for this light.

## Parameters of WODali
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
address |  | int | yes | Device address. For DALI address is between 1-64. For DMX the address is 100 + the channel, so DMX channel 5 is address 105; channel 1 is address 101 and 100 itself is not a DMX address
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
fade_time |  | int | no | DALI fade time. value is between 1-10
group |  | int | no | Set to 1 if address is a DALI group address, set to 0 otherwise.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
host |  | string | yes | Wago PLC IP address on the network
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
line |  | int | no | DALI bus line, usually 1
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
port |  | int | no | Wago ethernet port, default to 502
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true

## Conditions of WODali
Name | Description
---- | -----------
changed | Event on any change of value 
 value | Event when light is at this value 
 
## Actions of WODali
Name | Description
---- | -----------
down 5 | Decrease intensity by X percent 
 false | Switch the light off 
 hold press | Dynamically change light intensity when holding a switch (press action) 
 hold stop | Dynamically change light intensity when holding a switch (stop action) 
 impulse 200 | Do an impulse on light state. Set to true for X ms then reset to false 
 impulse 500 200 500 200 | Do an impulse on light state with a pattern.<br>Ex: 500 200 500 200 means: TRUE for 500ms, FALSE for 200ms, TRUE for 500ms, FALSE for 200ms<br>Ex: 500 loop 200 300 means: TRUE for 500ms, then loop the next steps for infinite, FALSE for 200ms, TRUE for 300ms<br>Ex: 100 100 200 old means: blinks and then set to the old start state (before impulse starts) 
 set 50 | Set light intensity and swith on if light is off 
 set off 50 | Set light value without switching on. This will be the light intensity for the next ON 
 set_state 50 | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 toggle | Invert the light state 
 true | Switch the light on 
 up 5 | Increase intensity by X percent 
 
## More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/750-641
* Calaos Wiki: http://calaos.fr/wiki/fr/dmx-lan


# WODaliRVB
#### Alias: WagoOutputDimmerRGB
RGB Light using DALI or DMX. To work you need 3 DALI/DMX channels. For DALI you need a 750-641 wago module. For DMX, a DMX4ALL-LAN device connected to the Wago PLC.


RGB light. Choose a color to be set for this light.

## Parameters of WODaliRVB
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
baddress |  | int | yes | Device address for blue channel. For DALI address is between 1-64. For DMX the address is 100 + the channel, so DMX channel 5 is address 105; channel 1 is address 101 and 100 itself is not a DMX address
bfade_time |  | int | no | DALI fade time for blue channel. value is between 1-10
bgroup |  | int | no | Set to 1 if address for blue channel is a DALI group address, set to 0 otherwise.
bline |  | int | no | DALI bus line for blue channel, usually 1
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gaddress |  | int | yes | Device address for green channel. For DALI address is between 1-64. For DMX the address is 100 + the channel, so DMX channel 5 is address 105; channel 1 is address 101 and 100 itself is not a DMX address
gfade_time |  | int | no | DALI fade time for green channel. value is between 1-10
ggroup |  | int | no | Set to 1 if address for green channel is a DALI group address, set to 0 otherwise.
gline |  | int | no | DALI bus line for green channel, usually 1
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
host |  | string | yes | Wago PLC IP address on the network
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
port |  | int | no | Wago ethernet port, default to 502
raddress |  | int | yes | Device address for red channel. For DALI address is between 1-64. For DMX the address is 100 + the channel, so DMX channel 5 is address 105; channel 1 is address 101 and 100 itself is not a DMX address
rfade_time |  | int | no | DALI fade time for red channel. value is between 1-10
rgroup |  | int | no | Set to 1 if address for red channel is a DALI group address, set to 0 otherwise.
rline |  | int | no | DALI bus line for red channel, usually 1
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true

## Conditions of WODaliRVB
Name | Description
---- | -----------
changed | Event on any change of value 
 value | Event when light is at this value 
 
## Actions of WODaliRVB
Name | Description
---- | -----------
down_blue 5 | Decrease intensity by X percent of blue channel 
 down_green 5 | Decrease intensity by X percent of green channel 
 down_red 5 | Decrease intensity by X percent of red channel 
 false | Switch the light off 
 set #AA1294 | Set color. Color can be represented by using HTML notation: #AABBCC, rgb(50, 10, 30), hsl(11, 22, 33) 
 set_blue 50 | Set blue channel to X percent 
 set_green 50 | Set green channel to X percent 
 set_red 50 | Set red channel to X percent 
 set_state #AA1294 | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 toggle | Invert the light state (ON/OFF) 
 true | Switch the light on 
 up_blue 5 | Increase intensity by X percent of blue channel 
 up_green 5 | Increase intensity by X percent of green channel 
 up_red 5 | Increase intensity by X percent of red channel 
 
## More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/750-641
* Calaos Wiki: http://calaos.fr/wiki/fr/dmx-lan


# WODigital
#### Alias: WagoOutputLight
Simple light or relay control using wago digital output modules (like 750-1504, ...)


Basic light. This light have only 2 states, ON or OFF. Can also be used to control simple relays output

## Parameters of WODigital
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
host |  | string | yes | Wago PLC IP address on the network
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
io_style |  | list | yes | GUI style display. This will control the icon displayed on the UI
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
knx |  | bool | no | Set to true if output is a KNX device (only for 750-849 with KNX/TP1 module)
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
port |  | int | no | Wago ethernet port, default to 502
var |  | int | yes | PLC address of the output
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
wago_841 |  | bool | yes | Should be false if PLC is 750-842, true otherwise

## Conditions of WODigital
Name | Description
---- | -----------
changed | Event on any change of value 
 false | Event when light is off 
 true | Event when light is on 
 
## Actions of WODigital
Name | Description
---- | -----------
false | Switch the light off 
 impulse 200 | Do an impulse on light state. Set to true for X ms then reset to false 
 impulse 500 200 500 200 | Do an impulse on light state with a pattern.<br>Ex: 500 200 500 200 means: TRUE for 500ms, FALSE for 200ms, TRUE for 500ms, FALSE for 200ms<br>Ex: 500 loop 200 300 means: TRUE for 500ms, then loop the next steps for infinite, FALSE for 200ms, TRUE for 300ms<br>Ex: 100 100 200 old means: blinks and then set to the old start state (before impulse starts) 
 set_state false | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 set_state true | Update internal light state without starting real action. This is useful when having updating the light state from an external source. 
 toggle | Invert light state 
 true | Switch the light on 
 
## More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/750-1504


# WOLOutputBool
Wake On Lan output object. Send wake-on-lan packet to a device on the network.

## Parameters of WOLOutputBool
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
address |  | string | yes | Ethernet MAC address of the host to wake up
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
interval |  | int | no | Interval between pings in ms. Default to 15 sec
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true

## Actions of WOLOutputBool
Name | Description
---- | -----------
true | Send wake on lan packet to the configured device 
 

# WOVolet
#### Alias: WagoOutputShutter
Simple shutter using wago digital output modules (like 750-1504, ...)


Simple shutter. This shutter supports open/close states, as well as impulse shutters.

## Parameters of WOVolet
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
host |  | string | yes | Wago PLC IP address on the network
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
impulse_time |  | int | no | Impulse time for shutter that needs impulse instead of holding up/down relays. If set to 0 impulse shutter is disabled. Time is in ms. Default to 0
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
knx |  | bool | no | Set to true if output is a KNX device (only for 750-849 with KNX/TP1 module)
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
port |  | int | no | Wago ethernet port, default to 502
stop_both |  | bool | no | If in impulse mode, some shutters needs to activate both up dans down relays when stopping the shutter
time |  | int | yes | Time in sec for shutter to open or close
var_down |  | int | yes | Digital output address on the PLC for closing the shutter
var_up |  | int | yes | Digital output address on the PLC for opening the shutter
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
wago_841 |  | bool | yes | Should be false if PLC is 750-842, true otherwise

## Conditions of WOVolet
Name | Description
---- | -----------
changed | Event on any change of shutter state 
 false | Event when shutter is closed 
 true | Event when shutter is open 
 
## Actions of WOVolet
Name | Description
---- | -----------
down | Close the shutter 
 impulse down 200 | Close shutter for X ms 
 impulse up 200 | Open shutter for X ms 
 set_state false | Update internal shutter state without starting real action. This is useful when having updating the shutter state from an external source. 
 set_state true | Update internal shutter state without starting real action. This is useful when having updating the shutter state from an external source. 
 stop | Stop the shutter 
 toggle | Invert shutter state 
 up | Open the shutter 
 
## More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/750-1504


# WOVoletSmart
#### Alias: WagoOutputShutterSmart
Smart shutter using wago digital output modules (like 750-1504, ...)


Smart shutter. This shutter calculates the position of the shutter based on the time it takes to open and close. It then allows to set directly the shutter at a specified position.

## Parameters of WOVoletSmart
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
host |  | string | yes | Wago PLC IP address on the network
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
impulse_time |  | int | no | Impulse time for shutter that needs impulse instead of holding up/down relays. If set to 0 impulse shutter is disabled. Time is in ms. Default to 0
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
knx |  | bool | no | Set to true if output is a KNX device (only for 750-849 with KNX/TP1 module)
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
name | Name | string | yes | Name of Input/Output.
port |  | int | no | Wago ethernet port, default to 502
stop_both |  | bool | no | If in impulse mode, some shutters needs to activate both up dans down relays when stopping the shutter
time_down |  | int | yes | Time in sec for shutter to fully closed. The more accurate, the better it will work
time_up |  | int | yes | Time in sec for shutter to be fully open. The more accurate, the better it will work
var_down |  | int | yes | Digital output address on the PLC for closing the shutter
var_up |  | int | yes | Digital output address on the PLC for opening the shutter
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true
wago_841 |  | bool | yes | Should be false if PLC is 750-842, true otherwise

## Conditions of WOVoletSmart
Name | Description
---- | -----------
changed | Event on any change of shutter state 
 false | Event when shutter is closed 
 true | Event when shutter is open 
 
## Actions of WOVoletSmart
Name | Description
---- | -----------
calibrate | Start calibration on shutter. This opens fully the shutter and resets all internal position values. Use this if shutter sync is lost. 
 down | Close the shutter 
 down 5 | Close the shutter by X percent 
 impulse down 200 | Close shutter for X ms 
 impulse up 200 | Open shutter for X ms 
 set 50 | Set shutter at position X in percent 
 set_state 50 | Update internal shutter position, in percent, without starting real action. This is useful when having updating the shutter state from an external source. 
 stop | Stop the shutter 
 toggle | Invert shutter state 
 up | Open the shutter 
 up 5 | Open the shutter by X percent 
 
## More Infos
* Calaos Wiki: http://calaos.fr/wiki/fr/750-1504


# Climate proxy
A climate appliance that regulates by itself (heat pump, ESPHome climate): Calaos reads its state and relays commands, it does not regulate.


Each link role reads the io of its link and writes its command_io, or the io itself when that is an output. A role that cannot be written is not offered.

Device of family `climate`, impl `proxy`.

## Parameters of Climate proxy
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
auto_payload (calaos:param) | Auto payload | list | no | What is written to the mode link for auto. Without it, the spelling the appliance last reported, else heat_cool.
changeover_delay (calaos:param) | Changeover delay | float | no | With emulate_range: seconds between the end of heat and the start of cool, or the reverse, at least min_off_time.
emulate_range (calaos:param) | Emulated dual setpoint | bool | no | For a unit with a single setpoint: the device keeps target_temp_low and target_temp_high itself (internal IOs, created when missing) and offers auto. In auto the unit runs in heat on the low setpoint below the range, in cool on the high setpoint above it, and is off in between. Needs a target_temp link and heat and cool among the modes. The room temperature is remote_temp_source, else current_temp; while it is stale the unit is left as it is. A mode changed on the unit itself suspends auto until it is selected again. auto is never sent to the unit.
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
fan_modes (calaos:param) | Fan modes | string | no | Comma separated fan modes the appliance accepts. Without it every fan_mode command is refused.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
hysteresis (calaos:param) | Hysteresis | float | no | With emulate_range: degrees past the range before the unit starts, in heat below the low setpoint minus hysteresis, in cool above the high setpoint plus hysteresis. It stops as soon as the room is back in the range.
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
impl | Implementation | string | yes | Implementation of the device family: the key of this document in devices.
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
min_off_time (calaos:param) | Minimum off time | float | no | With emulate_range: seconds the unit stays off at least before auto starts it again.
min_on_time (calaos:param) | Minimum on time | float | no | With emulate_range: seconds the unit runs at least in heat or cool before auto stops it. A mode selected by the user is applied at once.
min_range (calaos:param) | Minimum range | float | no | Minimum gap in degrees between target_temp_low and target_temp_high. A range closer than that is refused.
modes (calaos:param) | Modes | string | no | Comma separated modes the device offers (off, heat, cool, auto, dry, fan_only). Default to all of them.
name | Name | string | yes | Name of Input/Output.
readback_hold (calaos:param) | Readback hold | float | no | Seconds a range bound just commanded is trusted before its readback confirms it, so two quick partial commands are validated together. A number of seconds above 0, else the default is used.
remote_temp_keepalive (calaos:param) | Remote temperature refresh | float | no | Seconds between two relays of the remote_temp_source value to remote_temp, on top of each change. The appliance falls back to its own sensor when updates stop. A number of seconds above 0, else the default is used.
sensor_timeout (calaos:param) | Sensor timeout | float | no | Seconds a linked sensor may go without a reading, a repeated value included, before it counts as stale. A positive number; the default outlasts one lost hourly Zigbee report. A sensor older than half of it is asked for a reading when its protocol can (KNX).
swing_modes (calaos:param) | Swing modes | string | no | Comma separated swing modes the appliance accepts. Without it every swing_mode command is refused.
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true

## Link roles of Climate proxy
Writable: the device writes this link.

Role | Label | Value | Required | Writable | Description
---- | ----- | ----- | -------- | -------- | -----------
mode | Mode | string | yes | yes | Mode of the appliance. Needs a command_io unless its io is an output. Its availability is the appliance's: connected false in the state.
current_temp | Current temperature | number | no | no | Temperature the appliance measures.
target_temp | Setpoint | number | no | yes | Setpoint used in heat and cool.
target_temp_low | Low setpoint | number | no | yes | Low bound of the auto range. Both bounds are needed for a range.
target_temp_high | High setpoint | number | no | yes | High bound of the auto range. Both bounds are needed for a range.
action | Action | string | no | no | What the appliance is doing: off, idle, heating, cooling, drying, fan.
fan_mode | Fan mode | string | no | yes | Fan mode. Needs the fan_modes param.
swing_mode | Swing mode | string | no | yes | Swing mode. Needs the swing_modes param.
remote_temp | Remote temperature | number | no | yes | Where the temperature of another sensor is written, for appliances that take one. Needs remote_temp_source.
humidity | Humidity sensor | number | no | no | Relative humidity the appliance, or a room sensor, reports: shown in the state.
remote_temp_source | Room sensor | number | no | no | The sensor relayed to remote_temp. Needs remote_temp. Not relayed before it has read since the start, nor while it has sent nothing for sensor_timeout or reports itself disconnected: the appliance then falls back to its own sensor.

## Command of Climate proxy
The value is a mode (`off`) or a JSON object holding any of these fields, e.g. `{"mode":"heat","target_temp":21.5}`. A field is only applied when all its links exist and are writable on the instance.

Name | Type | Values or bounds | Requires links | Description
---- | ---- | ---------------- | -------------- | -----------
mode | enum | off, heat, cool, auto, dry, fan_only | mode | Operating mode. The modes an instance accepts are published in its capabilities.
target_temp | number | 7 to 35, step 0.5 °C | target_temp | The setpoint used in heat and cool. Bounds are those of the linked IO (value_min, value_max, step, unit), else 7 to 35 by 0.5 in °C. The effective ones are published in the capabilities.
target_temp_low | number | 7 to 35, step 0.5 °C | target_temp_low, target_temp_high | Low bound of the range used in auto. Bounds are those of the linked IO (value_min, value_max, step, unit), else 7 to 35 by 0.5 in °C. The effective ones are published in the capabilities.
target_temp_high | number | 7 to 35, step 0.5 °C | target_temp_low, target_temp_high | High bound of the range used in auto. Bounds are those of the linked IO (value_min, value_max, step, unit), else 7 to 35 by 0.5 in °C. The effective ones are published in the capabilities.
fan_mode | enum | values of the fan_modes param | fan_mode | Fan mode, one of the fan_modes param.
swing_mode | enum | values of the swing_modes param | swing_mode | Swing mode, one of the swing_modes param.


# Climate thermostat
A thermostat that regulates in Calaos: it switches output_heat and output_cool from current_temp and its setpoints. Its mode is stored by the device.


Night setback, away mode and the like are rules writing the setpoints or the mode. Missing setpoint and action IOs are created at load.

Device of family `climate`, impl `thermostat`.

## Parameters of Climate thermostat
Name | Label | Type | Mandatory | Description
---- | ----- | ---- | --------- | -----------
emitter (calaos:param) | Emitter | list | no | What the outputs drive. Only floor has an effect: with a humidity link, floor cooling then stops at humidity_max (dew point).
enabled | Enabled | bool | no | Enable the Input/Output. The default value is true. This parameter is added if it's not found in the configuration.
gui_type | Display type | string | no | Internal graphical type for all calaos objects. Set automatically, read-only parameter.
humidity_max (calaos:param) | Maximum humidity | float | no | Relative humidity (%) from which floor cooling stops, with emitter=floor. Not negative.
hysteresis (calaos:param) | Hysteresis | float | no | Degrees past the setpoint before an output starts switching on: heat below the setpoint minus hysteresis, cool above the setpoint plus hysteresis. Switching off comes as soon as the setpoint is crossed.
id | Identifier | string | yes | Unique ID identifying the Input/Output in calaos-server
impl | Implementation | string | yes | Implementation of the device family: the key of this document in devices.
io_type | IO type | string | yes | IO type, can be "input", "output", "inout"
log_history | History log | bool | no | If enabled, write an entry in the history event log for this IO
logged | InfluxDB | bool | no | If enabled, and if influxdb is enabled in local_config send the value to influxdb for this IO
min_off_time (calaos:param) | Minimum off time | float | no | Seconds an output stays off at least before it can start again.
min_on_time (calaos:param) | Minimum on time | float | no | Seconds an output stays on at least, except when the mode is off or a safety cut stops it.
min_range (calaos:param) | Minimum range | float | no | Minimum gap in degrees between target_temp_low and target_temp_high. A range closer than that is refused.
modes (calaos:param) | Modes | string | no | Comma separated modes the thermostat offers, among heat, cool and auto; off is always offered. Without it, the outputs decide: heat with output_heat, cool with output_cool, auto with both, and heat alone with output. A mode the outputs cannot run, or an unknown one, is ignored.
name | Name | string | yes | Name of Input/Output.
put_back_delay (calaos:param) | Put-back delay | float | no | Seconds before an output that something else keeps switching is put back; the wait doubles while the other writer keeps undoing it, up to put_back_delay_max. At least 0.001, else the default is used.
put_back_delay_max (calaos:param) | Maximum put-back delay | float | no | Longest wait between two put-backs, in seconds. Not below put_back_delay. It also sets the window of the log of writes from outside to an output: one line per source and output during that time.
sensor_timeout (calaos:param) | Sensor timeout | float | no | Seconds a linked sensor may go without a reading, a repeated value included, before it counts as stale. A positive number; the default outlasts one lost hourly Zigbee report. A sensor older than half of it is asked for a reading when its protocol can (KNX).
valid_max (calaos:param) | Highest valid temperature | float | no | Highest plausible reading of current_temp, in the unit of the sensor. A reading above it is treated as no reading, like valid_min. No check when empty. Not below valid_min.
valid_min (calaos:param) | Lowest valid temperature | float | no | Lowest plausible reading of current_temp, in the unit of the sensor. A reading below it is treated as no reading: the outputs go OFF at once, whatever min_on_time, until a reading is back within the bounds. No check when empty.
visible | Visible | bool | no | Display the Input/Output on all user interfaces if set. Default to true

## Link roles of Climate thermostat
Writable: the device writes this link.

Role | Label | Value | Required | Writable | Description
---- | ----- | ----- | -------- | -------- | -----------
current_temp | Current temperature | number | yes | no | Temperature the regulation reads. The outputs stay off until it has delivered a reading since the start (a cached or retained value does not count), while it has sent nothing for sensor_timeout, reports itself disconnected or reads outside valid_min..valid_max.
target_temp | Setpoint | number | yes | yes | Setpoint used in heat and cool. Created when missing.
target_temp_low | Low setpoint | number | no | yes | Low bound of the auto range. Created when missing, with output_cool.
target_temp_high | High setpoint | number | no | yes | High bound of the auto range. Created when missing, with output_cool.
action | Action | string | no | yes | Written by the thermostat: off, idle, heating or cooling. Created when missing.
output_heat | Heating output | bool | yes, or output | yes | Output driven for heating. Enables heat. An output belongs to a single thermostat: a second one linking it ignores the link.
output_cool | Cooling output | bool | no | yes | Output driven for cooling. Enables cool and auto. An output belongs to a single thermostat: a second one linking it ignores the link.
output | Zone valve | bool | no | yes | Valve or relay of the zone, ON for any demand, heating or cooling: the mode gives the direction. Instead of output_heat and output_cool, which cannot be linked with it; the modes param says what it offers. An output belongs to a single thermostat: a second one linking it ignores the link. Replaces output_heat, output_cool.
humidity | Humidity sensor | number | no | no | Relative humidity of the room. With emitter=floor, cooling stops while it reads humidity_max or more, has not read, is stale or disconnected. Without it, a floor cools with no dew-point cut.

## Command of Climate thermostat
The value is a mode (`off`) or a JSON object holding any of these fields, e.g. `{"mode":"heat","target_temp":21.5}`. A field is only applied when all its links exist and are writable on the instance.

Name | Type | Values or bounds | Requires links | Description
---- | ---- | ---------------- | -------------- | -----------
mode | enum | off, heat (needs output_heat), cool (needs output_cool), auto (needs output_heat + output_cool) |  | Operating mode. The modes an instance accepts are published in its capabilities. Heat needs the output_heat link, cool needs output_cool and auto needs both; with the output link, the modes param says which ones.
target_temp | number | 7 to 35, step 0.5 °C | target_temp | The setpoint used in heat and cool. Bounds are those of the linked IO (value_min, value_max, step, unit), else 7 to 35 by 0.5 in °C. The effective ones are published in the capabilities.
target_temp_low | number | 7 to 35, step 0.5 °C | target_temp_low, target_temp_high | Low bound of the range used in auto. Bounds are those of the linked IO (value_min, value_max, step, unit), else 7 to 35 by 0.5 in °C. The effective ones are published in the capabilities.
target_temp_high | number | 7 to 35, step 0.5 °C | target_temp_low, target_temp_high | High bound of the range used in auto. Bounds are those of the linked IO (value_min, value_max, step, unit), else 7 to 35 by 0.5 in °C. The effective ones are published in the capabilities.

