#include "wifi_ap.h"

WifiAp::WifiAp() {
    pinMode(kLedStatus_, OUTPUT);
    digitalWrite(kLedStatus_, LOW);
}

void WifiAp::Start(String ssid_name) {
    WiFi.softAP(ssid_name);
    return;
    if (WiFi.softAP(ssid_name.c_str())) {
        IPAddress IP = WiFi.softAPIP();
        Serial.print("AP IP address: ");
        Serial.println(IP);
        Serial.print("Wifi is :");
        Serial.println(WiFi.softAPSSID());
    } else {
        Serial.println("AP point failed");
    }
}

void WifiAp::Blink() {
    if (millis() - last_blink_AP_ > kBlinkingTimeAPMode_) {
        Serial.print("/");
        digitalWrite(kLedStatus_, !digitalRead(kLedStatus_));
        last_blink_AP_ = millis();
    }
}
