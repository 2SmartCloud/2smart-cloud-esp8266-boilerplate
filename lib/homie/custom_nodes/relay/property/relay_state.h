#pragma once

#define EB_HOLD 10000  // таймаут удержания кнопки

#include <Arduino.h>
#include <EncButton.h>

#include <map>
#include <string>

#include "property/property.h"

class RelayState : public Property {
 public:
    RelayState(const char* name, const char* id, Node* node, PROPERTY_TYPE type, bool settable, bool retained,
               const char* data_type);

    bool Init(Homie* homie) override;

    void HandleCurrentState() override;

    void HandleSettingNewValue() override;

 private:
    EncButton<EB_TICK, 0> button_;  // esp32 13 //sonoff 0

    const uint8_t kRelayPin = 12;  // esp32 4 // sonoff 12

    uint32_t period_loop_ = millis();
};
