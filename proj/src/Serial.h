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

#define RBR		0	/**< @brief Receiver Buffer Register - Read */
#define THR		0	/**< @brief Transmitter Holding Register - Write */
#define IER		1	/**< @brief Interrupt Enable Register - Read/ Write */
#define IIR		2	/**< @brief Interrupt Indentification Reg. - Read */
#define FCR		2	/**< @brief FIFO Control Register - Write */
#define LCR		3	/**< @brief Line Control Register - Read/ Write */
#define MCR		4	/**< @brief Modem Control Register - Read/ Write */
#define LSR		5	/**< @brief Line Status Register - Read */
#define MSR		6	/**< @brief Modem Status Register - Read */
#define SR		7	/**< @brief Scratchpad Register - Read/ Write */

/* Adresses 0 and 1 are overloaded - access depends of bit DLAB of the LCR register */
#define DLL		0	/**< @brief Divisor Latch LSB */
#define DLM		1	/**< @brief Divisor Latch MSB */

/* Line Control Register (LCR) */

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
#define LSR_RD		BIT(0)	/**< @brief Data for receiving */

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

/**@}*/

#endif /* __SERIAL_H */
