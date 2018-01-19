# tmk_samd21

Runs TMK keyboard core on top of a SAM D21 microcontroller. This project is compiled in Atmel Studio 7 and runs on top of ASF. 

Substantial portions of TMK core removed due to incompatibility with SAM D21 as well as they do not fit my needs.

As of 02/01/18, LEDs, custom layered matrices, mouse and sleep LEDs have not been tested or implemented. 

Configured with:
- 1ms timer interrupt
- Delay service
- STDIO over CDC

PID, VID will need to be reconfigured
