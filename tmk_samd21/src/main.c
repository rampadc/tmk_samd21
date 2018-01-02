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

	struct port_config pin_conf;
	port_get_config_defaults(&pin_conf);

	/* Configure LEDs as outputs, turn them off */
	pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(PIN_PB02, &pin_conf);
	port_pin_set_output_level(PIN_PB02, LED_0_INACTIVE);

	delay_init();
	
	keyboard_init();
	host_set_driver(&samd21_driver);

	#ifndef NO_PRINT
	stdio_usb_init();
	#else
	udc_start();
	#endif

	/* Sleep in main, USB callbacks are handled in usb_callbacks.c */
	while (1) {
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
	// toggle every ~100ms
	if (timer_read32() % 100 == 0) {
		port_pin_toggle_output_level(LED_0_PIN);
	}
}
