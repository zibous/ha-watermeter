import esphome.config_validation as cv
import esphome.codegen as cg
from esphome import automation
from esphome.const import CONF_ID, CONF_IP_ADDRESS, CONF_PORT, CONF_CLIENT_ID, CONF_LEVEL, CONF_PAYLOAD, CONF_TAG
from esphome.components.logger import LOG_LEVELS, is_log_level

CONF_STRIP_COLORS = "strip_colors"
CONF_ENABLE_LOGGER_MESSAGES = "enable_logger"
CONF_MIN_LEVEL = "min_level"

DEPENDENCIES = ['logger','network']

debug_ns = cg.esphome_ns.namespace('debug')
syslog_ns = cg.esphome_ns.namespace('syslog')

SyslogComponent = syslog_ns.class_('SyslogComponent', cg.Component)
SyslogLogAction = syslog_ns.class_('SyslogLogAction', automation.Action)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(SyslogComponent),
    cv.Optional(CONF_IP_ADDRESS, default="255.255.255.255"): cv.string_strict,
    cv.Optional(CONF_PORT, default=514): cv.port,
    cv.Optional(CONF_ENABLE_LOGGER_MESSAGES, default=True): cv.boolean,
    cv.Optional(CONF_STRIP_COLORS, default=True): cv.boolean,
    cv.Optional(CONF_MIN_LEVEL, default="DEBUG"): is_log_level,
})

SYSLOG_LOG_ACTION_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.use_id(SyslogComponent),
    cv.Required(CONF_LEVEL): cv.templatable(cv.int_range(min=0, max=7)),
    cv.Required(CONF_TAG): cv.templatable(cv.string),
    cv.Required(CONF_PAYLOAD): cv.templatable(cv.string),
})

def to_code(config):
    cg.add_library('Syslog', '2.0.0')

    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    
    cg.add(var.set_enable_logger_messages(config[CONF_ENABLE_LOGGER_MESSAGES]))
    cg.add(var.set_strip_colors(config[CONF_STRIP_COLORS]))
    cg.add(var.set_server_ip(config[CONF_IP_ADDRESS]))
    cg.add(var.set_server_port(config[CONF_PORT]))
    cg.add(var.set_min_log_level(LOG_LEVELS[config[CONF_MIN_LEVEL]]))

@automation.register_action('syslog.log', SyslogLogAction, SYSLOG_LOG_ACTION_SCHEMA)
def syslog_log_action_to_code(config, action_id, template_arg, args):
    paren = yield cg.get_variable(config[CONF_ID])
    var = cg.new_Pvariable(action_id, template_arg, paren)

    template_ = yield cg.templatable(config[CONF_LEVEL], args, cg.uint8)
    cg.add(var.set_level(template_))
    template_ = yield cg.templatable(config[CONF_TAG], args, cg.std_string)
    cg.add(var.set_tag(template_))
    template_ = yield cg.templatable(config[CONF_PAYLOAD], args, cg.std_string)
    cg.add(var.set_payload(template_))

    yield var
