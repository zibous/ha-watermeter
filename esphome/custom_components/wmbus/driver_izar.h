/*
  Based on: https://github.com/weetmuts/wmbusmeters/blob/master/src/driver_izar.cc
  Copyright (C) 2019 Jacek Tomasiak (gpl-3.0-or-later)
  Copyright (C) 2020-2022 Fredrik Öhrström (gpl-3.0-or-later)
  Copyright (C) 2021 Vincent Privat (gpl-3.0-or-later)
*/

#pragma once

#include "driver.h"

#define GET_BIT(var, pos) ((var >> pos) & 0x01)

struct Izar : Driver
{

  Izar() : Driver(std::string("izar")){};

  bool get_value(std::vector<unsigned char> &telegram, float &water_usage) override
  {
    //====DECRYPT====
    // Telegram: 1944A511780743434418A271150013CE5A12E8B1F69A2283DDCF
    // see: https://github.com/weetmuts/wmbusmeters/blob/master/src/driver_izar.cc
    bool ret_val = false;
    uint8_t *decoded = reinterpret_cast<uint8_t *>(telegram.data());
    uint8_t decoded_len = telegram.size();
    uint8_t decrypted[64] = {0};
    if ((this->decrypt(decoded, decoded_len, decrypted)) > 0)
    {

      /*  get the total value (watermeter display)
          The total water consumption recorded by this meter.
          double total_water_consumption_l_ = uint32FromBytes(decoded_content, 1, true);
          setNumericValue("total", Unit::L, total_water_consumption_l_);
      */
      water_usage = (this->uintFromBytesLittleEndian(decrypted + 1)) / 1000.0;

      /*
         get the last_month_measure_date
         The date when the meter recorded the most recent billing value.
         // get the date when the second measurement was taken
          uint16_t h0_year = ((decoded_content[10] & 0xF0) >> 1) + ((decoded_content[9] & 0xE0) >> 5);
          if (h0_year > 80) {
              h0_year += 1900;
          } else {
              h0_year += 2000;
          }
          uint8_t h0_month = decoded_content[10] & 0xF;
          uint8_t h0_day = decoded_content[9] & 0x1F;
          setStringValue("last_month_measure_date", tostrprintf("%d-%02d-%02d", h0_year, h0_month%99, h0_day%99));
      */


      /*
         get the last_month_total
         The total water consumption recorded by this meter around end of last month.
         double last_month_total_water_consumption_l_ = uint32FromBytes(decoded_content, 5, true);
         setNumericValue("last_month_total", Unit::L, last_month_total_water_consumption_l_);
      */

      /*
         get the curent_month_total
         double = water_usage - last_month_total
      */

      /*
         get the currentAlarmsText
         Alarms currently reported by the meter
         alarms.general_alarm = frame[11] >> 7;
         alarms.leakage_currently = frame[12] >> 7;
         alarms.leakage_previously = frame[12] >> 6 & 0x1;
         alarms.meter_blocked = frame[12] >> 5 & 0x1;
         alarms.back_flow = frame[13] >> 7;
         alarms.underflow = frame[13] >> 6 & 0x1;
         alarms.overflow = frame[13] >> 5 & 0x1;
         alarms.submarine = frame[13] >> 4 & 0x1;
         alarms.sensor_fraud_currently = frame[13] >> 3 & 0x1;
         alarms.sensor_fraud_previously = frame[13] >> 2 & 0x1;
         alarms.mechanical_fraud_currently = frame[13] >> 1 & 0x1;
         alarms.mechanical_fraud_previously = frame[13] & 0x1;
         setStringValue("current_alarms", currentAlarmsText(alarms));
      */

      /*
         get the previousAlarmsText
         Alarms previously reported by the meter.
         setStringValue("current_alarms", currentAlarmsText(alarms));
      */

      /*
        get the remaining_battery_life
        How many more years the battery is expected to last
        remaining_battery_life = (frame[12] & 0x1F) / 2.0;
        setNumericValue("remaining_battery_life", Unit::Year, remaining_battery_life);
      */

      /*
         get the manufacture_year
         string digits = to_string((origin[7] & 0x03) << 24 | origin[6] << 16 | origin[5] << 8 | origin[4]);
         // get the manufacture year
         uint8_t yy = atoi(digits.substr(0, 2).c_str());
         int manufacture_year = yy > 70 ? (1900 + yy) : (2000 + yy); // Maybe to adjust in 2070, if this code stills lives :D
         setStringValue("manufacture_year", tostrprintf("%d", manufacture_year));
      */

      /*
         get the transmit_period
         The period at which the meter transmits its data.
         int transmit_period_s = 1 << ((frame[11] & 0x0F) + 2);
         setNumericValue("transmit_period", Unit::Second, transmit_period_s);
      */

      /*
         get the RSSI ??
      */

      ret_val = true;
    }

    return ret_val;
  };

private:
  const uint8_t decoder[64] = {
      0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
      0xFF, 0xFF, 0xFF, 0x03, 0xFF, 0x01, 0x02, 0xFF,
      0xFF, 0xFF, 0xFF, 0x07, 0xFF, 0xFF, 0x00, 0xFF,
      0xFF, 0x05, 0x06, 0xFF, 0x04, 0xFF, 0xFF, 0xFF,
      0xFF, 0xFF, 0xFF, 0x0B, 0xFF, 0x09, 0x0A, 0xFF,
      0xFF, 0x0F, 0xFF, 0xFF, 0x08, 0xFF, 0xFF, 0xFF,
      0xFF, 0x0D, 0x0E, 0xFF, 0x0C, 0xFF, 0xFF, 0xFF,
      0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

  uint32_t uintFromBytes(uint8_t *data)
  {
    uint32_t result = data[0] << 24;
    result += data[1] << 16;
    result += data[2] << 8;
    result += data[3];
    return result;
  }

  uint32_t uintFromBytesLittleEndian(uint8_t *data)
  {
    uint32_t result = data[3] << 24;
    result += data[2] << 16;
    result += data[1] << 8;
    result += data[0];
    return result;
  }

  uint32_t hashShiftKey(uint32_t key)
  {
    for (uint8_t i = 0; i < 8; i++)
    {
      uint8_t bit = GET_BIT(key, 1) ^ GET_BIT(key, 2) ^
                    GET_BIT(key, 11) ^ GET_BIT(key, 31);
      key <<= 1;
      key |= bit;
    }
    return key;
  }

  uint8_t decrypt(uint8_t *encoded, uint8_t len, uint8_t *decoded)
  {
    if (len < 15)
    {
      return 0;
    }

    uint32_t key = 0xdfd109e8;
    key ^= uintFromBytes(encoded + 2);
    key ^= uintFromBytes(encoded + 6);
    key ^= uintFromBytes(encoded + 10);

    const uint8_t size = len - 15;
    for (uint8_t i = 0; i < size; i++)
    {
      key = hashShiftKey(key);
      decoded[i] = encoded[i + 15] ^ (key & 0xFF);
    }

    if (decoded[0] != 0x4B)
    {
      return 0;
    }

    return size;
  }
};