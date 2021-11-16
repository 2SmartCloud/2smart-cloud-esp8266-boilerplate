# WebServer

A class for working with a web server.

***

## API

- void WebServerInit()
- void WebServerHandle()


***

**bool WebServerInit()**

Initializes a web server. The method sets up handlers for the following routes:

**GET** /healthcheck  
Returns the 200 code.

**GET** /pair  
A method that saves new parameters from a mobile application. Reloads the ESP to apply the changes.
Query params:
  - ssid - user's email address.
  - psk - user's password.
  - wsp - MQTT broker's port.
  - token - user's password.
  - host - the address where the MQTT broker is installed.
  - brport - MQTT broker's port.

***

