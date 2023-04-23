import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.components import sensor
from esphome.const import CONF_ID, ICON_GAUGE

UNIT_BYTE = "B"

debug_ns = cg.esphome_ns.namespace('debug')
HeapMonitor = debug_ns.class_('HeapMonitor', cg.PollingComponent)
CONFIG_SCHEMA = sensor.sensor_schema(
    HeapMonitor,
    unit_of_measurement=UNIT_BYTE,
    icon=ICON_GAUGE,
    accuracy_decimals=0
).extend(cv.polling_component_schema('60s'))

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)
