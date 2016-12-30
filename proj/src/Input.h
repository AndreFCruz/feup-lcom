#ifndef __INPUT_H
#define __INPUT_H

/** @defgroup Input Input
 * @{
 * Functions for manipulating all the Inputs from the user/ player
 */

#include "i8042.h"
#include "keyboard.h"
#include "mouse.h"

// TODO Update with actual make/break codes!!
typedef enum {
	ESC_BREAK = ESC_BREAK_CODE,
	ENTER_BREAK = ENTER_BREAK_CODE,
	UP_MAKE = UP_MAKE_CODE,
	DOWN_MAKE = DOWN_MAKE_CODE,
	NO_KEY = 0x0
} keycode_t;

typedef struct {
	/* Keyboard Keys */
	keycode_t keycode;

	/* Mouse Buttons */
	int RMB;
	int LMB;
	int MMB;

	/* Mouse Position */
	int mouse_pos[2];

	/* Resolution */
	unsigned res[2];
} Input_t;

/**
 * @brief Creates a new input instance if there is none, otherwise returns the existing one (Singleton like behaviour).
 *
 * @return Pointer to a Input instance
 */
Input_t * input_instance();

/**
 * @brief Destroy the Input instance, freeing all the resources used by it
 */
void delete_input();

/**
 * @brief Keyboard Interrupt Handler
 */
void keyboard_handler();

/**
 * @brief Remove's the key code from the input buffer
 *
 * Compatible with double make/ brake codes
 *
 * @return Key code type (ESC, ENTER, UP, DOWN or NONE)
 */
keycode_t input_get_key();

/**
 * @brief Gets the current mouse position on the screen
 *
 * @return Array containing the current mouse position on the screen (x,y)
 */
const int * get_mouse_pos();

/**
 * @brief Gets the current state of the mouse right button
 *
 * @return Return 0 if mouse right button is not pressed, non zero otherwise
 */
int get_mouseRMB();

/**
 * @brief Gets the current state of the mouse left button
 *
 * @return Return 0 if mouse left button is not pressed, non zero otherwise
 */
int get_mouseLMB();

/**
 * @brief Gets the current state of the mouse middle button
 *
 * @return Return 0 if mouse middle button is not pressed, non zero otherwise
 */
int get_mouseMMB();

/**
 * @brief Mouse Interrupt Handler
 *
 * Reads the packet received and updates the Input instance accordingly
 *
 * @param packet mouse packet containing the mouse information
 */
void mouse_packet_handler(unsigned char * packet);

/**
 * @brief Verifies if the Mouse is inside a certain rectangle
 *
 * @param x_inital Initial position of the rectangle in the horizontal axis
 * @param y_initial Initial position of the rectangle in the vertical axis
 * @param x_final Final position of the rectangle in the horizontal axis
 * @param y_final Final position of the rectangle in the vertical axis
 *
 * @return Return 0 if Mouse is not inside the rectantgle, non-zero otherwise	TODO: nao devia ser ao contrario?
 */
int mouse_inside_rect(int x_initial, int y_initial, int x_final, int y_final);

/**
 * @brief Verifies if the Mouse is inside a certain circle
 *
 * @param x_center Position of the center of the circle in the horizontal axis
 * @param y_center Position of the center of the circle in the vertical axis
 * @param radius Radius of the circle
 *
 * @return Return 0 if Mouse is not inside the circle, non-zero otherwise	TODO: nao devia ser ao contrario?
 */
int mouse_inside_circle(int x_center, int y_center, int radius);

#endif
