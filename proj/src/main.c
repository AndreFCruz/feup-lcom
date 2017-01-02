#include <minix/syslib.h>
#include <minix/driver.h>
#include <minix/com.h>
#include <minix/sysutil.h>
#include <time.h>	// for the random seed
#include <stdlib.h>
#include "video_gr.h"
#include "Input.h"
#include "stddef.h"
#include "Bitmap.h"
#include "planetary.h"
#include "vbe.h"
#include "RTC.h"

/* Interrupt Handlers' Loop */
int main() {
	printf("\t\t\tSTART OF PROJECT SERVICE\n");
	sef_startup();

	srand(time(NULL));

	int ipc_status;
	message msg;

	/* Subscribe Interrupts */
	int keyboard_irq_set;
	if ((keyboard_irq_set = BIT(kbd_subscribe_int())) < 0) { // hook_id returned for keyboard
		printf("FAILED kbd_subscribe_int()\n");
		return 1;
	}
	int mouse_irq_set;
	if ((mouse_irq_set = BIT(mouse_subscribe_int())) < 0) {
		printf("FAILED mouse_subscribe_int()\n");
		return 1;
	}
	int timer_irq_set;
	unsigned timer_freq = 60;
	if ((timer_irq_set = BIT(timer_subscribe_int()))
			< 0|| timer_set_square(0, timer_freq) != OK) { // hook_id returned for Timer 0
		printf("FAILED timer_subscribe_int()\n");
		return 1;
	}
	int rtc_irq_set;
	if ((rtc_irq_set = BIT(rtc_subscribe_int())) < 0) {
		printf("FAILED rtc_subscribe_int()\n");
		return 1;
	}

	//Setting Serial configuration
	if (serial_set_conf() != OK) {
		printf("FAILED serial_set_conf()\n");
		return 1;
	}

	int serial_irq_set;
	if ((serial_irq_set = BIT(serial_subscribe_int())) < 0) {
		printf("FAILED serial_subscribe_int()\n");
		return 1;
	}
	if (serial_enable_interrupts() != OK) {
		printf("FAILED serial_enable_interrupts()\n");
		return 1;
	}
	/* ** */

	//Initiate Graphics Mode
	if ( OK != vg_init(MODE_800X600_64k)) {
		printf("main::vg_init Failed\n");
		return 1;
	}

	// Mouse Packets
	unsigned char packet[PACKET_NELEMENTS];
	unsigned short counter = 0; // Keeps the number of bytes ready in the packet

	int r;
	int gameRunning = 1;
	while (gameRunning) {
		/* Get a request message. */
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d\n", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & mouse_irq_set) {

					mouse_handler(packet, &counter);
					if (counter == PACKET_NELEMENTS) {
						mouse_packet_handler(packet);
						counter = 0;
					}
				}

				if (msg.NOTIFY_ARG & serial_irq_set) { /* serial interrupt */

					serial_handler();

				}

				if (msg.NOTIFY_ARG & keyboard_irq_set) { /* keyboard interrupt */

					keyboard_handler();
				}

				if (msg.NOTIFY_ARG & timer_irq_set) { /* timer interrupt */

					if ( OK != timer_handler()) {
						printf("Timer Handler Returned EXIT Code!\n");
						gameRunning = 0;
					}
					buffer_handler();

				}

				break;
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
			printf("Error: received unexpected standard message.\n");
			return 1;
		}
	}

	/* Unsubscribe All Interrupts */
	if (kbd_unsubscribe_int() < 0) {
		printf("FAILED kbd_unsubscribe_int()\n");
		return 1;
	}
	if (mouse_unsubscribe_int() < 0) {
		printf("FAILED mouse_unsubscribe_int()\n");
		return 1;
	}
	if (timer_unsubscribe_int() < 0) {
		printf("FAILED timer_unsubscribe_int()\n");
		return 1;
	}
	if (rtc_unsubscribe_int() < 0) {
		printf("FAILED rtc_unsubscribe_int()\n");
		return 1;
	}
	if (serial_unsubscribe_int() < 0) {
		printf("FAILED serial_unsubscribe_int()\n");
		return 1;
	}
	/* ** */

	unsigned char dummy;
	sys_inb(OUT_BUF, (unsigned long *) &dummy);	// Clear output buffer, so keyboard can be used after service call

	printf("\t\t\tPROJECT SERVICE ENDED\n");

	int ret = vg_exit();

	printf("***********************\n");
	printf("* Thanks for Playing! *\n");
	printf("***********************\n");

	return ret;
}

