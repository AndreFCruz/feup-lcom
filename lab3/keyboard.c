#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "keyboard.h"

#define DELAY_TO            20000   // KBC respond Time-Out

static int kbd_hook_id = KBD_INITIAL_HOOK_ID;

int kbd_subscribe_int(void)
{
	if ( sys_irqsetpolicy (KBC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hook_id) != OK ) {
		printf("kbd_subscribe_int() -> FAILED sys_irqsetpolicy()\n");
		return -1;
	}
	if ( sys_irqenable (&kbd_hook_id) != OK ) {
		printf("kbd_subscribe_int() -> FAILED sys_irqenable()\n");
		return -1;
	}

	return KBD_INITIAL_HOOK_ID;
}

int kbd_unsubscribe_int(void)
{
	if ( sys_irqdisable (&kbd_hook_id) != OK ) {
		printf("kbd_unsubscribe_int() -> FAILED sys_irqdisable()\n");
		return -1;
	}
	if ( sys_irqrmpolicy (&kbd_hook_id) != OK ) {
		printf("kbd_unsubscribe_int() -> FAILED sys_irqrmpolicy()\n");
		return -1;
	}

	return KBD_INITIAL_HOOK_ID;
}

u8_t keyboard_read()	// Reads Keyboard Data from OutPut Buffer
{
	u8_t stat, data;
	while( true ) {
		if ( sys_inb(STAT_REG, &stat) != OK ) {
			printf("keyboard_read() -> FAILED sys_inb()");
			return -1;
		}
		/* loop while 8042 output buffer is empty */
		if( stat & STAT_OBF ) {
			sys_inb(OUT_BUF, &data); /* assuming it returns OK */
			if ( (stat & (STAT_PARITY | STAT_TIMEOUT)) == 0 )	// Error Ocurred ?
				return data;
			else
				return -1;
		}
	//delay(WAIT_KBC);
	}
}
