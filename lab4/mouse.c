#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <minix/sysutil.h>
#include "mouse.h"
#include "i8042.h"

#define HORIZONTAL_TOLERANCE 30

static int mouse_hook_id = MOUSE_INITIAL_HOOK_ID;
static const unsigned maxIter = 50;         // Maximum iterations/tries when retrieving data


#define HELPER_NEG 0xFFFFFF00
int int_value (unsigned char delta_var, int sign)
{
	if (sign != 0)
		return HELPER_NEG | delta_var;
	else
		return (unsigned int) delta_var;
}


int mouse_write_cmd (char cmd)
{
	unsigned long stat;
	unsigned iter = 0;
	while ( iter++ < maxIter ) {
		if ( sys_inb (STAT_REG, &stat) != OK ) {
			printf("mouse_write_cmd() -> FAILED sys_inb()");
			return 1;
		}
		/* loop while 8042 input buffer is full */
		if ( (stat & STAT_IBF) == 0 ) {
			if ( sys_outb (KBC_CMD_REG, WRITE_B_MOUSE) != OK ) {
				printf("mouse_write_cmd() -> FAILED 1st sys_outb()\n");
           		return 1; 	//Failure
           		}
			if ( sys_outb(KBC_IN_BUF, cmd) != OK ) {
				printf("mouse_write_cmd() -> FAILED 2nd sys_outb()\n");
				return 1;
				}
			stat = mouse_read();
			if ( stat == IN_ACK ) {
				return OK;
			}
		}
	tickdelay(micros_to_ticks(DELAY_US));
	}

	printf("mous_write_cmd() -> Max Iterations Reached. Was %d.\n", iter);
	return 1;
}


int mouse_subscribe_int()
{
	if ( sys_irqsetpolicy (MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id) != OK ) {
			printf("mouse_subscribe_int() -> FAILED sys_irqsetpolicy()\n");
			return -1;
	}
	if ( sys_irqenable (&mouse_hook_id) != OK ) {
			printf("mouse_subscribe_int() -> FAILED sys_irqenable()\n");
			return -1;
	}

	// Enable stream mode
	mouse_write_cmd(SET_STREAM_MODE);
	mouse_write_cmd(ENABLE_DATA_R);

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
//	mouse_write_cmd(DISABLE_DATA_R);	// Just don't

	return MOUSE_INITIAL_HOOK_ID;
}

// NOTA: Movimentos bruscos do rato des-sincronizam o rato
int mouse_synchronize()	// Synchronizes with FIRST byte, returns data read
{
	int data;
	unsigned iter = 0;

	while (iter++ < maxIter) {
		if ( ((data = mouse_read()) & BYTE0_SYNC_BIT) != 0 )
			return data;
		printf("SyncData: %X\n", data);
	}

	printf("mouse_synchronize() -> Max Iterations Reached. Was %d.\n", iter);
	return -1;
}


int mouse_read()	// Reads Mouse Data from OutPut Buffer
{
	unsigned long stat, data;
	unsigned iter = 0;
	while( ++iter < (maxIter * 10) ) {	// sometimes requires hundreds of iterations...
		if ( sys_inb(STAT_REG, &stat) != OK ) {
			printf("mouse_read() -> FAILED sys_inb()\n");
			return -1;
		}
		/* loop while 8042 output buffer is empty */
		if( (stat & (STAT_OBF | STAT_AUX )) != 0 ) {
//		if ( ((stat & STAT_OBF) != 0) && ((stat & STAT_AUX) != 0) ) {
			if ( sys_inb(OUT_BUF, &data) != OK ) {
				printf("mouse_read() -> FAILED sys_inb()\n");
				return -1;	// Returns -1 or 0xFF on failure
			}
			return data;
		}
	tickdelay(micros_to_ticks(DELAY_US));
	}

	printf("mouse_read() -> Error: Max Iterations Reached. Was %d.\n", iter);
	return -1;
}

int mouse_handler (unsigned char * packet, unsigned short * counter)
{
	if (*counter == 0) {
		int sync_result = mouse_synchronize();	// Sync packet if expecting first byte

		if (sync_result == -1) {
			printf("mouse_handler() -> FAILED mouse_synchronize()\n");
			return 1;
		}
		packet[(*counter)++] = sync_result;
		return OK;
	}
	else
		packet[(*counter)++] = mouse_read();

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
	printf("X=%3d ", int_value(packet[1], packet[0] & BYTE0_X_SIGN));
	printf("Y=%3d\n", int_value(packet[2], packet[0] & BYTE0_Y_SIGN));	// TODO Perguntar se se imprime o valor raw de X,Y ou se tem em conta o sinal
}


int mouse_fetch_config(unsigned char * config)
{
	// TODO NECESSARY; BUT WHY ??
	unsigned char dummy;
	sys_inb(KBC_CMD_REG, (unsigned long *) &dummy);	// Clear input buffer

	if ( mouse_write_cmd(STATUS_REQUEST) != OK ) {
		printf("fetch_mouse_config::mouse_write_cmd FAILED\n");
		return 1;
	}

	int i;
	for (i = 0; i < 3; i++) {
		config[i] = mouse_read();
	}

	return OK;
}


//Make function that updates the type correctly?
void event_update (event_t * evt, const unsigned char *packet, short length)
{
	printf ("X Sign: %d. Y Sign: %d. ", packet[0] & BYTE0_X_SIGN ? 1 : 0, packet[0] & BYTE0_Y_SIGN ? 1 : 0);

	evt->x_delta += int_value(packet[1], packet[0] & BYTE0_X_SIGN);
	evt->y_delta += int_value(packet[2], packet[0] & BYTE0_Y_SIGN);

	if ((packet[0] & BYTE0_RB) == 0) {		//Ordem dos IF's e t importante! Se n for esta ordem julgo n dar
		evt->type = RUP;
		printf("RUB              ");
	}
	else if (evt->x_delta > HORIZONTAL_TOLERANCE || evt->x_delta < -HORIZONTAL_TOLERANCE) {
		evt->type = HORZ_TOL_BREACHED;
		printf("HOR_TOL_BREACHED ");
	}
	else if (evt->y_delta > length || evt->y_delta < -length) {
		evt->type = VERTLINE;
		printf("VERTICAL_LINE    ");
	}
	else if (packet[0] & BYTE0_RB) {
		evt->type = RDOWN;
		printf("RDOWN            ");
	}
}

void check_ver_line(event_t * evt, const unsigned char *packet, short length) {
	static state_t st = INIT; // initial state; keep state

	event_update (evt, packet, length);

	switch (st) {
	case INIT:
		if( evt->type == RDOWN )
		st = DRAW;
		evt->y_delta = 0;
		evt->x_delta = 0;
		break;
	case DRAW:
		if( evt->type == HORZ_TOL_BREACHED ) {
			evt->y_delta = 0;
			evt->x_delta = 0;
		} else if (evt->type == VERTLINE) {
			evt->complete_flag = 1;	// Useless, == vertline
		} else if ( evt->type == RUP ) {
			st = INIT;
			evt->y_delta = 0;
			evt->x_delta = 0;
		}
		break;
	default:
		printf("* ");
		break;
	}
}
