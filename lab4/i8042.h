#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 Keyboard Controller.
 */

#define BIT(n) (0x01<<(n))

#define MOUSE_IRQ				12	/**< @brief KBC IRQ line */
#define MOUSE_INITIAL_HOOK_ID	12	/**< @brief KBD Initial hook_id */

#define PACKET_NELEMENTS		3	/**< @brief Number of Elements in every mouse packet */

#define KBD_IN_BUF		0x64	/**< @brief KBC Input buffer register */
#define OUT_BUF			0x60	/**< @brief KBC Output buffer register */
#define MOUSE_IN_BUF	0x60	/**< @brief MOUSE Input buffer register */
#define STAT_REG		0x64	/**< @brief KBC Status register */

/* KBC Status Register BITs' Meaning*/

#define STAT_PARITY		BIT(7)
#define STAT_TIMEOUT	BIT(6)
#define STAT_AUX		BIT(5)
#define STAT_INH		BIT(4)
#define STAT_A2			BIT(3)
#define STAT_SYS		BIT(2)
#define STAT_IBF		BIT(1)
#define STAT_OBF		BIT(0)

/* PS/2 Mouse-Related KBC Commands*/

#define READ_CMD_B			0x20	/**< @brief Read Command Byte */
#define WRITE_CMD_B			0x60	/**< @brief Write Command Byte */
#define DISABLE_MOUSE		0xA7
#define ENABLE_MOUSE		0xA8
#define CHECK_MOUSE_ITF		0XA9	/**< @brief Check Mouse Interface */
#define WRITE_B_MOUSE		0xD4	/**< @brief Write Byte to Mouse */

/* PS/2 Mouse Commands passed as arguments of 0xD4*/

#define MOUSE_RESET			0XFF	/**< @brief Mouse reset */
#define MOUSE_RESEND		0XFE	/**< @brief For serial communications errors */
#define SET_DEFAULTS		0xF6	/**< @brief Set default values */
#define DISABLE_DATA_R		0xF5	/**< @brief Disable Data Reporting (In stream mode, should be sent before any other command) */
#define ENABLE_DATA_R		0xF4	/**< @brief Enable Data Reporting (In stream mode) */
#define SET_SAMPLE_RATE		0xF3	/**< @brief Sets state sampling rate */
#define SET_REMOTE_MODE		0XF0	/**< @brief Send data on request only */
#define READ_DATA			0xEB	/**< @brief Send data packet request */
#define SET_STREAM_MODE		0xEA	/**< @brief Send data on events */
#define STATUS_REQUEST		0xE9	/**< @brief Get mouse configuration (3 bytes) */
#define SET_RESOLUTION		0XE8
#define SET_SCALING_2_1		0xE7	/**< @brief Aceleration mode */
#define SET_SCALING_1_1		0xE6	/**< @brief Linear mode */

/* PS/2 Mouse Packet Meanings */

#define BYTE0_Y_OVF			BIT(7)
#define BYTE0_X_OVF			BIT(6)
#define BYTE0_Y_SIGN		BIT(5)
#define BYTE0_X_SIGN		BIT(4)
#define BYTE0_SYNC_BIT		BIT(3)	/** <@brief Bit that helps to synchronize the packet */
#define BYTE0_MB			BIT(2)
#define BYTE0_RB			BIT(1)
#define BYTE0_LB			BIT(0)

/* Mouse Controller Responses to Commands on 0xD4 */

#define RESPONSE_ACK		0xFA	/**< @brief Everything OK */
#define RESPONSE_NACK		0xFE	/**< @brief  invalid byte (may be because of a serial communication error) */
#define REPONSE_ERROR		0XFC	/**< @brief second consecutive invalid byte */

#define KBD_DELAY_US		20000   // KBC respond Time-Out in micro seconds

/**@}*/

#endif /* _LCOM_I8042_H */
