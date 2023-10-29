#include "micronova_climate.h"
#include "esphome/core/log.h"

namespace esphome {
namespace micronova {

void MicroNovaClimate::setup() {
  this->mode = climate::CLIMATE_MODE_HEAT;

  this->current_temperature_->add_on_state_callback([this](float state) {
    ESP_LOGD(TAG, "current_temperature_ add_on_state_callback %f",state);
    this->current_temperature = state;
    this->publish_state();
  });
  this->current_temperature = this->current_temperature_->state;

  this->target_temperature_->add_on_state_callback([this](float state) {
    ESP_LOGD(TAG, "target_temperature_ add_on_state_callback %f",state);
    this->target_temperature = state;
    this->publish_state();
  });
  this->target_temperature = this->target_temperature_->state;

  this->stove_switch_->add_on_state_callback([this](bool state) {
    ESP_LOGD(TAG, "stove_switch_ add_on_state_callback");
    if (state) {
      this->mode = climate::CLIMATE_MODE_HEAT;
    }
    else {
      this->mode = climate::CLIMATE_MODE_OFF;
    }
    this->publish_state();
  });

  if (this->stove_switch_->state) {
      this->mode = climate::CLIMATE_MODE_HEAT;
  }
  else {
     this->mode = climate::CLIMATE_MODE_OFF;
  }

  this->publish_state();
}

void MicroNovaClimate::loop() {
  bool state_changed = false;

  if (state_changed) {
    this->publish_state();
  }
}

void MicroNovaClimate::control(const climate::ClimateCall &call) {
  ESP_LOGD(TAG, "Control");
}

climate::ClimateTraits MicroNovaClimate::traits() {
  auto traits = climate::ClimateTraits();
  traits.set_supports_current_temperature(true);
  traits.add_supported_mode(climate::CLIMATE_MODE_HEAT);
  return traits;
}

void MicroNovaClimate::dump_config() {
  LOG_CLIMATE("", "MicroNova Climate", this);
  LOG_SWITCH(TAG, "    Stove switch", this->stove_switch_);
  LOG_SENSOR(TAG, "    Current temperature sensor", this->current_temperature_);
  LOG_NUMBER(TAG, "    Target temperature number", this->target_temperature_);
}

}  // namespace micronova
}  // namespace esphome
