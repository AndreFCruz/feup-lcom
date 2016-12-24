#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 Keyboard Controller.
 */

#define BIT(n) (0x01<<(n))

#define KBC_IRQ             1   /**< @brief KBC IRQ line */
#define KBD_INITIAL_HOOK_ID 1   /**< @brief KBD Initial hook_id */

#define ESC_BREAK_CODE      0x81    /**< @brief ESC key break code */
#define ENTER_BREAK_CODE	0x9C	/**< @brief Enter key break code */
#define UP_MAKE_CODE		0xE048	/**< @brief Up Arrow key make code */
#define DOWN_MAKE_CODE		0xE050	/**< @brief Down Arrow key make code */

// /* Timer 0 useful macros */
// #define TIMER0_IRQ	        0    /**< @brief Timer 0 IRQ line */
// #define TIMER0_IRQSET		0	 /**< @brief Timer 0 Policy Bit */

// /* I/O port addresses */
//
// #define TIMER_0			0x40 /**< @brief Timer 0 count register */
// #define TIMER_CTRL		0x43 /**< @brief Control register */

// INPUT_BUFFER is accessed by ports 0x60 or 0x64, depending on context
#define OUT_BUF         0x60 /**< @brief KBC Output buffer register */
#define STAT_REG        0x64 /**< @brief KBC Status register */
#define KBC_CMD_REG     0x64 /**< @brief KBC Command register */
#define KBD_IN_BUF      0x60 /**< @brief KBD Input buffer register */

/* KBC Status Register BITs' Meaning*/

#define STAT_PARITY     BIT(7)
#define STAT_TIMEOUT    BIT(6)
#define STAT_AUX        BIT(5)
#define STAT_INH        BIT(4)
#define STAT_A2         BIT(3)
#define STAT_SYS        BIT(2)
#define STAT_IBF        BIT(1)
#define STAT_OBF        BIT(0)

/* KBD Commands for PC-AT */

#define READ_CMD_B      0x20 /**< @brief Read Command Byte */
#define WRITE_CMD_B     0x60 /**< @brief Write Command Byte */
#define CHECK_KBC       0xAA /**< @brief KBC Self-Test */
#define CHECK_KBD_IFC   0xAB /**< @brief Check Keyboard Interface */
#define DISABLE_KBD_IFC 0xAD /**< @brief Disable KBD Interface */
#define ENABLE_KBD_IFC  0xAE /**< @brief Enable KBD Interface */

/* KBD Responses to Inputs*/
#define IN_RESEND       0XFE /**< @brief Error: Resend latest byte  */
#define IN_ERROR        0XFC /**< @brief Error: Restart the entire sequence */
#define IN_ACK			0xFA /**< @brief Success*/

///* LED's useful macros*/
//#define LED_TOGGLE_CMD	0xED /**< @brief Toggle Led Command */
//#define CAPS_LOCK			0x02
//#define NUM_LOCK			0X01
//#define SCROLL_LOCK		0x00

#define KBD_DELAY_US		20000   // KBC respond Time-Out in micro seconds

/**@}*/

#endif /* _LCOM_I8042_H */
