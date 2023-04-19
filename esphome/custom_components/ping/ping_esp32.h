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

#ifdef ARDUINO_ARCH_ESP32
#include "lwip/inet.h"
#include "lwip/sockets.h"
#include "ping_sock.h"

#include "esphome/components/sensor/sensor.h"

#define TAG "ping_esp32"

namespace esphome {
namespace ping {
class PingSensorESP32 : public PingSensor {
 public:
  void setup() override {
    init_ping();
    esp_ping_new_session(&ping_config, &cbs, &ping);
    esp_ping_start(ping);
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
    esp_ping_new_session(&ping_config, &cbs, &ping);
  }

 private:
  static void cb_ping_on_ping_success(esp_ping_handle_t hdl, void *context) {
    reinterpret_cast<PingSensorESP32 *>(context)->cmd_ping_on_ping_success(hdl);
  }

  static void cb_cmd_ping_on_ping_end(esp_ping_handle_t hdl, void *context) {
    reinterpret_cast<PingSensorESP32 *>(context)->cmd_ping_on_ping_end(hdl);
  }

  static void cb_cmd_ping_on_ping_timeout(esp_ping_handle_t hdl, void *context) {
    reinterpret_cast<PingSensorESP32 *>(context)->cmd_ping_on_ping_timeout(hdl);
  }

  void cmd_ping_on_ping_success(esp_ping_handle_t hdl) {
    uint8_t ttl;
    uint16_t seqno;
    uint32_t elapsed_time, recv_len;
    ip_addr_t target_addr;
    esp_ping_get_profile(hdl, ESP_PING_PROF_SEQNO, &seqno, sizeof(seqno));
    esp_ping_get_profile(hdl, ESP_PING_PROF_TTL, &ttl, sizeof(ttl));
    esp_ping_get_profile(hdl, ESP_PING_PROF_IPADDR, &target_addr, sizeof(target_addr));
    esp_ping_get_profile(hdl, ESP_PING_PROF_SIZE, &recv_len, sizeof(recv_len));
    esp_ping_get_profile(hdl, ESP_PING_PROF_TIMEGAP, &elapsed_time, sizeof(elapsed_time));
    ESP_LOGI(TAG, "%d bytes from %s icmp_seq=%d ttl=%d time=%d ms", recv_len, ipaddr_ntoa((ip_addr_t *) &target_addr),
             seqno, ttl, elapsed_time);
    this->incr_total_success_time(elapsed_time);
  }

  void cmd_ping_on_ping_end(esp_ping_handle_t hdl) {
    ip_addr_t target_addr;
    uint32_t transmitted;
    uint32_t received;
    uint32_t total_time_ms;
    int mean = 0;

    esp_ping_get_profile(hdl, ESP_PING_PROF_REQUEST, &transmitted, sizeof(transmitted));
    esp_ping_get_profile(hdl, ESP_PING_PROF_REPLY, &received, sizeof(received));
    esp_ping_get_profile(hdl, ESP_PING_PROF_IPADDR, &target_addr, sizeof(target_addr));
    esp_ping_get_profile(hdl, ESP_PING_PROF_DURATION, &total_time_ms, sizeof(total_time_ms));
    uint32_t loss = (uint32_t)((1 - ((float) received) / transmitted) * 100);
    if (received != 0) {
      mean = total_success_time / received;
    }

    if (IP_IS_V4(&target_addr)) {
      ESP_LOGI(TAG, "--- %s ping statistics ---", inet_ntoa(*ip_2_ip4(&target_addr)));
    } else {
      ESP_LOGI(TAG, "--- %s ping statistics ---", inet6_ntoa(*ip_2_ip6(&target_addr)));
    }
    ESP_LOGI(TAG, "%d packets transmitted, %d received, %d%% packet loss, total time %dms avg time %dms", transmitted,
             received, loss, total_time_ms, mean);

    this->set_latest_loss(loss);
    this->set_latest_latency(mean);
    this->reset();
    esp_ping_delete_session(hdl);
  }

  void cmd_ping_on_ping_timeout(esp_ping_handle_t hdl) {
    uint16_t seqno;
    ip_addr_t target_addr;
    esp_ping_get_profile(hdl, ESP_PING_PROF_SEQNO, &seqno, sizeof(seqno));
    esp_ping_get_profile(hdl, ESP_PING_PROF_IPADDR, &target_addr, sizeof(target_addr));
    ESP_LOGI(TAG, "From %s icmp_seq=%d timeout", ipaddr_ntoa((ip_addr_t *) &target_addr), seqno);
  }

 protected:
  std::string tag = "ping_esp32";
  esp_ping_config_t ping_config = ESP_PING_DEFAULT_CONFIG();
  esp_ping_handle_t ping;
  esp_ping_callbacks_t cbs;

  void init_ping() {
    ip_addr_t target_addr;
    int err;

    memset(&target_addr, 0, sizeof(target_addr));
    err = inet_pton(AF_INET, target.c_str(), &target_addr);
    if (err == 0) {
      ESP_LOGE(tag.c_str(), "invalid address: `%s`", target.c_str());
      this->status_set_warning();
      return;
    } else if (err < 0) {
      ESP_LOGE(tag.c_str(), "inet_pton(): %s", esp_err_to_name(errno));
      this->status_set_warning();
      return;
    }

    ping_config.target_addr = target_addr;
    ping_config.count = n_packet;

    cbs.on_ping_success = PingSensorESP32::cb_ping_on_ping_success;
    cbs.on_ping_timeout = PingSensorESP32::cb_cmd_ping_on_ping_timeout;
    cbs.on_ping_end = PingSensorESP32::cb_cmd_ping_on_ping_end;
    cbs.cb_args = this;
  }
  int total_success_time;
  void reset() { total_success_time = 0; }
  void incr_total_success_time(int time) { total_success_time += time; }
};

}  // namespace ping
}  // namespace esphome
#endif
