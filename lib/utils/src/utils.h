#pragma once
#include <sha256.h>

#include <string>

#include "Arduino.h"

std::string RandomString(uint8_t seed, uint8_t length);

String TimeToStr(uint8_t hours, uint8_t minutes);

String Sha_256(String data);
