ESPhome component for Micronova board based pellet stoves. 

There is a pull request in the ESPHome repo (https://github.com/esphome/esphome/pull/4760). Go there for a link to the complete documentation.
https://deploy-preview-2890--esphome.netlify.app/components/micronova.html

Example config:
```yaml
external_components:
  - source: github://Jorre05/micronova
    components: [ micronova ]

micronova:
  enable_rx_pin: 7
  update_interval: 10s
  scan_memory_location: 0x00
  room_temperature:
    memory_address: 0x01
    name: Ambient temperature
    id: ambient_temperature
  thermostat_temperature:
    name: Stove thermostat temperature
  fumes_temperature:
    name: Smoke temperature
  stove_power:
    name: Stove current power
  fan_speed:
    fan_rpm_offset: 240
    name: Fan RPM
    id: fan_rpm
  memory_address_sensor:
    memory_location: 0x20
    memory_address: 0x7d
    name: Memory Adres sensor
    id: mem_adres_sensor
  stove_state:
    memory_address: 0x21
    name: Stove Status
  stove_switch:
    memory_location: 0x80
    memory_address: 0x21
    memory_data_on: 0x01
    memory_data_off: 0x06
    name: Switch stove on/off
  but_temp_up:
    name: Increase thermostat
    memory_location: 0xA0
    memory_address: 0x7D
    memory_data: 0x21
  but_temp_down:
    name: Decrease thermostat
```
