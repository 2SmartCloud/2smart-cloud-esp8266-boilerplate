#include "mqtt_client.h"

#include "utils.h"

MqttClient::MqttClient() { CreateClient(); }

void MqttClient::CreateClient() {
    this->wifi_client_ = new WiFiClient;
    this->client_ = new PubSubClient(*wifi_client_);
}

bool MqttClient::Init(String username, String host, uint16_t port, String password, MQTT_CALLBACK_SIGNATURE) {
    username.toCharArray(username_, username.length() + 1);
    host.toCharArray(host_, host.length() + 1);

    this->port_ = port;
    password.toCharArray(password_, password.length() + 1);

    client_->setServer(this->host_, this->port_);
    client_->setCallback(callback);
    session_id_.append(RandomString(millis(), kSessionIdHashSize));
    return (this->Reconnect());
}

bool MqttClient::Reconnect() {
    if (!strcmp(this->username_, "")) return false;
    if (millis() - reconnect_mqtt_time_ < kDelayForReconnectMQTT_) return false;
    if (!client_->connected()) {
        if (client_->connect(session_id_.c_str(), this->username_, this->password_)) {
            Serial.println("MQTT connected");
            if (need_reconnect_) mqtt_reconnected_ = true;
            need_reconnect_ = false;
        } else {
            Serial.printf("failed mqtt, try again in %d ms\r\n", kDelayForReconnectMQTT_);
            need_reconnect_ = true;
            reconnect_mqtt_time_ = millis();
        }
    } else {
        if (need_reconnect_) mqtt_reconnected_ = true;
        need_reconnect_ = false;
    }
    return (!need_reconnect_);
}

bool MqttClient::Subscribe(const char* topic) {  // susbcribe 1 topic
    return client_->subscribe(topic);
}

bool MqttClient::Publish(const char* topic, const char* payload, bool retained = 1) {  // publish 1 topic
    if (!client_->connected()) {
        return false;
    }
    if (client_->publish(topic, payload, retained)) {
        return true;
    } else {
        Serial.printf("Err publishing %s\r\n", topic);
        return false;
    }
}

void MqttClient::MqttLoop() {
    if (!client_->connected()) Reconnect();
    client_->loop();
}

bool MqttClient::IsConnected() { return client_->connected(); }

bool MqttClient::IsReconnected() {
    if (mqtt_reconnected_) {
        mqtt_reconnected_ = false;
        return true;
    }
    return false;
}
