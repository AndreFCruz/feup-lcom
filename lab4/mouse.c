#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <minix/sysutil.h>
#include "mouse.h"
#include "i8042.h"

typedef int bool;

static int mouse_hook_id = MOUSE_INITIAL_HOOK_ID;
static const unsigned maxIter = 15;         // Maximum iterations/tries when retrieving data

int mouse_subscribe_int()
{
	//kbd_write(WRITE_B_MOUSE);			//TODO DAR DISABLE E ENABLE STREAM CONFIG E LER SP O OUTBUF (?)

	if ( sys_irqsetpolicy (MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id) != OK ) {
			printf("mouse_subscribe_int() -> FAILED sys_irqsetpolicy()\n");
			return -1;
	}
	if ( sys_irqenable (&mouse_hook_id) != OK ) {
			printf("mouse_subscribe_int() -> FAILED sys_irqenable()\n");
			return -1;
	}

	return MOUSE_INITIAL_HOOK_ID;
}

int mouse_unsubscribe_int()
{
	if ( sys_irqdisable (&mouse_hook_id) != OK ) {
			printf("mouse_unsubscribe_int() -> FAILED sys_irqdisable()\n");
			return -1;
	}
	if ( sys_irqrmpolicy (&mouse_hook_id) != OK ) {
			printf("mouse_unsubscribe_int() -> FAILED sys_irqrmpolicy()\n");
			return -1;
	}

	return MOUSE_INITIAL_HOOK_ID;
}

int mouse_synchronize()	// Syncrhonizes with FIRST byte, returns data read
{
	int data;
	unsigned iter = 0;

	while (iter++ < maxIter) {
		if ( (data = mouse_read()) & BYTE0_SYNC_BIT )
			return data;
	}

	printf("mouse_synchronize() -> Max Iterations Reached. Was %d.\n", iter);
	return -1;
}

//CONFIGURAR NO SUBSCRIBE A CENA DO STREAM. VOU TER DE REFORMULAR ISTO

int keyboard_read(void)	// Reads Keyboard Data from OutPut Buffer
{
	unsigned long stat, data;
	unsigned iter = 0;
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
			if ( (stat & (STAT_PARITY | STAT_TIMEOUT)) == 0 )	// Error Occurred ?
				return data;
			else
				return -1;	// Returns -1 or 0xFF on failure
		}
	tickdelay(micros_to_ticks(KBD_DELAY_US));
	}

	printf("keyboard_read() -> Error: Max Iterations Reached. Was %d.\n", iter);
	return -1;
}

int kbc_write(char data)		//Writes Data to the Keyboard Input Buffer
{
	unsigned long stat;
	unsigned iter = 0;
	while ( iter++ < maxIter ) {
		if ( sys_inb (STAT_REG, &stat) != OK ) {
			printf("keyboard-write() -> FAILED sys_inb()");
			return 1;
		}
		/* loop while 8042 input buffer is full */
		if ( (stat & STAT_IBF) == 0 ) {
			if ( sys_outb (KBD_IN_BUF, data) != OK ) {
				printf ("kbc_write() -> FAILED sys_outb()\n");
           		return 1; 	//Failure
		}
		if ( (stat & (STAT_PARITY | STAT_TIMEOUT)) == 0 )
			return OK;
		else
			return 1;		//Failure
		}
	tickdelay(micros_to_ticks(KBD_DELAY_US));
	}

	printf("kbc_write() -> Max Iterations Reached. Was %d.\n", iter);
	return 1;
}

