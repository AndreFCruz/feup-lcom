#include "video_gr.h"
#include "timer.h"


void *test_init(unsigned short mode, unsigned short delay) {
	vg_init(mode);
	timer_delay(delay);
	vg_exit();
}


int test_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color) {
	
	vg_init(MODE_5);

	while (kbd_esc_pressed() != OK)
	{

	}

	vg_exit();
}
	
}

int test_line(unsigned short xi, unsigned short yi, 
		           unsigned short xf, unsigned short yf, unsigned long color) {
	
	/* To be completed */
	
}

int test_xpm(unsigned short xi, unsigned short yi, char *xpm[]) {
	
	/* To be completed */
	
}	

int test_move(unsigned short xi, unsigned short yi, char *xpm[], 
				unsigned short hor, short delta, unsigned short time) {
	
	/* To be completed */
	
}					

int test_controller() {
	
	/* To be completed */
	
}					
	
