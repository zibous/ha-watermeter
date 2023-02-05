#include <WiFiUdp.h>
#include <string>
#include "esphome/core/log.h"
#include "esphome/core/application.h"
#include "influxdb_writer.h"

#ifdef USE_LOGGER
#include "esphome/components/logger/logger.h"
#endif

namespace esphome {
namespace influxdb {

static const char *TAG = "influxdb";

void InfluxDBWriter::setup() {
  ESP_LOGCONFIG(TAG, "Setting up InfluxDB Writer...");
  std::vector<Nameable*> objs;
  for(auto fun: setup_callbacks)
     objs.push_back(fun());

  if (publish_all) {
#ifdef USE_BINARY_SENSOR
    for (auto *obj : App.get_binary_sensors()) {
      if (!obj->is_internal() && std::none_of(objs.begin(), objs.end(), [&obj](Nameable *o) {return o == obj;}))
        obj->add_on_state_callback([this, obj](bool state) { this->on_sensor_update(obj, obj->get_object_id(), tags, state); });
    }
#endif
#ifdef USE_SENSOR
    for (auto *obj : App.get_sensors()) {
      if (!obj->is_internal() && std::none_of(objs.begin(),  objs.end(), [&obj](Nameable *o) {return o == obj;}))
        obj->add_on_state_callback([this, obj](float state) { this->on_sensor_update(obj, obj->get_object_id(), tags, state); });
    }
#endif
#ifdef USE_TEXT_SENSOR
    for (auto *obj : App.get_text_sensors()) {
      if (!obj->is_internal() && std::none_of(objs.begin(),  objs.end(), [&obj](Nameable *o) {return o == obj;}))
        obj->add_on_state_callback([this, obj](std::string state) { this->on_sensor_update(obj, obj->get_object_id(), tags, state); });
    }
#endif
  }
}

void InfluxDBWriter::loop() {
  if (packet_size > 0 && millis() >= packet_timeout) {
    udp.endPacket();
    packet_size = 0;
  }
}

void InfluxDBWriter::write(std::string measurement, std::string tags, const std::string value, bool is_string) 
{
  std::string line = measurement + tags + " value=" + (is_string ? ("\"" + value + "\"") : value);

  if (line.size() > max_packet_size) {
    ESP_LOGW(TAG, "Line too large to fit in UDP packet: %s", line.c_str());
    return;
  }
  
  if (packet_size + line.size() + 1 > max_packet_size) {
    udp.endPacket();
    packet_size = 0;
  }

  if (packet_size == 0) {
    packet_timeout = millis() + send_timeout;
    udp.beginPacket(host.c_str(), port);
  } else
    udp.write('\n');

  udp.write(line.data(), line.size());
  packet_size += line.size();
}

void InfluxDBWriter::dump_config() {
  ESP_LOGCONFIG(TAG, "InfluxDB Writer:");
  ESP_LOGCONFIG(TAG, "  Address: %s:%u", host.c_str(), port);
}

#ifdef USE_BINARY_SENSOR
void InfluxDBWriter::on_sensor_update(binary_sensor::BinarySensor *obj, std::string measurement, std::string tags, bool state) {
  write(measurement, tags, state ? "t" : "f", false);
}
#endif

#ifdef USE_SENSOR
void InfluxDBWriter::on_sensor_update(sensor::Sensor *obj, std::string measurement, std::string tags, float state) {
  write(measurement, tags, to_string(state), false);
}
#endif

#ifdef USE_TEXT_SENSOR
void InfluxDBWriter::on_sensor_update(text_sensor::TextSensor *obj, std::string measurement, std::string tags, std::string state) {
  write(measurement, tags, state, true);
}
#endif

}  // namespace api
}  // namespace esphome
