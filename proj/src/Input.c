#include "Input.h"

//Constructor
input_t * new_input(unsigned h_res, unsigned v_res) {

	input_t* n_input = (input_t*) malloc(sizeof(input_t));

	n_input->ESC = KEY_NONE;
	n_input->ENTER = KEY_NONE;
	n_input->RMB = UP;
	n_input->LMB = UP;
	n_input->MMB = UP;

	n_input->mouse_pos[0] = h_res/2;
	n_input->mouse_pos[1] = v_res/2;

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

	//Flags to know if i can increment the variations on x or y.
	char alterationX = 0;
	char alterationY = 0;

	//Checking the X borders
	if ((input->mouse_pos[0] + x_var) < 0) {
		input->mouse_pos[0] = 0;
		alterationX = 1;
	}
	else if (input->mouse_pos[0] + x_var >= input->res[0]) {
		input->mouse_pos[0] = input->res[0];
		alterationX = 1;
	}

	//Checking the Y borders
	if (input->mouse_pos[1] + y_var < 0) {
		input->mouse_pos[1] = 0;
		alterationY = 1;
	}
	else if (input->mouse_pos[1] + y_var >= input->res[1]) {
		input->mouse_pos[1] = input->res[1];
		alterationY = 1;
	}

	//Only if not at the borders, should be updated
	if (!alterationX)
		input->mouse_pos[0] += x_var;
	if (!alterationY)
		input->mouse_pos[1] += y_var;
}

int mouse_inside_area (input_t * input, int x_initial, int y_initial, int x_final, int y_final) {
	if (input->mouse_pos[0] >= x_initial && input->mouse_pos[0] <= x_final) {
		if (input->mouse_pos[1] >= y_initial && input->mouse_pos[1] <= y_final)
			return 0;
		else return 1;
	}
	else return 1;
}
