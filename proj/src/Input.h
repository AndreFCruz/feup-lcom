#ifndef __INPUT_H
#define __INPUT_H


typedef enum {UP, DOWN /* pressed/released? */} button_state;
typedef enum {	// TODO Update with actual keycodes!!
	ESC_MAKE = 0x0,
	ESC_BREAK = 0x0,
	ENTER_MAKE = 0x0,
	UP_MAKE = 0x0,
	DOWN_MAKE = 0x0,
	RIGHT_MAKE = 0x0,
	LEFT_MAKE = 0x0,
	NONE = 0x0
} key_t;

typedef struct {
	/* Keyboard Keys */
	key_t keycode;

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
key_t input_get_key();

int get_mouse_x();
int get_mouse_y();

void keyboard_update_input()
void update_mouse_position(unsigned char * packet);
int mouse_inside_area (int x_initial, int y_initial, int x_final, int y_final);	//Return 0 if true, non-zero otherwise

#endif
