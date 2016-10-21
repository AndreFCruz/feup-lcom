#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 Keyboard Controller.
 */

#define BIT(n) (0x01<<(n))

#define KBC_IRQ                 1   /**< @brief KBC IRQ line */
#define KBD_ORIGINAL_HOOK_ID    1   /**< @brief KBD HOOK_ID ORIGINAL */
#define DELAY_TO            20000   /**< @brief KBC respond Time-Out */

#define ESQ_BREAK_CODE      0x81    /**< @brief ESQ key break code */

#define TIMER0_IRQ	        0    /**< @brief Timer 0 IRQ line */

#define TIMER0_IRQSET		0	 /**< @brief Timer 0 Policy Bit */

/* I/O port addresses */

#define TIMER_0			0x40 /**< @brief Timer 0 count register */
#define TIMER_CTRL		0x43 /**< @brief Control register */

// INPUT_BUFFER is accessed by ports 0x60 or 0x64, depending on context
#define OUT_BUF         0x60 /**< @brief KBC Output buffer register */
#define STAT_REG        0x64 /**< @brief KBC Status register */
#define KBC_CMD_REG     0x64 /**< @brief KBC Command register */

#define SPEAKER_CTRL	0x61 /**< @brief Register for speaker control  */

/* KBD Commands for PC-AT */

#define READ_CMD_B      0x20 /**< @brief Read Command Byte */
#define WRITE_CMD_B     0x60 /**< @brief Write Command Byte */
#define CHECK_KBC       0xAA /**< @brief KBC Self-Test */
#define CHECK_KBD_IFC   0xAB /**< @brief Check Keyboard Interface */
#define DISABLE_KBD_IFC 0xAD /**< @brief Disable KBD Interface */
#define ENABLE_KBD_IFC  0xAE /**< @brief Enable KBD Interface */
