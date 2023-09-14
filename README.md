ESPhome component for Micronova board based pellet stoves. 

There is a pull request in the ESPHome repo (https://github.com/esphome/esphome/pull/4760).
Documentation: https://deploy-preview-2890--esphome.netlify.app/components/micronova.html

Documentation is currently not up-to-date. The component is now separated from the sensors, switches and buttons. Follow the example below.
The current version contains breaking changes. If you want the stable version, use this external component configuration:

```yaml
external_components:
  - source: github://Jorre05/micronova@v2.1.0
    components: [ micronova ]
```

Make sure to configure the correct IO pins for the UART and the enable_rx.
Users have reported than when you got the interface board from Philibert.c, then the pin-config below should work:

```yaml
uart:
  tx_pin: D4
  rx_pin: D3
  baud_rate: 1200
  stop_bits: 2

micronova:
  enable_rx_pin: D2
```

Example config:
```yaml
uart:
  tx_pin: 5
  rx_pin: 4
  baud_rate: 1200
  stop_bits: 2

external_components:
  - source: github://Jorre05/micronova
    components: [ micronova ]

micronova:
  enable_rx_pin: 4
  update_interval: 20s

text_sensor:
  - platform: micronova
    stove_state:
      name: Stove status

number:
  - platform: micronova
    thermostat_temperature:
      name: Thermostat temperature

sensor:
  - platform: micronova
    room_temperature:
      name: Room temperature
    fumes_temperature:
      name: Fumes temperature
    stove_power:
      name: Stove power level
    fan_speed:
      fan_rpm_offset: 240
      name: Fan RPM
    water_temperature:
      name: Water temperature
    water_pressure:
      name: Water pressure
    memory_address_sensor:
      memory_location: 0x20
      memory_address: 0x7d
      name: Custom Adres sensor

switch:
   - platform: micronova
     stove:
      name: Stove on/off switch

button:
  - platform: micronova
    temperature_up:
      name: Thermostat Up
    temperature_down:
      name: Thermostat Down
```

