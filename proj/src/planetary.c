/* includes */
#include <minix/syslib.h>
#include <minix/driver.h>
#include <minix/com.h>
#include <minix/sysutil.h>
#include "video_gr.h"
#include "timer.h"
#include "keyboard.h"
#include "i8042.h"
#include "read_xpm.h"
#include "math.h"
#include "vbe.h"
#include "pixmap.h"
#include "stddef.h"
#include "mouse.h"
/* ** */

int main()
{

	// Rudimentary Game Setup
	char * ptr;
	if ( (ptr = vg_init(MODE_1024X768_256)) == NULL ) {
		printf("test_square failed VRAM map in vg_init\n");
		return 1;
	}


	//Testes
	draw_line(ptr, 100,100,800,300,37);
	draw_circle(ptr, 300, 300, 200, 18);
	draw_square(ptr, 50, 300, 100, 23);
	draw_xpm(ptr, 500, 500, penguin);

	//Mouses-tests
	mouse_t * mouse = new_mouse();
	draw_mouse(ptr,mouse);

	timer_delay(5);

	return vg_exit();
}
