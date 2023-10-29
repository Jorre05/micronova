import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate,switch,sensor,number
from esphome.const import (
    CONF_ID,
)

from .. import (
    MicroNova,
    CONF_MICRONOVA_ID,
    micronova_ns,
)

CONF_STOVE_SWITCH = "stove_switch"
CONF_CURRENT_TEMPERATURE_SENSOR = "current_temperature_sensor"
CONF_TARGET_TEMPERATURE_NUMBER = "target_temperature_number"
CONF_STOVE_SWITCH = "stove_switch"

MicroNovaClimate = micronova_ns.class_("MicroNovaClimate", climate.Climate, cg.Component)

CONFIG_SCHEMA = cv.All(
    climate.CLIMATE_SCHEMA.extend(
        {
            cv.GenerateID(): cv.declare_id(MicroNovaClimate),
            cv.GenerateID(CONF_MICRONOVA_ID): cv.use_id(MicroNova),
            cv.Required(CONF_STOVE_SWITCH): cv.use_id(switch.Switch),
            cv.Required(CONF_CURRENT_TEMPERATURE_SENSOR): cv.use_id(sensor.Sensor),
            cv.Required(CONF_TARGET_TEMPERATURE_NUMBER): cv.use_id(number.Number),
        }
    ).extend(cv.COMPONENT_SCHEMA),
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await climate.register_climate(var, config)

    mv = await cg.get_variable(config[CONF_MICRONOVA_ID])

    sw = await cg.get_variable(config[CONF_STOVE_SWITCH])
    cg.add(var.set_stove_switch(sw))

    sens = await cg.get_variable(config[CONF_CURRENT_TEMPERATURE_SENSOR])
    cg.add(var.set_current_temperature_sensor(sens))

    num = await cg.get_variable(config[CONF_TARGET_TEMPERATURE_NUMBER])
    cg.add(var.set_target_temperature_number(num))

