#include "user_interface.h"
#include <Esp.h>
#include "esphome/core/log.h"
#ifdef USE_TIME
#include <ctime>
#include "esphome/components/time/real_time_clock.h"
#endif
#include "crash_info.h"

/*
Some info for possibly future IDF support
https://github.com/espressif/esp-idf/issues/5163#issuecomment-851677154
https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/fatal-errors.html
https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/core_dump.html
*/

namespace esphome {
namespace crash_info {

static const char *const TAG = "crash_info";

#define IS_CRASH_REASON(rst_reason) (rst_reason >= REASON_WDT_RST && rst_reason <= REASON_SOFT_WDT_RST)

struct crash_info_t {
  uint8_t reason;
  uint8_t exccause;
  uint32_t stack_frames[CRASH_INFO_MAX_STACK_FRAMES_SIZE];
#ifdef CRASH_INFO_STORE_FREE_HEAP
  uint16_t free_heap;
#endif
#ifdef USE_TIME
  time_t time;
#endif
} PACKED;

CrashInfo *g_crash_info = {};

extern "C" void custom_crash_callback(rst_info *rst_nfo, uint32_t stack, uint32_t stack_end) {
  if (g_crash_info) {
    auto stack_ptr = reinterpret_cast<const uint32_t *>(stack);
    auto stack_len = (stack_end - stack) / sizeof(uint32_t);
    g_crash_info->save_crash_info(rst_nfo->reason, rst_nfo->exccause, stack_ptr, stack_len);
  }
}

CrashInfo::CrashInfo() { g_crash_info = this; }

void CrashInfo::setup() {
  this->rtc_ = global_preferences->make_preference<crash_info_t>(fnv1_hash(TAG), this->is_store_in_flash());
}

void CrashInfo::dump_config() {
  crash_info_t ci{};
  bool has_crash = this->rtc_.load(&ci) && IS_CRASH_REASON(ci.reason);

  if (this->indicator_) {
    this->indicator_->publish_state(has_crash || IS_CRASH_REASON(ESP.getResetInfoPtr()->reason));
  }

  if (IS_CRASH_REASON(ESP.getResetInfoPtr()->reason)) {
    ESP_LOGD(TAG, "Reset Reason: %s", ESP.getResetReason().c_str());
  }

  if (!has_crash) {
    return;
  }
  ESP_LOGI(TAG, "Crash info: ");

#ifdef USE_TIME
  ESP_LOGI(TAG, "  Exception cause code: %d, time: %s", ci.exccause,
           time::ESPTime::from_epoch_local(ci.time).strftime("%F %T").c_str());
#else
  ESP_LOGI(TAG, "  Exception cause code: %d", ci.exccause);
#endif
#ifdef CRASH_INFO_STORE_FREE_HEAP
  ESP_LOGI(TAG, "  Free Heap: %u", ci.free_heap);
#endif
  ESP_LOGI(TAG, "  Stacktrace:");
  ESP_LOGV(TAG, "  Max stack frames size: %u", this->get_max_stack_frames_size());
  ESP_LOGV(TAG, "  Min stack frames addr: 0x%08x", this->get_min_stack_frames_addr());
  ESP_LOGV(TAG, "  Max stack frames addr: 0x%08x", this->get_max_stack_frames_addr());
  ESP_LOGV(TAG, "  %s", format_hex(reinterpret_cast<const uint8_t *>(&ci), sizeof(ci)).c_str());
  std::string s;
  s.reserve(10);
  for (int i = 0; i < this->get_max_stack_frames_size(); i++) {
    if (ci.stack_frames[i] == 0) {
      break;
    }
    s.clear();
    for (int j = 0; j < this->get_frames_in_line() && i < this->get_max_stack_frames_size(); j++, i++) {
      if (ci.stack_frames[i] == 0) {
        break;
      }
      s += str_snprintf(" %08x", 9, ci.stack_frames[i]);
    }
    ESP_LOGI(TAG, "    %s", s.c_str());
  }
}

uint16_t CrashInfo::get_free_heap() {
#ifdef CRASH_INFO_STORE_FREE_HEAP
  crash_info_t ci{};
  if (this->rtc_.load(&ci)) {
    return ci.free_heap;
  }
#endif
  return 0;
}

std::string CrashInfo::get_stack_frames() {
  std::string s;
  crash_info_t ci{};
  if (this->rtc_.load(&ci) && IS_CRASH_REASON(ci.reason)) {
    for (int i = 0; i < this->get_max_stack_frames_size(); i++) {
      if (!s.empty()) {
        s += " ";
      }
      s += str_snprintf("%08x", 8, ci.stack_frames[i]);
    }
  } else if (IS_CRASH_REASON(ESP.getResetInfoPtr()->reason)) {
    s = ESP.getResetInfo().c_str();
  }
  return s;
}

void CrashInfo::save_crash_info(uint8_t reason, uint8_t exccause, const uint32_t *stack, size_t stack_size) {
  crash_info_t ci{.reason = reason,
                  .exccause = exccause,
                  .stack_frames = {},
#ifdef CRASH_INFO_STORE_FREE_HEAP
                  .free_heap = static_cast<uint16_t>(ESP.getFreeHeap()),
#endif
#ifdef USE_TIME
                  .time = std::time(nullptr)
#endif
  };

  for (uint32_t i = 0, cnt = 0; i < stack_size && cnt < this->get_max_stack_frames_size(); i++) {
    auto frame = stack[i];
    // try to store only decodable stack frames
    if (frame >= this->get_min_stack_frames_addr() && frame < this->get_max_stack_frames_addr()) {
      ci.stack_frames[cnt++] = frame;
    }
  }
  this->rtc_.save(&ci);
  if (this->is_store_in_flash()) {
    global_preferences->sync();
  }
}

void CrashInfo::reset() {
  crash_info_t ci{};
  this->rtc_.save(&ci);
  if (this->indicator_) {
    this->indicator_->publish_state(false);
  }
}

}  // namespace crash_info
}  // namespace esphome
