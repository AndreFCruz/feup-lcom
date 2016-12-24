#include <minix/syslib.h>
#include "Input.h"

static Input_t * input = NULL;

//Private Constructor -- to ensure only one instance of this type is running
static Input_t * new_input() {

	Input_t* Input = (Input_t*) malloc(sizeof(Input_t));


	Input->keycode = NONE;

	Input->RMB = 0;
	Input->LMB = 0;
	Input->MMB = 0;

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

// Removes keycode from Input's buffer, compatible with double byte make/break codes
keycode_t input_get_key() {
	static byteE0 = 0;	// flag corresponding to whether first byte 0xE0 was received
	keycode_t tmp = input_instance()->keycode;
	input_instance()->keycode = NONE;	// Key handled

	if (0xE0 == tmp) {
		byteE0 = 1;
		return NONE; // NONE == 0x0
	} else if (byteE0) {
		byteE0 = 0; // clear flag
		return (0xE0 << 8) | tmp;
	} else {
		return tmp;
	}
}

//Destructor
void delete_input() {
	free(input_instance());
	input = NULL;
}

const int * get_mouse_pos() {
	return input_instance()->mouse_pos;
}

int get_mouseRMB() {
	if (input_instance()->RMB) {
		input_instance()->RMB = 0;
		return 1;
	} else {
		return 0;
	}
}

int get_mouseLMB() {
	if (input_instance()->LMB) {
		input_instance()->LMB = 0;
		return 1;
	} else {
		return 0;
	}
}

int get_mouseMMB() {
	if (input_instance()->MMB) {
		input_instance()->MMB = 0;
		return 1;
	} else {
		return 0;
	}
}

void mouse_packet_handler(unsigned char * packet) {
	Input_t * Input = input_instance();

	/** Update Buttons **/
	Input->RMB = packet[0] & BYTE0_RB;
	Input->LMB = packet[0] & BYTE0_LB;
	Input->MMB = packet[0] & BYTE0_MB;


	/** Update Position **/
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
		Input->mouse_pos[0] = Input->res[0] - 1;
		validX = 0;
	}

	//Checking the Y borders
	if (Input->mouse_pos[1] - y_value < 0) {
		Input->mouse_pos[1] = 0;
		validY = 0;
	}
	else if (Input->mouse_pos[1] - y_value >= Input->res[1]) {
		Input->mouse_pos[1] = Input->res[1] - 1;
		validY = 0;
	}


	//Only if not at the borders, should be updated
	if ( validX )
		Input->mouse_pos[0] += x_value;
	if ( validY )
		Input->mouse_pos[1] -= y_value;
}

int mouse_inside_rect(int x_initial, int y_initial, int x_final, int y_final) {
	Input_t * Input = input_instance();

	if (Input->mouse_pos[0] >= x_initial && Input->mouse_pos[0] <= x_final &&
			Input->mouse_pos[1] >= y_initial && Input->mouse_pos[1] <= y_final)
		return 1;
	else
		return 0;
}

int mouse_inside_circle (int x_center, int y_center, int radius) {
	Input_t * Input = input_instance();

	int x_var = Input->mouse_pos[0] - x_center;
	int y_var = Input->mouse_pos[1] - y_center;

	if (x_var * x_var + y_var * y_var <= radius * radius) // x² + y² <= r²
		return 1;
	else
		return 0;
}
