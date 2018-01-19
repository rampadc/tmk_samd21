/*
 * tmk_host.c
 *
 * Created: 2/01/2018 4:24:02 PM
 *  Author: cong
 * Bindings for ATSAMD21
 */ 

#include <asf.h>
#include <stdint.h>
#include "host.h"

extern uint8_t led_states;

/* Host driver */
static uint8_t keyboard_leds(void);
static void send_keyboard(report_keyboard_t *report);
static void send_mouse(report_mouse_t *report);
static void send_system(uint16_t data);
static void send_consumer(uint16_t data);

host_driver_t samd21_driver = {
	keyboard_leds,
	send_keyboard,
	send_mouse,
	send_system,
	send_consumer
};

static uint8_t keyboard_leds(void)
{
	return led_states;
}
static void send_keyboard(report_keyboard_t *report)
{
	// unused
}

static void send_mouse(report_mouse_t *report)
{
}
static void send_system(uint16_t data)
{
	// unused
}

static void send_consumer(uint16_t data)
{
	// unused
}