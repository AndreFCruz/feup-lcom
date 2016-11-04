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

//Ve isto Andre, eu nao usei no meu codigo, mas acabei por faze-la ao fazer tb o mouse write. Antes desta e ncesseario a
//chamada do bcd_write c o comando WRITE_B_MOUSE . Confirma se esta certa.. Eu n a testei, mas deve tar quase certa, se n certa!!
//Podes sp apaga la caso seja inutil ou mal feita xD

int mouse_synchronize(void)
{
	int data = mouse_read();
	unsigned iter = 0;

	while (iter++ < maxIter)
	{
		if (data & BYTE1_SYNC_BIT)		//Bit 3 of BYTE1 is always set to 1
			return data;
		else
			data = mouse_read();
	}

	printf("mouse_synchronize() -> Max Iterations Reached. Was %d.\n", iter);
	return -1;
}

int mouse_write (char data)		//Writes Data to the Keyboard Input Buffer
{
	unsigned long stat;
	unsigned iter = 0;
	while ( iter++ < maxIter ) {
		if ( sys_inb (STAT_REG, &stat) != OK ) {
			printf("mouse_write() -> FAILED sys_inb()\n");
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
			printf("mouse_read() -> FAILED sys_inb()\n");
			return -1;
		}
		/* loop while 8042 output buffer is empty */
		if( (stat & STAT_OBF) && (stat & STAT_AUX) ) {
			if ( sys_inb(OUT_BUF, &data) != OK ) {
				printf("mouse_read() -> FAILED sys_inb()\n");
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

int mouse_handler (unsigned char * packet, unsigned short * counter)
{
	if (*counter == PACKET_BYTE1) { //Every time we start a new packet, re-check synchronization
		int sync_result = mouse_synchronize();

		if (sync_result == -1)
		{
			printf("mouse_handler() -> FAILED mouse_synchronize()\n");
			return 1;
		}
		packet[*counter] = sync_result;	//Saving the last value tried on mouse_synchronize() into the packet
		*counter++;
		return 0;
	}

		if (*counter == PACKET_NELEMENTS)
		{
			*counter = PACKET_BYTE1;
			if (print_packet(packet) != OK) {
				printf("mouse_handler() -> FAILED print_packet()\n");
				return 1;
			}
			return 0;
		}

		//Only when *counter == PACKET_BYTE2
		packet[*counter] = mouse_read();
		*counter++;
		return 0;
}


int print_packet (unsigned char * packet)
{
	printf("B1=0x%02x\t", packet[PACKET_BYTE1]);
	printf("B2=0x%02x\t", packet[PACKET_BYTE2]);
	printf("B3=0x%02x\t", packet[PACKET_BYTE3]);
	printf("LB=%d ", packet[PACKET_BYTE1] & BYTE1_LB);
	printf("MB=%d ", packet[PACKET_BYTE1] & BYTE1_MB);
	printf("RB=%d ", packet[PACKET_BYTE1] & BYTE1_RB);
	printf("XOV=%d ", packet[PACKET_BYTE1] & BYTE1_X_OVF);
	printf("YOV=%d ", packet[PACKET_BYTE1] & BYTE1_Y_OVF);
	printf("X=%d\t", packet[PACKET_BYTE2]);
	printf("Y=%d\n", packet[PACKET_BYTE3]);

	return 0;	//Provavlemte posso meter como sendo um return void. Ver se me lembro de possiveis erros...
}
