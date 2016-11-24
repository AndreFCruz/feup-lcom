#include "video_gr.h"
#include "timer.h"
#include "defs.h"
#include "keyboard.h"

void *test_init(unsigned short mode, unsigned short delay) {
	if (mode > 0x10C || mode < 0x100) {
		printf("test_init-> invalid mode, was %X.\n", mode);
		return NULL;
	}
	void * tmp = vg_init(mode);
	timer_delay(delay);
	vg_exit();

	return tmp;
}


int test_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color) {
	
	vg_init(MODE_5);

	// Draw Square
	unsigned i, j;
	for (i = x; i < size + x; i++) {
		for (j = y; j < size + y; j++) {
			paintPixel(x, y, color);
		}
	}

	wait_esc_release();

	vg_exit();
}
	

int test_line(unsigned short xi, unsigned short yi, 
		           unsigned short xf, unsigned short yf, unsigned long color) {
	
	/* To be completed */
	
}

int test_xpm(unsigned short xi, unsigned short yi, char *xpm[]) {
	
	int width, height;

	// Iterar pelo xpm, usar read_xpm, paintPixel com respetiva cor
	
}	

int test_move(unsigned short xi, unsigned short yi, char *xpm[], 
				unsigned short hor, short delta, unsigned short time) {
	
	// Desenhar xpm, esperar 1/60 segundos, desenhar novamente na posição updated
	
}					

int test_controller() {
	
	// Funcao GET_CONTROLLER_INFO
	
}					
	
