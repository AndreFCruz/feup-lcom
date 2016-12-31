#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <minix/sysutil.h>
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
	helper_IER = helper_IER | IER_RDA | IER_THRE;

	if (sys_outb((COM1_PORT + IER), helper_IER) != OK) {
		printf("serial_enable_interrupt -> Failed sys_outb.\n");
		return 1;
	}

	//TODO: Tickdelay no keyboard mas com timer

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
	helper_IER = helper_IER & (~IER_RDA) & (~IER_THRE);

	if (sys_outb((COM1_PORT + IER), helper_IER) != OK) {
		printf("serial_disable_interrupt -> Failed sys_outb.\n");
		return 1;
	}

	//TODO: tickdelay como no keyboard mas com timer

	return OK;
}

/**
 * " (..)both ends of the serial line must agree on the communication parameters.
 * In the case of asynchronous communication these include the bit rate,
 * the number of bits per character, the number of stop bits and the parity.
 */
int serial_set_conf() {

	unsigned long configuration = 0;

	//Setting the bit-rate
	unsigned long bit_rate = SERIAL_BASE_BR / SERIAL_BIT_RATE;
	char lsb = bit_rate & 0xFF;
	char msb = (bit_rate << 8) & 0xFF;

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
	helper_DLAB = helper_DLAB & (~LCR_DLAB);

	if (sys_outb((COM1_PORT + LCR), helper_DLAB) != OK) {
		printf(" serial_set_conf -> Failed sys_outb for DLAB.\n");
		return 1;
	}

	//Fetching LCR
	if (sys_inb((COM1_PORT + LCR), &configuration) != OK) {
		printf(" serial_set_conf -> Failed sys_inb for configuration.\n");
		return 1;
	}

	//Setting Word Length - 5 bits
	configuration = configuration & (~LCR_BPC0) & (~LCR_BPC1);

	//Setting Number of Stop Bits - 1 bit
	configuration = configuration & (~LCR_SB);

	//Setting Partiy
	configuration = configuration & (~LCR_PC0);

	//Updating Configuration
	if (sys_outb((COM1_PORT + LCR), configuration) != OK) {
		printf(" serial_set_conf -> Failed sys_outb for configuration.\n");
		return 1;
	}

	return OK;
}

//Polled mode
//unsigned char serial_read() {
//	unsigned long helper_LSR = 0;
//
//	if (sys_inb((COM1_PORT + LSR), &helper_LSR) != OK) {
//		printf(" serial_read -> Failed sys_inb for Line Status Register.\n");
//		return 0;
//	}
//
//	//If a character was received and put on the Receiver Buffer, LSR_RD is set
//	if (helper_LSR & LSR_RD) {
//
//		unsigned long received_char = 0;
//
//		//Reading character put on the Receiver Buffer
//		if (sys_inb((COM1_PORT + RBR), &received_char) != OK) {
//			printf(" serial_read -> Failed sys_inb for Received character.\n");
//			return 0;
//		}
//
//		return received_char; //TODO: cast automatico e como ta unsigned n ha problema?
//	}
//
//	return 0;
//}

//Interrupt mode?
unsigned char serial_read() {
	unsigned long received_char = 0;

	//Reading character put on the Receiver Buffer
	if (sys_inb((COM1_PORT + RBR), &received_char) != OK) {
		printf(" serial_read -> Failed sys_inb for Received character.\n");
		return 0;
	}

	return received_char; //TODO: cast automatico e como ta unsigned n ha problema?
}

//Polled mode?
//void serial_write(unsigned char info) {
//	long unsigned empty_flag = 0;
//	int flag = 0;
//
//// Process will only start if THRE is empty (set to 1)
//	while (flag) {
//
//		if (sys_inb((COM1_PORT + LSR), &empty_flag) != OK)
//			printf(
//					" serial_write -> Failed sys_inb for Transmitter Register.\n");
//
//		if (empty_flag & LSR_THRE)
//			flag = 0; //Cycle will stop -> THRE is set to 1
//		else {
//			flag = 1;
//			empty_flag = 0;
//		}
//	}
//
////Sending the information
//	if (sys_outb((COM1_PORT + THR), info) != OK) {
//		printf(" serial_write -> Failed sys_outb of information.\n");
//	}
//
////Wait for message to get his destination with timer
////TODO: timer.wait call
//
//}

int serial_write(unsigned char info) {
	if (sys_outb((COM1_PORT + THR), info) != OK) {
		printf(" serial_write -> Failed sys_outb of information.\n");
		return 1;
	}

	//TODO: Tickdelay no keyboard mas com timer

	return OK;
}

/* Passing information associated functions */

#define GAME_READY		BIT(0)
#define GAME_OVER		BIT(1)
//TODO : Isto nao necessita de estar aqui? E fora ver o que se recebe a que macro e igual.
////::::::::::::::::::::::::::::::::

//
///* Interrupt Handling - TODO: Check Transaltion made with macros*/
//// Not finished because dont know wut case stuff means
//
//void ser_ih() {
//	sys_inb(ser_port + IIR, &iir);
//if( iir & ~IIR_NPI) {
//	switch( iir & IIR_IP2 & IIR_IP1 & IIR_IP0) {
//		case SER_RX_INT:
//			... /* read received character */
//		case SER_TX_INT:
//			... /* put character to sent */
//		case SER_RX_ERR:
//			... /* notify upper level */
//		case SER_XXXX:
//			... /* depends on XXX */
//		}
//	}
//}
