# HalloweenESPSpeakers

[![Build Status](https://travis-ci.com/debsahu/HalloweenESPSpeakers.svg)](https://travis-ci.com/debsahu/HalloweenESPSpeakers) [![License: MIT](https://img.shields.io/github/license/debsahu/HalloweenESPSpeakers.svg)](https://opensource.org/licenses/MIT) [![version](https://img.shields.io/github/release/debsahu/HalloweenESPSpeakers.svg)](https://github.com/debsahu/HalloweenESPSpeakers/releases/tag/1.2.0) [![LastCommit](https://img.shields.io/github/last-commit/debsahu/HalloweenESPSpeakers.svg?style=social)](https://github.com/debsahu/HalloweenESPSpeakers/commits/master)

**Halloween Special: ESP8266 based speakers w/ Motion-sensing & LEDS | Pumpkin carving with powertools**

[![HalloweenESPSpeakers](https://img.youtube.com/vi/RaEtZnQZODk/0.jpg)](https://www.youtube.com/watch?v=RaEtZnQZODk)

Using an ESP8266 with PCM5102 to play MP3 files stored in SPIFFs. There is a motion sensor to trigger playback and LEDs for Halloween effects.

## Wiring

### [PCM5102 DAC > ESP8266/NODEMCU]

* VCC  > 3.3
* 3.3V > 3.3
* GND  > GND
* FLT  > GND
* DMP  > GND
* SCL  > GND
* BCK  > GPIO15/D8 (I2S CLOCK)
* DIN  > GPIO3/RX  (I2S DATA)
* LCK  > GPIO2/D4  (L/R CONTROL)
* FMT  > GND
* XMT  > 3.3

### [AM312 > ESP8266/NODEMCU]

* VCC > 3.3
* SIG > D5/GPIO14
* GND > GND

### [WS2812B > ESP8266/NODEMCU]

* VCC > 5V
* DATA/DIN > D7/GPIO13
* GND > GND


## Libraries Needed

[platformio.ini](https://github.com/debsahu/HalloweenESPSpeakers/blob/master/platformio.ini) is included, use [PlatformIO](https://platformio.org/platformio-ide) and it will take care of installing the following libraries.

| Library                   | Link                                                       | Platform    |
|---------------------------|------------------------------------------------------------|-------------|
|FastLED                    |https://github.com/FastLED/FastLED                          |ESP8266/32   |
|ESP8266Audio               |https://github.com/earlephilhower/ESP8266Audio              |ESP8266/32   |

 
