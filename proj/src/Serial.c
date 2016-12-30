#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <minix/sysutil.h>
//#include <minix/com.h>
//#include <minix/sysutil.h>
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

// If BIT(0) - 1 esta pronto a correr
// If BIT(1) - 1 acabou de correr

void serial_write(unsigned char info) {
	unsigned empty_flag = 0;
	int flag = 0;

	// Process will only start if THRE is empty (set to 1)
	while (flag) {

		if (sys_inb((long unsigned int *)(COM1_PORT + LSR, &empty_flag)) != OK)
			printf(
					" serial_wirte -> Failed sys_inb for Transmitter Register.\n");

		if (empty_flag & LSR_THRE)
			flag = 0; //Cycle will stop -> THRE is set to 1
		else
			flag = 1;
	}

	//Sendingo the information
	if (sys_outb(COM1_PORT + THR, info) != OK) {
		printf(" serial_write -> Failed sys_outb of information.\n");
	}

	//Wait for message to get his destination with timer
	//TODO: timer.wait call

}

///* LSR and Error Detection */
//
//if( sys_inb(ser_port + LSR, &st) != OK ) {
//	...
//} else if {
//	if ( st & (LSR_OE | LSR_PE
//	| LSR_FE) {
//		... // Handle error
//	}
//}
//
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
//
////::::::::::::::::::::::::::::::::
//
////Using the FIFO's
//
//sys_outb(ser_port + FCR, 0x??); // Enable FIFOs
//
//sys_inb(ser_port + IIR, &iir); // Check FIFO state
//
//void ser_ih() { // serial port IH
//	...
//	while( lsr & SER_RX_RDY ) { // Read all characters in FIFO
//	... // check errors
//	sys_inb(ser_port + RBR(?), &c);
//	... // "process" character read
//	sys_inb(ser_port + LSR, &lsr);
//	}
