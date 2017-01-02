#ifndef __COMMUNICATION_H
#define __COMMUNICATION_H

#include "Serial.h"

/** @defgroup Input Input
 * @{
 * Functions for manipulating all the Inputs from the user/ player
 */

/* Codes for communication between Computers */
#define MP_WAITING		0x01
#define MP_STARTED		0x02
#define	MP_ENDED		0x03

#define ACK				0xFF


typedef enum {
	WAITING_START, ONGOING, ENDED, NONE
} serial_state_t;

/**
 * @brief Serial Port interrupt handler.
 */
void serial_handler();

/**
 * @brief Sets the Communication State. Used for multiplayer state.
 */
void setComState(serial_state_t state);

/**
 * @brief Get method for the current Communication State
 * @return The current serial_state_t
 */
serial_state_t getComState();

/**@}*/

#endif /* __COMMUNICATION_H */
