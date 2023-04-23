#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace debug {

class HeapMonitor : public sensor::Sensor, public PollingComponent {
 public:
  void update() override;
  float get_setup_priority() const override { return setup_priority::LATE; };
};

}  // namespace debug
}  // namespace esphome
