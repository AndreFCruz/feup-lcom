#ifndef __INPUT_H
#define __INPUT_H


typedef enum {UP, DOWN /* pressed/released? */} button_state;
typedef enum {KEY_MAKE, KEY_BREAK, KEY_NONE /* pressed/released? */} key_state;

typedef struct {
	/* Keyboard Keys */
	key_state ESC;
	key_state ENTER;
	key_state UP;
	key_state DOWN;
	key_state RIGHT;
	key_state LEFT;

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

Input_t * new_input(unsigned h_res, unsigned v_res);	// Constructor
void delete_input(Input_t * Input);	// Destructor

int get_mouse_x(Input_t * Input);
int get_mouse_y(Input_t * Input);

void update_mouse_position(Input_t * Input, int  x_var, int y_var);
int mouse_inside_area (Input_t * Input, int x_initial, int y_initial, int x_final, int y_final);	//Return 0 if true, non-zero otherwise

#endif
