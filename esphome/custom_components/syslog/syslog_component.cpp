#include "syslog_component.h"

#include "esphome/core/log.h"
#include "esphome/core/application.h"

#ifdef USE_LOGGER
#include "esphome/components/logger/logger.h"
#endif
/*
#include "esphome/core/helpers.h"
#include "esphome/core/defines.h"
#include "esphome/core/version.h"
*/

namespace esphome {
namespace syslog {

static const char *TAG = "syslog";

//https://github.com/arcao/Syslog/blob/master/src/Syslog.h#L37-44
//https://github.com/esphome/esphome/blob/5c86f332b269fd3e4bffcbdf3359a021419effdd/esphome/core/log.h#L19-26
static const uint8_t esphome_to_syslog_log_levels[] = {0, 3, 4, 6, 5, 7, 7, 7};

SyslogComponent::SyslogComponent() {
    this->settings_.client_id = App.get_name();
    // Get the WifiUDP client here instead of getting it in setup() to make sure we always have a client when calling log()
    // Calling log() without the device connected should not be an issue since there is a wifi connected check and WifiUDP fails "silently" and doesn't generate an exception anyways
    this->udp_ = new WiFiUDP(); 
}

void SyslogComponent::setup() {
    this->log(ESPHOME_LOG_LEVEL_INFO , "syslog", "Syslog started");
    ESP_LOGI(TAG, "Started");

    #ifdef USE_LOGGER
    if (logger::global_logger != nullptr) {
        logger::global_logger->add_on_log_callback([this](int level, const char *tag, const char *message) {
            if(!this->enable_logger || (level > this->settings_.min_log_level)) return;
            if(this->strip_colors) { //Strips the "033[0;xxx" at the beginning and the "#033[0m" at the end of log messages
                std::string org_msg(message);
                this->log(level, tag, org_msg.substr(7, org_msg.size() -7 -4));
            } else {
                this->log(level, tag, message);
            }
        });
    }
    #endif
}

void SyslogComponent::loop() {
}

void SyslogComponent::log(uint8_t level, const std::string &tag, const std::string &payload) {
    level = level > 7 ? 7 : level;

    // Simple check to make sure that there is connectivity, if not, log the issue and return
    if(WiFi.status() != WL_CONNECTED) {
        ESP_LOGW(TAG, "Tried to send \"%s\"@\"%s\" with level %d but Wifi isn't connected yet", tag.c_str(), payload.c_str(), level);
        return;
    }

    Syslog syslog(
        *this->udp_,
        this->settings_.address.c_str(),
        this->settings_.port,
        this->settings_.client_id.c_str(),
        tag.c_str(),
        LOG_KERN
    );
    if(!syslog.log(esphome_to_syslog_log_levels[level],  payload.c_str())) {
        ESP_LOGW(TAG, "Tried to send \"%s\"@\"%s\" with level %d but but failed for an unknown reason", tag.c_str(), payload.c_str(), level);
    }
}

float SyslogComponent::get_setup_priority() const {
    return setup_priority::AFTER_WIFI;
}

}  // namespace syslog
}  // namespace esphome
