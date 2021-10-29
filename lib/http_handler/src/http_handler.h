
#pragma once
#include <ESP8266WebServer.h>

#include "device/device.h"

extern const char *http_username;

extern char ssid_name[33];
extern char ssid_password[65];
extern char person_mail[65];
extern char person_id[65];
extern char token[11];
extern char host[65];
extern uint16_t broker_port;
extern char device_id[32];

extern const char *firmware_name;

void WebServerInit();
void WebServerHandle();

const uint8_t kPort_ = 80;
const uint16_t kResponseDelay_ = 500;  // delay before reboot
