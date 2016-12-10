#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "vbe.h"
//#include "defs.h"

/* Static global variables */
static void *video_mem;		/* Process address to which VRAM is mapped */
static void *buffer_ptr;

static unsigned h_res;		/* Horizontal screen resolution in pixels */
static unsigned v_res;		/* Vertical screen resolution in pixels */
static unsigned vram_size;
static unsigned vram_base;
static unsigned bits_per_pixel;

unsigned vg_getHorRes() {
	return h_res;
}

unsigned vg_getVerRes() {
	return v_res;
}

void * vg_getBufferPtr() {
	return buffer_ptr;
}

void paint_pixel(int x, int y, int color) {
	*((uint16_t *)buffer_ptr + x + y * h_res) = color;
}

int is_valid_pos(unsigned short x, unsigned short y) {
	return (x < h_res && y < v_res) ? OK : 1;
}

void fill_screen(uint16_t color) {
	memset(buffer_ptr, color, h_res * v_res);
}

// Snippet based on the PDF
void *vg_init(unsigned short mode) {
	struct reg86u r;

	r.u.b.ah = VBE_CALL;
	r.u.b.al = SET_VBE_MODE;
	r.u.w.bx = 1<<14 | mode; // set bit 14: linear framebuffer
	r.u.b.intno = VBE_INTERRUPT;

	if( sys_int86(&r) != OK ) {
		printf("set_vbe_mode: sys_int86() failed\n");
		return NULL;
	}
	if ( OK != vbe_assert_error(r.u.b.ah) )
		return NULL;

	int n;
	struct mem_range mr;
	vbe_mode_info_t* vbe_mode_p = malloc(sizeof(vbe_mode_info_t));

	if (vbe_get_mode_info(mode, vbe_mode_p) != OK) {
		printf("vg_init(): vbe_get_mode_info failed\n");
		return NULL;
	}

	h_res = vbe_mode_p->XResolution;
	v_res = vbe_mode_p->YResolution;
	bits_per_pixel = vbe_mode_p->BitsPerPixel;
	vram_size = h_res * v_res * bits_per_pixel;

	/* Allow memory mapping */
	mr.mr_base = (phys_bytes) vbe_mode_p->PhysBasePtr;
	mr.mr_limit = mr.mr_base + vram_size;

	if( OK != (n = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		panic("sys_privctl (ADD_MEM) failed: %d\n", n);

	/* Map memory */
	video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
	if(video_mem == MAP_FAILED)
		panic("couldn’t map video memory");

	//Buffer initialization for use in double buffering
	buffer_ptr = (void *) malloc(vram_size);

	return buffer_ptr;
}

int vg_exit() {
  struct reg86u reg86;

  reg86.u.b.intno = 0x10; /* BIOS video services */

  reg86.u.b.ah = 0x00;    /* Set Video Mode function */
  reg86.u.b.al = 0x03;    /* 80x25 text mode*/

  if( sys_int86(&reg86) != OK ) {
      printf("\tvg_exit(): sys_int86() failed \n");
      return 1;
  } else
      return 0;
}

//Added Functions
int draw_line (unsigned short xi, unsigned short yi,
		           unsigned short xf, unsigned short yf, unsigned long color) {


	if ( OK != is_valid_pos(xi, yi) || OK != is_valid_pos(xf, yf) ) {
		printf("draw_line: invalid position for line. Was (%d,%d) to (%d,%d).\n", xi, yi, xf, yf);
		return 1;
	}
	if ( color >= 0x40 ) {
		printf("draw_line: invalid color. Was 0x%X.\n", color);
		return 1;
	}

	// x and y variation
	int x_variation = xf-xi;
	int y_variation = yf-yi;

	//Need this for functions whose slopes are != 1
	float x = (float) xi;
	float y = (float) yi;
	int n;	//number of steps

	//Getting the number of steps value
	if (x_variation < 0 && y_variation < 0 ) {
		if (-x_variation > -y_variation)
			n = -x_variation;
		else
			n = -y_variation;
	}
	else if (x_variation < 0) {
		if (-x_variation > y_variation)
			n = -x_variation;
		else
			n = y_variation;
	}
	else if (y_variation < 0) {
		if (x_variation > -y_variation)
			n = x_variation;
		else
			n = -y_variation;
	}
	else {
		if (x_variation > y_variation)
			n = x_variation;
		else
			n = y_variation;
	}

	unsigned i;
	for (i = 0; i <= n; ++i) {
		paint_pixel(x, y, color);
		x += (x_variation / (float) n);
		y += (y_variation / (float) n);
	}

	return OK;
}

int draw_circle (unsigned short center_x, unsigned short center_y, unsigned short radius, unsigned long color) {

	if ( OK != is_valid_pos(center_x+radius, center_y+radius) || OK != is_valid_pos(center_x-radius, center_y-radius) ) {
		printf("draw_circle: invalid position for circle. Center was: (%d, %d). Radius was %d.\n", center_x, center_y, radius);
		return 1;
	}

	unsigned x_var = (center_x-radius);
	unsigned y_var = (center_y-radius);

	while( y_var <= (center_y+radius)) {
		if ((x_var-center_x)*(x_var-center_x) + (y_var-center_y)*(y_var-center_y) <= radius*radius)
			paint_pixel(x_var, y_var, color);

		++x_var;
		if (x_var > center_x+radius) {
			x_var = (center_x-radius);
			y_var++;
		}
	}

	return OK;
}

int draw_square (unsigned short x, unsigned short y, unsigned short size, unsigned long color) {
	// Argument Checks
	if ( OK != is_valid_pos(x, y) || OK != is_valid_pos(x + size, y + size) ) {
		printf("draw_square: invalid position for square. Was (%d,%d) to (%d,%d).\n", x, y, x + size, y + size);
		return 1;
	}
	if ( color >= 0xFFFFFF) {
		printf("draw_square: invalid color. Was 0x%X.\n", color);
		return 1;
	}

	// Draw Square
	unsigned i, j;
	for (i = x; i < size + x; i++) {
		for (j = y; j < size + y; j++) {
			paint_pixel(i, j, color);
		}
	}

	return OK;
}

int draw_mouse_cross (unsigned short xi, unsigned short yi) {

	if (!is_valid_pos(xi,yi)) {
		printf("Invalid Position for Draw Mouse.\n");
		return 1;
	}

	//Drawing the horizontal line of the cross
	if (xi-10 < 0) {
		draw_line(0,yi-1,xi+10,yi-1,0);
		draw_line(0,yi,xi+10,yi,0);
		draw_line(0,yi+1,xi+10,yi+1,0);
	}
	else if (xi+10 > h_res) {
		draw_line(xi-10,yi-1,h_res-1,yi-1,0);
		draw_line(xi-10,yi,h_res-1,yi,0);
		draw_line(xi-10,yi+1,h_res-1,yi+1,0);
	}
	else {
		draw_line(xi-10,yi-1,xi+10,yi-1,0);
		draw_line(xi-10,yi,xi+10,yi,0);
		draw_line(xi-10,yi+1,xi+10,yi+1,0);
	}

	//Drawing the vertical line of the cross
	if (yi-10 < 0) {
		draw_line(xi-1,0,xi-1,yi+10,0);
		draw_line(xi,0,xi,yi+10,0);
		draw_line(xi+1,0,xi+1,yi+10,0);
	}
	else if (yi+10 > v_res) {
		draw_line(xi-1,yi-10,xi-1,v_res-1,0);
		draw_line(xi,yi-10,xi,v_res-1,0);
		draw_line(xi+1,yi-10,xi+1,v_res-1,0);
	}
	else {
		draw_line(xi-1,yi-10,xi-1,yi+10,0);
		draw_line(xi,yi-10,xi,yi+10,0);
		draw_line(xi+1,yi-10,xi+1,yi+10,0);
	}

	//draw_line(ptr,xi-10,yi,xi+10,yi,0);
	//draw_line(ptr,xi,yi-10,xi,yi+10,0);

	return OK;
}

// TODO Melhorar...
void draw_missile(Missile * m) {
	unsigned thickness = 4, idx = 0;
	for (; idx < thickness; ++idx) {
		draw_line(missile_getInitX(m), missile_getInitY(m), missile_getPosX(m), missile_getPosY(m), missile_getColor(m));
	}
}


/* TODO: Acrescentar  as condicoes de superior a 255, Por aql return muito mais bonitinho e explicado, Terminar as restantes macros das cores */
uint16_t rgb (char red_value, char green_value, char blue_value) {
	uint16_t return_value;
	return (((red_value>>2) << 11) + ((green_value>>2) << 5) + (blue_value>>2));
}

void buffer_handler () {
	memcpy(video_mem,buffer_ptr,vram_size);
}
