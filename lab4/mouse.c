#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <minix/sysutil.h>
#include "mouse.h"
#include "i8042.h"

typedef int bool;

static int mouse_hook_id = MOUSE_INITIAL_HOOK_ID;
static const unsigned maxIter = 15;         // Maximum Iterations when retrieving data

int mouse_subscribe_int(void)
{
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

int mouse_unsubscribe_int(void)
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


//TODO: funcao generica mouse_write_command que escreve um comando no mouse. Serve para funcoes 1,2 e 3! Usa kbd_read e kbd_write!

//Draft
int mouse_write_command()
{
	int kbdResponse;

	//meter isto num ciclo while com maxItere c as respotas como no write command anterior, que devem ser iguais
	if (keyboard_write (WRITE_B_MOUSE) != OK)
		return 1;		//Print of error is done in keyboard_write()

	if ( (kbdResponse = keyboard_read ()) == -1 ) {
		printf ("mouse_write_command() -> FAILED sys_inb()\n");
		return 1;
	}

	if //se for ACK, passar para os mouse_write e mouse_read

}

//TODO: uma funcao que garanta a sincronizacao.

int mouse_write(char data)		//Writes Data to the Keyboard Input Buffer
{
	unsigned long stat;
	unsigned iter = 0;
	while ( iter++ < maxIter ) {
		if ( sys_inb (STAT_REG, &stat) != OK ) {
			printf("mouse_write() -> FAILED sys_inb()");
			return 1;
		}
		/* loop while 8042 input buffer is full */
		if ( (stat & STAT_IBF) == 0 ) {
			if ( sys_outb (MOUSE_IN_BUF, data) != OK ) {
				printf ("mouse_write() -> FAILED sys_outb()\n");
           		return 1; 	//Failure
		}
		if ( (stat & (STAT_PARITY | STAT_TIMEOUT)) == 0 )
			return OK;
		else
			return 1;		//Failure
		}
	tickdelay(micros_to_ticks(KBD_DELAY_US));
	}

	printf("mouse_write() -> Max Iterations Reached. Was %d.\n", iter);
	return 1;
}

int mouse_read(void)	// Reads Mouse Data from OutPut Buffer
{
	unsigned long stat, data;
	unsigned iter = 0;
	while( iter++ < maxIter ) {
		if ( sys_inb(STAT_REG, &stat) != OK ) {
			printf("mouse_read() -> FAILED sys_inb()");
			return -1;
		}
		/* loop while 8042 output buffer is empty */
		if( (stat & STAT_OBF) && (stat & STAT_AUX) ) {
			if ( sys_inb(OUT_BUF, &data) != OK ) {
				printf("mouse_read() -> FAILED sys_inb()");
				return -1;	// Returns -1 or 0xFF on failure
			}
			if ( (stat & (STAT_PARITY | STAT_TIMEOUT)) == 0 )	// Error Occurred ?
				return data;
			else
				return -1;	// Returns -1 or 0xFF on failure
		}
	tickdelay(micros_to_ticks(KBD_DELAY_US));
	}

	printf("mouse_read() -> Error: Max Iterations Reached. Was %d.\n", iter);
	return -1;
}

int mouse_handler()
{
	if ( print_packet(packet, PACKET_N_ELEMENTS) != OK ) {
		printf("mouse_handler() -> FAILED print_packet()");
		return 1;
	}
}


int print_packet (unsigned char * packet, int nElements)
{
	//Imprimir como mostrado no guiao do lab o packet
}
