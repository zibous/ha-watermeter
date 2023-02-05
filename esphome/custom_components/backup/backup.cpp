
#include "esphome/core/log.h"
#include "esphome/core/application.h"
#include "esphome/core/util.h"
#include "esphome/components/json/json_util.h"

#include "StreamString.h"

#include <cstdlib>

#include "backup.h"

namespace esphome {
namespace backup {

static const char *const TAG = "backup";
static const char *const URL = "/config.yaml";

void Backup::setup() {
  ESP_LOGCONFIG(TAG, "Setting up backup handler...");
  this->base_->init();
  this->base_->get_server()->on(URL, HTTP_GET, [this](AsyncWebServerRequest *request) {
    if (this->using_auth() && !request->authenticate(this->username_, this->password_)) {
      return request->requestAuthentication();
    }
    auto response = request->beginResponse_P(200, "plain/text;charset=UTF-8", this->data_, this->size_);
    response->addHeader("Content-Encoding", "gzip");
    request->send(response);
  });
}

void Backup::dump_config() {
  ESP_LOGCONFIG(TAG, "Backup:");
  ESP_LOGCONFIG(TAG, "  URL path is %s", URL);
  if (this->using_auth()) {
    ESP_LOGCONFIG(TAG, "  Basic authentication enabled");
  }
}

}  // namespace backup
}  // namespace esphome
