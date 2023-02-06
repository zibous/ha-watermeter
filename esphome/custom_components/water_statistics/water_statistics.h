#pragma once

#include "esphome/core/component.h"
#include "esphome/core/preferences.h"

#include "esphome/components/sensor/sensor.h"
#include "esphome/components/time/real_time_clock.h"

namespace esphome {
namespace water_statistics {

using sensor::Sensor;

class WaterStatistics : public Component {
 public:
  float get_setup_priority() const override { return setup_priority::DATA; }
  void dump_config() override;
  void setup() override;
  void loop() override;

  void set_time(time::RealTimeClock *time) { this->time_ = time; }
  void set_total(Sensor *sensor) { this->total_ = sensor; }

  void set_water_today(Sensor *sensor) { this->water_today_ = sensor; }
  void set_water_yesterday(Sensor *sensor) { this->water_yesterday_ = sensor; }
  void set_water_week(Sensor *sensor) { this->water_week_ = sensor; }
  void set_water_month(Sensor *sensor) { this->water_month_ = sensor; }

 protected:
  ESPPreferenceObject pref_;
  time::RealTimeClock *time_;

  // input sensors
  Sensor *total_{nullptr};

  // exposed sensors
  Sensor *water_today_{nullptr};
  Sensor *water_yesterday_{nullptr};
  Sensor *water_week_{nullptr};
  Sensor *water_month_{nullptr};

  // start day of week configuration
  int water_week_start_day_{2};
  // start day of month configuration
  int water_month_start_day_{1};

  struct water_data_t {
    uint16_t current_day_of_year{0};
    float start_today{NAN};
    float start_yesterday{NAN};
    float start_week{NAN};
    float start_month{NAN};
  } water_;

  void process_(float total);
  void save_();
};  // class waterStatistics

}  // namespace water_statistics
}  // namespace esphome
