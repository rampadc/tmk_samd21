/*
* matrix.c
*
* Created: 2/01/2018 12:49:01 AM
*  Author: cong
*
* Using SW0 as a button
* - Imagine there is 1 row and 1 col
* -- Rows are outputs (GND), cols are inputs (SW0)
* -- Col is active-low
*/

#include <asf.h>
#include <stdio.h>

#include "matrix.h"
#include "keyboard.h"

#ifdef KBD_ONEKEY

static void configure_extint_channel(void);
static void configure_extint_callbacks(void);
static void column_interrupt_callback(void);

volatile bool needs_matrix_scan = false;

/************************************************************************/
/* Matrix implementation functions                                      */
/************************************************************************/
static matrix_row_t matrix[MATRIX_ROWS];

void matrix_init(void) {
	needs_matrix_scan = false;
	// prep for column interrupt mode
	configure_extint_channel();
	configure_extint_callbacks();
}

uint8_t matrix_scan() {
	uint8_t r;
	uint8_t c;
	for (r = 0; r < MATRIX_ROWS; r++) {
		// strobe row: SW0 already GNDed, nothing to do here
		for (c = 0; c < MATRIX_COLS; c++) {
			bool pin_state = port_pin_get_input_level(BUTTON_0_PIN);
			if (pin_state == BUTTON_0_ACTIVE) {
				matrix[r] |= 1 << c;
				} else {
				matrix[r] &= ~(1 << c);
			}
		}
	}

	needs_matrix_scan = false;
	return 1;
}

matrix_row_t matrix_get_row(uint8_t row) {
	return matrix[row];
}

bool matrix_is_on(uint8_t row, uint8_t col) {
	return (matrix[row] & (1 << col));
}

/************************************************************************/
/* Configuration functions                                              */
/************************************************************************/
void column_interrupt_callback() {
	// awake from slumber
	needs_matrix_scan = true;
}

void configure_extint_channel() {
	struct extint_chan_conf config_extint_chan;
	extint_chan_get_config_defaults(&config_extint_chan);

	// SW0: PA15,
	config_extint_chan.gpio_pin = PIN_PA15A_EIC_EXTINT15;
	config_extint_chan.gpio_pin_mux = MUX_PA15A_EIC_EXTINT15;
	config_extint_chan.gpio_pin_pull = EXTINT_PULL_UP;
	config_extint_chan.filter_input_signal = true;
	config_extint_chan.detection_criteria = EXTINT_DETECT_BOTH;
	extint_chan_set_config(BUTTON_0_EIC_LINE, &config_extint_chan);
}

void configure_extint_callbacks() {
	extint_register_callback(column_interrupt_callback, BUTTON_0_EIC_LINE, EXTINT_CALLBACK_TYPE_DETECT);
	extint_chan_enable_callback(BUTTON_0_EIC_LINE, EXTINT_CALLBACK_TYPE_DETECT);
}

#endif
