/*
Copyright 2013 Jun Wako <wakojun@gmail.com>

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

Modified 02/01/18: functions are modified to support ASF UDC HID functions
*/
#include "host.h"
#include "report.h"
#include "debug.h"
#include "action_util.h"
#include "timer.h"

#include <stdint.h>

static uint8_t real_mods = 0;
static uint8_t weak_mods = 0;

report_keyboard_t *keyboard_report = &(report_keyboard_t){};

void send_keyboard_report(void) {
	keyboard_report->mods  = real_mods;
	keyboard_report->mods |= weak_mods;

	host_keyboard_send(keyboard_report);
}

/* key */
void add_key(uint8_t key)
{
	host_keyboard_down(key);
}

void del_key(uint8_t key)
{
	host_keyboard_up(key);
}

void clear_keys(void)
{
	// not clear mods
	// todo: implement this by exposing udi_hid_kbd_report
	for (int8_t i = 1; i < KEYBOARD_REPORT_SIZE; i++) {
		keyboard_report->raw[i] = 0;
	}
}


/* modifier */
uint8_t get_mods(void) { return real_mods; }
void add_mods(uint8_t mods) {
	host_keyboard_modifier_down(mods);
	real_mods |= mods;
}
void del_mods(uint8_t mods) {
	host_keyboard_modifier_up(mods);
	real_mods &= ~mods;
}
void set_mods(uint8_t mods) {
	// todo: implement this by exposing udi_hid_kbd_report
	real_mods = mods;
}
void clear_mods(void) {
	// todo: implement this by exposing udi_hid_kbd_report
	real_mods = 0;
}

/* weak modifier */
uint8_t get_weak_mods(void) {
	return weak_mods;
}
void add_weak_mods(uint8_t mods) {
	host_keyboard_modifier_down(mods);
	weak_mods |= mods;
}
void del_weak_mods(uint8_t mods) {
	host_keyboard_modifier_up(mods);
	weak_mods &= ~mods;
}
void set_weak_mods(uint8_t mods) {
	// todo: implement this by exposing udi_hid_kbd_report
	weak_mods = mods;
}
void clear_weak_mods(void) {
	// todo: implement this by exposing udi_hid_kbd_report
	weak_mods = 0;
}
