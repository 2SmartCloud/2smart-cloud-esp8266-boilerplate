#pragma once
#include <Arduino.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#include "time_client.h"

class NtpTimeClient : public TimeClient {
 public:
    NtpTimeClient();
    void Init() override;

    uint8_t GetHours() override;
    uint8_t GetMinutes() override;

 private:
    WiFiUDP* udp_client_ = nullptr;
    NTPClient* ntp_client_ = nullptr;
    void CreateClients();
};
