/*
 * keymap.c
 *
 * Created: 2/01/2018 4:15:02 PM
 *  Author: cong
 */ 

 #include <asf.h>
 #include "keycode.h"
 #include "action.h"

#ifdef KBD_ONEKEY
 const action_t actionmaps[][MATRIX_ROWS][MATRIX_COLS] = {
	{{KC_A}}
 };
#endif