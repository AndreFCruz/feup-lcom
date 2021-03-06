#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "i8042.h"
#include "test4.h"
#include "mouse.h"
#include "timer.h"


int test_packet(unsigned short cnt) {
	int ipc_status;
	message msg;

	int mouse_irq_set;
	if ( (mouse_irq_set = BIT(mouse_subscribe_int())) < 0 ) {
		printf("test_packet() -> FAILED mouse_subscribe_int()\n");
		return 1;
	}

	unsigned char packet[PACKET_NELEMENTS];
	unsigned short counter = 0; // Keeps the number of bytes ready in the packet

	int r;
	while( cnt > 0 ) {	// Exits when cnt reaches 0
		/* Get a request message. */
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d\n", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE: /* hardware interrupt notification */
					if (msg.NOTIFY_ARG & mouse_irq_set) { /* subscribed interrupt */

						mouse_handler(packet, & counter);
						if (counter == PACKET_NELEMENTS) {
							print_packet(packet);
							--cnt;
							counter = 0;
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

	if ( mouse_unsubscribe_int() < 0 ) {
		printf("test_packet() -> FAILED mouse_unsubscribe_int()\n");
		return 1;
	}

	unsigned char dummy;
	sys_inb(OUT_BUF, (unsigned long *) &dummy);	// Clear output buffer, so keyboard can be used after service call

	printf("\n* test_packet() Finished *\n");

	return 0;
}

int test_async(unsigned short idle_time) {
	int ipc_status;
	message msg;

	/* Subscribe all interrupts */
	int mouse_irq_set;
	if ( (mouse_irq_set = BIT(mouse_subscribe_int())) < 0 ) {
		printf("test_async() -> FAILED mouse_subscribe_int()\n");
		return 1;
	}
	int timer_irq_set;
	if ( (timer_irq_set = BIT(timer_subscribe_int())) < 0 ) {
	printf("test_async() -> FAILED timer_subscribe_int()\n");
	return 1;
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
							print_packet(packet);
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
		return 1;
	}
	if ( timer_unsubscribe_int() < 0 ) {
	printf("test_async() -> FAILED timer_unsubscribe_int()\n");
	return 1;
	}
	/* */

	unsigned char dummy;
	sys_inb(OUT_BUF, (unsigned long *) &dummy);	// Clear output buffer, so keyboard can be used after service call

	printf("\n* test_async() Finished *\n");

	return 0;
}

int test_config(void) {
	if ( mouse_subscribe_int() < 0 ) {
		printf("test_config() -> FAILED mouse_subscribe_int()\n");
		return 1;
	}

	unsigned char config[3];
	if ( mouse_fetch_config(config) != OK ) {
		printf("test_config failed\n");
		return 1;
	}

	printf("\n-> BYTE 0: 0x%02X\n", config[0]);
	printf("%-15s: %s\n", "Operating Mode", config[0] & BIT(6) ? "Remote mode" : "Stream mode");
	printf("%-15s: %s\n", "Data Reporting", config[0] & BIT(5) ? "Enabled" : "Disabled");
	printf("%-15s: %s\n", "Scaling Mode", config[0] & BIT(4) ? "2:1" : "1:1");
	printf("%-15s: %s\n", "Middle Button", config[0] & BIT(2) ? "Pressed" : "Released");
	printf("%-15s: %s\n", "Right Button", config[0] & BIT(1) ? "Pressed" : "Released");
	printf("%-15s: %s\n", "Left Button", config[0] & BIT(0) ? "Pressed" : "Released");

	printf("\n-> BYTE 1: 0x%02X\n", config[1]);
	printf("%-15s: %d\n", "Resolution", BIT(config[1]));

	printf("\n-> BYTE 3: 0x%02X\n", config[2]);
	printf("%-15s: %d\n", "Sample Rate", config[1]);
	if (mouse_write_cmd(ENABLE_DATA_R) != OK) {
		printf("test_config() -> FAILED mouse_write_cmd(ENABLE_DATA_R)\n");
		return 1;
	}

	if ( mouse_unsubscribe_int() < 0 ) {
		printf("test_config() -> FAILED mouse_unsubscribe_int()\n");
		return 1;
	}

	return OK;
}

int test_gesture(short length) {
	if (length == 0) {
		printf("Invalid argument for test_gestute, length must be non-zero.\n");
		return 1;
	}

	int ipc_status;
	message msg;

	int mouse_irq_set;
	if ( (mouse_irq_set = BIT(mouse_subscribe_int())) < 0 ) {
		printf("test_gesture() -> FAILED mouse_subscribe_int()\n");
		return 1;
	}

	unsigned char packet[PACKET_NELEMENTS];
	unsigned short counter = 0; // Keeps the number of bytes ready in the packet

	event_t evt;
	evt.type = RUP;
	evt.y_delta = 0;
	evt.x_delta = 0;
	evt.dir = UPWARDS;	// Will be reset on first call of event_handler

	int r;
	while( evt.type != VERTLINE ) {
		/* Get a request message. */
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d\n", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE: /* hardware interrupt notification */
					if (msg.NOTIFY_ARG & mouse_irq_set) { /* subscribed interrupt */

						mouse_handler(packet, & counter);
						if (counter == PACKET_NELEMENTS) {
							print_packet(packet);
							counter = 0;

							check_ver_line(&evt,packet,length);
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

	if ( mouse_unsubscribe_int() < 0 ) {
		printf("test_gesture() -> FAILED mouse_unsubscribe_int()\n");
		return 1;
	}

	unsigned char dummy;
	sys_inb(OUT_BUF, (unsigned long *) &dummy);	// Clear output buffer, so keyboard can be used after service call

	printf("\n* test_gesture() Finished *\n");

	return 0;
}
