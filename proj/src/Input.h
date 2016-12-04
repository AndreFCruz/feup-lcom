#ifndef __INPUT_H
#define __INPUT_H


typedef enum {UP, DOWN /* pressed/released? */} button_state;
typedef enum {KEY_MAKE, KEY_BREAK, KEY_NONE /* pressed/released? */} key_state;

typedef struct {
	/* Keyboard Keys */
	key_state ESC;
	key_state ENTER;

	/* Mouse Buttons */
	button_state RMB;
	button_state LMB;
	button_state MMB;

	/* Mouse Position */
	int mouse_pos[2];

	/* Resolution */
	unsigned res[2];
} input_t;

//TODO: Add Documentation

input_t *new_input(unsigned h_res, unsigned v_res);	// Constructor
void delete_input(input_t * input);	// Destructor

int get_mouse_x(input_t * input);
int get_mouse_y(input_t * input);

void update_mouse_position(input_t * input, int  x_var, int y_var);
int mouse_inside_area (input_t * input, int x_initial, int y_initial, int x_final, int y_final);	//Return 0 if true, non-zero otherwise

#endif
