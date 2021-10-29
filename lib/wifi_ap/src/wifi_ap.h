#pragma once
#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif

class WifiAp {
 public:
    WifiAp();
    ~WifiAp() { Serial.println("WifiAp destroyed"); }

    void Start(String ssid_name);

    void Blink();

 private:
    const uint8_t kLedStatus_ = 13;              // esp32 2 // sonoff 13
    const uint16_t kBlinkingTimeAPMode_ = 1000;  // once in N msec
    uint32_t last_blink_AP_ = 0;                 // millis();
};
