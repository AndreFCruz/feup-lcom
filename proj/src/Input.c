#include <minix/syslib.h>
#include "Input.h"

static Input_t * input = NULL;

//Private Constructor -- to ensure only one instance of this type is running
static Input_t * new_input() {

	Input_t* Input = (Input_t*) malloc(sizeof(Input_t));


	Input->keycode = NONE;

	Input->RMB = UP;
	Input->LMB = UP;
	Input->MMB = UP;

	Input->res[0] = vg_getHorRes();
	Input->res[1] = vg_getVerRes();

	Input->mouse_pos[0] = Input->res[0] / 2;
	Input->mouse_pos[1] = Input->res[1] / 2;

	return Input;
}

Input_t * input_instance() {
	if ( NULL == input ) {
		input = new_input();
	}
	else
		return input;
}

// Keyboard Handler : to be called on keyboard interrupts
void keyboard_handler() {
	if ( (input_instance()->keycode = keyboard_read()) < 0 )
		printf("keyboard_handler::keyboard_read() failed\n");
}

keycode_t input_get_key() {
	keycode_t tmp = input_instance()->keycode;
	input_instance()->keycode = NONE;	// Key handled
	return tmp;
}

//void keyboard_update_input() {
//
//	switch(keyboard_read()) {
//	/* Esc Key */
//	case ESC_MAKE_CODE:
//		Input->ESC = KEY_MAKE;
//		break;
//	case ESC_BREAK_CODE:
//		Input->ESC = KEY_BREAK;
//		break;
//
//	/* Enter Key */
//	case ENTER_MAKE_CODE:
//		Input->ENTER = KEY_MAKE;
//		break;
//	case ENTER_BREAK_CODE:
//		Input->ENTER = KEY_BREAK;
//		break;
//
//	/* Up Arrow Key */
//	case UP_MAKE_CODE:
//		Input->UP = KEY_MAKE;
//		break;
//	case UP_BREAK_CODE:
//		Input->UP = KEY_BREAK;
//		break;
//
//	/* Down Arrow Key */
//	case DOWN_MAKE_CODE:
//		Input->DOWN = KEY_MAKE;
//		break;
//	case DOWN_BREAK_CODE:
//		Input->DOWN = KEY_BREAK;
//		break;
//
//	/* Right Arrow Key */
//	case RIGHT_MAKE_CODE:
//		Input->RIGHT = KEY_MAKE;
//		break;
//	case RIGHT_BREAK_CODE:
//		Input->RIGHT = KEY_BREAK;
//		break;
//
//	/* Left Arrow Key */
//	case LEFT_MAKE_CODE:
//		Input->LEFT = KEY_MAKE;
//		break;
//	case LEFT_BREAK_CODE:
//		Input->LEFT = KEY_BREAK;
//		break;
//
//	default:
//		break;
//	}
//}

//Destructor
void delete_input() {
	free(input_instance());
	input = NULL;
}

int get_mouse_x() {
	return input_instance()->mouse_pos[0];
}

int get_mouse_y() {
	return input_instance()->mouse_pos[1];
}

//Not a int cause no message of error can happen
void update_mouse_position(unsigned char * packet) {
	Input_t * Input = input_instance();

	int x_value = int_value(packet[1], packet[0] & BYTE0_X_SIGN);
	int y_value = int_value(packet[2], packet[0] & BYTE0_Y_SIGN);

	// Handle Overflow
	if ( (packet[0] & BYTE0_X_OVF) != 0 )
		x_value += (packet[0] & BYTE0_X_SIGN ? -255 : 255);
	if ( (packet[0] & BYTE0_Y_OVF) != 0 )
		y_value += (packet[0] & BYTE0_Y_SIGN ? -255 : 255);

	// Flags to check whether the Border was reached
	int validX = 1;
	int validY = 1;

	//Checking the X borders
	if ((Input->mouse_pos[0] + x_value) < 0) {
		Input->mouse_pos[0] = 0;
		validX = 0;
	}
	else if (Input->mouse_pos[0] + x_value >= Input->res[0]) {
		Input->mouse_pos[0] = Input->res[0];
		validX = 0;
	}

	//Checking the Y borders
	if (Input->mouse_pos[1] + y_value < 0) {
		Input->mouse_pos[1] = 0;
		validY = 0;
	}
	else if (Input->mouse_pos[1] + y_value >= Input->res[1]) {
		Input->mouse_pos[1] = Input->res[1];
		validY = 0;
	}

	//Only if not at the borders, should be updated
	if ( validX )
		Input->mouse_pos[0] += x_value;
	if ( validY )
		Input->mouse_pos[1] += y_value;
}

int mouse_inside_rect(int x_initial, int y_initial, int x_final, int y_final) {
	Input_t * Input = input_instance();
	if (Input->mouse_pos[0] >= x_initial && Input->mouse_pos[0] <= x_final &&
			Input->mouse_pos[1] >= y_initial && Input->mouse_pos[1] <= y_final)
		return 1;
	else
		return 0;
}
