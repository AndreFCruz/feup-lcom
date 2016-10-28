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
						if ( keyboard_handler(&status, ass) != OK ) {
							printf("kbd_test_scan() -> FAILED keyboard_handler()\n");
							return 1;
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

	printf("\n* keyboard_test_scan() Finished *\n\n");

	return 0;
}

int kbd_test_leds(unsigned short n, unsigned short *leds)
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
	unsigned short idx = 0;	// Index in leds array
	unsigned timerCount = 0;
	unsigned char led_status = 0; //Initialize all LEDs as 0

	int r;
	while( idx < n ) { // While Index not equal to array number of elements
		/* Get a request message. */
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d\n", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE: /* hardware interrupt notification */
//					if (msg.NOTIFY_ARG & keyboard_irq_set) { /* keyboard interrupt */
//						// Do nothing
//					}
					if (msg.NOTIFY_ARG & timer_irq_set) { /* timer interrupt */
						if ( timerCount++ % 60 == 0 )	//Assuring 1 sec between led toggles
							if ( keyboard_toggle_led(leds[idx++], & led_status) != OK ) {
								printf("kdt_test_leds() -> FAILED keyboard_toggle_led()\n");
								return 1;
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
		printf("kbd_test_leds() -> FAILED kbd_unsubscribe_int()\n");
		return 1;
	}
	 if ( timer_unsubscribe_int() < 0 ) {
	 	printf("kbd_test_leds() -> FAILED timer_unsubscribe_int()\n");
	 	return 1;
	 }

	printf("\n* keyboard_test_leds() Finished *\n\n");
	return 0;
}


int kbd_test_timed_scan(unsigned short n)
{
	int ipc_status;
	message msg;

	int keyboard_irq_set;
	if ( (keyboard_irq_set = BIT(kbd_subscribe_int())) < 0 ) { // hook_id returned for keyboard
		printf("kbd_test_timed_scan() -> FAILED kbd_subscribe_int()\n");
		return 1;
	}
	 int timer_irq_set;
	 if ( (timer_irq_set = BIT(timer_subscribe_int())) < 0 || timer_set_square(0, 60) != OK ) { // hook_id returned for Timer 0
	 	printf("kbd_test_timed_scan() -> FAILED timer_subscribe_int()\n");
	 	return 1;
	 }

	int status = 0;	// keyboard status flag
	unsigned short seconds = 0; //Counter of seconds elapsed
	unsigned timerCount = 0;

	int r;
	while( status != 2 && seconds < n ) { // While ESC BreakCode not detected or number of seconds elapsed not equal to n
		/* Get a request message. */
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d\n", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE: /* hardware interrupt notification */
					if (msg.NOTIFY_ARG & keyboard_irq_set) { /* keyboard interrupt */
						if ( keyboard_handler(&status, 0) != OK ) {
							printf("kbd_test_timed_scan() -> FAILED keyboard_handler()\n");
							return 1;
						}
						// Restart time count on key pressed
						timerCount = 0;
						seconds = 0;
					}
					if (msg.NOTIFY_ARG & timer_irq_set) { /* timer interrupt */
						if ( (++timerCount % 60) == 0 )
							seconds++;
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
		printf("kbd_test_timed_scan() -> FAILED kbd_unsubscribe_int()\n");
		return 1;
	}
	 if ( timer_unsubscribe_int() < 0 ) {
	 	printf("kbd_test_timed_scan() -> FAILED timer_unsubscribe_int()\n");
	 	return 1;
	 }

	printf("\n* keyboard_test_timed_scan() Finished *\n\n");
	return 0;
}




