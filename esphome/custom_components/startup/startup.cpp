#include "esphome/core/log.h"
#include "esphome/core/helpers.h"
#include "esphome/core/hal.h"

#include "startup.h"

namespace esphome {
namespace startup {

static const char *const TAG = "startup";

StartupSensor::StartupSensor() : start_(millis()){};

void StartupSensor::dump_config() { LOG_SENSOR("", "Startup Sensor", this); }

void StartupSensor::update() {
  if (this->startup_ != 0) {
    this->publish_state((float) this->startup_);
  }
}

void StartupSensor::loop() {
  if (startup_ != 0) {
    return;
  }

  auto now = this->time_->now();
  if (!now.is_valid()) {
    // time is not sync yet
    return;
  }

  this->startup_ = now.timestamp - (millis() - this->start_) / 1000;
}

}  // namespace startup
}  // namespace esphome
