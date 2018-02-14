/*
* usb_callbacks.c
*
* Created: 1/01/2018 5:02:57 PM
*  Author: cong
*/
#include <asf.h>
#include <stdio.h>
#include <stdint.h>

#include "conf_usb.h"
#include "led.h"

volatile bool usb_remote_wakeup_enabled = false;
volatile bool usb_suspended = false;
volatile bool usb_hid_kbd_enabled = false;
volatile bool usb_hid_mouse_enabled = false;
volatile bool usb_cdc_port_opened = false;

volatile uint8_t led_states = 0;

/************************************************************************/
/*  USB Device callbacks                                                */
/************************************************************************/
void usb_callback_vbus_action(bool b_vbus_high) {
}

void usb_callback_sof_action(void) {

}

void usb_callback_suspend_action(void) {
	usb_suspended = true;
}

void usb_callback_resume_action(void) {
	usb_suspended = false;
}

void usb_callback_remotewakeup_enable(void) {
	usb_remote_wakeup_enabled = true;
}

void usb_callback_remotewakeup_disable(void) {
	usb_remote_wakeup_enabled = false;
}
bool usb_callback_extra_string(void) {
	return true;
}

/************************************************************************/
/*  CDC callbacks                                                       */
/************************************************************************/
bool cdc_callback_enable(void) {
	#ifndef NO_PRINT
	stdio_usb_enable();
	usb_cdc_port_opened = true;
	return true;
	#else
	return false;
	#endif
}
void cdc_callback_disable(void) {
	#ifndef NO_PRINT
	stdio_usb_disable();
	usb_cdc_port_opened = false;
	#endif
}
void cdc_callback_rx_notify(uint8_t port) {
	
}
void cdc_callback_tx_empty_notify(uint8_t port) {

}
void cdc_callback_set_dtr(uint8_t port, bool b_enable) {

}
void cdc_callback_set_rts(uint8_t port, bool b_enable) {

}

/************************************************************************/
/*  HID Keyboard callbacks                                              */
/************************************************************************/
bool hid_kbd_callback_enable(void) {
	usb_hid_kbd_enabled = true;
	return true;
}
void hid_kbd_callback_disable(void) {
	usb_hid_kbd_enabled = false;
}
void hid_kbd_callback_led(uint8_t value) {
	led_set(value);
}

/************************************************************************/
/*  HID Mouse callbacks                                                 */
/************************************************************************/
bool hid_mouse_callback_enable(void) {
	usb_hid_mouse_enabled = true;
	return true;
}
void hid_mouse_callback_disable(void) {
	usb_hid_mouse_enabled = false;
}