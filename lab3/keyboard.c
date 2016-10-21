#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "keyboard.h"

int kbd_subscribe_int(int * hook_id)
{
	if ( sys_irqsetpolicy (KBC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, hook_id) != OK ) {
			printf("kbd_subscribe_int() -> FAILED sys_irqsetpolicy()\n");
			return -1;
		}

	if ( sys_irqenable (hook_id) != OK ) {
		printf("kbd_subscribe_int() -> FAILED sys_irqenable()\n");
		return -1;
	}

	return *hook_id;
}

int kbd_unsubscribe_int(int * hook_id) {
	if ( sys_irqdisable (hook_id) != OK ) {
		printf("kbd_unsubscribe_int() -> FAILED sys_irqdisable()\n");
		return -1;
	}

	if ( sys_irqrmpolicy (hook_id) != OK ) {
		printf("kbd_unsubscribe_int() -> FAILED sys_irqrmpolicy()\n");
		return -1;
	}

	return OK;
}
