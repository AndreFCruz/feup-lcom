#include <minix/syslib.h>
#include <minix/driver.h>
#include <minix/com.h>
#include <minix/sysutil.h>
#include "video_gr.h"
#include "timer.h"
#include "defs.h"
#include "keyboard.h"
#include "i8042.h"
#include "read_xpm.h"
#include "math.h"
#include "vbe.h"

#define BLACK	0

// TODO argument checks and error messages
// TODO remove hard coded constants

void *test_init(unsigned short mode, unsigned short delay) {
	if (mode > 0x10C || mode < 0x100) {		//TODO: Retirar magic numbers?
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
	
//Based on DDA Algorithm
int test_line(unsigned short xi, unsigned short yi, 
		           unsigned short xf, unsigned short yf, unsigned long color) {

	//Check limis of Res?

	char * ptr = vg_init(MODE_5);
	
	// x and y variation
	int x_variation = xf-xi;
	int y_variation = yf-yi;

	//Need this for functions whose slopes are != 1
	float x = (float) xi;
	float y = (float) yi;
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
		paint_pixel(x, y, color, ptr);
		x += (x_variation / (float) n);
		y += (y_variation / (float) n);
		printf("xi: %d \n yi: %ul", x, y);
	}

	wait_esc_release();

	return vg_exit();
}

int test_xpm(unsigned short xi, unsigned short yi, char *xpm[]) {

	int width, height;
	// xpm to pix_map, update width and height
	char * pix_map = read_xpm(xpm, &width, &height);

	char * ptr;
	if ( (ptr = vg_init(MODE_5)) == NULL) {
		printf("test_xpm: Failed vg_init.\n");
		return 1;
	}

	unsigned i, j;
	for (i = 0; i < width; i++) {
		for (j = 0; j < height; j++) {
			paint_pixel(i + xi, j + yi, *(pix_map + i + j * width), ptr);
		}
	}

	//Wait for Esc BreakCode
	wait_esc_release();
	return vg_exit();
}

// Rounds float to nearest integer
int round_float(float f) {
	return (f > (floor(f) + 0.5)) ? ceil(f) : floor(f);
}

// To Operate only in mode 0x105 - According to Handout
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
	int timer_irq_set; unsigned timer_freq = 60;
	if ( (timer_irq_set = BIT(timer_subscribe_int())) < 0 || timer_set_square(0, timer_freq) != OK ) { // hook_id returned for Timer 0
		printf("test_move() -> FAILED timer_subscribe_int()\n");
		return 1;
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
	unsigned pos[2] = {xi, yi};	// Image position

//	printf("Update Vector: (%d, %d)\n", (int) update[0], (int) update[1]);

	//Initiate Graphics Mode
	char *ptr;
	if ( (ptr = vg_init(MODE_5)) == NULL) {
		printf("test_xpm: Failed vg_init.\n");
		return 1;
	}

	int r;
	while( ! esc_flag ) { // While ESC BreakCode not detected
		/* Get a request message. */
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			 printf("driver_receive failed with: %d\n", r);
			 continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
					case HARDWARE: /* hardware interrupt notification */
						if (msg.NOTIFY_ARG & keyboard_irq_set) { /* keyboard interrupt */
							if ( keyboard_read() == ESQ_BREAK_CODE ) {
								esc_flag = 1;
								break;
							}
						}

						if (msg.NOTIFY_ARG & timer_irq_set) { /* timer interrupt */
							int i,j;

							int border[2] = {update[0] * (update[0]>0 ? 1:-1), update[1] * (update[1]>0 ? 1:-1)};
							// Draw XPM + paint a black box of thickness abs(update) + 2 around it
							for (i = -border[0] - 2; i < width + border[0] + 2; i++) {
								for (j = -border[1] - 2; j < height + border[1] + 2; j++) {
									if (i < 0 || i >= width || j < 0 || j >= height) {
										paint_pixel(i + pos[0], j + pos[1], BLACK, ptr);
									} else {
										paint_pixel(i + pos[0], j + pos[1], *(pix_map + i + j * width), ptr);
									}
								}
							}

							/* Deprecated option, causes image to 'flash' */
//							// Refresh Screen - Fill with Black
//							for (i = 0; i < H_RES_0X105; i++) {
//								for (j = 0; j < V_RES_0X105; j++) {
//									paint_pixel(i, j, BLACK, ptr);
//								}
//							}
//
//							// Draw XPM
//							for (i = 0; i < width; i++) {
//								for (j = 0; j < height; j++) {
//									paint_pixel(i + pos[0], j + pos[1], *(pix_map + i + j * width), ptr);
//								}
//							}
							/* * */

							// Check if delta was reached
							if ( delta > 0 && (hor ? cumulative_update[0] : cumulative_update[1]) > delta ) {
								continue;
							}
							else if ( delta < 0 && (hor ? cumulative_update[0] : cumulative_update[1]) < delta ) {
								continue;
							}

							//Update Cumullative_Update
							cumulative_update[0] += update[0];
							cumulative_update[1] += update[1];

							//Update Position
							int tmp;
							if ( (tmp = round_float(cumulative_update[0] + xi - pos[0]) * (delta>0 ? 1:-1)) > 1 ) {
								pos[0] += tmp * (delta>0 ? 1:-1);
							}
							else if ( (tmp = round_float(cumulative_update[1] + yi - pos[1]) * (delta>0 ? 1:-1)) > 1 ) {
								pos[1] += tmp * (delta>0 ? 1:-1);
							}
						}

						break;
				default:
						break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
				/* no standard messages expected: do nothing */
			printf("Error: received unexpected standard message.\n");
			return 1;
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

	return vg_exit();
}					

int test_controller() {
	// Pointer to vbe_info_block struct instance
	vbe_info_block* vbe_info_p = (vbe_info_block*) malloc(sizeof(vbe_info_block));

	//Initialization of vbe_info_p
	if (vbe_get_controller_info(vbe_info_p) != 0) {
		printf ("test_controler: Failed vbe_get_controller_info");
		return 1;
	}

	//Now display the info saved on vbe_info_p
	printf("\n	VBE Controller Information\n\n");		//TODO: Ask teacher the capabilities, not explicit on VESA
	printf("VESA Signature: %s \n", (*vbe_info_p).VESASignature );
	//printf("Capabilites of Graphics Controller: 0x%x\n", (*vbe_info_p).Capabilities[0]); //Em hexadecimal pois queremos analisar os bytes enão o valor decimal
	printf("List of mode supported:\n");
	//Ciclo que imprime os elementos do (*vbe_info_p).VideoModePointer
	printf("VRAM memory size: %d KB.\n\n", (*vbe_info_p).TotalMemory * 64); //Number of 64kb memory blocks * number of blocks = number of KB's

	return 0;
}

