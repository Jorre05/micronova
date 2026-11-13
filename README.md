# ESPhome component for Micronova board based pellet stoves. 

The Micronova component is now part of ESPHome. This repo is still valid and is the first place where new features and bugfixes show up.
Full ESPHome documentation: https://esphome.io/components/micronova

You have to build a simple circuit to interface with your stove see ESPHome documentation for the details. You can also order a board from 
[@philibertc](https://github.com/philibertc), that should make it dead easy.

## Example configuration

The hardest part is to configure the correct IO pins for the UART and the enable_rx. 
Users have reported that when you got the interface board from [@philibertc](https://github.com/philibertc), then the pin-config below should work:

```yaml
uart:
  tx_pin: D4
  rx_pin: D3
  baud_rate: 1200
  stop_bits: 2

micronova:
  enable_rx_pin: D2
  serial_reply_delay: 100ms
```

A generic example below. All buttons, sensors, text_sensors, switch and numbers accept a memory_location and memory_address. Specify those if the defaults don't work for you.

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
  enable_rx_pin: 7
  update_interval: 20s
  serial_reply_delay: 100ms

text_sensor:
  - platform: micronova
    stove_state:
      name: Stove status

number:
  - platform: micronova
    thermostat_temperature:
      name: Thermostat temperature
      step: 1
    power_level:
      name: Power level
      max_value: 4

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
    custom_button:
      name: Custom button
      memory_location: 0x20
      memory_address: 0x7d
      memory_data: 0x08

```

## Stoves that store the power level with an offset

On some stoves the power level is not stored as a plain number. Instead, a fixed offset is added to it (for example, level 3 is stored as 139 with an offset of 136). These boards also copy their settings from EEPROM to RAM at startup and only use the RAM copy afterwards, so writing the power level to EEPROM alone has no effect until the stove restarts.

Two options on `power_level` handle this:

- `power_level_offset` (default `0`): subtracted from the value read from the stove, and added to the value written. `max_value` + `power_level_offset` must not exceed 255.
- `ram_write_address` (optional): when set, the new power level is also written to this RAM address, so the change takes effect immediately. `ram_write_location` defaults to `0x80`. When `ram_write_address` is omitted, no extra write is sent.

Example for such a stove:

> **Tested on:** La Nordica-Extraflame Comfort Idro 13 kW (older pellet insert model, not the current Comfort Idro L80). Addresses and the power level offset may differ on newer models or other Micronova board versions.

```yaml
number:
  - platform: micronova
    power_level:
      name: Power level
      memory_location: 0x20    # read from EEPROM
      memory_address: 0x60     # stored with offset (level 3 = 139)
      max_value: 5
      power_level_offset: 136
      ram_write_address: 0x37  # RAM copy, also stored with offset

sensor:
  - platform: micronova
    stove_power:
      name: Stove power level
      memory_location: 0x00
      memory_address: 0x38     # actual power, stored WITHOUT offset (level 3 = 3)
```

On this stove type, EEPROM `0x60` and RAM `0x37` both hold the power level with the offset added, while RAM `0x38` (the actual running power) holds the plain value. That's why `power_level_offset` is set on the number but not needed on the `stove_power` sensor.

## Water temperature reported at half the real value

By default `water_temperature` divides the raw stove value by 2 (0.5 °C resolution). If your stove reports whole degrees, the sensor shows half the real temperature. Correct it with a standard ESPHome filter:

```yaml
sensor:
  - platform: micronova
    water_temperature:
      name: Water temperature
      filters:
        - multiply: 2
```

## Creating a climate component overlay

**I'm working on a climate component for the Mirconova platform, in the future this may no longer be necessary**

You can configure a climate component that links, back and forth, to the Micronova component. The example below will synchronize the thermostat setpoint and switch the stove on and off. Also when you change the thermostat setting on the stove or via the number component, that value will link back to the climate component. The same goes voor the stove switch state. 

  The if-conditions in the Lambda are there to make sure not to create an endless loop.

  This climate component looks just like any other climate control in Home Assistant.

![Climate screenshot](images/climate_pellet.png?raw=true "Climate overlay")

```yaml
...
sensor:
  - platform: micronova
    room_temperature:
      name: Micronova Room Temperature
      id: micronova_room_temperature
...
number:
  - platform: micronova
    thermostat_temperature:
      name: Micronova Thermostat
      id: micronova_thermostat
      step: 1
      on_value:
        then:
          - lambda: |-
              if ( id(ha_thermostat).target_temperature != id(micronova_thermostat).state ) {
                auto call = id(ha_thermostat).make_call();
                call.set_target_temperature(id(micronova_thermostat).state);
                call.perform();
              }
...
switch:
   - platform: micronova
     stove:
      name: Stove on/off
      id: stove_switch
      on_turn_on:
        - lambda: |-
            if ( id(ha_thermostat).mode != CLIMATE_MODE_HEAT ) {
              auto call = id(ha_thermostat).make_call();
              call.set_mode("HEAT");
              call.perform();
            }
      on_turn_off:
        - lambda: |-
            if ( id(ha_thermostat).mode != CLIMATE_MODE_OFF ) {
              auto call = id(ha_thermostat).make_call();
              call.set_mode("OFF");
              call.perform();
            }
...
climate:
  - platform: thermostat
    name: HA thermostaat
    id: ha_thermostat
    sensor: micronova_room_temperature
    min_idle_time: 0s
    min_heating_off_time: 0s
    min_heating_run_time: 0s
    default_preset: Startup preset
    on_boot_restore_from: default
    preset:
      - name: Startup preset
        default_target_temperature_low: 22
    visual:
      min_temperature:  0 °C
      max_temperature: 40 °C
     temperature_step:
        target_temperature: 1
        current_temperature: 0.5
    idle_action:
      - lambda: |-
          ESP_LOGD("main","Bogus idle action");
    heat_action:
      - lambda: |-
          ESP_LOGD("main","Bogus heat action");
    heat_mode:
      - lambda: |-
          if ( ! id(stove_switch).state ) {
            id(stove_switch).turn_on();
          }
    off_mode:
      - lambda: |-
          if ( id(stove_switch).state ) {
            id(stove_switch).turn_off();
          }
    target_temperature_change_action:
       - lambda: |-
           if ( id(ha_thermostat).target_temperature != id(micronova_thermostat).state ) {
             auto call = id(micronova_thermostat).make_call();
             call.set_value(id(ha_thermostat).target_temperature);
             call.perform();
           }
```
