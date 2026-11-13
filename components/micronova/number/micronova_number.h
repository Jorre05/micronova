#pragma once

#include "esphome/components/micronova/micronova.h"
#include "esphome/components/number/number.h"

namespace esphome {
namespace micronova {

class MicroNovaNumber : public number::Number, public MicroNovaSensorListener {
 public:
  MicroNovaNumber() {}
  MicroNovaNumber(MicroNova *m) : MicroNovaSensorListener(m) {}
  void dump_config() override;
  void control(float value) override;
  void request_value_from_stove() override {
    this->micronova_->request_address(this->memory_location_, this->memory_address_, this);
  }
  void process_value_from_stove(int value_from_stove) override;

  void set_memory_write_location(uint8_t l) { this->memory_write_location_ = l; }
  uint8_t get_memory_write_location() { return this->memory_write_location_; }

  void set_power_level_offset(uint8_t o) { this->power_level_offset_ = o; }
  uint8_t get_power_level_offset() { return this->power_level_offset_; }

  // Optional second write of the same value, e.g. to the RAM copy of an EEPROM setting
  void set_ram_write(uint8_t location, uint8_t address) {
    this->ram_write_location_ = location;
    this->ram_write_address_ = address;
    this->has_ram_write_ = true;
  }

 protected:
  uint8_t memory_write_location_ = 0;
  uint8_t power_level_offset_ = 0;
  bool has_ram_write_ = false;
  uint8_t ram_write_location_ = 0;
  uint8_t ram_write_address_ = 0;
};

}  // namespace micronova
}  // namespace esphome
