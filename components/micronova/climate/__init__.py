import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate,switch,sensor
from esphome.const import (
    CONF_ID,
    ICON_POWER,
    CONF_SENSOR,
)

from .. import (
    MicroNova,
    MicroNovaFunctions,
    CONF_MICRONOVA_ID,
    MICRONOVA_LISTENER_SCHEMA,
    micronova_ns,
)

CONF_STOVE_SENSOR = "stove_sensor"
CONF_STOVE_SWITCH = "stove_switch"

MicroNovaClimate = micronova_ns.class_("MicroNovaClimate", climate.Climate, cg.Component)

CONFIG_SCHEMA = cv.All(
    climate.CLIMATE_SCHEMA.extend(
        {
            cv.GenerateID(): cv.declare_id(MicroNovaClimate),
            cv.GenerateID(CONF_MICRONOVA_ID): cv.use_id(MicroNova),
            cv.Required(CONF_STOVE_SENSOR): cv.use_id(sensor.Sensor),
            cv.Required(CONF_STOVE_SWITCH): cv.use_id(switch.Switch),
        }
    ).extend(cv.COMPONENT_SCHEMA),
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await climate.register_climate(var, config)

    mv = await cg.get_variable(config[CONF_MICRONOVA_ID])

    sensor = await cg.get_variable(config[CONF_STOVE_SENSOR])
    cg.add(var.set_sensor(sensor))

    switch = await cg.get_variable(config[CONF_STOVE_SWITCH])
    cg.add(var.set_swtich(switch))

