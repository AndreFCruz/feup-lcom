#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "i8042.h"
#include "test4.h"
#include "mouse.h"
#include "keyboard.h"
#include "timer.h"

int test_packet(unsigned short cnt) {
	int ipc_status;
	message msg;

	int mouse_irq_set;
	if ( (mouse_irq_set = BIT(mouse_subscribe_int())) < 0 ) { // hook_id returned for keyboard
		printf("test_packet() -> FAILED mouse_subscribe_int()\n");
		return 1;
	}

	int iter = 0;
	unsigned char packet[PACKET_NELEMENTS];
	unsigned short counter = 0; //Keeps the number of bytes ready in the packet

	int r;
	while( iter < count ) { // Do this cnt times
		/* Get a request message. */
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d\n", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE: /* hardware interrupt notification */
					if (msg.NOTIFY_ARG & mouse_irq_set) { /* subscribed interrupt */
						if ( mouse_handler(packet, & counter) != OK ) {
							printf("test_packet() -> FAILED mouse_handler()\n");
							iter = count;	// rude way of exiting loop and proceding to unsubscribe interrupts
							break;
						}
						if (conter == PACKET_NELEMENTS)
							iter++;
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

	if ( mouse_unsubscribe_int() < 0 ) {
		printf("test_packet() -> FAILED mouse_unsubscribe_int()\n");
		return 1;
	}

	printf("\n* test_packet() Finished *\n\n");

	return 0;
}	
	
int test_async(unsigned short idle_time) {
    /* To be completed ... */
}
	
int test_config(void) {
    /* To be completed ... */
}	
	
int test_gesture(short length) {
    /* To be completed ... */
}
