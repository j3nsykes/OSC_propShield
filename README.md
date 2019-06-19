# OSC_propShield

## Teensy Propshield Orientation sent over OSC

Using Adafruit AirLift to transmit Yaw, Pitch and Roll over OSC to Max Msp and/or Processing. 

Wiring config for Teensy
* Reset Pin = 5
* Busy Pin = 7
* Chip Select(CS) Pin = 10
* MOSI = 11
* MISO = 12
* SCK = 13
* GND = GND
* Vin = 5V

### Dependencies: 
* NPX Motion Sense Library by Paul Stoffregen [here](https://github.com/PaulStoffregen/NXPMotionSense)
* Adafruit's fork of the WifiNINA library [here](https://learn.adafruit.com/adafruit-airlift-breakout/arduino)
* CNMAT OSC library [here](https://github.com/CNMAT/OSC)

