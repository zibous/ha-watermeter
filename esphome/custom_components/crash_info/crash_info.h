#pragma once

#include "esphome/core/component.h"
#include "esphome/core/preferences.h"
#include "esphome/core/defines.h"
#include "esphome/components/binary_sensor/binary_sensor.h"

// maximum number of stack frames saved to RTC or FLASH.
#ifndef CRASH_INFO_MAX_STACK_FRAMES_SIZE
#define CRASH_INFO_MAX_STACK_FRAMES_SIZE 10
#endif
// minimum stack frame address.
#ifndef CRASH_INFO_MIN_STACK_FRAMES_ADDR
#define CRASH_INFO_MIN_STACK_FRAMES_ADDR 0x40000000
#endif
// maximum stack frame address.
#ifndef CRASH_INFO_MAX_STACK_FRAMES_ADDR
#define CRASH_INFO_MAX_STACK_FRAMES_ADDR 0x50000000
#endif
// number of frames to show in line
#ifndef CRASH_INFO_FRAMES_IN_LINE
#define CRASH_INFO_FRAMES_IN_LINE 4
#endif

namespace esphome {
namespace crash_info {

class CrashInfo : public Component {
 public:
  CrashInfo();
  float get_setup_priority() const override { return setup_priority::LATE; }
  void setup() override;
  void dump_config() override;

  void set_indicator(binary_sensor::BinarySensor *indicator) { this->indicator_ = indicator; }

  void reset();

  void save_crash_info(uint8_t reason, uint8_t exccause, const uint32_t *stack, size_t stack_size);

  uint32_t get_max_stack_frames_size() const { return CRASH_INFO_MAX_STACK_FRAMES_SIZE; }
  uint32_t get_min_stack_frames_addr() const { return CRASH_INFO_MIN_STACK_FRAMES_ADDR; }
  uint32_t get_max_stack_frames_addr() const { return CRASH_INFO_MAX_STACK_FRAMES_ADDR; }
  size_t get_frames_in_line() const { return CRASH_INFO_FRAMES_IN_LINE; }

  bool is_store_in_flash() const {
#ifdef CRASH_INFO_STORE_IN_FLASH
    return true;
#else
    return false;
#endif
  }

  std::string get_stack_frames();
  uint16_t get_free_heap();

 protected:
  ESPPreferenceObject rtc_;
  binary_sensor::BinarySensor *indicator_{};
};

}  // namespace crash_info
}  // namespace esphome
