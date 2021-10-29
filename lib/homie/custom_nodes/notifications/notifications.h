#pragma once

#include <Arduino.h>

#include <map>
#include <string>

#include "node/node.h"

class Notifications : public Node {
 public:
    Notifications(const char* name, const char* id, Device* device);

    bool Init(Homie* homie) override;

    void HandleCurrentState() override;

    bool LoadNotifySettings();
    bool SaveNotifySettings();

 private:
    typedef struct {
        bool system_;
        bool update_;
    } NotificationSettings;

    uint32_t period_loop_ = millis();

    const uint16_t kLoopDelay_ = 10 * 1000;  // sec

    NotificationSettings notifySettings = {true, true};
};
