#ifndef __SERIAL_H
#define __SERIAL_H

/** @defgroup Serial Port
 * @{
 *
 * Functions for using the Serial Port
 */

/* Useful Macros for the Serial Port */

#define BIT(n) (0x01<<(n))
#define OK		0

#define COM1_IRQ 4
#define COM2_IRQ 3

#define COM1_PORT 0x3F8
#define COM2_PORT 0x2F8

#define SERIAL_INITIAL_HOOK_ID	4


/* UART Acessible (8-bit) Registers */

#define RBR		0x00	/**< @brief Receiver Buffer Register - Read */
#define THR		0x00	/**< @brief Transmitter Holding Register - Write */
#define IER		0x01	/**< @brief Interrupt Enable Register - Read/ Write */
#define IIR		0x02	/**< @brief Interrupt Indentification Reg. - Read */
#define FCR		0x02	/**< @brief FIFO Control Register - Write */
#define LCR		0x03	/**< @brief Line Control Register - Read/ Write */
#define MCR		0x04	/**< @brief Modem Control Register - Read/ Write */
#define LSR		0x05	/**< @brief Line Status Register - Read */
#define MSR		0x06	/**< @brief Modem Status Register - Read */
#define SR		0x07	/**< @brief Scratchpad Register - Read/ Write */

/* Adresses 0 and 1 are overloaded - access depends of bit DLAB of the LCR register */
#define DLL		0x00	/**< @brief Divisor Latch LSB */
#define DLM		0x01	/**< @brief Divisor Latch MSB */

/* Line Control Register (LCR) */

#define SERIAL_BIT_RATE		19200 // Good Value?
#define SERIAL_BASE_BR		115200

#define LCR_DLAB	BIT(7)	/**< @brief Divisor Latch Access */
#define LCR_BC		BIT(6)	/**< @brief Break Control */
#define LCR_PC2		BIT(5)	/**< @brief Parity Control */
#define LCR_PC1		BIT(4)	/**< @brief Parity Control */
#define LCR_PC0		BIT(3)	/**< @brief Parity Control */
#define LCR_SB		BIT(2)	/**< @brief Stop Bit */
#define LCR_BPC1	BIT(1)	/**< @brief Number of bits per char */
#define LCR_BPC0	BIT(0)	/**< @brief Number of bits per char */

/* Line Status Register (LSR) */

#define LSR_FIFO_E	BIT(7)	/**< @brief FIFO with Errors */
#define LSR_TER		BIT(6)	/**< @brief Transmitter Empty Register */
#define LSR_THRE	BIT(5)	/**< @brief Transmitter Holding Register Empty */
#define LSR_BI		BIT(4)	/**< @brief Break Interrupt */
#define LSR_FE		BIT(3)	/**< @brief Framing Error */
#define LSR_PE		BIT(2)	/**< @brief Parity Error */
#define LSR_OE		BIT(1)	/**< @brief Overrun Error */
#define LSR_RD		BIT(0)	/**< @brief Receiver is Ready */

/* Interrupt Enable Register (IER) - Enables certain Interrupts*/

#define IER_MODEM	BIT(3)	/**< @brief Enables the MODEM Status Interrupt */
#define IER_RLS		BIT(2)	/**< @brief Enables the Receiver Line Status Interrupt */
#define IER_THRE	BIT(1)	/**< @brief Enables the Transmitter Holding Register Empty Interrupt */
#define IER_RDA		BIT(0)	/**< @brief Enables the REceived Data Available Interrupt */

/* Interrupt Identification Register (IIR) */

#define IIR_IP2		BIT(3)	/**< @brief Pending Interrupts Information */
#define IIR_IP1		BIT(2)	/**< @brief Pending Interrupts Information */
#define IIR_IP0		BIT(1)	/**< @brief Pending Interrupts Information */
#define IIR_NPI		BIT(0)	/**< @brief Non Pending Interrupts */

/* UART FIFO Control Register */

#define FIFO_RCVR1	BIT(7)	/**< @brief RCVR FIFO Trigger Level (Bytes) (for 16 byte FIFOs)
 */
#define FIFO_RCVR2	BIT(6)	/**< @brief RCVR FIFO Trigger Level (Bytes) (for 16 byte FIFOs)
 */
#define FIFO_EN64	BIT(5)	/**< @brief Enable 64 byte FIFO */
#define FIFO_CX		BIT(2)	/**< @brief Clear bytes in XMIT FIFO */
#define FIFO_CR		BIT(1)	/**< @brief Clear bytes in RCVR FIFO */
#define FIFO_EN		BIT(0)	/**< @brief Enable both FIFO's */

/**
 * @brief Subscribes and enables Serial Port interrupts
 *
 * @return Returns bit order in interrupt mask; negative value on failure
 */
int serial_subscribe_int(void);

/**
 * @brief Unsubscribes Serial Port interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int serial_unsubscribe_int(void);

/**
 * @brief Enables Interrupt Mode for the Serial Port
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int serial_enable_interrupts();

/**
 * @brief Disables Interruptions in the Serial Port
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int serial_disable_interrupts();

/**
 * @brief Sets the desired configuration of the UART registers
 *
 * This function should be called in both serial port ends, and any transmission of information
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int serial_set_conf();

/**
 * @brief Reads information received using the serial port
 *
 * @return unsigned char read from the Receiver Buffer
 */
unsigned char serial_read();

/**
 * @brief Sends information using the serial port
 *
 * @param info information going to be send
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int serial_write(unsigned char info);

/**@}*/

#endif /* __SERIAL_H */
