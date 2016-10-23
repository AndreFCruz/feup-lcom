#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "i8042.h"
#include "test3.h"
#include "keyboard.h"
#include "timer.h"

int kbd_test_scan(unsigned short ass)
{
	int ipc_status;
	message msg;

	int keyboard_irq_set;
	if ( (keyboard_irq_set = BIT(kbd_subscribe_int())) < 0 ) { // hook_id returned for keyboard
		printf("kbd_test_scan() -> FAILED kbd_subscribe_int()\n");
		return 1;
	}

	int status = 0;	// keyboard status flag

	int r;
	while( status != 2 ) { // While ESC BreakCode not detected
		/* Get a request message. */
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d\n", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE: /* hardware interrupt notification */
					if (msg.NOTIFY_ARG & keyboard_irq_set) { /* subscribed interrupt */

						// TODO: Choose ASM or C implementation according to argument

						if ( keyboard_handler(&status) != OK ) {
							printf("kbd_test_scan() -> FAILED keyboard_handler()\n");
							// return 1;
						}
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

	if ( kbd_unsubscribe_int() < 0 ) {
		printf("kbd_test_scan() -> FAILED kbd_unsubscribe_int()\n");
		return 1;
	}

	printf("\n* keyboard_test_scan() Finished *\n");

	return 0;
}



int kbd_test_leds(unsigned short n, unsigned short *leds) {
    /* To be completed */
}


// ONLY A DRAFT . TODO: Finish interrupt logic
int kbd_test_timed_scan(unsigned short n)
{
	int ipc_status;
	message msg;

	int keyboard_irq_set;
	if ( (keyboard_irq_set = BIT(kbd_subscribe_int())) < 0 ) { // hook_id returned for keyboard
		printf("kbd_test_scan() -> FAILED kbd_subscribe_int()\n");
		return 1;
	}
	 int timer_irq_set;
	 if ( (timer_irq_set = BIT(timer_subscribe_int())) < 0 || timer_set_square(0, 60) != OK ) { // hook_id returned for Timer 0
	 	printf("kbd_test_scan() -> FAILED timer_subscribe_int()\n");
	 	return 1;
	 }

	int status = 0;	// keyboard status flag

	int r;
	while( status != 2 ) { // While ESC BreakCode not detected
		/* Get a request message. */
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d\n", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE: /* hardware interrupt notification */
					if (msg.NOTIFY_ARG & keyboard_irq_set) { /* keyboard interrupt */
						if ( keyboard_handler(&status) != OK ) {
							printf("kbd_test_scan() -> FAILED keyboard_handler()\n");
							// return 1;
						}
					}
					if (msg.NOTIFY_ARG & timer_irq_set) { /* timer interrupt */
//						if ( keyboard_handler(&status) != OK ) {
//							printf("kbd_test_scan() -> FAILED keyboard_handler()\n");
//							// return 1;
//						}
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

	if ( kbd_unsubscribe_int() < 0 ) {
		printf("kbd_test_scan() -> FAILED kbd_unsubscribe_int()\n");
		return 1;
	}
	 if ( timer_unsubscribe_int() < 0 ) {
	 	printf("kbd_test_scan() -> FAILED timer_unsubscribe_int()\n");
	 	return 1;
	 }

	printf("\n* keyboard_test_scan() Finished *\n");
	return 0;
}
