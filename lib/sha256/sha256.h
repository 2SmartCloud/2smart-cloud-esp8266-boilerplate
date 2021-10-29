/*********************************************************************
 * Filename:   sha256.h
 * Author:     Brad Conte (brad AT bradconte.com)
 * Copyright:
 * Disclaimer: This code is presented "as is" without any guarantees.
 * Details:    Defines the API for the corresponding SHA1 implementation.
 *********************************************************************/

#pragma once

/*************************** HEADER FILES ***************************/
#include <stddef.h>
#include <stdint.h>

/****************************** MACROS ******************************/
#define SHA256_BLOCK_SIZE 32  // SHA256 outputs a 32 byte digest

/**************************** DATA TYPES ****************************/
typedef unsigned char BYTE;  // 8-bit byte
typedef unsigned int WORD;   // 32-bit word, change to "long" for 16-bit machines

class Sha256 {
 public:
    Sha256();
    void update(const BYTE data[], size_t len);
    void final(BYTE hash[]);

 private:
    BYTE data[64] = {0};
    WORD datalen;
    uint64_t bitlen;
    WORD state[8];
    void transform();
};
