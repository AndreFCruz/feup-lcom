#include "Input.h"

//Constructor
input_t * new_input(unsigned h_res, unsigned v_res) {

	input_t* n_input = (input_t*) malloc(sizeof(input_t));

	n_input->ESC = KEY_NONE;
	n_input->ENTER = KEY_NONE;
	n_input->RMB = UP;
	n_input->LMB = UP;
	n_input->MMB = UP;

	n_input->mouse_pos[0] = 512;
	n_input->mouse_pos[1] = 384;

	n_input->res[0] = h_res;
	n_input->res[1] = v_res;
}

//Destructor
void delete_input(input_t * input) {
	free(input);
}

int get_mouse_x(input_t * input) {
	return input->mouse_pos[0];
}

int get_mouse_y(input_t * input) {
	return input->mouse_pos[1];
}

//Not a int cause no message of error can happen
void update_mouse_position(input_t * input, int  x_var, int y_var) {

	if (input->mouse_pos[0] + x_var < 0)
		input->mouse_pos[0] = 0;
	else if (input->mouse_pos[0] + x_var > input->res[0])
		input->mouse_pos[0] = input->res[0];

	if (input->mouse_pos[1] + y_var < 0)
		input->mouse_pos[1] = 0;
	else if (input->mouse_pos[1] + x_var > input->res[1])
		input->mouse_pos[1] = input->res[1];

	//If any of the positions was altered, I should not mess with them now
	if (input->mouse_pos[0] != 0 && input->mouse_pos[0] != input->res[0])
		input->mouse_pos[0] += x_var;
	if (input->mouse_pos[1] != 0 && input->mouse_pos[1] != input->res[1])
		input->mouse_pos[1] += y_var;
}
