/*
* led.c
*
* Created: 2/01/2018 4:12:32 PM
*  Author: cong
*/

#include <asf.h>
#include <stdio.h>
#include "io_pins.h"
#include "led.h"

void led_set(uint8_t usb_led) {
	bool capslockOn = usb_led & (1 << USB_LED_CAPS_LOCK);
	bool numlockOn = usb_led & (1 << USB_LED_NUM_LOCK);
	bool scrollLockOn = usb_led & (1 << USB_LED_SCROLL_LOCK);
	printf("capslockOn: %d\r\n", capslockOn);
	printf("numlockOn: %d\r\n", numlockOn);
	printf("scrollLockOn: %d\r\n", scrollLockOn);
	//if (usb_led & (1 << USB_LED_CAPS_LOCK)) {
		//port_pin_set_output_level(PIN_PB02, LED_0_ACTIVE);
		//} else {
		//port_pin_set_output_level(PIN_PB02, LED_0_INACTIVE);
	//}
}