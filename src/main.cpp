#include "main.h"

#include "custom_nodes/auto_upd_firmw/auto_upd_firmw.h"
#include "custom_nodes/notifications/notifications.h"
#include "custom_nodes/relay/property/relay_state.h"
#include "file_system.h"
#include "gpio.h"
#include "homie.h"
#include "http_handler.h"
#include "mqtt_сlient.h"
#include "notifier/notifier.h"
#include "ntp_time_client/ntp_time_client.h"
#include "wifi_ap.h"
#include "wifi_client.h"

MqttClient *mqtt_client = new MqttClient();
Homie homie(mqtt_client);
Notifier notifier(mqtt_client);
Device device(&homie);
NtpTimeClient *time_client = new NtpTimeClient();

WifiClient wifi_client;

#define DEBUG_LOGI

void setup() {
    Serial.begin(115200);
    // setGpios();
    delay(1000);
    if (!InitFiles() || !LoadConfig()) {
        delay(5000);
        ESP.restart();
    }

    String mac = WiFi.macAddress().c_str();
    if (!strcmp(device_id, "")) {
        String bufferMacAddr = WiFi.macAddress();
        bufferMacAddr.toLowerCase();
        bufferMacAddr.replace(":", "-");
        snprintf(device_id, sizeof(device_id), "%s", bufferMacAddr.c_str());
    }
    String ip_addr = WiFi.localIP().toString();

    // ---------------------------------------------- Homie convention init

    AutoUpdateFw *firmware = new AutoUpdateFw("Firmware", "firmware", &device);                   // (name, id, device)
    Notifications *notifications = new Notifications("Notifications", "notifications", &device);  // (name, id, device)
    Node *relay = new Node("Relay", "relay", &device);                                            // (name, id,device)

    Property *update_status = new Property("update status", "updatestate", firmware, SENSOR, false, false, "string");
    Property *update_button = new Property("update button", "update", firmware, SENSOR, true, false, "boolean");
    Property *update_time = new Property("update time", "updatetime", firmware, SENSOR, true, true, "string");
    Property *auto_update = new Property("autoUpdate", "autoupdate", firmware, SENSOR, true, true, "boolean");
    Property *fw_version = new Property("version", "version", firmware, SENSOR, false, true, "integer");
    // -------------notification`s properties
    Property *system_notification =
        new Property("System Notifications", "system", notifications, SENSOR, true, true, "boolean");
    Property *update_notification =
        new Property("Update Notifications", "update", notifications, SENSOR, true, true, "boolean");

    RelayState *relay_state = new RelayState("State", "state", relay, SENSOR, true, true, "boolean");

    DeviceData device_data{device_name, device_version, product_id, ip_addr.c_str(),
                           "esp8285",     mac.c_str(),    "ready",    device_id};
    notifier.SetUserHash(person_id);

    device.SetCredentials(device_data);
    device.SetNotifier(&notifier);

    Property *dev_ip = new Property("ipw", "ipw", &device, TELEMETRY, false, true, "string");

    device.AddProperty(dev_ip);

    firmware->AddProperty(fw_version);
    firmware->AddProperty(update_status);
    firmware->AddProperty(update_button);
    firmware->AddProperty(update_time);
    firmware->AddProperty(auto_update);
    firmware->SetTimeClient(time_client);
    device.AddNode(firmware);

    notifications->AddProperty(system_notification);
    notifications->AddProperty(update_notification);
    device.AddNode(notifications);

    relay->AddProperty(relay_state);
    device.AddNode(relay);

    homie.SetDevice(&device);

    WifiAp wifiAP;
    if (!strcmp(ssid_name, "")) {
        WiFi.softAP(device_name);
        WebServerInit();

        while (true) {
            device.HandleCurrentState();
            delay(1);
            wifiAP.Blink();
            WebServerHandle();
        }
        wifiAP.Start(device_name);
    }
    while (!strcmp(ssid_name, "")) {
        // Handling buttons and offline logic
        device.HandleCurrentState();
        wifiAP.Blink();
    }

    wifi_client.SetCredentials(ssid_name, ssid_password);
    while (!wifi_client.Connect()) {
        // Handling buttons and offline logic
        device.HandleCurrentState();
        if (erase_flag) {
            EraseFlash();
        }
        delay(1);
    }
    time_client->Init();

    ip_addr = WiFi.localIP().toString();
    Serial.print("IP: ");
    Serial.println(ip_addr);

    while (!homie.Init(person_id, host, broker_port, token, HandleMessage)) {
        device.HandleCurrentState();
    }
    dev_ip->SetValue(ip_addr);
    Serial.println("8266 space");
    Serial.printf("[httpUpdate]  - free Space: %d\n", ESP.getFreeSketchSpace());
    Serial.printf("[httpUpdate]  - current Sketch Size: %d\n", ESP.getSketchSize());

    // ---------------------------------------------- Homie convention end
}

void loop() {
    wifi_client.Connect();

    homie.HandleCurrentState();  // mqttLoop();

    if (erase_flag) {
        EraseFlash();
    }
}

void HandleMessage(char *topic, byte *payload, unsigned int length) {
    Serial.println("mess hendled");
    homie.HandleMessage(String(topic), payload, length);
}
