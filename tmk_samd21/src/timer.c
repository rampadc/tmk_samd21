/*
* timer.c
*
* Created: 30/12/2017 3:14:23 PM
*  Author: cong
* Boilerplate to support TMK Keyboard in SAM D21
* 
*/

#include <asf.h>
#include "timer.h"

extern void hook_timer_1ms(void);

volatile uint32_t timer_count;

/************************************************************************/
/* SAM D21 TC configuration to fire an event every 1ms                  */
/************************************************************************/
#define CONF_TC_MODULE		TC3

void configure_tc(void);
void configure_tc_callbacks(void);
void tc_callback_to_incr_ms(struct tc_module *const module_inst);

struct tc_module tc_instance;

void tc_callback_to_incr_ms(struct tc_module *const module_inst) {
	timer_count += 1;
	tc_set_count_value(module_inst, 0);

	hook_timer_1ms();
}

void configure_tc(void) {
	struct tc_config config_tc;
	tc_get_config_defaults(&config_tc);

	config_tc.counter_size = TC_COUNTER_SIZE_16BIT;
	config_tc.clock_source = GCLK_GENERATOR_1;
	config_tc.clock_prescaler = TC_CLOCK_PRESCALER_DIV1;
	config_tc.counter_16_bit.compare_capture_channel[0] = 31;


	tc_init(&tc_instance, CONF_TC_MODULE, &config_tc);

	tc_enable(&tc_instance);
}

void configure_tc_callbacks(void) {
	tc_register_callback(&tc_instance, tc_callback_to_incr_ms,
	TC_CALLBACK_CC_CHANNEL0);
	tc_enable_callback(&tc_instance, TC_CALLBACK_CC_CHANNEL0);
}

/************************************************************************/
/* Timer implementation                                                 */
/************************************************************************/
void timer_init() {
	timer_count = 0;

	configure_tc();
	configure_tc_callbacks();
}

void timer_clear() {
	timer_count = 0;
}

uint16_t timer_read(void) {
	return (uint16_t) timer_count;
}

uint32_t timer_read32(void) {
	return timer_count;
}

uint16_t timer_elapsed(uint16_t last) {
	return TIMER_DIFF_16((uint16_t)timer_count, last);
}

uint32_t timer_elapsed32(uint32_t last) {
	return TIMER_DIFF_32(timer_count, last);
}