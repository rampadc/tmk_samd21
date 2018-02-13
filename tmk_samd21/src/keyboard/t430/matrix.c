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

#include "io_pins.h"

#ifdef KBD_T430

#define KEY_DOWN		false // active-low
#define DRIVE_LOW		false
#define DRIVE_HIGH	true

// see preprocessor symbols definitions for MATRIX_ROWS, MATRIX_COLS
// for this program, MATRIX_ROWS=8, MATRIX_COLS=16
#define NUMBER_OF_SENSE_PINS			MATRIX_ROWS
#define NUMBER_OF_DRIVING_PINS		MATRIX_COLS

static void configure_matrix_io(void);
static void configure_inputs(void);
static void configure_outputs(void);
static void configure_extint_channels(void);

volatile bool needs_matrix_scan = false;

extern uint32_t sense_pins[];
extern uint32_t sense_eic_pins[];
extern uint32_t sense_mux[];
extern uint32_t drive_pins[];
extern Pin_t drive_ports[];
extern Pin_t sense_ports[];

/************************************************************************/
/* Matrix implementation functions                                      */
/************************************************************************/
static matrix_row_t matrix[NUMBER_OF_SENSE_PINS];

void matrix_init(void) {
	needs_matrix_scan = false;

	// prep for column interrupt mode
	configure_matrix_io();
}

uint8_t matrix_scan() {
	uint8_t s, d;
	//printf("scan\r\n");
	out_set_high_all();
	for (d = 0; d < NUMBER_OF_DRIVING_PINS; d++) {
		// set test output pin
		out_set_low(drive_ports[d]);

		for (s = 0; s < NUMBER_OF_SENSE_PINS; s++) {
			bool pin_state = in_get_value(sense_ports[s]);
			if (pin_state == KEY_DOWN) {
				matrix[s] |= 1 << d;
				//printf("%d,%d\r\n", s, d);
				} else {
				matrix[s] &= ~(1 << d);
			}
		}
		// reset test output pin
		out_set_high(drive_ports[d]);
	}

	needs_matrix_scan = false;
	
	// reset all driving pins for interrupt
	out_set_low_all();
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
void configure_matrix_io() {
	configure_inputs();
	configure_outputs();
	
	out_set_high_all();

	configure_extint_channels();
	
	out_set_low_all();
}

void configure_inputs() {
	struct port_config pin_conf;
	port_get_config_defaults(&pin_conf);

	pin_conf.direction  = PORT_PIN_DIR_INPUT;
	pin_conf.input_pull = PORT_PIN_PULL_UP;

	uint8_t i;
	for (i = 0; i < NUMBER_OF_SENSE_PINS; i++) {
		port_pin_set_config(sense_pins[i], &pin_conf);
	}
}

void configure_outputs() {
	struct port_config pin_conf;
	port_get_config_defaults(&pin_conf);
	pin_conf.direction  = PORT_PIN_DIR_OUTPUT;

	uint8_t i;
	for (i = 0; i < NUMBER_OF_DRIVING_PINS; i++) {
		port_pin_set_output_level(drive_pins[i], DRIVE_LOW);
		port_pin_set_config(drive_pins[i], &pin_conf);
	}
}

void configure_extint_channels() {
	struct extint_chan_conf config_extint_chan;
	extint_chan_get_config_defaults(&config_extint_chan);

	config_extint_chan.gpio_pin_pull = EXTINT_PULL_UP;
	config_extint_chan.filter_input_signal = true;
	config_extint_chan.detection_criteria = EXTINT_DETECT_BOTH;

	uint8_t i;
	for (i = 0; i < NUMBER_OF_SENSE_PINS; i++) {
		config_extint_chan.gpio_pin = sense_eic_pins[i];
		config_extint_chan.gpio_pin_mux = sense_mux[i];
		extint_chan_set_config(i, &config_extint_chan);
	}
}

#endif
