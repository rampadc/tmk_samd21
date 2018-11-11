#include <asf.h>
#include <stdio.h>
#include <inttypes.h>

#include "keyboard.h"
#include "backlight.h"
#include "io_pins.h"

#include "host.h"
#include "timer.h"

bool eic_interrupt_detected();
void hook_timer_1ms(void);

extern bool needs_matrix_scan;
extern host_driver_t samd21_driver;

extern bool usb_suspended;
extern bool usb_remote_wakeup_enabled;

int main (void)
{
	system_init();
	irq_initialize_vectors();
	cpu_irq_enable();
	sleepmgr_init();

	backlight_init();
	timer_init();
	delay_init();
	
#ifdef TRACKPOINT

#endif

#ifdef KEYBOARD_T430
	keyboard_init();
	host_set_driver(&samd21_driver);
#endif
	
	#ifndef NO_PRINT
	stdio_usb_init();
	#else
	udc_start();
	#endif

	/* Sleep in main, USB callbacks are handled in usb_callbacks.c */
	while (1) {
#ifdef KEYBOARD_T430
		needs_matrix_scan = eic_interrupt_detected();
		
		if (needs_matrix_scan) {
			if (usb_suspended && usb_remote_wakeup_enabled) {
				udc_remotewakeup();
			}
			keyboard_task();
		}
#endif
		sleepmgr_enter_sleep();
	}
}

void hook_timer_1ms() {
#ifdef BACKLIGHT
	backlight_timer_logic();
#endif
}

bool eic_interrupt_detected() {
	uint8_t i;
	for (i = 0; i < 15; i++) {
		if (extint_chan_is_detected(i)) {
			extint_chan_clear_detected(i);
			return true;
		}
	}
	return false;
}