#pragma once
#ifndef SYSLOG_COMPONENT_H_0504CB6C_15D8_4AB4_A04C_8AF9063B737F
#define SYSLOG_COMPONENT_H_0504CB6C_15D8_4AB4_A04C_8AF9063B737F

#include "esphome/core/component.h"
#include "esphome/core/defines.h"
#include "esphome/core/automation.h"
#include "esphome/core/log.h"
#include <Syslog.h>
#include <Udp.h>

#if defined ESP8266 || defined ARDUINO_ESP8266_ESP01
    #include <ESP8266WiFi.h>
#else
    #include <WiFi.h>
#endif

#include <WiFiUdp.h>

namespace esphome {
namespace syslog {

struct SYSLOGSettings {
    std::string address;
    uint16_t port;
    std::string client_id;
    int min_log_level;
};

//class UDP;

class SyslogComponent : public Component  {
    public:
        SyslogComponent();

        float get_setup_priority() const override;

        void setup() override;
        void loop() override;

        void set_server_ip(const std::string &address)        { this->settings_.address   = address; }
        void set_server_port(uint16_t port)                   { this->settings_.port      = port; }
        void set_client_id(const std::string &client_id)      { this->settings_.client_id = client_id; }
        void set_min_log_level(int log_level)                 { this->settings_.min_log_level = log_level; }

        void set_enable_logger_messages(bool en) { this->enable_logger = en; }
        void set_strip_colors(bool strip_colors) { this->strip_colors = strip_colors; }

        void log(uint8_t level, const std::string &tag, const std::string &payload);
    protected:
        bool strip_colors;
        bool enable_logger;
        SYSLOGSettings settings_;
        UDP *udp_ = NULL;
};

template<typename... Ts> class SyslogLogAction : public Action<Ts...> {
    public:
        SyslogLogAction(SyslogComponent *parent) : parent_(parent) {}
        TEMPLATABLE_VALUE(uint8_t, level)
        TEMPLATABLE_VALUE(std::string, tag)
        TEMPLATABLE_VALUE(std::string, payload)

        void play(Ts... x) override {
            this->parent_->log(this->level_.value(x...), this->tag_.value(x...), this->payload_.value(x...));
        }

    protected:
        SyslogComponent *parent_;
};

}  // namespace syslog
}  // namespace esphome

#endif
