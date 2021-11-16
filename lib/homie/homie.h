#pragma once
#include <Arduino.h>

#include <functional>
#include <map>
#include <string>

#include "mqtt_client.h"

class Device;
class Node;
class Property;

class Homie {
 public:
    explicit Homie(MqttClient* mqtt_client);
    ~Homie() { Serial.println("Homie destroyed"); }

    void SetDevice(Device* device);

    bool Init(String user_hash, String host, uint16_t broker_port, String token, MQTT_CALLBACK_SIGNATURE);

    bool Publish(const Device& device, String attribute, String value, bool retained);      // for device
    bool Publish(const Node& node, String attribute, String value, bool retained);          // for node
    bool Publish(const Property& property, String attribute, String value, bool retained);  // for property

    bool SubscribeTopic(const Property& property);

    void HandleMessage(String topic, byte* payload, unsigned int length);

    void HandleCurrentState();

    bool IsConnected();

 private:
    const uint16_t kDelayForReconnectHomie = 5 * 1000;  // 5 sec

    uint32_t reinit_homie_time_ = 0;

    String user_hash_;

    MqttClient* mqtt_client_ = nullptr;
    Device* device_ = nullptr;

    String GetTopicForEntity(const Device& device, String attribute);  // for device
    String GetTopicForEntity(const Node& Node, String attribute);      // for node
    String GetTopicForEntity(const Property& property);                // for property
    String GetValueTopicForEntity(Property* property);
};
