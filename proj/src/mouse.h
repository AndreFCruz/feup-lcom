#ifndef __MOUSE_H
#define __MOUSE_H

typedef struct {

	//Mouse Position
	short x_position;
	short y_position;

	//Mouse Buttons - 1 if pressed, 0 if not.
	char rmb_pressed;
	char lmb_pressed;
	//TODO: No need to check the other buttons? Ou mais do que um para disparar

} mouse_t;

mouse_t * new_mouse();	// Constructor
void delete_mouse(mouse_t * mouse);	// Destructor
int draw_mouse(char * ptr, mouse_t * mouse);

#endif
