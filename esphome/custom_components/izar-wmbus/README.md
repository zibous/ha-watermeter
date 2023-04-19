# Izar water meter reader (ESP8266 and ESP32 -based)
The idea behind the project is to read Izar water meter current consumption

Implementation is naive, and hardcoded in many places. It was meant to be simple ;)

## Required components
- NodeMCU (however any ESP8266-based device should work fine)
- CC1101 module (ideally with an antena tuned to 868MHz *)

_Note_: While CC1101 chip is versatile and may by configured programatically to use different frequency, some PCB components has to be selected for better performance. Therefore, please pay attention to get the right one.More info here: https://wiki.fhem.de/wiki/Selbstbau_CUL

I was able to use CC1101 board tuned for 433MHz correctly, though ;)

## Usage

```
IzarWmbus reader;

void setup() {
    //somewhere in the setup
    reader.init(0);
}


IzarResultData data;

void loop() {
    FetchResult result = reader.fetchPacket(&data);
    if (result == FETCH_SUCCESSFUL) {
        // we have new package!
    }
}

```


## Wiring
###  ESP8266
```
CC1101  -> NodeMCU
==================
SCK     -> D5
MISO    -> D6
MOSI    -> D7
CSN     -> D8
VCC     -> 3V
GND     -> GND
```

### ESP32-CAM
My main challenge with wiring ESP32-CAM was use of the default SPI connection by camera module, so I had to divert to other pin set
```

CC1101  -> ESP32(left side)
============================
           5V
           GND
MISO    -> GPIO12
MOSI    -> GPIO13
CSN     -> GPIO15
SCK     -> GPIO14
           GPIO2
           GPIO4


(right side)
VCC     -> 3V3
GDN     -> GND
```

More details about wiring in library this tool is using for connecting with CC1101: https://github.com/LSatan/SmartRC-CC1101-Driver-Lib

### CC1101 868Mhz pinout
(connectors on the left, ANT on the right)

```
VCC   -----------------
GND   |               |
MOSI  |               |
SCK   | CC1101        | GND
MISO  |               | ANT
GDO2  |               | GND
GDO0  |     crystal   |
CSN   |_______________-
```

## Building and usage
The library is based on PlatformIO and is build through PlatformIO's toolchain.

## Related materials:
- Application note on using CC1101 with Wirless MBUS: https://www.ti.com/lit/an/swra234a/swra234a.pdf
- CULFW source code: https://github.com/heliflieger/a-culfw/
- App filtering WMBUS packets from RTL: https://github.com/xaelsouth/rtl-wmbus
- Almighty WMBUS meters decoders library: https://github.com/weetmuts/wmbusmeters
