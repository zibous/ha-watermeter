#pragma once

#include "esphome/core/component.h"
#include "esphome/core/controller.h"
#include "esphome/components/web_server_base/web_server_base.h"

#include <vector>

namespace esphome {
namespace backup {

class Backup : public Component {
 public:
  explicit Backup(web_server_base::WebServerBase *base) : base_(base) {}

  void setup() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::WIFI - 1.0f; }

  void set_config(const uint8_t *data, size_t size) {
    this->data_ = data;
    this->size_ = size;
  }

  void set_username(const char *username) { this->username_ = username; }

  void set_password(const char *password) { this->password_ = password; }

  bool using_auth() { return this->username_ != nullptr && this->password_ != nullptr; }

 protected:
  web_server_base::WebServerBase *base_;
  const uint8_t *data_{nullptr};
  size_t size_{0};
  const char *username_{nullptr};
  const char *password_{nullptr};
};

}  // namespace backup
}  // namespace esphome
