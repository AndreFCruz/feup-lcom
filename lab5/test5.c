#include "video_gr.h"
#include "timer.h"
#include "defs.h"
#include "keyboard.h"
#include "read_xpm.h"
//#include "vbe.h"

// TODO argument checks and error messages
// TODO remove hard coded constants

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
	
	char * ptr;
	if ( (ptr = vg_init(MODE_5)) == NULL) {
		printf("test_square failed VRAM map in vg_init\n");
		return 1;
	}

	// Draw Square
	unsigned i, j;
	for (i = x; i < size + x; i++) {
		for (j = y; j < size + y; j++) {
//			*(ptr + i + j*1024) = color;
			paint_pixel(i, j, color, ptr);
		}
	}

	// Wait for Esc BreakCode
	wait_esc_release();

	return vg_exit();
}
	

int test_line(unsigned short xi, unsigned short yi, 
		           unsigned short xf, unsigned short yf, unsigned long color) {
	
	/* To be completed */
	
}

int test_xpm(unsigned short xi, unsigned short yi, char *xpm[]) {
	
	int width, height;

	// xpm to pix_map, update width and height
	char * pix_map = read_xpm(xpm, &width, &height);

	char * ptr;
	if ( (ptr = vg_init(MODE_5)) == NULL) {
		return 1;
	}

	unsigned i, j;
	for (i = 0; i < width; i++) {
		for (j = 0; j < height; j++) {
			paint_pixel(i + xi, j + yi, *(pix_map + i + j * width), ptr);
		}
	}
	
	// Wait for Esc BreakCode
	wait_esc_release();

	return vg_exit();
}	

int test_move(unsigned short xi, unsigned short yi, char *xpm[], 
				unsigned short hor, short delta, unsigned short time) {
	
	int width, height;

	// xpm to pix_map
	char * pix_map = read_xpm(xpm, &width, &height);

	char * ptr;
	if ( (ptr = vg_init(MODE_5)) == NULL) {
		return 1;
	}

	// Update position, draw xpm, wait 1/60 secs

	
	// Wait for Esc BreakCode
	wait_esc_release();

	return vg_exit();
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

