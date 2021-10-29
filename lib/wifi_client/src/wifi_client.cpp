#include "wifi_client.h"

void WifiClient::SetCredentials(String ssid_name, String ssid_password) {
    ssid_name_ = ssid_name;
    ssid_password_ = ssid_password;
    pinMode(kLedStatus_, OUTPUT);
    digitalWrite(kLedStatus_, LOW);
}

bool WifiClient::Connect() {
    if (WiFi.status() != WL_CONNECTED) {  // If not connected
        if (millis() - wifi_reconnect_time_ > kWifiReconnectDelay_) {
            Serial.println("try to connect");
            WiFi.mode(WIFI_STA);
            WiFi.begin(ssid_name_.c_str(), ssid_password_.c_str());
            Serial.println(ssid_name_);
            Serial.println(ssid_password_);
            wifi_reconnect_time_ = millis();
        } else if (millis() - kBlinkingPairingDelay_ > last_blink_time_) {
            last_blink_time_ = millis();
            digitalWrite(kLedStatus_, !digitalRead(kLedStatus_));
        }
        return false;
    } else {
        delay(1);
    }
    digitalWrite(kLedStatus_, false);
    return true;
}
