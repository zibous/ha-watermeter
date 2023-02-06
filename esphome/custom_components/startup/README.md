# Startup Sensor

The startup sensor is mostly like the [Uptime Sensor](https://esphome.io/components/sensor/uptime.html) and allows you to track the time the ESP has stayed up, but uses `Home Assistant` device class `timestamp`, so HA calculates uptime for you automatically.

```yaml
# Example configuration entry
sensor:
  - platform: startup
    name: Startup
```

## Configuration variables:
* **update_interval** (*Optional*, [Time](https://esphome.io/guides/configuration-types.html#config-time)): The interval to check the sensor. Defaults to 60s.
* **id** (*Optional*, [ID](https://esphome.io/guides/configuration-types.html#config-id)): Set the ID of this sensor for use in lambdas.
* All other options from Sensor.
