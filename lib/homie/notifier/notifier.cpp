#include "notifier.h"

Notifier::Notifier(MqttClient* mqtt_client) {
    mqtt_client_ = mqtt_client;
    Serial.println("Notifier object init");
}

void Notifier::SetUserHash(const char* user_hash) {
    if (strlen(user_hash) == kUserHashLength) snprintf(user_hash_, sizeof(user_hash_), "%s", user_hash);
}

bool Notifier::CreateNotification(const char* text) {
    if (strlen(user_hash_) == kUserHashLength) {
        String topic = user_hash_;
        topic += "/notifications/create";
        return mqtt_client_->Publish(topic.c_str(), text, false);
    }
    return false;
}
