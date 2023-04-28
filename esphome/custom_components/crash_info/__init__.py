import logging
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.core import CORE
from esphome.cpp_types import Component
from esphome.components import binary_sensor
from esphome.const import (
    DEVICE_CLASS_PROBLEM,
    ENTITY_CATEGORY_DIAGNOSTIC,
    PLATFORM_ESP8266,
    CONF_ID,
)

_LOGGER = logging.getLogger(__name__)

ESP_PLATFORMS = [PLATFORM_ESP8266]
AUTO_LOAD = ["binary_sensor"]

CONF_MAX_STACK_FRAMES_SIZE = "max_stack_frames_size"
CONF_MIN_STACK_FRAMES_ADDR = "min_stack_frames_addr"
CONF_MAX_STACK_FRAMES_ADDR = "max_stack_frames_addr"
CONF_INDICATOR = "indicator"
CONF_STORE_IN_FLASH = "store_in_flash"
CONF_STORE_FREE_HEAP = "store_free_heap"
CONF_FRAMES_IN_LINE = "frames_in_line"

MAX_STACK_FRAMES = 120

crash_info_ns = cg.esphome_ns.namespace("crash_info")
CrashInfo = crash_info_ns.class_("CrashInfo", Component)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(CrashInfo),
        cv.Optional(CONF_MAX_STACK_FRAMES_SIZE, default=10): cv.All(
            cv.positive_int, cv.int_range(4, MAX_STACK_FRAMES)
        ),
        cv.Optional(CONF_MIN_STACK_FRAMES_ADDR, default=0x40000000): cv.All(
            cv.positive_int, cv.int_range(min=1, max=0x70000000)
        ),
        cv.Optional(CONF_MAX_STACK_FRAMES_ADDR, default=0x50000000): cv.All(
            cv.positive_int, cv.int_range(min=1, max=0x70000000)
        ),
        cv.Optional(CONF_INDICATOR): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_PROBLEM,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
        cv.Optional(CONF_STORE_IN_FLASH, default=False): cv.boolean,
        cv.Optional(CONF_STORE_FREE_HEAP, default=False): cv.boolean,
        cv.Optional(CONF_FRAMES_IN_LINE, default=4): cv.All(
            cv.positive_int, cv.int_range(min=1, max=MAX_STACK_FRAMES)
        ),
    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    if CONF_INDICATOR in config:
        conf = config[CONF_INDICATOR]
        sens = cg.new_Pvariable(conf[CONF_ID])
        await binary_sensor.register_binary_sensor(sens, conf)
        cg.add(var.set_indicator(sens))

    max_stack_frames_size = config[CONF_MAX_STACK_FRAMES_SIZE]
    cg.add_define("CRASH_INFO_MAX_STACK_FRAMES_SIZE", max_stack_frames_size)
    min_stack_frames_addr = config[CONF_MIN_STACK_FRAMES_ADDR]
    cg.add_define("CRASH_INFO_MIN_STACK_FRAMES_ADDR", min_stack_frames_addr)
    max_stack_frames_addr = config[CONF_MAX_STACK_FRAMES_ADDR]
    cg.add_define("CRASH_INFO_MAX_STACK_FRAMES_ADDR", max_stack_frames_addr)
    frames_in_line = config[CONF_FRAMES_IN_LINE]
    cg.add_define("CRASH_INFO_FRAMES_IN_LINE", frames_in_line)

    if config[CONF_STORE_IN_FLASH]:
        cg.add_define("CRASH_INFO_STORE_IN_FLASH")

    num_bytes = 1 + 1 + (config[CONF_MAX_STACK_FRAMES_SIZE] * 4)
    extended_with = []

    if config[CONF_STORE_FREE_HEAP]:
        cg.add_define("CRASH_INFO_STORE_FREE_HEAP")
        num_bytes += 2
        extended_with.append("free_heap")

    if "time" in CORE.loaded_integrations:
        num_bytes += 8
        extended_with.append("time")

    if len(extended_with) > 0:
        extended_with = f" (extended with {', '.join(extended_with)})"
    else:
        extended_with = ""

    save_type = "FLASH" if config[CONF_STORE_IN_FLASH] else "RTC"
    _LOGGER.info(
        "Crash info will take %u bytes of %s memory%s",
        num_bytes,
        save_type,
        extended_with,
    )
