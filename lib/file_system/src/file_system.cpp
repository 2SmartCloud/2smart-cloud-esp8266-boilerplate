#include "file_system.h"

bool InitFiles() {
    if (!LittleFS.begin()) {
        Serial.println("An Error has occurred while mounting LittleFS");
        return false;
    }

    File root = LittleFS.open("/", "r");
    File file = root.openNextFile();

    while (file) {
        file = root.openNextFile();
    }
    return true;
}

bool LoadConfig() {
    UserData user_data = {"", "", "", "", "", "", 0, ""};

    if (!ReadSettings("/config.txt", reinterpret_cast<byte *>(&user_data), sizeof(user_data))) {
        EraseFlash();
    }

    if (strcmp(user_data.ssid_name, "")) snprintf(ssid_name, sizeof(ssid_name), "%s", user_data.ssid_name);
    if (strcmp(user_data.ssid_password, "")) snprintf(ssid_password, sizeof(ssid_password), "%s", user_data.ssid_password);
    if (strcmp(user_data.person_mail, "")) snprintf(person_mail, sizeof(person_mail), "%s", user_data.person_mail);
    if (strcmp(user_data.person_id, "")) snprintf(person_id, sizeof(person_id), "%s", user_data.person_id);
    if (strcmp(user_data.token, "")) snprintf(token, sizeof(token), "%s", user_data.token);
    if (strcmp(user_data.host, "")) snprintf(host, sizeof(host), "%s", user_data.host);
    if (strcmp(user_data.device_id, "")) snprintf(device_id, sizeof(device_id), "%s", user_data.device_id);
    if (user_data.broker_port) broker_port = user_data.broker_port;

    return true;
}

bool SaveConfig() {
    UserData user_data = {"", "", "", "", "", "", 0, ""};

    snprintf(user_data.ssid_name, sizeof(user_data.ssid_name), "%s", ssid_name);
    snprintf(user_data.ssid_password, sizeof(user_data.ssid_password), "%s", ssid_password);
    snprintf(user_data.person_mail, sizeof(user_data.person_mail), "%s", person_mail);
    snprintf(user_data.person_id, sizeof(user_data.person_id), "%s", person_id);
    snprintf(user_data.token, sizeof(user_data.token), "%s", token);
    snprintf(user_data.host, sizeof(user_data.host), "%s", host);
    user_data.broker_port = broker_port;
    snprintf(user_data.device_id, sizeof(user_data.device_id), "%s", device_id);

    return WriteSettings("/config.txt", reinterpret_cast<byte *>(&user_data), sizeof(user_data));
}

bool EraseFlash() {
    UserData user_data = {"", "", "", "", "", "", 0, ""};

    if (WriteSettings("/config.txt", reinterpret_cast<byte *>(&user_data), sizeof(user_data))) ESP.restart();
    return false;
}

bool WriteSettings(const char *file_path, uint8_t *data, size_t size_of_data) {
    File configFile = LittleFS.open(file_path, "w");
    if (!configFile) {
        Serial.printf("Failed to open %s for writing\r\n", file_path);
        return false;
    }
    configFile.write(data, size_of_data);
    configFile.close();
    Serial.printf("File %s saved\r\n", file_path);
    return true;
}

bool ReadSettings(const char *file_path, uint8_t *data, size_t size_of_data) {
    File configFile = LittleFS.open(file_path, "r");
    if (!configFile) {
        Serial.printf("Failed to open %s for reading\r\n", file_path);
        return false;
    }
    size_t size = configFile.size();
    if (size > 1024) {
        Serial.printf("file %s is too large \r\n", file_path);
        return false;
    }

    configFile.read(data, size_of_data);
    configFile.close();

    return true;
}
