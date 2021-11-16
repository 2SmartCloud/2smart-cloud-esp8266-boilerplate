#include "utils.h"

#include <cstdlib>

#include "sha256.h"
std::string RandomString(uint8_t seed, uint8_t length) {
    srand(seed);
    const char* letters[62] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p",
                               "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "1", "2", "3", "4", "5", "6",
                               "7", "8", "9", "0", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L",
                               "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"};
    std::string randString = "";
    for (int i = 0; i < length; i++) {
        randString.append(letters[rand() % 61]);
    }
    return randString;
}

String TimeToStr(uint8_t hours, uint8_t minutes) {
    String result = "";
    if (hours < 10) result += "0";
    result += hours;
    result += ":";
    if (minutes < 10) result += "0";
    result += minutes;
    return result;
}

String Sha_256(String data) {
    BYTE hash[SHA256_BLOCK_SIZE];
    char texthash[2 * SHA256_BLOCK_SIZE + 1];

    Sha256* sha256Instance = new Sha256();
    BYTE text[64];

    data.getBytes(text, data.length() + 1);
    sha256Instance->update(text, strlen((const char*)text));
    sha256Instance->final(hash);

    // cppcheck-suppress sizeofCalculation
    for (int i = 0; i < SHA256_BLOCK_SIZE; ++i) snprintf(texthash + 2 * i, sizeof(texthash + 2 * i), "%02x", hash[i]);
    Serial.println(texthash);

    delete sha256Instance;
    return texthash;
}
