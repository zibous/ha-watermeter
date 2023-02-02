# Wemos D1 Mini, CUL + CC1101 (esphome)



![Wemos D1 Min, CUL + CC1101](../docs/d1min_cc1101.png)

### Requirements
- ESPHOME Docker
- Wemos D1 Mini
- CC1101


After Wemos D1 Min, CUL + CC1101 has been wired, the application can be flashed with ESPHOME

```yaml
## ---------------------------------------------------
## WMBUS DEVICE D1MINI WEMOS
## ---------------------------------------------------
wmbus:
  mosi_pin: GPIO13 #D7  Attached to Hardware SPI controller MOSI
  miso_pin: GPIO12 #D6  Attached to Hardware SPI controller MISO
  clk_pin: GPIO14  #D5  Attached to Hardware SPI controller CLK
  cs_pin: GPIO15   #D8  Controls Boot Mode; Attached to Hardware SPI controller CS
  gdo0_pin: GPIO04 #D2  High Impedance
  gdo2_pin: GPIO05 #D1  High Impedance
```  
<hr>

[Wemos D1 Min, CUL + CC1101 configuration see](wmbus-minid1.yaml)

## Tools

The easiest way to use the D! To create mini is to use ESPHOME as a docker application.

### DOCKER ESPHOME

```bash
#!/bin/bash
# ---------------------------------------------
# sudo bash esphome.sh
# see: https://hub.docker.com/r/esphome/esphome
# ----------------------------------------------
IDu=$(id -u) # UID
IDg=$(id -g) # GID
CURRENTURL=http://$(hostname)
DOCKER_TIMEZONE=Europe/Berlin

DOCKER_APPSDIR=/apps/
DOCKER_TIMEZONE=Europe/Berlin
DOCKERIMAGE=esphome/esphome:latest
CONTAINERLABEL=esphome
APPSDATA=$PWD${DOCKER_APPSDIR}${CONTAINERLABEL}

echo "Create persistent data folder and log folder"
mkdir -p ${APPSDATA} 2>&1

echo "Try to remove previuos installation..."
docker stop ${CONTAINERLABEL} >/dev/null 2>&1
docker rm ${CONTAINERLABEL} >/dev/null 2>&1

echo "Install Docker container ${CONTAINERLABEL}."
docker run --detach \
  --name ${CONTAINERLABEL} \
  --privileged=true \
  --env TZ=${DOCKER_TIMEZONE} \
  --restart unless-stopped \
  --volume /run/udev:/run/udev \
  --volume /etc/localtime:/etc/localtime:ro \
  --volume ${APPSDATA}/config:/config \
  --network host \
  ${DOCKERIMAGE}

echo "Docker container ${CONTAINERLABEL} ready."
echo "Run WEBGUI: ${CURRENTURL}:6052"
```

