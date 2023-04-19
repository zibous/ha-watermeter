#ifndef IZAR_UTILS
#define IZAR_UTILS

#include <Arduino.h>

#define GET_BIT(var, pos) ((var >> pos) & 0x01)

uint32_t uintFromBytes(uint8_t* data);
uint32_t uintFromBytesLittleEndian(uint8_t* data);
uint16_t uint16FromBytes(uint8_t* data);
uint8_t decrypt(uint8_t* encoded, uint8_t len, uint8_t* decoded);
uint8_t decode3of6Single(uint8_t* encoded, uint8_t* decoded);
uint16_t crc16(uint16_t crcVal, uint8_t dataByte);

#endif

