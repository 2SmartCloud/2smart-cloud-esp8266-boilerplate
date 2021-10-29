#include <http_handler.h>

#include "file_system.h"
#include "utils.h"
ESP8266WebServer http_(kPort_);

void SetupWebServer();

void HandleHealthcheck();
void HandlePair();

void WebServerInit() { SetupWebServer(); }
void WebServerHandle() { http_.handleClient(); }

void SetupWebServer() {
    http_.on("/pair", HandlePair);
    http_.on("/healthcheck", HandleHealthcheck);
    http_.begin();
    Serial.println("WebServer started!");
}

void HandleHealthcheck() {
    http_.send(200, "text / plain", "OK");  // Oтправляем ответ Reset OK
}

void HandlePair() {
    if (!http_.hasArg("ssid") || !http_.hasArg("psk") || !http_.hasArg("wsp") || !http_.hasArg("token") ||
        !http_.hasArg("host") || !http_.hasArg("brport")) {
        http_.send(400, "text/plain", "Incorrect data");
        return;
    }
    snprintf(ssid_name, sizeof(ssid_name), "%s", http_.arg("ssid").c_str());
    snprintf(ssid_password, sizeof(ssid_password), "%s", http_.arg("psk").c_str());
    snprintf(person_mail, sizeof(person_mail), "%s", http_.arg("wsp").c_str());
    snprintf(token, sizeof(token), "%s", http_.arg("token").c_str());
    snprintf(host, sizeof(host), "%s", http_.arg("host").c_str());
    broker_port = http_.arg("brport").toInt();

    String devId = WiFi.macAddress();
    devId.toLowerCase();
    devId.replace(":", "-");
    snprintf(device_id, sizeof(device_id), "%s", devId.c_str());
    snprintf(person_id, sizeof(person_id), "%s", Sha_256(person_mail).c_str());

    if (!strcmp(ssid_name, "")) {
        http_.send(400, "text/plain", "Wifi name is NULL");
        return;
    }
    if (!SaveConfig()) {
        http_.send(500, "text/plain", "Server error");
        return;
    }

    http_.send(200, "text/plain", "OK");
    delay(kResponseDelay_);
    ESP.restart();
}
