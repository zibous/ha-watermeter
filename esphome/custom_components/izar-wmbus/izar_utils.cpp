#include "izar_utils.h"

#include <Arduino.h>

#define CRC_POLYNOM 0x3D65


const uint8_t decoder[] = {
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0x03,0xFF,0x01,0x02,0xFF,
    0xFF,0xFF,0xFF,0x07,0xFF,0xFF,0x00,0xFF,
    0xFF,0x05,0x06,0xFF,0x04,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0x0B,0xFF,0x09,0x0A,0xFF,
    0xFF,0x0F,0xFF,0xFF,0x08,0xFF,0xFF,0xFF,
    0xFF,0x0D,0x0E,0xFF,0x0C,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};

uint32_t uintFromBytes(uint8_t* data) {
    uint32_t result = data[0] << 24;
    result += data[1] << 16;
    result += data[2] << 8;
    result += data[3];
    return result;
}

uint32_t uintFromBytesLittleEndian(uint8_t* data) {
    uint32_t result = data[3] << 24;
    result += data[2] << 16;
    result += data[1] << 8;
    result += data[0];
    return result;
}

uint16_t uint16FromBytes(uint8_t* data) {
    uint16_t result = data[0] << 8;
    result += data[1];
    return result;
}

uint32_t hashShiftKey(uint32_t key) {
    for (uint8_t i = 0; i < 8; i++) {
        uint8_t bit = GET_BIT(key, 1) ^ GET_BIT(key, 2) ^ GET_BIT(key, 11) ^
                      GET_BIT(key, 31);
        key <<= 1;
        key |= bit;
    }
    return key;
}

uint8_t decode3of6Single(uint8_t* encoded, uint8_t* decoded) {
    uint8_t data[4];

    data[0] = decoder[encoded[2] & 0x3F];
    data[1] = decoder[((encoded[2] & 0xC0) >> 6) | ((encoded[1] & 0x0F) << 2)];
    data[2] = decoder[((encoded[1] & 0xF0) >> 4) | ((encoded[0] & 0x03) << 4)];
    data[3] = decoder[((encoded[0] & 0xFC) >> 2)];

    for (uint8_t i = 0; i < 4; i++) {
        if (data[i] == 0xFF) {
            return -1;
        }
    }

    // - Shift the encoded values into a byte buffer -
    decoded[0] = (data[3] << 4) | (data[2]);
    decoded[1] = (data[1] << 4) | (data[0]);

    return 0;
}

uint8_t decrypt(uint8_t* encoded, uint8_t len, uint8_t* decoded) {
    if (len < 15) {
        return 0;
    }

    uint32_t key = 0xdfd109e8;
    key ^= uintFromBytes(encoded + 2);
    key ^= uintFromBytes(encoded + 6);
    key ^= uintFromBytes(encoded + 10);

    const uint8_t size = len - 15;
    for (uint8_t i = 0; i < size; i++) {
        key = hashShiftKey(key);
        decoded[i] = encoded[i + 15] ^ (key & 0xFF);
    }

    if (decoded[0] != 0x4B) {
        return 0;
    }

    return size;
}

uint16_t crc16(uint16_t crcVal, uint8_t dataByte) {
    for (int i = 0; i < 8; i++) {
        if (((crcVal & 0x8000) >> 8) ^ (dataByte & 0x80)) {
            crcVal = (crcVal << 1) ^ CRC_POLYNOM;
        } else {
            crcVal = (crcVal << 1);
        }

        dataByte <<= 1;
    }

    return crcVal;
}