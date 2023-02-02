# wmbusmeters with NANO-CUL

## Raspbian Buster + wmbusmeters with NANO-CUL (mbus) 868 Mhz

**NANO-CUL (mbus) 868** is a lot easier to use because it only requires `wmbusmeters`. The resources (memory) required are also less compared to the variant with RTL-SDR and rtl-wmbus.


![nanoCUL868](docs/nanoCUL868.png)
Please not, that you do need the NANO-CUL (mbus) 868 Mhz for `wmbusmeters` **not for** `***\*FHEM\****`!! . smart-home-komponente.net can flash the firmware for `wmbusmeters`.
<br>

### Install dependencies

- `wmbusmeters`
  Program receives and decodes WMBus telegrams
  https://github.com/weetmuts/wmbusmeters,
  build see: [build-wmbusmeters](docs/build-wmbusmeters.md)

### Install dependencies

- `wmbusmeters`
  Program receives and decodes WMBus telegrams
  https://github.com/weetmuts/wmbusmeters,
  build see: [build-wmbusmeters](docs/build-wmbusmeters.md)

## Integration

In the `wmbusmeters`  reporting mode, data will be published to the MQTT broker topic "`tele/wasser/verbrauch`".

This data can be subscribed to and processed by other applications. From this point forward your options are endless. Example for simple mode - publish all smartmeter display data:

```json
{
    "media": "water",
    "meter": "izar",
    "name": "watermeter",
    "id": "1231150",
    "total_m3": 166.625,
    "last_month_total_m3": 162.614,
    "last_month_measure_date": "2020-07-01",
    "remaining_battery_life_y": 13,
    "current_alarms": "no_alarm",
    "previous_alarms": "no_alarm",
    "timestamp": "2020-07-16T11:32:38Z"
}
```

### Continuous Daemon/Service

You most probably want to execute the program **continuously in the background**. This can be done either by using the internal daemon or cron.

**Attention:** Daemon mode must be enabled in the configuration file (default).

1. Systemd service - on systemd powered systems the **recommended** option

   ```bash
   sudo cp /opt/ha-watermeter/service.template /etc/systemd/system/ha-watermeter.service

   sudo systemctl daemon-reload
   sudo systemctl start ha-watermeter.service
   sudo systemctl status ha-watermeter.service

   sudo systemctl enable ha-watermeter.service
   ```
