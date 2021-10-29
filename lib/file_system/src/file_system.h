#pragma once
#include <Arduino.h>

#include "LittleFS.h"

typedef struct {
    char ssid_name[33];      // WiFi name
    char ssid_password[65];  // WiFi password
    char person_mail[65];
    char person_id[65];
    char token[11];
    char host[65];
    uint16_t broker_port;
    char device_id[32];
    char product_id[32];
} UserData;

extern char ssid_name[33];
extern char ssid_password[65];
extern char person_mail[65];
extern char person_id[65];
extern char token[11];
extern char host[65];
extern uint16_t broker_port;
extern char device_id[32];

extern bool erase_flag;

bool InitFiles();
bool LoadConfig();
bool SaveConfig();
bool EraseFlash();
bool WriteSettings(const char *file_path, uint8_t *data, size_t size_of_data);
bool ReadSettings(const char *file_path, uint8_t *data, size_t size_of_data);
bool WriteSandboxData();
