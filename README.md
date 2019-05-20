# tmk_samd21

![maintenance-no-2018](https://img.shields.io/maintenance/no/2018.svg)

Runs TMK keyboard core on top of a SAM D21 microcontroller. This project is compiled in Atmel Studio 7 and runs on top of ASF. 

Substantial portions of TMK core removed due to incompatibility with SAM D21 as well as they do not fit my needs.

----------------------------------------------------------------------------
T430 example implemented:
 - Keymap layer switching with FN pin
 - LED callbacks (numlock, capslock, scroll-lock)
 - Backlight
 - Remote wake-uo

To do:
 - TrackPoint
 - Sleep LED (on power button)
 - Bluetooth HID support (BTLC1000ZR)
 - Media, consumer keycodes (included in keymap but OS not acknowledging)

Base USB HID project configured with:
- 1ms timer interrupt
- Delay service
- STDIO over CDC

PID, VID will need to be reconfigured
