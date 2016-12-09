#include "Input.h"

//Constructor
Input_t * new_input(unsigned h_res, unsigned v_res) {

	Input_t* Input = (Input_t*) malloc(sizeof(Input_t));

	Input->ESC = KEY_NONE;
	Input->ENTER = KEY_NONE;
	Input->UP = KEY_NONE;
	Input->DOWN = KEY_NONE;
	Input->RIGHT = KEY_NONE;
	Input->LEFT = KEY_NONE;

	Input->RMB = UP;
	Input->LMB = UP;
	Input->MMB = UP;

	Input->mouse_pos[0] = h_res/2;
	Input->mouse_pos[1] = v_res/2;

	Input->res[0] = h_res;
	Input->res[1] = v_res;

	return Input;
}

// Keyboard Handler : to be called on keyboard interrupts
void keyboard_update_input(Input_t * Input) {

	switch(keyboard_read()) {
	/* Esc Key */
	case ESC_MAKE_CODE:
		Input->ESC = KEY_MAKE;
		break;
	case ESC_BREAK_CODE:
		Input->ESC = KEY_BREAK;
		break;

	/* Enter Key */
	case ENTER_MAKE_CODE:
		Input->ENTER = KEY_MAKE;
		break;
	case ENTER_BREAK_CODE:
		Input->ENTER = KEY_BREAK;
		break;

	/* Up Arrow Key */
	case UP_MAKE_CODE:
		Input->UP = KEY_MAKE;
		break;
	case UP_BREAK_CODE:
		Input->UP = KEY_BREAK;
		break;

	/* Down Arrow Key */
	case DOWN_MAKE_CODE:
		Input->DOWN = KEY_MAKE;
		break;
	case DOWN_BREAK_CODE:
		Input->DOWN = KEY_BREAK;
		break;

	/* Right Arrow Key */
	case RIGHT_MAKE_CODE:
		Input->RIGHT = KEY_MAKE;
		break;
	case RIGHT_BREAK_CODE:
		Input->RIGHT = KEY_BREAK;
		break;

	/* Left Arrow Key */
	case LEFT_MAKE_CODE:
		Input->LEFT = KEY_MAKE;
		break;
	case LEFT_BREAK_CODE:
		Input->LEFT = KEY_BREAK;
		break;

	default:
		break;
	}
}

//Destructor
void delete_input(Input_t * Input) {
	free(Input);
}

int get_mouse_x(Input_t * Input) {
	return Input->mouse_pos[0];
}

int get_mouse_y(Input_t * Input) {
	return Input->mouse_pos[1];
}

//Not a int cause no message of error can happen
void update_mouse_position(Input_t * Input, int  x_var, int y_var) {

	//Flags to know if i can increment the variations on x or y.
	char alterationX = 0;
	char alterationY = 0;

	//Checking the X borders
	if ((Input->mouse_pos[0] + x_var) < 0) {
		Input->mouse_pos[0] = 0;
		alterationX = 1;
	}
	else if (Input->mouse_pos[0] + x_var >= Input->res[0]) {
		Input->mouse_pos[0] = Input->res[0];
		alterationX = 1;
	}

	//Checking the Y borders
	if (Input->mouse_pos[1] + y_var < 0) {
		Input->mouse_pos[1] = 0;
		alterationY = 1;
	}
	else if (Input->mouse_pos[1] + y_var >= Input->res[1]) {
		Input->mouse_pos[1] = Input->res[1];
		alterationY = 1;
	}

	//Only if not at the borders, should be updated
	if (!alterationX)
		Input->mouse_pos[0] += x_var;
	if (!alterationY)
		Input->mouse_pos[1] += y_var;
}

int mouse_inside_area (Input_t * Input, int x_initial, int y_initial, int x_final, int y_final) {
	if (Input->mouse_pos[0] >= x_initial && Input->mouse_pos[0] <= x_final) {
		if (Input->mouse_pos[1] >= y_initial && Input->mouse_pos[1] <= y_final)
			return 0;
		else return 1;
	}
	else return 1;
}
