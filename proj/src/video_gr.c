#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "vbe.h"
//#include "defs.h"

/* Static global variables */
static void *video_mem;		/* Process address to which VRAM is mapped */

static unsigned h_res;		/* Horizontal screen resolution in pixels */
static unsigned v_res;		/* Vertical screen resolution in pixels */
static unsigned vram_size;
static unsigned vram_base;
static unsigned bits_per_pixel;


void paint_pixel(int x, int y, int color, char * ptr) {
	*(ptr + x + y * h_res) = color;
}

int is_valid_pos(unsigned short x, unsigned short y) {
	return (x < h_res && y < v_res) ? OK : 1;
}

void fill_screen(unsigned char color) {
	memset(video_mem, color, h_res * v_res);
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

	return video_mem;
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
