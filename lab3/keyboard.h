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
