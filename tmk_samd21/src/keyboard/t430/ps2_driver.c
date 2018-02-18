/*
* ps2_driver.c
*
* Created: 14/02/2018 8:50:16 PM
*  Author: Cong
*/

#include "ps2.h"
#include "io_pins.h"
#include <asf.h>

uint8_t ps2_clock_eic_channel = 15;

#define CLOCK_PIN	PIN_PA27
#define DATA_PIN	PIN_PB23
#define RESET_PIN	PIN_PA02

#define CLOCK_GROUP	0
#define DATA_GROUP	1
#define RESET_GROUP	0
#define CLOCK_MASK	PORT_PA27
#define DATA_MASK	PORT_PB23
#define RESET_MASK	PORT_PA02
#define CLOCK_OFFSET	27
#define DATA_OFFSET		23
#define RESET_OFFSET	2

Pin_t clock_in_pin = {((uint32_t *) &REG_PORT_IN0), PORT_PA27};
Pin_t data_in_pin = {((uint32_t *) &REG_PORT_IN1), PORT_PB23};

static void enable_clock_interrupt();
static void disable_clock_interrupt();
static bool read_line(line_t line);
static void set_line(line_t line, bool state);
static void toggle_reset_line(void);

ps2_driver_t ps2_driver = {
	set_line,
	read_line,
	enable_clock_interrupt,
	disable_clock_interrupt,
	toggle_reset_line
};

void toggle_reset_line(void) {
	PORT->Group[RESET_GROUP].DIR.reg |= RESET_MASK;
	PORT->Group[RESET_GROUP].OUT.reg |= RESET_MASK;
	delay_us(3);
	PORT->Group[RESET_GROUP].OUT.reg &= ~RESET_MASK;
	
}

void enable_clock_interrupt() {
	// initial setup phase, we can use ASF here
}

void disable_clock_interrupt() {
	// initial setup phase, we can use ASF here
}

bool read_line(line_t line) {
	switch (line) {
		case CLOCK_LINE:
		return in_get_value(clock_in_pin);
		case DATA_LINE:
		return in_get_value(data_in_pin);
	}
}

void set_line(line_t line, bool state) {
	if (line == CLOCK_LINE) {
		if (state == false) {
			// output low, 6.6us
			PORT->Group[CLOCK_GROUP].OUT.reg &= ~CLOCK_MASK;
			PORT->Group[CLOCK_GROUP].DIR.reg |= CLOCK_MASK;
		}
		else {
			// input pull-up, 4.8us
			PORT->Group[CLOCK_GROUP].PINCFG[CLOCK_OFFSET].bit.INEN = 1;
			PORT->Group[CLOCK_GROUP].PINCFG[CLOCK_OFFSET].bit.PULLEN = 1;
			PORT->Group[CLOCK_GROUP].OUT.reg |= CLOCK_MASK;
			PORT->Group[CLOCK_GROUP].DIR.reg &= ~CLOCK_MASK;
		}
	}
	else if (line == DATA_LINE) {
		if (state == false) {
			// output low, 6.6us
			PORT->Group[DATA_GROUP].OUT.reg &= ~DATA_MASK;
			PORT->Group[DATA_GROUP].DIR.reg |= DATA_MASK;
		}
		else {
			// input pull-up, 4.8us
			PORT->Group[DATA_GROUP].PINCFG[DATA_OFFSET].bit.INEN = 1;
			PORT->Group[DATA_GROUP].PINCFG[DATA_OFFSET].bit.PULLEN = 1;
			PORT->Group[DATA_GROUP].OUT.reg |= DATA_MASK;
			PORT->Group[DATA_GROUP].DIR.reg &= ~DATA_MASK;
		}
	}
}