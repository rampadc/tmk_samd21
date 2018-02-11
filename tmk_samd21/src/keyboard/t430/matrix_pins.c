/*
* matrix_pins.c
*
* Created: 10/02/2018 2:11:38 AM
*  Author: cong
*/

#include <asf.h>
#include "matrix_pins.h"

#define NUMBER_OF_SENSE_PINS			MATRIX_ROWS
#define NUMBER_OF_DRIVING_PINS		MATRIX_COLS

/************************************************************************/
/* ASF setup ports and pins                                             */
/************************************************************************/
// 16x interruptable input pins, active-low
// array arranged in order of increasing EXINT[i] for i = 0...7
volatile uint32_t sense_pins[NUMBER_OF_SENSE_PINS] = {
	PIN_PA16, PIN_PA17, PIN_PA18, PIN_PA19,
	PIN_PA20, PIN_PA21, PIN_PB06, PIN_PB07
};

volatile uint32_t sense_eic_pins[NUMBER_OF_SENSE_PINS] = {
	PIN_PA16A_EIC_EXTINT0, PIN_PA17A_EIC_EXTINT1, PIN_PA18A_EIC_EXTINT2, PIN_PA19A_EIC_EXTINT3,
	PIN_PA20A_EIC_EXTINT4, PIN_PA21A_EIC_EXTINT5, PIN_PB06A_EIC_EXTINT6, PIN_PB07A_EIC_EXTINT7
};

volatile uint32_t sense_mux[NUMBER_OF_SENSE_PINS] = {
	MUX_PA16A_EIC_EXTINT0, MUX_PA17A_EIC_EXTINT1, MUX_PA18A_EIC_EXTINT2, MUX_PA19A_EIC_EXTINT3,
	MUX_PA20A_EIC_EXTINT4, MUX_PA21A_EIC_EXTINT5, MUX_PB06A_EIC_EXTINT6, MUX_PB07A_EIC_EXTINT7
};

// 8x driving pins
volatile uint32_t drive_pins[NUMBER_OF_DRIVING_PINS] = {
	PIN_PB04, PIN_PB05, PIN_PB08, PIN_PB09,
	PIN_PA08, PIN_PA09, PIN_PA10, PIN_PA11,
	PIN_PB10, PIN_PB11, PIN_PB13, PIN_PB14,
	PIN_PB15, PIN_PA12, PIN_PA13, PIN_PA15
};

/************************************************************************/
/* Convenience pins definitions for speedy matrix scans                 */
/************************************************************************/
volatile Pin_t drive_ports[NUMBER_OF_DRIVING_PINS] = {
	  {((uint32_t *) &REG_PORT_OUT1), PORT_PB04},  {((uint32_t *) &REG_PORT_OUT1), PORT_PB05},  {((uint32_t *) &REG_PORT_OUT1), PORT_PB08},  {((uint32_t *) &REG_PORT_OUT1), PORT_PB09},
	  {((uint32_t *) &REG_PORT_OUT0), PORT_PA08},  {((uint32_t *) &REG_PORT_OUT0), PORT_PA09},  {((uint32_t *) &REG_PORT_OUT0), PORT_PA10},  {((uint32_t *) &REG_PORT_OUT0), PORT_PA11},
	  {((uint32_t *) &REG_PORT_OUT1), PORT_PB10},  {((uint32_t *) &REG_PORT_OUT1), PORT_PB11},  {((uint32_t *) &REG_PORT_OUT1), PORT_PB13},  {((uint32_t *) &REG_PORT_OUT1), PORT_PB14},
	  {((uint32_t *) &REG_PORT_OUT1), PORT_PB15},  {((uint32_t *) &REG_PORT_OUT0), PORT_PA12},  {((uint32_t *) &REG_PORT_OUT0), PORT_PA13},  {((uint32_t *) &REG_PORT_OUT0), PORT_PA15}
};

volatile Pin_t sense_ports[NUMBER_OF_SENSE_PINS] = {
	 {((uint32_t *) &REG_PORT_IN0), PORT_PA16},  {((uint32_t *) &REG_PORT_IN0), PORT_PA17},  {((uint32_t *) &REG_PORT_IN0), PORT_PA18},  {((uint32_t *) &REG_PORT_IN0), PORT_PA19},
	 {((uint32_t *) &REG_PORT_IN0), PORT_PA20},  {((uint32_t *) &REG_PORT_IN0), PORT_PA21},  {((uint32_t *) &REG_PORT_IN1), PORT_PB06},  {((uint32_t *) &REG_PORT_IN1), PORT_PB07}
};

void out_set_low(Pin_t pin) {
	*pin.portw &= ~pin.port_mask;
}

void out_set_high(Pin_t pin) {
	*pin.portw |= pin.port_mask;
}

bool in_get_value(Pin_t pin) {
	return (*pin.portw & pin.port_mask) ? true: false;
}

void out_set_low_all() {
	uint8_t i;
	for (i = 0; i < NUMBER_OF_DRIVING_PINS; i++) {
		out_set_low(drive_ports[i]);
	}
}
void out_set_high_all() {
	uint8_t i;
	for (i = 0; i < NUMBER_OF_DRIVING_PINS; i++) {
		out_set_high(drive_ports[i]);
	}
}