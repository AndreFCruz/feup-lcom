#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "i8042.h"
#include "test3.h"


int kbd_subscribe_int(int * hook_id) {
	int savedvalue = *hook_id;

	if ( sys_irqsetpolicy (KBC_IRQ, IRQ_REENABLE, hook_id) != OK ) {
			fprintf(stderr, "Error: %s.\n", kernel_call_failure);
			return -1;
		}

	if ( sys_irqsetpolicy (KBC_IRQ, IRQ_EXCLUSIVE, hook_id) != OK ) {
			fprintf(stderr, "Error: %s.\n", kernel_call_failure);
			return -1;
		}

	if ( sys_irqenable (hook_id) != OK ) {
		fprintf(stderr, "Error: %s.\n", kernel_call_failure);
		return -1;
	}

	return *hook_id;

}

int kbd_unsubscribe_int(int * hook_id) {
	if ( sys_irqdisable ( hook_id) != OK ) {
		fprintf(stderr, "Error: %s.\n", kernel_call_failure);
		return -1;
	}

	if ( sys_irqrmpolicy ( hook_id) != OK ) {
		fprintf(stderr, "Error: %s.\n", kernel_call_failure);
		return -1;
	}

	return OK;
}

int kbd_test_scan(unsigned short ass) {
	/* To be completed */
}
int kbd_test_leds(unsigned short n, unsigned short *leds) {
    /* To be completed */
}
int kbd_test_timed_scan(unsigned short n) {
    /* To be completed */
}
