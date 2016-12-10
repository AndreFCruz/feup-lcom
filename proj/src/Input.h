#ifndef __INPUT_H
#define __INPUT_H

#include "i8042.h"
#include "keyboard.h"
#include "mouse.h"


typedef enum {UP, DOWN /* pressed/released? */} button_state;

// TODO Update with actual make/break codes!!
typedef enum {
	ESC_MAKE = 0x1,
	ESC_BREAK = ESC_BREAK_CODE,
	ENTER_MAKE = 0x3,
	UP_MAKE = 0x4,
	DOWN_MAKE = 0x5,
	RIGHT_MAKE = 0x6,
	LEFT_MAKE = 0x7,
	NO_KEY = 0x0
} keycode_t;

typedef struct {
	/* Keyboard Keys */
	keycode_t keycode;

	/* Mouse Buttons */
	button_state RMB;
	button_state LMB;
	button_state MMB;

	/* Mouse Position */
	int mouse_pos[2];

	/* Resolution */
	unsigned res[2];
} Input_t;

//TODO: Add Documentation

Input_t * input_instance();	// For singleton-like behaviour

void delete_input();		// Destructor

void keyboard_handler();
keycode_t input_get_key();

int get_mouse_x();
int get_mouse_y();

void keyboard_update_input();
void update_mouse_position(unsigned char * packet);
int mouse_inside_area (int x_initial, int y_initial, int x_final, int y_final);	//Return 0 if true, non-zero otherwise

#endif
