#include "video_gr.h"
#include "timer.h"
#include "defs.h"
#include "keyboard.h"
#include "math.h"
//#include "vbe.h"

void *test_init(unsigned short mode, unsigned short delay) {
	if (mode > 0x10C || mode < 0x100) {		//TODO: Retirar magic numbers?
		printf("test_init-> invalid mode, was %X.\n", mode);
		return NULL;
	}

	void * tmp = vg_init(mode);
	timer_delay(delay);						//TODO: Mudar para o Timer 0
	vg_exit();

	return tmp;
}


int test_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color) {
	
	char * ptr = vg_init(MODE_5);

	// Draw Square
	unsigned i, j;
	for (i = x; i < size + x; i++) {
		for (j = y; j < size + y; j++) {
//			*(ptr + i + j*1024) = color;
			paint_pixel(i, j, color, ptr);
		}
	}

	// TODO Exit Contition -> ESC BreakCode
	wait_esc_release();

	vg_exit();
}
	
//Based on DDA Algorithm
int test_line(unsigned short xi, unsigned short yi, 
		           unsigned short xf, unsigned short yf, unsigned long color) {

	//Check limis of Res?

	char * ptr = vg_init(MODE_5);
	
	// x and y variation
	int x_variation = xf-xi;
	int y_variation = yf-yi;
	int n;	//number of steps

	//Getting the number of steps value
	if (x_variation < 0 && y_variation < 0 ) {
		if (-x_variation > -y_variation)
			n = -x_variation;
		else
			n = -y_variation;
	}
	else if (x_variation < 0) {
		if (-x_variation > y_variation)
			n = -x_variation;
		else
			n = y_variation;
	}
	else if (y_variation < 0) {
		if (x_variation > -y_variation)
			n = x_variation;
		else
			n = -y_variation;
	}
	else {
		if (x_variation > y_variation)
			n = x_variation;
		else
			n = y_variation;
	}

	unsigned i;
	for (i = 0; i <= n; ++i) {
		paint_pixel(xi, yi, color, ptr);
		xi += (x_variation / (float) n);	//Since x it is a int, it will round himself
		yi += (y_variation / (float) n);
		printf("xi: %ul \n yi: %ul", xi, yi);
	}

	wait_esc_release();

	vg_exit();

}
}	

int round_float(float f) {
	int n;
	n = n * 10.0f;
	n = (n > (floor(n)+0.5f)) ? ceil(n) : floor(n);
	n = n / 10.0f;
	return n;
}

int test_move(unsigned short xi, unsigned short yi, char *xpm[], 
				unsigned short hor, short delta, unsigned short time) {
	
	int width, height;

	// xpm to pix_map
	char * pix_map = read_xpm(xpm, &width, &height);

	int ipc_status;
	message msg;

	int keyboard_irq_set;
	if ( (keyboard_irq_set = BIT(kbd_subscribe_int())) < 0 ) { // hook_id returned for keyboard
		printf("test_move() -> FAILED kbd_subscribe_int()\n");
		return 1;
	}
	else {
		if (x_variation > y_variation)
			n = x_variation;
		else
			n = y_variation;
	}

	int esc_flag = 0;	// Flag for Esc BreakCode

	/* Calculate Update Increments */
	float update[2] = {0, 0};	// Update Vector
	if (hor) {
		update[0] = delta / (float) (time * 60);
	} else {
		update[1] = delta / (float) (time * 60);
	}
	float cumulative_update[2] = {0, 0};

	// Debug TODO
	printf("update vector: %f \n", update[0]);

	vg_exit();

}

int test_xpm(unsigned short xi, unsigned short yi, char *xpm[]) {
	
	int width, height;

						// Update Cumullative Update
						cumulative_update[0] += update[0];
						cumulative_update[1] += update[1];

						// Update Position
						if (cumulative_update[0] > 1) {
							int tmp = round_float(cumulative_update[0]);
							yi += tmp;
							cumulative_update[0] -= tmp;
						} else if (cumulative_update[1] > 1) {
							int tmp = round_float(cumulative_update[1]);
							yi += tmp;
							cumulative_update[1] -= tmp;
						}
					}

}	

	if ( kbd_unsubscribe_int() < 0 ) {
		printf("test_move() -> FAILED kbd_unsubscribe_int()\n");
		return 1;
	}
	if ( timer_unsubscribe_int() < 0 ) {
	printf("test_move() -> FAILED timer_unsubscribe_int()\n");
	return 1;
	}

	if (vg_exit() == OK) {
		printf("vg_exit success\n");
	} else {
		printf("vg_exit failes\n");
	}

	return OK;
}					

int test_controller() {
//	//struct mem_range mr;
//	vbe_info_block* vbe_info_p = malloc(sizeof(vbe_info_block));
//
//	//Initialization of vbe_info_p
//	if (vbe_get_controller_info(vbe_info_p) != 0) {
//		printf ("test_controler: Failed vbe_get_controller_info");
//		return 1;
//	}
//
//	//Now display the info saved on vbe_info_p
//	printf("\n	VBE Controller Information\n\n");		//TODO: Ask teacher the capabilities, not explicit on VESA
//	printf("Capabilites of Graphics Controller: 0x%x\n", (*vbe_info_p).Capabilities[0]); //Em hexadecimal pois queremos analisar os bytes enão o valor decimal
//	printf("List of mode supported:\n");
//	//Ciclo que imprime os elementos do (*vbe_info_p).VideoModePointer
//	pritnf("VRAM memory size: %d KB.\n\n", (*vbe_info_p).TotalMemory * 64); //Number of 64kb memory blocks * number of blocks = number of KB's

	return 0;
}

