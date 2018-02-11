#include <asf.h>
#include <stdio.h>
#include <inttypes.h>

#include "keyboard.h"
#include "host.h"
#include "timer.h"

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

	delay_init();

	#ifndef NO_PRINT
	stdio_usb_init();
	#else
	udc_start();
	#endif

	keyboard_init();
	host_set_driver(&samd21_driver);

	/* Sleep in main, USB callbacks are handled in usb_callbacks.c */
	while (1) {
		if (usb_suspended && usb_remote_wakeup_enabled) {
			udc_remotewakeup();
		}
		keyboard_task();
		//
		//if (needs_matrix_scan) {
			//
		//}
		
		sleepmgr_enter_sleep();
	}
}

void hook_timer_1ms() {
	// toggle every ~100ms
	//if (timer_read32() % 100 == 0) {
		//port_pin_toggle_output_level(LED_0_PIN);
	//}
}
