#include "Arduino.h"
#include "ArduinoJson.h"
#include "esphome.h"
#include "izar_wmbus.h"

// id for the current watermeter
int meterId = 0x43410778;

int waterUsage_temp;
int waterUsage;
IzarWmbus reader;

namespace MyTextData {
  TextSensor *my_text_sensor = new TextSensor();
}

class MyTextSensor : public Component, public TextSensor {
   public:
    TextSensor *my_text_sensor = MyTextData::my_text_sensor;
};

class MySensor : public Component, public Sensor {
   public:
    Sensor *my_sensor = new Sensor();

    void setup() {
        // to display all meters in range
        reader.init(0);

        // uncomment the line after identifying your meter
        // reader.init(meterId);
    }

    IzarResultData data;

    void loop() {
        
        FetchResult result = reader.fetchPacket(&data);
        
        if (result == FETCH_SUCCESSFUL) {

            waterUsage_temp = data.waterUsage;
            
            if ((waterUsage_temp > 0) and (waterUsage_temp < 10000000)) {  //data filter
                waterUsage = waterUsage_temp;
                ESP_LOGD("IZAR WATERMETER", "waterUsage: %d", waterUsage);
            }

            //Uncomment from reading all the meters in the range. Comment this line after identifying your meter ID.
            int meterId = data.meterId;

            char ID_text[32];
            itoa(meterId, ID_text, 16);

            // publish the ID to the custom text sensor
            ESP_LOGD("IZAR WATERMETER", "watermeterid:  [0x%08X]", meterId);
            MyTextData::my_text_sensor->publish_state(ID_text);

            // publish the liter to the custom sensor
            my_sensor->publish_state(waterUsage);


        } else {
            // wait for 300 ms
            delay(300);
            reader.ensureRx();
        }
    }
};
