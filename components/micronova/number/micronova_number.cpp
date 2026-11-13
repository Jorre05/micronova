#include "micronova_number.h"

namespace esphome {
namespace micronova {

void MicroNovaNumber::dump_config() {
  LOG_NUMBER("", "Micronova number", this);
  if (this->get_function() == MicroNovaFunctions::STOVE_FUNCTION_POWER_LEVEL) {
    ESP_LOGCONFIG(TAG, "  Power level offset: %d", this->power_level_offset_);
  }
  if (this->has_ram_write_) {
    ESP_LOGCONFIG(TAG, "  RAM write location:%02X, address:%02X", this->ram_write_location_,
                  this->ram_write_address_);
  }
}

void MicroNovaNumber::process_value_from_stove(int value_from_stove) {
  float new_sensor_value = 0;

  if (value_from_stove == -1) {
    return;
  }

  switch (this->get_function()) {
    case MicroNovaFunctions::STOVE_FUNCTION_THERMOSTAT_TEMPERATURE:
      new_sensor_value = ((float) value_from_stove) * this->traits.get_step();
      break;
    case MicroNovaFunctions::STOVE_FUNCTION_POWER_LEVEL:
      if (value_from_stove < this->power_level_offset_) {
        ESP_LOGW(TAG, "Power level 0x%02X is below power_level_offset %d, ignoring", value_from_stove,
                 this->power_level_offset_);
        return;
      }
      new_sensor_value = (float) (value_from_stove - this->power_level_offset_);
      break;
    default:
      break;
  }
  this->publish_state(new_sensor_value);
}

void MicroNovaNumber::control(float value) {
  uint8_t new_number = 0;

  switch (this->get_function()) {
    case MicroNovaFunctions::STOVE_FUNCTION_THERMOSTAT_TEMPERATURE:
      new_number = (uint8_t) (value / this->traits.get_step());
      break;
    case MicroNovaFunctions::STOVE_FUNCTION_POWER_LEVEL:
      new_number = (uint8_t) value + this->power_level_offset_;
      break;
    default:
      break;
  }
  this->micronova_->queue_write_request(this->memory_write_location_, this->memory_address_, new_number);
  if (this->has_ram_write_) {
    this->micronova_->queue_write_request(this->ram_write_location_, this->ram_write_address_, new_number);
  }
  this->micronova_->update();
}

}  // namespace micronova
}  // namespace esphome
