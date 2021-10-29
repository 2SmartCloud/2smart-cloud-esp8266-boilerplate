# Homie boilerplate

## Structure

1. [Homie](lib/homie/README.md) - a set of tools for controlling devices and their state.
2. [Device](lib/homie/device/README.md) - tool for creating/controlling a device.
3. [Node](lib/homie/node/README.md) - tool for creating/controlling nodes.
4. [Property](lib/homie/property/README.md) - tool for creating/controlling properties.


## Description

The protocol in the project is based on the convention [homie.io](https://homieiot.github.io/specification/spec-core-v3_0_1/) of the **3.0.1** version.

### Root topic in 2smart

sweet-home/#

### Extension of the protocol

1.  The device has a group of attributes `$options`, which in format repeats the `$properties` from the node. Used to store device settings, both editable and not. Example: `sweet-home/device/$options/some-option`.
2.  The device has a group of attributes `$telemetry`, which in format repeats the `$properties` from the node. Used to store various telemetry and statistics. `$stats` is not used. Example: `sweet-home/device/$telemetry/some-telemetry`.
3.  A node has a `$state` attribute, similar to a device. Example: `sweet-home/device/node/$state`.
4.  The node has a group of attributes `$options`, similar to the device. Example: `sweet-home/device/node/$options/some-option`.
5.  The node has a group of attributes `$telemetry`, similar to the device. Example: `sweet-home/device/node/$telemetry/some-telemetry`.
6.  A mechanism for validating the set values has been added to the application:
    *  On the received message, in case of incorrect values, the error body should be sent to a topic like `errors/sweet-home/device/node...` The error body should be in the JSON format of the string "{" code ": 'error_code'," message ": 'error message'}". (For example, if an incorrect value has come to the topic `sweet-home/device/node/sensor/set`, then the error should be sent to the topic `errors/sweet-home/device/node/sensor`)
     *  Valid for any editable values (`$properties`,`$options`, `$telemetry`).

7.  Device and node indicators are taken from the following attributes:
     *  Status (state) - `$state`.
     *  Signal strength - `$telemetry/signal`.
     *  Battery charge level - `$telemetry/battery`.


### Required state for creating an entity in an application

* For device: `$homie`, `$name`, `$localip`, `$mac`, `$fw/name`, `$fw/version`, `$implementation`, `$state`.
* For node: `$name`, `$state` plus at least one property.
* For properties: (`$properties`, `$options`, `$telemetry`): `$name`.


### Using in interfaces

*  `$options`, `$telemetry` for a device and properties, `$options`, `$telemetry` for a node are displayed in separate tabs in the same way. Any attribute from any group can be editable. If `$settable=true`, then a controller should be displayed next to the field, the type of which depends on `$datatype`.

*  Homieiot does not support device and node configuration, which is why we add `$options`. It is similar to Properties so that it is easy to add new parameters and reuse controllers to control them.

*  Homieiot has `$stats`, but the set of metrics is fixed and not self-descriptive. Those. it is impossible not only to add a new metric, but for the existing ones it is necessary to flash the field names, their type, etc. in the interfaces. Therefore, we added `$telemetry` and we also do them according to the principle of Properties.


### Device heartbeat 

The device must publish a non-retain message to the broker in a dedicated topic so that the system can understand if the device is working.
The device has an attribute `$heartbeat`, where the device should send a message any message, for example - 'ping', with a frequency of 10 seconds

Example: `sweet-home/device/$heartbeat`


### Device example in MQTT

```
{
    "sweet-home/weather-station/$name": "Weather",
    "sweet-home/weather-station/$fw/name": "#",
    "sweet-home/weather-station/$fw/version": "#",
    "sweet-home/weather-station/$localip": "#",
    "sweet-home/weather-station/$mac": "#",
    "sweet-home/weather-station/$implementation": "#",
    "sweet-home/weather-station/$state": "ready",
    "sweet-home/weather-station/$options": "location",
    "sweet-home/weather-station/$options/location": "Kyiv",
    "sweet-home/weather-station/$options/location/$name": "Location",
    "sweet-home/weather-station/$options/location/$settable": "true",
    "sweet-home/weather-station/$options/location/$retained": "true",
    "sweet-home/weather-station/$options/location/$datatype": "string",
    "sweet-home/weather-station/$options/location/$unit": "#",
    "sweet-home/weather-station/$nodes": "thermometer,sky-indicator,vane",
    "sweet-home/weather-station/thermometer/$name": "Thermometer",
    "sweet-home/weather-station/thermometer/$type": "#",
    "sweet-home/weather-station/thermometer/$state": "ready",
    "sweet-home/weather-station/thermometer/$properties": "temperature-sensor",
    "sweet-home/weather-station/thermometer/temperature-sensor": "-9",
    "sweet-home/weather-station/thermometer/temperature-sensor/$name": "Current temperature",
    "sweet-home/weather-station/thermometer/temperature-sensor/$settable": "false",
    "sweet-home/weather-station/thermometer/temperature-sensor/$retained": "true",
    "sweet-home/weather-station/thermometer/temperature-sensor/$datatype": "float",
    "sweet-home/weather-station/thermometer/temperature-sensor/$unit": "°C",
    "sweet-home/weather-station/thermometer/$options": "desc",
    "sweet-home/weather-station/thermometer/$options/desc": "Temperature in Kyiv: -9°C",
    "sweet-home/weather-station/thermometer/$options/desc/$name": "Description",
    "sweet-home/weather-station/thermometer/$options/desc/$settable": "false",
    "sweet-home/weather-station/thermometer/$options/desc/$retained": "true",
    "sweet-home/weather-station/thermometer/$options/desc/$datatype": "string",
    "sweet-home/weather-station/thermometer/$options/desc/$unit": "#",
    "sweet-home/weather-station/sky-indicator/$name": "Sky indicator",
    "sweet-home/weather-station/sky-indicator/$type": "#",
    "sweet-home/weather-station/sky-indicator/$state": "ready",
    "sweet-home/weather-station/sky-indicator/$properties": "sky-sensor",
    "sweet-home/weather-station/sky-indicator/sky-sensor": "Partly Cloudy",
    "sweet-home/weather-station/sky-indicator/sky-sensor/$name": "Current sky state",
    "sweet-home/weather-station/sky-indicator/sky-sensor/$settable": "false",
    "sweet-home/weather-station/sky-indicator/sky-sensor/$retained": "true",
    "sweet-home/weather-station/sky-indicator/sky-sensor/$datatype": "string",
    "sweet-home/weather-station/sky-indicator/sky-sensor/$unit": "#",
    "sweet-home/weather-station/vane/$name": "Wind indicator",
    "sweet-home/weather-station/vane/$type": "#",
    "sweet-home/weather-station/vane/$state": "ready",
    "sweet-home/weather-station/vane/$properties": "speed-indicator,direction-indicator",
    "sweet-home/weather-station/vane/speed-indicator": "9",
    "sweet-home/weather-station/vane/speed-indicator/$name": "Wind speed",
    "sweet-home/weather-station/vane/speed-indicator/$settable": "false",
    "sweet-home/weather-station/vane/speed-indicator/$retained": "true",
    "sweet-home/weather-station/vane/speed-indicator/$datatype": "integer",
    "sweet-home/weather-station/vane/speed-indicator/$unit": "km/h",
    "sweet-home/weather-station/vane/direction-indicator": "250",
    "sweet-home/weather-station/vane/direction-indicator/$name": "Wind direction",
    "sweet-home/weather-station/vane/direction-indicator/$settable": "false",
    "sweet-home/weather-station/vane/direction-indicator/$retained": "true",
    "sweet-home/weather-station/vane/direction-indicator/$datatype": "integer",
    "sweet-home/weather-station/vane/direction-indicator/$unit": "°"
}
```

## To Do
* [ ] TLS support
* [ ] web configuration panel
