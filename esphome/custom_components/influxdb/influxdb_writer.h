#pragma once

#include <vector>
#include <WiFiUdp.h>

#include "esphome/core/component.h"
#include "esphome/core/controller.h"
#include "esphome/core/defines.h"
#include "esphome/core/log.h"

namespace esphome {
namespace influxdb {

// struct SensorConfig {
//   bool ignore;
//   std::string measurement;
//   std:vector<std::pair<std::string, std::string>> tags;
// }

class InfluxDBWriter : public Component {
 public:
  InfluxDBWriter() : packet_size(0) { };
  void setup() override;
  void loop() override;
  void dump_config() override;
#ifdef USE_BINARY_SENSOR
  void on_sensor_update(binary_sensor::BinarySensor *obj, std::string measurement, std::string tags, bool state);
#endif
#ifdef USE_SENSOR
  void on_sensor_update(sensor::Sensor *obj, std::string measurement, std::string tags, float state);
#endif
#ifdef USE_TEXT_SENSOR
  void on_sensor_update(text_sensor::TextSensor *obj, std::string measurement, std::string tags, std::string state);
#endif

  void set_host(std::string host) { this->host = host; };
  void set_port(uint16_t port) { this->port = port; };
  void set_send_timeout(int timeout) { send_timeout = timeout; };
  void set_max_packet_size(int size) { max_packet_size = size; };
  void set_tags(std::string tags) { this->tags = tags; };
  void set_publish_all(bool all) { publish_all = all; };
  void add_setup_callback(std::function<Nameable*()> fun) { setup_callbacks.push_back(fun); };

 protected:
  void write(std::string measurement, std::string tags, const std::string value, bool is_string);

  uint16_t port;
  std::string host;
  int max_packet_size;
  int send_timeout;
  std::string tags;
  bool publish_all;

  std::vector<std::function<Nameable*()>> setup_callbacks;

  WiFiUDP udp;
  int packet_size;
  int packet_timeout;
};

}  // namespace api
}  // namespace esphome
