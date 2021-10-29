#pragma once
#include <Arduino.h>

// ----------------------------------------------------------Variables--------------------
bool erase_flag = false;

// ----------------------------------------------------------HTTP-----------
char ssid_name[33] = "";      // WiFi name
char ssid_password[65] = "";  // WiFi password
char person_mail[65] = "";
char person_id[65] = "";
char token[11] = "";
char host[65] = "cloud.2smart.com";
uint16_t broker_port = 1883;
// -------------------------------------------------------Production settings
char device_id[32] = "";                      // DeviceID/ MAC:adress
const char* product_id = "Your Product ID";  // production id
// -------------------------------------------------------MQTT variables

const char* device_name = "2Smart Device";
const char* device_version = "1";

void HandleMessage(char* topic, byte* payload, unsigned int length);
