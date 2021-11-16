#pragma once
#include <Arduino.h>
#include <PubSubClient.h>
#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif

#include <functional>
#include <map>
#include <string>

class MqttClient {
 public:
    MqttClient();
    ~MqttClient() { Serial.println("MqttClient destroyed"); }

    bool Init(String username, String host, uint16_t port, String password, MQTT_CALLBACK_SIGNATURE);

    void MqttLoop();
    bool Subscribe(const char *topic);
    bool Publish(const char *topic, const char *value, bool retained);

    bool IsConnected();
    bool IsReconnected();

 private:
    const uint16_t kDelayForReconnectMQTT_ = 5 * 1000;  // 5 sec

    const uint8_t kSessionIdHashSize = 15;

    bool need_reconnect_ = false;
    bool mqtt_reconnected_ = false;
    uint32_t reconnect_mqtt_time_ = 0;

    char username_[65] = "";
    char host_[65] = "";
    uint16_t port_ = 0;
    char password_[11] = "";
    std::string session_id_ = "session_";

    WiFiClient *wifi_client_;
    PubSubClient *client_;

    bool Reconnect();

    void CreateClient();
};