### Log
```
14:29:56	[I]	[main:314]	 -------  SET NEW VALUES !!!!!
14:30:05	[I]	[wmbus:054]	Using driver 'izar' for ID [0x43430778] RSSI: -55 dBm T: 1944A511780743434418A241150013CE0766324C94EE48EEF6C8 (26)
14:30:18	[I]	[wmbus:054]	Using driver 'izar' for ID [0x43430778] RSSI: -55 dBm T: 1944A511780743434418A251150013CE10497BE7B53D9135EFCA (26)
14:30:22	[I]	[wmbus:054]	Using driver 'izar' for ID [0x43430778] RSSI: -55 dBm T: 1944A511780743434418A261150013CE2938A11AD749FB58C4CD (26)
14:30:31	[I]	[wmbus:054]	Using driver 'izar' for ID [0x43430778] RSSI: -55 dBm T: 1944A511780743434418A271150013CE3E17E8B1F69A2283DDCF (26)
14:30:49	[I]	[wmbus:054]	Using driver 'izar' for ID [0x43430778] RSSI: -55 dBm T: 1944A511780743434418A211150013CE4CF45D4B3272F6598BC0 (26)
14:30:57	[I]	[wmbus:054]	Using driver 'izar' for ID [0x43430778] RSSI: -55 dBm T: 1944A511780743434418A221150013CE758587B650069C34A0C7 (26)
14:31:05	[I]	[wmbus:054]	Using driver 'izar' for ID [0x43430778] RSSI: -55 dBm T: 1944A511780743434418A231150013CE62AACE1D71D545EFB9C5 (26)
14:31:14	[I]	[wmbus:054]	Using driver 'izar' for ID [0x43430778] RSSI: -55 dBm T: 1944A511780743434418A241150013CE0766324C94EE48EEF6C8 (26)
14:31:22	[I]	[wmbus:054]	Using driver 'izar' for ID [0x43430778] RSSI: -55 dBm T: 1944A511780743434418A251150013CE10497BE7B53D9135EFCA (26)
14:31:31	[I]	[wmbus:054]	Using driver 'izar' for ID [0x43430778] RSSI: -55 dBm T: 1944A511780743434418A261150013CE2938A11AD749FB58C4CD (26)
14:31:39	[I]	[wmbus:054]	Using driver 'izar' for ID [0x43430778] RSSI: -55 dBm T: 1944A511780743434418A271150013CE3E17E8B1F69A2283DDCF (26)
14:31:48	[I]	[wmbus:054]	Using driver 'izar' for ID [0x43430778] RSSI: -55 dBm T: 1944A511780743434418A201150013CE5BDB14E013A12F8292C2 (26)
14:31:57	[I]	[wmbus:054]	Using driver 'izar' for ID [0x43430778] RSSI: -55 dBm T: 1944A511780743434418A211150013CE4CF45D4B3272F6598BC0 (26)
14:32:13	[I]	[wmbus:054]	Using driver 'izar' for ID [0x43430778] RSSI: -55 dBm T: 1944A511780743434418A231150013CE62AACE1D71D545EFB9C5 (26)
14:32:31	[I]	[wmbus:054]	Using driver 'izar' for ID [0x43430778] RSSI: -55 dBm T: 1944A511780743434418A251150013CE13497BE7B53D9135EFCA (26)
.....
```

## Decoded telegram

`1944A511780743434418A241150013CE0766324C94EE48EEF6C8`

```
Auto driver  : izar
Best driver  : unknown 00/00
Using driver : izar 00/00
000   : 19 length (25 bytes)
001   : 44 dll-c (from meter SND_NR)
002   : a511 dll-mfct (DME)
004   : 43434418 dll-id (18444343)
008   : 78 dll-version
009   : 07 dll-type (Water meter)
010   : a2 tpl-ci-field (Mfct specific)
011 C?: 71150013CE3C17E8B1F69A2283DDCF mfct specific

{
    "media":"water",
    "meter":"izar",
    "name":"",
    "id":"18444343",
    "total_m3":451.399,
    "last_month_total_m3":451.114,
    "last_month_measure_date":"2023-02-01",
    "remaining_battery_life_y":10.5,
    "current_alarms":"no_alarm",
    "previous_alarms":"no_alarm",
    "transmit_period_s":8,
    "timestamp":"2023-02-02T13:49:16Z"
}


Using: wmbusmeters: 1.11.0-60-g89c42ab
89c42ab27dc8e7ac566e8b3a7810243a3bebd84b
```

<hr>

## Homeassitant Device
![Homeassistant](docs/ha-Device.png)

## Tools

[Telegram Decoder](https://wmbusmeters.org/)
<br>
[esphome-flasher, a tool to flash ESPs over USB](https://github.com/esphome/esphome-flasher)
<br>
[Self-contained NodeMCU flasher with GUI based on esptool.py and wxPython](https://github.com/marcelstoer/nodemcu-pyflasher)


### WMBUSMETERS custom components
[Szczepan's esphome custom components](https://github.com/SzczepanLeon/esphome-components)
