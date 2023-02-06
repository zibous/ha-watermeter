import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, time
from esphome.const import (
    CONF_TIME_ID,
    DEVICE_CLASS_TIMESTAMP,
    ENTITY_CATEGORY_DIAGNOSTIC,
    UNIT_EMPTY,
)

ICON_CLOCK_START = "mdi:clock-start"

CODEOWNERS = ["@zdzichu6969"]

DEPENDENCIES = ["time"]

startup_ns = cg.esphome_ns.namespace("startup")
StartupSensor = startup_ns.class_("StartupSensor", sensor.Sensor, cg.PollingComponent)

CONFIG_SCHEMA = (
    sensor.sensor_schema(
        StartupSensor,
        unit_of_measurement=UNIT_EMPTY,
        icon=ICON_CLOCK_START,
        accuracy_decimals=0,
        device_class=DEVICE_CLASS_TIMESTAMP,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    )
    .extend(
        {
            cv.GenerateID(CONF_TIME_ID): cv.use_id(time.RealTimeClock),
        }
    )
    .extend(cv.polling_component_schema("60s"))
)


async def to_code(config):
    """Code generation entry point"""
    var = await sensor.new_sensor(config)
    await cg.register_component(var, config)

    tm = await cg.get_variable(config[CONF_TIME_ID])
    cg.add(var.set_time(tm))
