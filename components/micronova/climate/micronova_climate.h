#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/number/number.h"
#include "esphome/components/switch/switch.h"
#include "esphome/components/micronova/micronova.h"
#include "esphome/components/climate/climate.h"


namespace esphome {
namespace micronova {

class MicroNovaClimate : public climate::Climate, public Component {
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;

  void set_stove_switch(switch_::Switch *s) { this->stove_switch_ = s; }
  void set_current_temperature_sensor(sensor::Sensor *s) { this->current_temperature_ = s; }
  void set_target_temperature_number(number::Number *n) { this->target_temperature_ = n; }

  void set_micronova_parent(MicroNova *parent) { this->micronova_ = parent; }

 protected:
  /// Override control to change settings of the climate device.
  void control(const climate::ClimateCall &call) override;

  /// Return the traits of this controller.
  climate::ClimateTraits traits() override;

  MicroNova *micronova_;

  switch_::Switch *stove_switch_{nullptr};
  sensor::Sensor *current_temperature_{nullptr};
  number::Number *target_temperature_{nullptr};
};

}  // namespace micronova
}  // namespace esphome
