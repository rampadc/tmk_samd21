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
#include <asf.h>
#include <stdio.h>

#define PWM_MODULE      TC4
#define PWM_OUT_PIN     PIN_PB12E_TC4_WO0
#define PWM_OUT_MUX     MUX_PB12E_TC4_WO0

struct tc_module tc_instance;

backlight_config_t backlight_config;

/************************************************************************/
/* Modified for SAM D21                                                 */
/************************************************************************/
void backlight_init(void) {
	#ifdef BACKLIGHT_ENABLE
	// setup PB12 for backlight PWM
	struct tc_config config_tc;
	tc_get_config_defaults(&config_tc);
	
	config_tc.counter_size    = TC_COUNTER_SIZE_8BIT;
	config_tc.clock_source = GCLK_GENERATOR_0;
	config_tc.clock_prescaler = TC_CLOCK_PRESCALER_DIV8;
	config_tc.wave_generation = TC_WAVE_GENERATION_NORMAL_PWM;
	config_tc.counter_8_bit.period = 100;
	// CC0 is the duty cycle as a whole number from 0 to 100
	
	config_tc.pwm_channel[0].enabled = true;
	config_tc.pwm_channel[0].pin_out = PWM_OUT_PIN;
	config_tc.pwm_channel[0].pin_mux = PWM_OUT_MUX;
	
	tc_init(&tc_instance, PWM_MODULE, &config_tc);
	
	backlight_config.enable = false;
	
	backlight_set(backlight_config.enable ? backlight_config.level : 0);
	#endif
}

void backlight_set(uint8_t level) {
	if (backlight_config.enable) {
		uint8_t cc0 = backlight_config.level * 10;
		tc_set_compare_value(&tc_instance, TC_COMPARE_CAPTURE_CHANNEL_0, cc0);
		tc_enable(&tc_instance);
	} else {
		tc_disable(&tc_instance);
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