#ifndef __MOUSE_H
#define __MOUSE_H

/** @defgroup mouse mouse
 * @{
 *
 * Functions for using the i8042 KBC/KBD
 */

/* Types needed for function type_gesture() - State Machine */
typedef enum {INIT, DRAW, COMP} state_t;
typedef enum {RDOWN, RUP, VERTLINE} ev_type_t;
typedef enum {UPWARDS, DOWNWARDS} direction_t;

typedef struct {
	ev_type_t type;
	int y_delta, x_delta;
	direction_t dir;
} event_t;

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
  * @param pointer to previous iteration's status
  *
  * @return Return 0 upon success and non-zero otherwise
  */
 int mouse_handler();

 /**
  * @brief Prints the packet in a human friendly way
  *
  * @param pointer to the array that contains the bytes from the packet
  */
 void print_packet (unsigned char * packet);

 /**
  * @brief Fetches mouse configuration
  *
  * @param pointer to 3 element array
  *
  * @return Return 0 upon success and non-zero otherwise
  */
 int mouse_fetch_config(unsigned char *);

 /**
  * @brief Updates the event
  *
  * @param Pointer to the event
  *
  * Pointer to packet that is being processed
  *
  * length upon which test_gesture finishes
  */
 void event_update (event_t * evt, const unsigned char *packet, short length);

 /**
  * @brief Represents the state machine
  *
  * @param Pointer to the event
  *
  * Pointer to packet that is being processed
  *
  * length upon which test_gesture fails
  */
 void check_ver_line(event_t * evt, const unsigned char *packet, short length);

#endif
