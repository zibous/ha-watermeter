# Water Statistics

Gather statistics for:
* today
* yesterday
* week
* month

```yaml
# Example configuration entry
...
external_components:
  - source:
      type: local
      path: custom_components
    components: [water_statistics]
    refresh: 0s
...
sensor:
  - platform: "water_statistics"
    total: total
    water_today:
      name: "$name Water Today"
    water_yesterday:
      name: "$name Water Yesterday"
    water_week:
      name: "$name Water Week"
    water_month:
      name: "$name Water Month"
```

## Configuration variables:
* **id** (*Optional*, [ID](https://esphome.io/guides/configuration-types.html#config-id)): Manually specify the ID used for code generation.
* **total** (**Required**, [ID](https://esphome.io/guides/configuration-types.html#config-id)): The ID of the total power sensor.
* **water_today** (*Optional*, Sensor):
  * Any options from [Sensor](https://esphome.io/components/sensor/index.html#config-sensor).
* **water_yesterday** (*Optional*, Sensor):
  * Any options from [Sensor](https://esphome.io/components/sensor/index.html#config-sensor).
* **water_week** (*Optional*, Sensor):
  * Any options from [Sensor](https://esphome.io/components/sensor/index.html#config-sensor).
* **water_month** (*Optional*, Sensor):
  * Any options from [Sensor](https://esphome.io/components/sensor/index.html#config-sensor).
