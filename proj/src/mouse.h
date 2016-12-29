#ifndef __MOUSE_H
#define __MOUSE_H

/** @defgroup mouse mouse
 * @{
 *
 * Functions for using the i8042 KBC/KBD
 */

 /**
  * @brief Function to get the signed value of an unsigned value
  *
  * @param Unsigned value that will be processed
  *
  * Sign of the unsigned value
  *
  * @return Return the signed value
  */
 int int_value (unsigned char delta_var, int sign);

 /**
  * @brief Writes a command in the mouse input buffer
  *
  * @param Command that is written in the input buffer
  *
  * @return Return 0 upon success and non-zero otherwise
  */
 int mouse_write_cmd (char cmd);

 /**
  * @brief Subscribes and enables Mouse interrupts
  *
  * @return Returns bit order in interrupt mask; negative value on failure
  */
 int mouse_subscribe_int(void);

 /**
  * @brief Unsubscribes Mouse interrupts
  *
  * @return Return 0 upon success and non-zero otherwise
  */
 int mouse_unsubscribe_int(void);

 /**
  * @brief Synchronizes the mouse
  *
  * @return Return byte1 upon sucess and -1 otherwise
 */
 int mouse_synchronize(void);

 /**
  * @brief Reads data from the mouse output buffer
  *
  * @return Return value read upon sucess and -1 otherwise
 */
 int mouse_read(void);

 /**
  * @brief Mouse Interrupt Handler
  *
  * Fetches Mouse output and prints packets in a human friendly way
  *
  * @param pointer to packet that shall be processed
  *
  * pointer to counter that controls the packet count of bytes
  *
  * @return Return 0 upon success and non-zero otherwise
  */
 int mouse_handler(unsigned char * packet, unsigned short * counter);

 /**@}*/


 #define BIT(n) (0x01<<(n))

 /**
  * Useful Mouse Macros
  */

 #define MOUSE_IRQ				12	/**< @brief KBC IRQ line */
 #define MOUSE_INITIAL_HOOK_ID	12	/**< @brief KBD Initial hook_id */

 #define PACKET_NELEMENTS		3	/**< @brief Number of Elements in every mouse packet */

 /* PS/2 Mouse-Related KBC Commands*/

 #define READ_CMD_B			0x20	/**< @brief Read Command Byte */
 #define WRITE_CMD_B		0x60	/**< @brief Write Command Byte */
 #define DISABLE_MOUSE		0xA7
 #define ENABLE_MOUSE		0xA8
 #define CHECK_MOUSE_ITF	0XA9	/**< @brief Check Mouse Interface */
 #define WRITE_B_MOUSE		0xD4	/**< @brief Write Byte to Mouse */

 /* PS/2 Mouse Commands passed as arguments of 0xD4*/

 #define MOUSE_RESET		0XFF	/**< @brief Mouse reset */
 #define MOUSE_RESEND		0XFE	/**< @brief For serial communications errors */
 #define SET_DEFAULTS		0xF6	/**< @brief Set default values */
 #define DISABLE_DATA_R		0xF5	/**< @brief Disable Data Reporting (In stream mode, should be sent before any other command) */
 #define ENABLE_DATA_R		0xF4	/**< @brief Enable Data Reporting (In stream mode) */
 #define SET_SAMPLE_RATE	0xF3	/**< @brief Sets state sampling rate */
 #define SET_REMOTE_MODE	0XF0	/**< @brief Send data on request only */
 #define READ_DATA			0xEB	/**< @brief Send data packet request */
 #define SET_STREAM_MODE	0xEA	/**< @brief Send data on events */
 #define STATUS_REQUEST		0xE9	/**< @brief Get mouse configuration (3 bytes) */
 #define SET_RESOLUTION		0XE8
 #define SET_SCALING_2_1	0xE7	/**< @brief Aceleration mode */
 #define SET_SCALING_1_1	0xE6	/**< @brief Linear mode */

 /* PS/2 Mouse Packet Meanings */

 #define BYTE0_Y_OVF		BIT(7)
 #define BYTE0_X_OVF		BIT(6)
 #define BYTE0_Y_SIGN		BIT(5)
 #define BYTE0_X_SIGN		BIT(4)
 #define BYTE0_SYNC_BIT		BIT(3)	/** <@brief Bit that helps to synchronize the packet */
 #define BYTE0_MB			BIT(2)
 #define BYTE0_LB			BIT(1)
 #define BYTE0_RB			BIT(0)


 #define DELAY_US		20000   // KBC respond Time-Out in micro seconds


#endif
