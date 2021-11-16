#pragma once
#include <Arduino.h>

#include "mqtt_client.h"

class Notifier {
 public:
    explicit Notifier(MqttClient* mqtt_client);
    ~Notifier() { Serial.println("Notifier destroyed"); }

    bool CreateNotification(const char* text);
    void SetUserHash(const char* user_hash);

 private:
    const uint8_t kUserHashLength = 64;

    char user_hash_[65] = "";

    MqttClient* mqtt_client_ = nullptr;
};
