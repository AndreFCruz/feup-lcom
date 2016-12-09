/* includes */
#include <minix/syslib.h>
#include <minix/driver.h>
#include <minix/com.h>
#include <minix/sysutil.h>
#include "video_gr.h"
#include "timer.h"
#include "keyboard.h"
#include "i8042.h"
#include "read_xpm.h"
#include "math.h"
#include "Input.h"
#include "vbe.h"
#include "pixmap.h"
#include "stddef.h"
#include "mouse.h"
#include "Bitmap.h"

/* ** */

int main()
{
	sef_startup();

	// Rudimentary Game Setup
	char * ptr;
	if ( (ptr = vg_init(MODE_800X600_64k)) == NULL ) {
		printf("test_square failed VRAM map in vg_init\n");
		return 1;
	}


	//Testes
	//draw_line(ptr, 100,100,800,300,37);
	//draw_circle(ptr, 300, 300, 200, 18);
	//draw_square(ptr, 50, 300, 100, 23);
	//draw_xpm(ptr, 500, 500, penguin);

	//Mouses-tests
	Bitmap* background = loadBitmap("/home/lcom/svn/proj/res/background.bmp");
	input_t * input = new_input(get_Xres(), get_Yres());

	unsigned short idle_time = 5;
	int ipc_status;
	message msg;

	/* Subscribe all interrupts */
	int mouse_irq_set;
	if ( (mouse_irq_set = BIT(mouse_subscribe_int())) < 0 ) {
		printf("test_async() -> FAILED mouse_subscribe_int()\n");
	}
	int timer_irq_set;
	if ( (timer_irq_set = BIT(timer_subscribe_int())) < 0 ) {
		printf("test_async() -> FAILED timer_subscribe_int()\n");
	}
	/* */

	unsigned int timerCount = 0;	// Number of timer interrupts
	unsigned short elapsed = 0;		// Time inactive in seconds
	unsigned char packet[PACKET_NELEMENTS];
	unsigned short counter = 0;		// Keeps the number of bytes ready in the packet

	int r;
	while( elapsed < idle_time ) {	// Exits when cnt reaches 0
		/* Get a request message. */
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d\n", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE: /* hardware interrupt notification */
					if (msg.NOTIFY_ARG & mouse_irq_set) {
						mouse_handler(packet, & counter);

						if (counter == PACKET_NELEMENTS) {
							//fill_screen(1);
							drawBitmap(ptr,background,0,0,ALIGN_LEFT);
							update_mouse_position(input, int_value(packet[1], packet[0] & BYTE0_X_SIGN), -int_value(packet[2], packet[0] & BYTE0_Y_SIGN));
							if (mouse_inside_area(input, 462, 334,562, 434) == OK)
								draw_square(ptr, 462, 334,100, WHITE);
							//draw_xpm(ptr, get_mouse_x(input), get_mouse_y(input), cross);
							draw_mouse_cross(ptr,get_mouse_x(input), get_mouse_y(input));

							counter = 0;
						}
						timerCount = elapsed = 0;
					}
					if (msg.NOTIFY_ARG & timer_irq_set) {
						if ( ++timerCount % 60 == 0 )
							++elapsed;
					}
					break;
				default:
					break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
			printf("Error: received unexpected standard message.\n");
		}
	}

	/* Unsubscribe all interrupts */
	if ( mouse_unsubscribe_int() < 0 ) {
		printf("test_async() -> FAILED mouse_unsubscribe_int()\n");
	}
	if ( timer_unsubscribe_int() < 0 ) {
#define M_TOLERANCE 2	// Mouse Tolerance for small direction changes
		printf("test_async() -> FAILED timer_unsubscribe_int()\n");
	}
	/* */

	unsigned char dummy;
	sys_inb(OUT_BUF, (unsigned long *) &dummy);	// Clear output buffer, so keyboard can be used after service call

	deleteBitmap(background);
	/* Finish of Mouse Test */

	//timer_delay(5);

	return vg_exit();
}
