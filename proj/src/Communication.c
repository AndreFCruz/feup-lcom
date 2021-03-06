#include "Serial.h"
#include "Communication.h"
#include <minix/syslib.h>


// Current state of communication
static serial_state_t comState = NONE;
static int flag = 0;
static int first = 0;

void serial_handler() {
	
	// Check type of interrupt
	/*
	unsigned long iir = 0;
	sys_inb(COM1_PORT + IIR, &iir);
	if ( iir & ~IIR_NPI ) {
		switch ( iir & IIR_ID ) {
		case IIR_RX:
			printf("Serial Interrupt: Received!!\n");
			break;
		default:
			printf("** RECEIVED UNINTENDED INTERRUPT ?? **\n");
			return;
		}
	}*/
	//if (comState == NONE)

	unsigned long iir = 0;
	unsigned char proceed = 0;

	sys_inb(COM1_PORT + IIR, &iir);

	if ( iir & IIR_NPI ) {

		//This should not happen
		printf("Serial Handler Interrupt error: No pending interrupts!\n");
		return;

	} else {

		if ( (iir & IIR_LSR) == IIR_LSR) {
			unsigned long error;
			sys_inb(COM1_PORT + LSR, &error);
			printf("RECEIVED ERROR INTERRUPT. It was: 0x%x.\n", (unsigned char) error & 0xff);
		}

		if ( (iir & IIR_RX) == IIR_RX) {
			proceed = 1;
			printf("Serial Interrupt: Received!!\n");
		}
	}

	if (!proceed)
		return;

	unsigned char received = serial_read();

	printf("-SH- State: %x. Received: %x.\n", (int) comState, received);

	switch (comState) {
	case MP_WAITING:
		if ( MP_WAITING == received ) {
			serial_write(MP_ONGOING);
			comState = MP_ONGOING;
		} else if ( MP_ONGOING == received ) {
			serial_write(MP_ACK);
			comState = MP_ONGOING;
		} else {
			printf("*serial handler-NOT cool* ");
		}
		break;
	case MP_ONGOING:
		if ( MP_ENDED == received ) {
			serial_write(MP_ACK);
			comState = MP_ENDED;
		} else if ( MP_ACK == received ) {
			printf("*serial handler-COOL* ");
		} else {
			printf("*serial handler-NOT cool?* ");
		}
		break;
	case MP_ENDED:
		printf("** SH - is this fine? **\n");
		break;
	case NONE:
		printf("** SH - received com while out of MP **\n");
		break;
	default:
			printf("\n** SH - SHOULD NOT BE POSSIBLE **\n");
		}
}

void setComState(serial_state_t state) {
	printf("SetComState called. State: %x.\n", (char) state);

	// appropriately write to serial on state change
	comState = state;
	serial_write((char)state);
}

serial_state_t getComState() {
	return comState;
}

int getflag() {
	return flag;
}
