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

 /**
  * @brief Synchronizes the mouse
  *
  * @return Return byte1 upon sucess and -1 otherwise
 */
 int mouse_synchronize(void);

 /**
  * @brief Writes data to the mouse input buffer
  *
  * @param parameter that is written in the input buffer
  *
  * @return Return 0 upon success and non-zero otherwise
  */
 int mouse_write(char data);

 /**
  * @brief Reads data from the mouse output buffer
  *
  * @return Return value read upon sucess and -1 otherwise
 */
 int mouse_read(void);

 /**
  * @brief Prints the packet in a human friendly way
  *
  * @param pointer to the array that contains the bytes from the packet
  */
 void print_packet (unsigned char * packet);

 //TODO: REVIEW THE DOCUMENTATION
 /**
  * @brief Mouse Interrupt Handler
  *
  * Fetches Mouse output and prints packets in a human friendly way
  *
  * @param pointer to previous iteration's status
  *
  * @return Return 0 upon success and non-zero otherwise
  */
 int mouse_handler();

 /**
  * @brief Fetches mouse configuration
  *
  * @param pointer to 3 element array
  *
  * @return Return 0 upon success and non-zero otherwise
  */
 int mouse_fetch_config(unsigned char *);

#endif
