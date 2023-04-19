/*
 * Copyright (c) 2021 Tomoyuki Sakurai <y@trombik.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#pragma once

#ifdef ARDUINO_ARCH_ESP8266
#include "AsyncPing.h"

#include "esphome/components/sensor/sensor.h"

#define EACH_RESULT true
#define END false
#define TAG "ping"

namespace esphome {
namespace ping {
class PingSensorESP8266 : public PingSensor {
 public:
  void setup() override {
    ping.on(EACH_RESULT, [this](const AsyncPingResponse &response) {
      if (response.answer) {
        ESP_LOGI(TAG, "%d bytes from %s: icmp_seq=%d ttl=%d time=%d ms", response.size, target.c_str(),
                 response.icmp_seq, response.ttl, response.time);

        this->incr_total_success_time(response.time);
      } else {
        ESP_LOGI(TAG, "no reply from %s", target.c_str());
      }
      return false;
    });

    /* at the end, set the result */
    ping.on(END, [this](const AsyncPingResponse &response) {
      float loss = 0;
      int total_failed_count = response.total_sent - response.total_recv;
      if (response.total_sent != 0) {
        loss = (float) total_failed_count / response.total_sent;
      }

      int mean = 0;
      if (response.total_recv != 0) {
        mean = total_success_time / response.total_recv;
      }

      this->set_latest_loss(loss * 100);
      this->set_latest_latency(mean);

      ESP_LOGI(TAG, "packet loss: %0.1f %% latency: %d ms", loss * 100, mean);
      this->reset();
      return true;
    });
    ping.begin(target.c_str(), n_packet, timeout);
  }

  void update() override {
    float loss;
    int latency_ms;

    loss = this->get_latest_loss();
    latency_ms = this->get_latest_latency();

    if (loss >= 0 && this->packet_loss_sensor_ != nullptr) {
      packet_loss_sensor_->publish_state(loss);
    }
    if (latency_ms >= 0 && this->latency_sensor_ != nullptr) {
      latency_sensor_->publish_state((float) latency_ms / 1000);
    }
    ping.begin(target.c_str(), n_packet, timeout);
  }

 private:
 protected:
  int total_success_time;
  void reset() { total_success_time = 0; }
  void incr_total_success_time(int time) { total_success_time += time; }

  AsyncPing ping;
};

}  // namespace ping
}  // namespace esphome
#endif
