/*
Copyright 2013 Mathias Andersson <wraul@dbox.se>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
/*
* backlight.c
*
* Modified: 12/02/2018 for SAM D21
*/

#include "backlight.h"
#include "io_pins.h"
#include <asf.h>
#include <stdio.h>

struct tc_module tc_instance;

backlight_config_t backlight_config;
uint8_t backlight_count = 0;

extern Pin_t backlight_pin;

/************************************************************************/
/* Modified for SAM D21                                                 */
/************************************************************************/
void backlight_init(void) {
	#ifdef BACKLIGHT_ENABLE

	struct port_config pin_conf;
	port_get_config_defaults(&pin_conf);
	pin_conf.direction  = PORT_PIN_DIR_OUTPUT;

	port_pin_set_output_level(PIN_PB12, false);
	port_pin_set_config(PIN_PB12, &pin_conf);
	
	backlight_config.enable = false;
	
	backlight_set(backlight_config.enable ? backlight_config.level : 0);
	#endif
}

void backlight_set(uint8_t level) {
	#ifdef BACKLIGHT_ENABLE
	#endif
}

void backlight_timer_logic(void) {
	if (backlight_config.enable) {
		if (backlight_count < backlight_config.level) {
			out_set_high(backlight_pin);
			} else {
			out_set_low(backlight_pin);
		}
		backlight_count++;
		
		if (backlight_count == BACKLIGHT_LEVELS) {
			backlight_count = 0;
		}
	} else {
		out_set_low(backlight_pin);
	}
}
/************************************************************************/
/* Modified to remove EEPROM writes                                     */
/************************************************************************/
void backlight_increase(void)
{
	#ifdef BACKLIGHT_ENABLE
	if(backlight_config.level < BACKLIGHT_LEVELS)
	{
		backlight_config.level++;
		backlight_config.enable = 1;
	}
	backlight_set(backlight_config.level);
	#endif
}

void backlight_decrease(void)
{
	#ifdef BACKLIGHT_ENABLE
	if(backlight_config.level > 0)
	{
		backlight_config.level--;
		backlight_config.enable = !!backlight_config.level;
	}
	backlight_set(backlight_config.level);
	#endif
}

void backlight_toggle(void)
{
	#ifdef BACKLIGHT_ENABLE
	backlight_config.enable ^= 1;
	backlight_set(backlight_config.enable ? backlight_config.level : 0);
	#endif
}

void backlight_step(void)
{
	#ifdef BACKLIGHT_ENABLE
	backlight_config.level++;
	if(backlight_config.level > BACKLIGHT_LEVELS)
	{
		backlight_config.level = 0;
	}
	backlight_config.enable = !!backlight_config.level;
	
	backlight_set(backlight_config.level);
	#endif
}

void backlight_level(uint8_t level)
{
	#ifdef BACKLIGHT_ENABLE
	backlight_config.level ^= level;
	backlight_config.enable = !!backlight_config.level;
	backlight_set(backlight_config.level);
	#endif
}