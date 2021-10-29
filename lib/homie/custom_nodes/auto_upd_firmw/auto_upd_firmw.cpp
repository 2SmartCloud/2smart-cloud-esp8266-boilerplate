#include "auto_upd_firmw.h"

#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
// #include <WiFiClientSecure.h>

#include "file_system.h"
#include "utils.h"

void AutoUpdateFw::UpdateFirmware() {
    if (WiFi.status() == WL_CONNECTED) {
        String firmware_state = fwStates_.find(FW_UPDATING)->second.c_str();
        properties_.find("updatestate")->second->SetValue(firmware_state);
        HTTPClient httpClient;

        String url = kUrlFirmwareFilePath_;
        url += product_id;
        url += ".bin";

        t_httpUpdate_return ret = ESPhttpUpdate.update(*wifi_client_, host, 80, url);

        delay(1000);

        switch (ret) {
            case HTTP_UPDATE_FAILED:
                Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s\n", ESPhttpUpdate.getLastError(),
                              ESPhttpUpdate.getLastErrorString().c_str());
                Serial.print(url);
                break;

            case HTTP_UPDATE_NO_UPDATES:
                Serial.println("HTTP_UPDATE_NO_UPDATES");
                break;

            case HTTP_UPDATE_OK:
                Serial.println("HTTP_UPDATE_OK");
                break;
        }
    }
}

AutoUpdateFw::AutoUpdateFw(const char* name, const char* id, Device* device) : Node(name, id, device) {}

bool AutoUpdateFw::Init(Homie* homie) {
    bool status = Node::Init(homie);
    LoadFwSettings();

    uint16_t fw_ver_actual = device_->GetFirmwareVersion().toInt();
    uint16_t fw_ver_memory = properties_.find("version")->second->GetValue().toInt();

    if (fw_ver_actual != fw_ver_memory) {
        Serial.println("Firmware updated");
        fw_updated_ = true;

    } else {
        String firmware_state = fwStates_.find(FW_ACTUAL)->second.c_str();
        properties_.find("updatestate")->second->SetValue(firmware_state);
    }
    this->wifi_client_ = new WiFiClient;

    return status;
}

void AutoUpdateFw::HandleCurrentState() {
    if (fw_updated_) {
        bool is_fw_notif_enabled = device_->IsFwNotifyEnabled();
        if (is_fw_notif_enabled) device_->SendNotification(fwStates_.find(FW_UPDATED)->second.c_str());
        fw_settings.version_ = device_->GetFirmwareVersion().toInt();
        properties_.find("version")->second->SetValue(String(fw_settings.version_));
        SaveFwSettings();
        fw_updated_ = false;
    }
    if (millis() - period_loop > kLoopDelay_) {
        CheckFirmware(fw_settings.version_);

        if (properties_.find("updatetime")->second->HasNewValue()) {
            SetUpdateTime(properties_.find("updatetime")->second->GetValue());
            properties_.find("updatetime")->second->SetHasNewValue(false);
            new_fw_settings_ = true;

            // logic
        }

        if (properties_.find("update")->second->HasNewValue()) {
            properties_.find("update")->second->SetHasNewValue(false);
            force_update_ = true;
            CheckFirmware(properties_.find("version")->second->GetValue().toInt());
        }

        if (new_fw_settings_) {
            if (SaveFwSettings()) new_fw_settings_ = false;
        }

        period_loop = millis();
    }
}

void AutoUpdateFw::CheckFirmware(uint8_t firmwareVer) {
    if (millis() - last_millis_firmware_ > kFirmwareDelay_) {
        bool fw_auto_update = properties_.find("autoupdate")->second->GetValue() == "true";
        if (fw_auto_update && time_client_) {
            Serial.println("auto update is true");
            uint8_t hours = time_client_->GetHours();
            Serial.printf("actual time is %i:", hours);
            if (hours == fw_settings.hours_) {
                uint8_t minutes = time_client_->GetMinutes();
                Serial.printf("%i\r\n", minutes);

                if (minutes == fw_settings.minutes_) {
                    if (CheckFirmwareVersion() > firmwareVer) {
                        Serial.println("Available new firmware");
                        UpdateFirmware();
                    } else {
                        Serial.println("Already on latest version");
                    }
                }
            }
        }
        last_millis_firmware_ = millis();
    }
    if (force_update_) {
        String firmware_state = fwStates_.find(FW_CHECK)->second.c_str();
        properties_.find("updatestate")->second->SetValue(firmware_state);
        if (CheckFirmwareVersion() > firmwareVer) {
            Serial.println("Available new firmware");
            UpdateFirmware();
        } else {
            String firmware_state = fwStates_.find(FW_ACTUAL)->second.c_str();
            properties_.find("updatestate")->second->SetValue(firmware_state);
            Serial.println("Already on latest version");
            force_update_ = false;
        }
    }
    if (new_fw_settings_) {
        if (SaveFwSettings()) new_fw_settings_ = false;
    }
}

uint8_t AutoUpdateFw::CheckFirmwareVersion() {
    uint8_t new_version = 0;
    HTTPClient httpClient;
    String url = "http://";
    url += host;
    url += kUrlFirmwareVersionPath_;
    url += product_id;
    url += "/firmware-version";
    if (httpClient.begin(*wifi_client_, url)) {  // HTTPS
        Serial.println(url);
        int http_code = httpClient.GET();
        if (http_code == 200) {
            String newFWVersion = httpClient.getString();
            StaticJsonDocument<100> doc;
            deserializeJson(doc, newFWVersion);
            new_version = doc["firmware_version"];
        } else {
            Serial.print("Firmware version check failed, got HTTP response code ");
            Serial.println(http_code);
        }
        httpClient.end();
    } else {
        Serial.println("Http client not started, couldn't check version");
    }
    return new_version;
}

bool AutoUpdateFw::LoadFwSettings() {
    ReadSettings("/fwconf.txt", reinterpret_cast<byte*>(&fw_settings), sizeof(fw_settings));

    if (fw_settings.version_) {
        properties_.find("version")->second->SetValue(String(fw_settings.version_));
        properties_.find("version")->second->SetHasNewValue(false);

        String auto_update_value = fw_settings.autoUpdate_ ? "true" : "false";
        properties_.find("autoupdate")->second->SetValue(auto_update_value);
        properties_.find("autoupdate")->second->SetHasNewValue(false);

        properties_.find("updatetime")->second->SetValue(TimeToStr(fw_settings.hours_, fw_settings.minutes_).c_str());
        properties_.find("updatetime")->second->SetHasNewValue(false);

        return true;
    }

    return false;
}

void AutoUpdateFw::SetTimeClient(TimeClient* time_client) { this->time_client_ = time_client; }

void AutoUpdateFw::SetUpdateTime(String time) {
    uint8_t separator_pos = time.indexOf(":");
    if (separator_pos == 1 || separator_pos == 2) {
        uint8_t hours = time.substring(0, separator_pos).toInt();
        uint8_t minutes = time.substring(separator_pos + 1).toInt();
        if (hours < 24 && minutes < 60) {
            fw_settings.hours_ = hours;
            fw_settings.minutes_ = minutes;
            new_fw_settings_ = true;
        }
    }
}

bool AutoUpdateFw::SaveFwSettings() {
    return WriteSettings("/fwconf.txt", reinterpret_cast<byte*>(&fw_settings), sizeof(fw_settings));
}
