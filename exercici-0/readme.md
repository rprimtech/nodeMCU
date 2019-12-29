# Instal·lació del suport per ESP8266 a l'Arduino IDE

Starting with 1.8.10, Arduino allows installation of third-party platform packages using Boards Manager. We have packages available for Windows, Mac OS, and Linux (32 and 64 bit).

* Install the current upstream Arduino IDE at the 1.8 level or later. The current version is at the Arduino website.
* Start Arduino and open Preferences window.
* Enter `http://arduino.esp8266.com/stable/package_esp8266com_index.json ` into Additional Board Manager URLs field. You can add multiple URLs, separating them with commas.
* Open Boards Manager from Tools > Board menu and install esp8266 platform (and don't forget to select your ESP8266 board from Tools > Board menu after installation).

Recomanació d'utilització de les GPIO's:

| GPIO | Consell |
|---|---|
| ADC0 | ENTRADA ANALÒGICA |
| GPIO4 | I2C (SDA) |
| GPIO5 | I2C (SCL) |
| GPIO9 | NO UTILITZAR MAI |
| GPIO10 | UTILITZAR PER SENSORS |
| GPIO0 | PER SORTIDA, AMB PULL-UP |
| GPIO2 | PER SORTIDA, AMB PULL-UP |
| GPIO15 | PER NEOPIXEL |
| GPIO1 | NO UTILITZAR |
| GPIO3 | NO UTILITZAR |
| GPIO16 | BUILTIN LED |

![nodeMCU](https://i2.wp.com/www.esploradores.com/wp-content/uploads/2016/08/PINOUT-NodeMCU-1_1.8.png?fit=1026%2C570)
