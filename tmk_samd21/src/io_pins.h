/*
 * matrix_pins.h
 *
 * Created: 10/02/2018 2:26:11 PM
 *  Author: Cong
 */ 


#ifndef IO_PINS_H_
#define IO_PINS_H_

typedef struct {
	volatile uint32_t *portw;
	uint32_t port_mask;
} Pin_t;

void out_set_low(Pin_t pin);
void out_set_high(Pin_t pin);
bool in_get_value(Pin_t pin);

void out_set_low_all();
void out_set_high_all();

#endif /* MATRIX_PINS_H_ */