int kbc_write_command(char command, unsigned char arg)
{
    long kbdResponse;
    unsigned inner = 0, outter = 0;

    while ( outter++ < maxIter ) {

		if (kbc_write (command) != OK)
			return 1;		//Print of error is done in kbc_write()

		if ( (kbdResponse = mouse_read()) == -1 ) {
			printf ("kbc_write_command() -> FAILED sys_inb()\n");
			return 1;
		}

        //A byte different from the 3 expected was received
        if (kbdResponse != IN_RESEND && kbdResponse != IN_ERROR && kbdResponse != IN_ACK) {
            printf ("kbc_write_command() Outer Loop -> ERROR: unknown response from the KBD. Was %x.\n", kbdResponse);
            continue;
        }

        if (kbdResponse == IN_ACK) // Success
        {
            while (inner++ < maxIter) {
        		if (kbc_write (arg) != OK)
        			return 1;		//Print of error is done in kbc_write()

        		if ( (kbdResponse = mouse_read ()) == -1) {
        			printf ("kbc_write_command() -> FAILED sys_inb()\n");
        			return 1;
        		}

                if (kbdResponse != IN_RESEND && kbdResponse != IN_ERROR && kbdResponse != IN_ACK) {
                    printf ("kbc_write_command() Inner Loop -> ERROR: unknown response from the KBD. Was %x.\n", kbdResponse);
                    return 1;
                }

                if (kbdResponse == IN_ACK) //Success on both cycles
                    return OK;

                if (kbdResponse == IN_ERROR) //Restart everything
                    break;
                }
        }
    } // tick_delay is called in both write and read functions

    printf("kbc_write_command() -> ERROR: Maximum iterations/tries reached. Outter: %d; Inner: %x.\n", outter, inner);
    return 1;
}


int mouse_read()	// Reads Mouse Data from OutPut Buffer
{
	unsigned long stat, data;
	unsigned iter = 0;
	while( iter++ < maxIter ) {
		if ( sys_inb(STAT_REG, &stat) != OK ) {
			printf("mouse_read() -> FAILED sys_inb()\n");
			return -1;
		}
		/* loop while 8042 output buffer is empty */
		if( (stat & STAT_OBF) && (stat & STAT_AUX) ) {
			if ( sys_inb(OUT_BUF, &data) != OK ) {
				printf("mouse_read() -> FAILED sys_inb()\n");
				return -1;	// Returns -1 or 0xFF on failure
			}
			return data;
		}
	tickdelay(micros_to_ticks(KBD_DELAY_US));
	}

	printf("mouse_read() -> Error: Max Iterations Reached. Was %d.\n", iter);
	return -1;
}

int mouse_write (char data)
{
	// TODO: check if in default/stream mode, and disable etc (?)
	return kbc_write(data);
}

int mouse_handler (unsigned char * packet, unsigned short * counter)
{
	if (*counter == 0) {
		int sync_result = mouse_synchronize();	// Sync packet if expecting first byte

		if (sync_result == -1) {
			printf("mouse_handler() -> FAILED mouse_synchronize()\n");
			return 1;
		}
		packet[*counter++] = sync_result;
		return OK;
	}
	else
		packet[*counter++] = mouse_read();

	return OK;
}


void print_packet (unsigned char * packet)
{
	printf("B1=0x%02X\t", packet[0]);
	printf("B2=0x%02X\t", packet[1]);
	printf("B3=0x%02X\t", packet[2]);
	printf("LB=%d ", packet[0] & BYTE0_LB ? 1 : 0);
	printf("MB=%d ", packet[0] & BYTE0_MB ? 1 : 0);
	printf("RB=%d ", packet[0] & BYTE0_RB ? 1 : 0);
	printf("XOV=%d ", packet[0] & BYTE0_X_OVF ? 1 : 0);
	printf("YOV=%d ", packet[0] & BYTE0_Y_OVF ? 1 : 0);

	// check sign of X and Y before printing
	printf("X=%d\t", packet[1]);
	printf("Y=%d\n", packet[2]);
}


int fetch_mouse_config(unsigned char * config)
{
	if ( kbc_write_command(WRITE_B_MOUSE, STATUS_REQUEST) != OK ) {
		printf("fetch_mouse_config::kbc_write_command FAILED\n");
		return 1;
	}

	int i;
	for (i = 0; i < 3; i++) {
		config[i] = mouse_read();
	}

	return OK;
}
