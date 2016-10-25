#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <minix/sysutil.h>
#include "keyboard.h"
#include "i8042.h"

#define DELAY_US		20000   // KBC respond Time-Out in micro seconds

typedef int bool;

static int kbd_hook_id = KBD_INITIAL_HOOK_ID;
static const unsigned maxIter = 15; 	// Maximum Iterations when retrieving data from KBC

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

unsigned char keyboard_read()	// Reads Keyboard Data from OutPut Buffer
{
	unsigned long stat, data;
	unsigned iter;
	while( iter++ < maxIter ) {
		if ( sys_inb(STAT_REG, &stat) != OK ) {
			printf("keyboard_read() -> FAILED sys_inb()");
			return -1;
		}
		/* loop while 8042 output buffer is empty */
		if( stat & STAT_OBF ) {
			if ( sys_inb(OUT_BUF, &data) != OK ) {
				printf("keyboard_read() -> FAILED sys_inb()");
				return -1;	// Returns -1 or 0xFF on failure
			}
			if ( (stat & (STAT_PARITY | STAT_TIMEOUT)) == 0 )	// Error Ocurred ?
				return data;
			else
				return -1;	// Returns -1 or 0xFF on failure
		}
	tickdelay(micros_to_ticks(DELAY_US));
	}
}

unsigned char keyboard_write(char data)		//Writes Data to the Keyboard Input Buffer
{
	unsigned long stat;
	unsigned iter,
	while ( iter++ < maxIter ) {
		if ( sys_inb (STAT_REG, &stat) != OK ) {
			print("keyboard-write() -> FAILED sys_inb()");
			return -1;
		}
		/* loop while 8042 input buffer is full */
		if ( stat & STAT_IBF ) {
			if ( sys_outb (KBD_IN_BUF, data) != OK) {
				printf ("keyboard_write() -> FAILED sys_outb()\n");
           			return -1; 	//Failure
		}
		if ( (stat & (STAT_PARITY | STAT_TIMEOUT)) == 0 )
			return OK;
		else
			return -1;		//Failure
		}
	tickdelay(micros_to_ticks(DELAY_US));
	}
}

int keyboard_write_command(char command, char arg)
{
    unsigned long kbdResponse;
    unsigned iter;

    while ( iter++ < maxIter ) {
		if (keyboard_write (command) != OK)
			return -1;		//Print of error is done in keyboard_write()

		if ( (kbdResponse = keyboard_read ()) == -1) {	//Long = char, corrigir
			printf ("keyboard_write() -> FAILED sys_inb()\n");
			return 1;
		}

        //A byte different from the 3 expected was received
        if (kbdResponse != IN_RESEND && kbdResponse != IN_ERROR && kbdResponse != IN_ACK) {
            printf ("keyboard_write() -> ERROR: unknown response from the KBD\n");
            return 1;
        }

        if (kbdResponse == IN_ACK) //Success upon 1st cycle
        {
            while (iter++ < maxIter) {
        		if (keyboard_write (arg) != OK)
        			return -1;		//Print of error is done in keyboard_write()

        		if ( (kbdResponse = keyboard_read ()) == -1) {	//Long = char, corrigir
        			printf ("keyboard_write() -> FAILED sys_inb()\n");
        			return 1;
        		}

                if (kbdResponse != IN_RESEND && kbdResponse != IN_ERROR && kbdResponse != IN_ACK) {
                    printf ("keyboard_write() -> ERROR: unknown response from the KBD\n");
                    return 1;
                }

                if (kbdResponse == IN_ACK) //Success on both cycles
                    return OK;

                if (kbdResponse == IN_ERROR) //Restart evverything
                    break;
                }
        }
    } // tick_delay is called in both write and read functions
}

/*		//ANTES DE APAGAR CONFIRMAR A FUNCIONALIDADE DA SUBSTITUTA
int keyboard_write(char command, char arg) {
    unsigned long kbcResponse;
    unsigned iter;

    while ( iter++ < maxIter ) {
        if (STAT_REG & STAT_IBF)
            return 1; //Input buffer is full

        if (sys_outb (KBD_IN_BUF, command) != OK) {
            printf ("keyboard_write() -> FAILED sys_outb()\n");
            return 1;
        }

        if (sys_inb (OUT_BUF, & kbcResponse) != OK) {
            printf ("keyboard_write() -> FAILED sys_inb()\n");
            return 1;
        }

        //A byte different from the 3 expected was received
        if (kbcResponse != IN_RESEND && kbcResponse != IN_ERROR && kbcResponse != IN_ACK) {
            printf ("keyboard_write() -> ERROR: unknown response from the KBD\n");
            return 1;
        }

        if (kbcResponse == IN_ACK) //Success upon 1st cycle
        {
            while (1) {
                if (sys_outb (KBD_IN_BUF, arg) != OK) {
                    printf ("keyboard_write() -> FAILED sys_outb()\n");
                    return 1;
                }

                if (sys_inb (OUT_BUF, & kbcResponse) != OK) {
                    printf ("keyboard_write() -> FAILED sys_inb()\n");
                    return 1;
                }

                if (kbcResponse != IN_RESEND && kbcResponse != IN_ERROR && kbcResponse != IN_ACK) {
                    printf ("keyboard_write() -> ERROR: unknown response from the KBD\n");
                    return 1;
                }

                if (kbcResponse == IN_ACK) //Success on both cycles
                    return OK;

                if (kbcResponse == IN_ERROR) //Restart evverything
                    break;
                }
        }
	tickdelay(micros_to_ticks(DELAY_US));
    }
} */

int keyboard_toggle_led (char bit)
{
	if (bit != 0 || bit != 1 || bit != 2) {
		printf("kbd_toggle_led argument must be 0, 1 or 2\n");
		return -1;
	}

	if ( keyboard_write_command (LED_TOOGLE_CMD, bit) != OK)
		return -1;	//Print Error done in keyboard_write_command()
	return OK;
}

int print_scan_code(unsigned char data, int * status)
{
	if ( data == 0xE0 ) {
		*status = 1;	// Set status to 1 (awaiting next byte)
		return OK;
	}

	// MakeCode or BreakCode ?
	bool isBreak = data & BIT(7);
	if ( isBreak )
		printf("Breakcode: ");
	else
		printf("Makecode:  ");

//	printf("DATA: %x\n", data);
//	return OK;

	if ( data == 0x81 ) {	// ESC BreakCode
		printf("%02x - ESC BreakCode\n", data);
		*status = 2;	// Set status to 2 (ESC BreakCode Detected)
		return OK;
	} else if ( *status == 1) {
		printf("0xE0 %02X\n", data);
		*status = 0;	// Set status to 0
		return OK;
	} else if ( *status == 0 ) {
		printf("%02X\n", data);
		*status = 0;	// Set status to 0
		return OK;
	} else {
		printf("print_scan_code::status is not in range [0, 2]\n");
		return 1;
	}
}

int keyboard_handler(int * status)	// To be called on KBC Interrupt
{
	unsigned char data;
	if ( (data = keyboard_read()) == -1 ) {
		printf("keyboard_handler() -> FAILED keyboard_read()");
		return 1;
	}
	if ( print_scan_code(data, status) != OK ) {
		printf("keyboard_handler() -> FAILED print_scan_code()");
		return 1;
	}

	return OK;
}
