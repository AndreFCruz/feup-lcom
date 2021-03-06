#include <minix/syslib.h>
#include <minix/drivers.h>

#include "Serial.h"

static int serial_hook_id = SERIAL_INITIAL_HOOK_ID;

int serial_subscribe_int(void) {
	if (sys_irqsetpolicy(COM1_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE,
			&serial_hook_id) != OK) {
		printf("serial_subscribe_int() -> FAILED sys_irqsetpolicy()\n");
		return -1;
	}
	if (sys_irqenable(&serial_hook_id) != OK) {
		printf("serial_subscribe_int() -> FAILED sys_irqenable()\n");
		return -1;
	}

	return SERIAL_INITIAL_HOOK_ID;
}

int serial_unsubscribe_int(void) {
	if (sys_irqdisable(&serial_hook_id) != OK) {
		printf("serial_unsubscribe_int() -> FAILED sys_irqdisable()\n");
		return -1;
	}
	if (sys_irqrmpolicy(&serial_hook_id) != OK) {
		printf("serial_unsubscribe_int() -> FAILED sys_irqrmpolicy()\n");
		return -1;
	}

	return SERIAL_INITIAL_HOOK_ID;
}

/* Interrupt Handling */

/**
 * "The use of bits 0 and 1 should be obvious: if set, the UART will generate an interrupt whenever
 * a character is received and whenever it is ready to accept a new character for transmission, respectively."
 */
int serial_enable_interrupts() {

	unsigned long helper_IER = 0;

	if (sys_inb((COM1_PORT + IER), &helper_IER) != OK) {
		printf("serial_enable_interrupt -> Failed sys_inb.\n");
		return 1;
	}

	//Setting Bit 0 and 1 of the IIR
	helper_IER = helper_IER | (IER_RDA | IER_RLS);

	if (sys_outb((COM1_PORT + IER), helper_IER) != OK) {
		printf("serial_enable_interrupt -> Failed sys_outb.\n");
		return 1;
	}

	tickdelay(micros_to_ticks(10000));

	return OK;
}

//Opposite purpose of serial_enable_interrupt
int serial_disable_interrupts() {

	unsigned long helper_IER = 0;

	if (sys_inb((COM1_PORT + IER), &helper_IER) != OK) {
		printf("serial_disable_interrupt -> Failed sys_inb.\n");
		return 1;
	}

	//Changing Bit 0 and 1 of the IIR to 0.
	helper_IER = helper_IER & (~IER_RDA) ;
	helper_IER = helper_IER & (~IER_RLS) ;

	if (sys_outb((COM1_PORT + IER), helper_IER) != OK) {
		printf("serial_disable_interrupt -> Failed sys_outb.\n");
		return 1;
	}

	tickdelay(micros_to_ticks(10000));

	return OK;
}

/**
 * " (..)both ends of the serial line must agree on the communication parameters.
 * In the case of asynchronous communication these include the bit rate,
 * the number of bits per character, the number of stop bits and the parity.
 */
int serial_set_conf() {

	unsigned long configuration = 0;

	if (serial_disable_interrupts() != OK) {
		printf("FAILED serial_enable_interrupts()\n");
		return 1;
	}

	//Fetching LCR
	if (sys_inb((COM1_PORT + LCR), &configuration) != OK) {
		printf(" serial_set_conf -> Failed sys_inb for configuration.\n");
		return 1;
	}

	//Needed (?)
	configuration &= 0x40;
	configuration |= 0x80;

	//Setting Word Length - 8 bits
	configuration = configuration | (LCR_BPC0 | LCR_BPC1);

	//Setting Number of Stop Bits - 1 bit
	configuration = configuration & (~LCR_SB);
	//configuration |= 0;

	//Setting Partiy
	configuration = configuration & (~LCR_PC0);
	//configuration |= 0;

	//Updating Configuration
	if (sys_outb((COM1_PORT + LCR), configuration) != OK) {
		printf(" serial_set_conf -> Failed sys_outb for configuration.\n");
		return 1;
	}

	//Setting the bit-rate frequency dividor
	unsigned long bit_rate = SERIAL_BASE_BR / SERIAL_BIT_RATE;
	char msb = (bit_rate >> 8) & 0xFF;
	char lsb = bit_rate & 0xFF;

	//Setting DLAB to 1
	unsigned long helper_DLAB = 0;

	if (sys_inb((COM1_PORT + LCR), &helper_DLAB) != OK) {
		printf(" serial_set_conf -> Failed sys_inb for DLAB.\n");
		return 1;
	}

	helper_DLAB = helper_DLAB | LCR_DLAB;

	if (sys_outb((COM1_PORT + LCR), helper_DLAB) != OK) {
		printf(" serial_set_conf -> Failed sys_outb for DLAB.\n");
		return 1;
	}

	//Writing MSB to DLM register
	if (sys_outb((COM1_PORT + DLM), msb) != OK) {
		printf(" serial_set_conf -> Failed sys_outb for DLM.\n");
		return 1;
	}

	//Writing LSB to DLL register
	if (sys_outb((COM1_PORT + DLL), lsb) != OK) {
		printf(" serial_set_conf -> Failed sys_outb for DLL.\n");
		return 1;
	}

	//Re setting the DLAB register to 0
	helper_DLAB ^= LCR_DLAB;

	if (sys_outb((COM1_PORT + LCR), helper_DLAB) != OK) {
		printf(" serial_set_conf -> Failed sys_outb for DLAB.\n");
		return 1;
	}

	return OK;
}

unsigned char serial_read() {
	unsigned long received = 0;
	unsigned long status = 0;

	sys_inb(COM1_PORT + LSR, &status);

	if (status & SERIAL_RECEIVED_DATA) {
		sys_inb(COM1_PORT + RBR, &received);
		return (unsigned char) (received & 0xFF);
	}

	return 0;
}

int serial_write(unsigned char info) {
	printf("SerialWrite: %x\n", info);

	// Transmitter Holding Register is empty ?
	unsigned long received = 0;
	do {
		sys_inb(COM1_PORT + LSR, &received);
		printf("_-_");
	} while( ! (received & SERIAL_THR_EMPTY) );

	if (sys_outb((COM1_PORT + THR), info) != OK) {
		printf(" serial_write -> Failed sys_outb of information.\n");
		return 1;
	}

	tickdelay(micros_to_ticks(10000));

	return OK;
}
