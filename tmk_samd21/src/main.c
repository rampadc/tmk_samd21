#include <asf.h>
#include <stdio.h>
#include <inttypes.h>

#include "keyboard.h"
#include "backlight.h"

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

	timer_init();
	delay_init();
	
	#ifndef NO_PRINT
	stdio_usb_init();
	#else
	udc_start();
	#endif

	//backlight_init();
	keyboard_init();
	host_set_driver(&samd21_driver);

	/* Sleep in main, USB callbacks are handled in usb_callbacks.c */
	while (1) {
		needs_matrix_scan = eic_interrupt_detected();
		
		if (needs_matrix_scan) {
			if (usb_suspended && usb_remote_wakeup_enabled) {
				udc_remotewakeup();
			}
			keyboard_task();
		}
		
		sleepmgr_enter_sleep();
	}
}

void hook_timer_1ms() {

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