#ifndef __KEYBOARD_H
#define __KEYBOARD_H



/** @defgroup keyboard keyboard
 * @{
 *
 * Functions for using the i8042 KBC/KBD
 */


 /**
  * @brief Subscribes and enables Keyboard interrupts
  *
  * @return Returns bit order in interrupt mask; negative value on failure
  */
 int kbd_subscribe_int(void);

 /**
  * @brief Unsubscribes Keyboard interrupts
  *
  * @return Return 0 upon success and non-zero otherwise
  */
 int kbd_unsubscribe_int(void);

 /**
  * @brief Reads data from the keyboard output buffer
  *
  * @return Return value read upon sucess and -1 otherwise
 */
 int keyboard_read(void);

 /**
  * @brief Writes data to the keyboard input buffer
  *
  * @param parameter that is written in the input buffer
  *
  * @return Return 0 upon success and non-zero otherwise
  */
 int keyboard_write(char data);

 /**
  * @brief	Writes command to kbd, followed by arg
  *
  * @param  command of the action we want to execute and the argument that will be used in that action
  * @param  arg argument of the command
  *
  * @return Return 0 upon success and non-zero otherwise
  */
 int keyboard_write_command(char command, unsigned char arg);

 /**
  * @brief	Function that finishes when ESC key is pressed
  *
  * @return	Return 0 upon success and non-zero otherwise
  */
 int wait_esc_release(void);


#endif
