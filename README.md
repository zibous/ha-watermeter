# Easy Watermeter Integration for Home Assistant

[![License][license-shield]][license]
[![ESPHome release][esphome-release-shield]][esphome-release]
[![Open in Visual Studio Code][open-in-vscode-shield]][open-in-vscode]
[![Support author][donate-me-shield]][donate-me]


[license-shield]: https://img.shields.io/static/v1?label=License&message=MIT&color=orange&logo=license
[license]: https://opensource.org/licenses/MIT

[esphome-release-shield]: https://img.shields.io/static/v1?label=ESPHome&message=2023.5.0&color=green&logo=esphome
[esphome-release]: https://GitHub.com/esphome/esphome/releases/

[open-in-vscode-shield]: https://img.shields.io/static/v1?label=+&message=Open+in+VSCode&color=blue&logo=visualstudiocode
[open-in-vscode]: https://open.vscode.dev/zibous/ha-watermeter

[donate-me-shield]: https://img.shields.io/static/v1?label=+&color=orange&message=Buy+me+a+coffee
[donate-me]: https://www.buymeacoff.ee/zibous

<br>

## Water meter with IZAR module (Diehl IZAR RC 868 I R4 PL)

![diehl_metering](./docs/diehl_metering.jpg)


I did some experiments with **wmbusmeters**, **DVT SDR_STICK**, **NANOCUL** and ended up with: **ESPHome - ESP32 (v4) + CC1101**.

With the NANOCUL solutions, I always had problems with the USB input and with the high resource consumption on the NUC or Raspberry computers.

<br>

## ESPHome + ESP32 (v4) + CC1101

ESPHome - ESP32 (v4) + CC1101 is the best solution for me because the resource requirement is low.

<br>

## WEMOS D1MINI  vs ESP32 (v4)
The version with WEMOS D1MINI was my first version, but it was not stable when integrated into Homeassistant. When i disable the `api`
both D1MINI are working.  It looks like the 2nd D1MINI has a memory problem.

|  Device | Remarks   | State  | 
|---------|-----------|--------|
| D1MINI 1 |  CC1101   |  ✔︎ works |
| D1MINI 2 |  CC1101   |  ✕ JSON Error and keeps rebooting      |
| ESP32    |  CC1101   |  ✔︎ works      |

> Note: Both D1MINI are wired indent and have the same hardware and
from the same supplier and same order. One works fine and the other always boots with a JSON error.

The ESP32 variant works w/o theese errors.

<br>

## ESP32 (v4) Variante

![ESPHome - ESP32 (v4) + CC1101](./esphome/docs/esp32_cc1101.png)

### Requirements

- Water meter with IZAR module (Diehl IZAR RC 868 I R4 PL),
  IZAR Radio Compact Hall is designed for mobile reading and fixed network remote reading of Diehl Metering meters.
  <br>
- [ ESP32 Dev Kit C V4 NodeMCU](https://amzn.eu/d/eUNLyYc)
- [Fayme CC1101 868MHZ Funk ÜBertragung Antennen Transceiver Modul, Grün](https://amzn.eu/d/i5YwBkR)


### Meter types:

Currently supported meter types (wmbus version 2.0.5):

- amiplus
- apator08
- apator162
- apatoreitn
- bmeters
- elf
- evo868
- fhkvdataiii
- hydrocalm3
- itron
- izar
- mkradio3
- mkradio4
- qheat
- qwater
- sharky774
- topaseskr
- ultrimis
- unismart
- vario451

see: <https://github.com/SzczepanLeon/esphome-components>

## Installation / Details
[ESPHome - ESP32 (v4) + CC1101](./esphome/README.md)

### What I still miss
Only the `total_m3` are currently decoded from the telegram. I am still looking for support on how to determine the further information from the telegram:

- Alarm (current_alarms, previous_alarms)
- Water last month (last_month_total_m3)
- Last Month date (last_month_measure_date)
- Battery (remaining_battery_life_y)
- Periode (transmit_period_s)

> see: <https://github.com/weetmuts/wmbusmeters/discussions/835>


## Result

![ESPHOME-WATERMETER](./esphome/docs/eshome_webui.png)


<hr>

## Other solutions (alternatively)

- 1. [NANOCUL-DOCKER.md](NANOCUL-DOCKER.md)
- 2. [NANO-CUL.md](NANO-CUL.md)
- 3. [RB3B_DVBT - RTL232-md](RTL232-md)



### For more information see:

- <https://github.com/weetmuts/wmbusmeters>

- <https://github.com/maciekn/izar-wmbus-esp>
- <https://github.com/MariuszWoszczynski/ESPhome-IZAR-meter-reader>

- <https://github.com/SzczepanLeon/esphome-components>

- <https://osmocom.org/projects/rtl-sdr/wiki/Rtl-sdr>
- <https://github.com/xaelsouth/rtl-wmbus>
- <https://github.com/ZeWaren/izar-prios-smart-meter-collector>

#### DVB-T receiver or Nano CUL Adapter
- <https://www.amazon.de/Tragbarer-Mini-Digitaler-TV-Stock-Unterst%C3%BCtzung-SDR-Stimmer-Empf%C3%A4nger/dp/B013Q94CT6/ref=sr_1_2?__mk_de_DE=%C3%85M%C3%85%C5%BD%C3%95%C3%91&crid=3F9GCJKEPPNO7&dchild=1&keywords=dvbt+dongle&qid=1594904776&sprefix=dvbt+dong%2Caps%2C184&sr=8-2>
<https://www.smart-home-komponente.de/nano-cul/nano-cul-868-extra/>

- <https://www.diehl.com/metering/en/portfolio/software-system-components/software-system-components-products/software-system-components-product/izar-radio-compact-hall/74843/>

