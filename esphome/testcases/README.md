# Testcases Watermeter Firmware

## water-meter-izar-test.bin
- WMBUS METER D1 MINI 32 + CC1101 for Diehl IZAR RC 868 I R4 PL
- HARDWARE: ESP32 240MHz, 320KB RAM, 4MB Flash

## water-meter-dm32-test.bin
- WMBUS METER D1 MINI 32 + CC1101 for Diehl IZAR RC 868 I R4 PL
- HARDWARE: ESP32 240MHz, 320KB RAM, 4MB Flash

## water-meter-esp32-izar-test.bin
- WMBUS METER az-delivery-devkit-v4 + CC1101 for Diehl IZAR RC 868 I R4 PL
- Hardware: ESP32 240MHz, 520KB RAM, 4MB Flash

<br>

## Connect to WIFI - Access Point Mode
The ESP device is prepared as a Wi-Fi access point and create its own network. Connect to the Wi-Fi access point to set your local WIFI.

![Access Point Mode](./WLAN_ACCESS.png)

<br>
### LOGGING w/o CC1101 connected

Simple Log from the device, no CC1101 allready connected.

```
INFO ESPHome 2023.8.3
INFO Reading configuration /config/wm-d1mini32-izar-test.yaml...
INFO Updating https://github.com/SzczepanLeon/esphome-components.git@main
INFO Starting log output from water-meter-dm32-test.local using esphome API
INFO Successfully connected to water-meter-dm32-test.local
[15:14:57][I][app:102]: ESPHome version 2023.8.3 compiled on Sep 14 2023, 15:06:36
[15:14:57][I][app:104]: Project Diehl_IZAR_RC_868.Watermeter-Test version 2.0.8
[15:14:57][C][logger:301]: Logger:
[15:14:57][C][logger:302]:   Level: VERBOSE
[15:14:57][C][logger:303]:   Log Baud Rate: 0
[15:14:57][C][logger:305]:   Hardware UART: UART0
[15:14:57][C][logger:309]:   Level for 'wmbus': VERBOSE
[15:14:57][C][logger:309]:   Level for 'wMBus-lib': VERBOSE
[15:14:57][C][logger:309]:   Level for 'sensor': WARN
[15:14:57][C][logger:309]:   Level for 'sensor.filter': WARN
[15:14:57][C][logger:309]:   Level for 'text_sensor': WARN
[15:14:57][C][logger:309]:   Level for 'api.service': ERROR
[15:14:57][C][logger:309]:   Level for 'json': ERROR
[15:14:57][C][logger:309]:   Level for 'mqtt': WARN
[15:14:57][C][logger:309]:   Level for 'scheduler': ERROR
[15:14:57][C][logger:309]:   Level for 'wifi': WARN
[15:14:57][C][logger:309]:   Level for 'component': ERROR
[15:14:57][C][logger:309]:   Level for 'api': WARN
[15:14:57][C][restart:068]: Restart Switch 'Restart Device'
[15:14:57][C][restart:070]:   Icon: 'mdi:restart'
[15:14:57][C][restart:091]:   Restore Mode: always OFF
[15:14:57][C][safe_mode_switch:068]: Safe Mode Switch 'Restart (Safe Mode)'
[15:14:57][C][safe_mode_switch:070]:   Icon: 'mdi:restart-alert'
[15:14:57][C][safe_mode_switch:091]:   Restore Mode: always OFF
[15:14:57][C][factory_reset.switch:068]: Factory Reset Switch 'Restart factory default'
[15:14:57][C][factory_reset.switch:070]:   Icon: 'mdi:lock-reset'
[15:14:57][C][factory_reset.switch:091]:   Restore Mode: always OFF
[15:14:57][C][captive_portal:088]: Captive Portal:
[15:14:57][C][web_server:161]: Web Server:
[15:14:57][C][web_server:162]:   Address: water-meter-dm32-test.local:80
[15:14:57][C][sntp:053]: SNTP Time:
[15:14:57][C][sntp:054]:   Server 1: '0.at.pool.ntp.org'
[15:14:57][C][sntp:055]:   Server 2: '0.pool.ntp.org'
[15:14:57][C][sntp:056]:   Server 3: ''
[15:14:57][C][sntp:057]:   Timezone: 'CET-1CEST,M3.5.0,M10.5.0/3'
[15:14:57][C][mdns:112]: mDNS:
[15:14:57][C][mdns:113]:   Hostname: water-meter-dm32-test
[15:14:57][V][mdns:114]:   Services:
[15:14:57][V][mdns:116]:   - _esphomelib, _tcp, 6053
[15:14:57][V][mdns:118]:     TXT: version = 2023.8.3
[15:14:57][V][mdns:118]:     TXT: mac = 4022d83cfb64
[15:14:57][V][mdns:118]:     TXT: platform = ESP32
[15:14:57][V][mdns:118]:     TXT: board = wemos_d1_mini32
[15:14:57][V][mdns:118]:     TXT: network = wifi
[15:14:57][V][mdns:118]:     TXT: project_name = Diehl_IZAR_RC_868.Watermeter-Test
[15:14:57][V][mdns:118]:     TXT: project_version = 2.0.8
[15:14:57][V][mdns:116]:   - _http, _tcp, 80
[15:14:57][C][ota:093]: Over-The-Air Updates:
[15:14:57][C][ota:094]:   Address: water-meter-dm32-test.local:3232
[15:14:57][C][ota:097]:   Using Password.
[15:14:57][C][wifi_signal.sensor:009]: WiFi Signal 'Device WLAN RSSI Quality'
[15:14:57][C][wifi_signal.sensor:009]:   Device Class: 'signal_strength'
[15:14:57][C][wifi_signal.sensor:009]:   State Class: 'measurement'
[15:14:57][C][wifi_signal.sensor:009]:   Unit of Measurement: '%'
[15:14:57][C][wifi_signal.sensor:009]:   Accuracy Decimals: 0
[15:14:57][V][wifi_signal.sensor:009]:   Unique ID: '4022d83cfb64-wifisignal'
[15:14:57][C][wmbus:394]: wM-Bus v2.2.29:
[15:14:57][C][wmbus:411]:   CC1101 SPI bus:
[15:14:57][C][wmbus:412]:     MOSI Pin: GPIO23
[15:14:57][C][wmbus:413]:     MISO Pin: GPIO19
[15:14:57][C][wmbus:414]:     CLK Pin:  GPIO18
[15:14:57][C][wmbus:415]:     CS Pin:   GPIO5
[15:14:57][C][wmbus:416]:     GDO0 Pin: GPIO21
[15:14:57][C][wmbus:417]:     GDO2 Pin: GPIO22
[15:14:57][E][wmbus:430]:   Check connection to CC1101!

```
