#include "mouse.h"
#include "pixmap.h"
//#include "video_gr.c"

mouse_t * new_mouse() {

	mouse_t* n_mouse = (mouse_t*) malloc(sizeof(mouse_t));

	//Initialize the mouse at the middle of the screen
	n_mouse->x_position = 512;
	n_mouse->y_position = 384;

	n_mouse->rmb_pressed = 0;
	n_mouse->lmb_pressed = 0;

}

int draw_mouse(char * ptr, mouse_t * mouse) {
	if (draw_xpm(ptr, mouse->x_position, mouse->y_position, cross) != 0) {
		printf("draw_mouse: Failed on function call draw_xpm(), at position (%d, %d).\n", mouse->x_position, mouse->y_position);
		return 1;
	}
}
