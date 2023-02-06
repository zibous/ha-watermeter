#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/time/real_time_clock.h"

namespace esphome {
namespace startup {

class StartupSensor : public sensor::Sensor, public PollingComponent {
 public:
  StartupSensor();

  float get_setup_priority() const override { return setup_priority::AFTER_CONNECTION; };
  void dump_config() override;
  void loop() override;
  void update() override;

  std::string unique_id() override { return get_mac_address() + "-startup"; };

  void set_time(time::RealTimeClock *time) { this->time_ = time; }

 protected:
  time::RealTimeClock *time_;
  time_t startup_{0};
  unsigned long start_;
};

}  // namespace startup
}  // namespace esphome
