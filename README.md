ESPhome component for Micronova board based pellet stoves. 

There is a pull request in the ESPHome repo (https://github.com/esphome/esphome/pull/4760). Go there for a link to the complete documentation.

Example config:
```yaml
external_components:
  - source: github://Jorre05/micronova
    components: [ micronova ]


micronova:
  enable_rx_pin: 7
  update_interval: 10s
    #  scan_memory_location: 0x20
  room_temperature:
    memory_address: 0x01
    name: ${long_name} Kamertemperatuur
    id: ${short_name}_kamertemperatuur
  thermostat_temperature:
    name: ${long_name} Thermostaat
    id: ${short_name}_thermostaat
  fumes_temperature:
    name: ${long_name} Rookgastemperatuur
    id: ${short_name}_rookgastemperatuur
  stove_power:
    name: ${long_name} Power
    id: ${short_name}_power
  fan_speed:
    fan_rpm_offset: 240
    name: ${long_name} Ventilator RPM
    id: ${short_name}_rpm
  memory_address_sensor:
    memory_location: 0x20
    memory_address: 0x7d
    name: ${long_name} Adres sensor
    id: ${short_name}_adres_sensor
  stove_state:
    memory_address: 0x21
    name: ${long_name} status
    id: ${short_name}_status
  stove_switch:
    memory_location: 0x80
    memory_address: 0x21
    memory_data_on: 0x01
    memory_data_off: 0x06
    name: ${long_name} Kachel aan/uit
  but_temp_up:
    name: ${long_name} Temp. omhoog
  but_temp_down:
    name: ${long_name} Temp omlaag
```
