#ifndef __MOUSE_H
#define __MOUSE_H



/** @defgroup mouse mouse
 * @{
 *
 * Functions for using the i8042 KBC/KBD
 */


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
