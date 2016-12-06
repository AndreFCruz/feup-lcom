#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H

/** @defgroup video_gr video_gr
 * @{
 *
 * Functions for outputing data to screen in graphics mode
 */

int is_valid_pos(unsigned short x, unsigned short y);

void fill_screen(unsigned char color);

void paint_pixel(int x, int y, int color, char * ptr);

/**
 * @brief Initializes the video module in graphics mode
 * 
 * Uses the VBE INT 0x10 interface to set the desired
 *  graphics mode, maps VRAM to the process' address space and
 *  initializes static global variables with the resolution of the screen, 
 *  and the number of colors
 * 
 * @param mode 16-bit VBE mode to set
 * @return Virtual address VRAM was mapped to. NULL, upon failure.
 */
void *vg_init(unsigned short mode);

 /**
 * @brief Returns to default Minix 3 text mode (0x03: 25 x 80, 16 colors)
 * 
 * @return 0 upon success, non-zero upon failure
 */
int vg_exit(void);

//Added functions - Need to finish documentation
unsigned get_Xres();
unsigned get_Yres();

int draw_line (char * ptr, unsigned short xi, unsigned short yi,
		           unsigned short xf, unsigned short yf, unsigned long color);
int draw_circle (char * ptr, unsigned short center_x, unsigned short center_y, unsigned short radius, unsigned long color);
int draw_square (char * ptr, unsigned short x, unsigned short y, unsigned short size, unsigned long color);
int draw_xpm (char * ptr, unsigned short xi, unsigned short yi, char *xpm[]);

int draw_mouse_cross (char * ptr, unsigned short xi, unsigned short yi);
 /** @} end of video_gr */
 
#endif /* __VIDEO_GR_H */
