#ifndef __INPUT_H
#define __INPUT_H

typedef enum {MAKE, BREAK, NONE /* pressed/released? */} key_state;

typedef enum {UP, DOWN /* pressed/released? */} button_state;

typedef struct {
	/* Keyboard Keys */
	key_state ESC;
	key_state ENTER;

	/* Mouse Buttons */
	button_state RMB;
	button_state LMB;
	button_state MMB;

	/* Mouse Position */
	unsigned mouse_pos[2];
} input_t;

input_t * new_input();	// Constructor
void delete_input(input_t * input);	// Destructor

#endif
