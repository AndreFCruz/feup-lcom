#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H

/** @defgroup video_gr video_gr
 * @{
 *
 * Functions for outputing data to screen in graphics mode
 */

#include <stdint.h>
#include "Missile.h"

/* RGB Color Defnitions */
#define BLACK 			rgb(0,0,0)
#define WHITE			rgb(255,255,255)
#define TRANSPARENCY	rgb(0,255,0)
#define STRONG_PINK		rgb(255,0,127)
#define NAVY			rgb(0,128,255)
#define RED				rgb(255,0,0)
#define YELLOW			rgb(255,255,0)

/**
 * @brief Gets the horizontal Resolution of the screen
 *
 * @return Return the horizontal Resolution of the screen
 */
unsigned vg_getHorRes();

/**
 * @brief Gets the vertical Resolution of the screen
 *
 * @return Return the vertical Resolution of the screen
 */
unsigned vg_getVerRes();

/**
 * @brief Gets the pointer to the graphics Buffer used for double-buffering
 *
 * @return Pointer to the graphics Buffer used for double-buffering
 */
void * vg_getBufferPtr();

/**
 * @brief Checks if position (x,y) is inside the screen
 *
 * @param x Position in the horizontal axis
 * @param y Position in the vertical axis
 *
 * @return Return 0 upon success, non-zero otherwise
 */
int is_valid_pos(unsigned short x, unsigned short y);

/**
 * @Paints the pixel at a certain position with a certain color
 *
 * @param x Position in the horizontal axis
 * @param y Position in the vertical axis
 * @param color Color to paint the pixel
 */
void paint_pixel(int x, int y, uint16_t color);

/**
 * @brief Initializes the video module in graphics mode
 * 
 * Uses the VBE INT 0x10 interface to set the desired
 *  graphics mode, maps VRAM to the process' address space and
 *  initializes static global variables with the resolution of the screen, 
 *  and the number of colors
 * 
 * @param mode 16-bit VBE mode to set
 *
 * @return 0 upon success, non-zero on failure
 */
int vg_init(unsigned short mode);

/**
 * @brief Returns to default Minix 3 text mode (0x03: 25 x 80, 16 colors)
 * 
 * @return Return 0 upon success, non-zero otherwise
 */
int vg_exit(void);

/**
 * @brief Draws a line on the screen, given a color, a initial position and a final position
 *
 * @param xi Initial position of the line in the horizontal axis
 * @param yi Initial position of the line in the vertical axis
 * @param xf Final position of the line in the horizontal axis
 * @param yf Final position of the line in the vertical axis
 * @param color Color to paint the line
 *
 * @return Return 0 upon success, non-zero otherwise
 */
int draw_line(unsigned short xi, unsigned short yi, unsigned short xf,
		unsigned short yf, uint16_t color);

/**
 * @brief Draws a circle on the screen, given a color, a center position and a radius
 *
 * @param center_x Center position of the circle in the horizontal axis
 * @param center_y Center position of the circle in the vertical axis
 * @param radius Radius of the circle
 * @param color Color to paint the circle
 *
 * @return Return 0 upon success, non-zero otherwise
 */
int draw_circle(unsigned short center_x, unsigned short center_y,
		unsigned short radius, uint16_t color);

/**
 * @brief Draws the mouse cross at a certain position with a certain color
 *
 * @param mouse_pos Array containing the position of the mouse cross (x,y)
 * @param color Color to paint the mouse cross
 *
 * @return Return 0 upon success, non-zero otherwise
 */
int draw_mouse_cross(const int * mouse_pos, uint16_t color);

/**
 * @brief Draws a missile
 *
 * @param Pointer to the Missile in question
 */
void draw_missile(Missile * ptr);

/**
 * @brief Draws a number at certain position
 *
 * @param num number to be drawn
 * @param font Font used as an array of 10 numbers
 * @param size_x Horizontal size of the Font, in pixels
 * @param posX Position of the number in the horizontal axis
 * @param posY Position of the number in the vertical axis
 */
void draw_number(unsigned num, Bitmap ** font, unsigned size_x, unsigned posX, unsigned posY);

/**
 * @brief Draws a score at certain position
 *
 * @param num score to be drawn
 * @param posX Position of the score in the horizontal axis
 * @param posY Position of the score in the vertical axis
 */
void draw_score(unsigned num, unsigned posX, unsigned posY);

/**
 * @brief Draws a big score at certain position
 *
 * Similar to draw_score() but with larger numbers
 *
 * @param num score to be drawn
 * @param posX Position of the big score in the horizontal axis
 * @param posY Position of the big score in the vertical axis
 */
void draw_score_big(unsigned num, unsigned posX, unsigned posY);

/**
 * @brief Converts a rgb color to a hexadecimal of 6 bytes
 *
 * @param red_value Value of red, between [0,255]
 * @param red_value Value of green, between [0,255]
 * @param red_value Value of blue, between [0,255]
 */
uint16_t rgb(unsigned char red_value, unsigned char green_value,
		unsigned char blue_value);

/**
 * @brief Copies the contents of the video buffer to video_mem at once.
 * (Implementation of double_buffering)
 */
void buffer_handler();

/** @} end of video_gr */

#endif /* __VIDEO_GR_H */
