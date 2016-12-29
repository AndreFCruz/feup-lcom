#include "Serial.h"
#include <minix/syslib.h>

static int serial_hook_id = SERIAL_INITIAL_HOOK_ID;

int serial_subscribe_int(void)
{
	if ( sys_irqsetpolicy (COM1_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &serial_hook_id) != OK ) {
		printf("serial_subscribe_int() -> FAILED sys_irqsetpolicy()\n");
		return -1;
	}
	if ( sys_irqenable (&serial_hook_id) != OK ) {
		printf("serial_subscribe_int() -> FAILED sys_irqenable()\n");
		return -1;
	}

	return SERIAL_INITIAL_HOOK_ID;
}

int serial_unsubscribe_int(void)
{
	if ( sys_irqdisable (&serial_hook_id) != OK ) {
		printf("serial_unsubscribe_int() -> FAILED sys_irqdisable()\n");
		return -1;
	}
	if ( sys_irqrmpolicy (&serial_hook_id) != OK ) {
		printf("serial_unsubscribe_int() -> FAILED sys_irqrmpolicy()\n");
		return -1;
	}

	return SERIAL_INITIAL_HOOK_ID;
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
///* Polled Operation: Transimission */
//
///* no error checking */
//...
///* busy wait for transmitter ready */
//while( !(lsr & LSR_THRE) {
//	ticksdelay();
//	sys_inb(ser_port + LSR, &lsr);
//}
///* send character */
//sys_outb(ser_port + RBR, c);
//
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
//
////Atençao usar char var ou usar uma queue para receber a info? Acho que da a char,
////pois a informaçao passada e so o score
//
////Polled operation: Need to disable the interrupts in the UART
////Interrupt operation: Need to subscribe the UART interrupts inexclusive mode
//
//To ensure that this device driver does not interfere with your code, you can disable all interrupts from the UART,
//as long as your code does not use interrupts. If it does, you have to invoke sys_irqsetpolicy() not
////only with IRQ_REENABLE policy, but also with the IRQ_EXCLUSIVE policy set, as done in previous labs.
