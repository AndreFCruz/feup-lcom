#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "vbe.h"
#include "defs.h"

/* Constants for VBE 0x105 mode */

/* The physical address may vary from VM to VM.
 * At one time it was 0xD0000000
 *  #define VRAM_PHYS_ADDR    0xD0000000 
 * Currently on lab B107 is 0xF0000000
 * Better run my version of lab5 as follows:
 *     service run `pwd`/lab5 -args "mode 0x105"
 */
//#define VRAM_PHYS_ADDR	0xF0000000	// Standard
//#define VRAM_PHYS_ADDR		0xE0000000	// PC Andre
#define BITS_PER_PIXEL		8

/* Private global variables */

static void *video_mem;		/* Process address to which VRAM is mapped */

static unsigned h_res;		/* Horizontal screen resolution in pixels */
static unsigned v_res;		/* Vertical screen resolution in pixels */
static unsigned vram_size = H_RES_0X105 * V_RES_0X105 * BITS_PER_PIXEL;
static unsigned vram_base;
static unsigned bits_per_pixel;

//static unsigned bytesperline = H_RES_0X105;


void paint_pixel(int x, int y, int color, char * ptr) {
	*(ptr + x + y * h_res) = color;
}


// Codigo baseado no pdf VESA
//void paintPixel(int x,int y,int color)
//{
//    long addr = (long)y * bytesperline + x;
//    setBank((int)(addr >> 16));
//    *(video_mem + (addr & 0xFFFF)) = (char)color;
//}


// baseado no pdf VESA
//void paintPixelP(int x, int y, int color)
//{
//	char dummy_read;
//	long addr = (long)y * bytesperline + (x/8);
//	setBank((int)(addr >> 16));
//
//	outp(0x3CE,8);
//	outp(0x3CF,0x80 >> (x & 7));
//	dummy_read = *(screenPtr + (addr & 0xFFFF));
//	*(screenPtr + (addr & 0xFFFF)) = (char)color;
//}


// Snippet based on the PDF
void *vg_init(unsigned short mode) {
	struct reg86u r;

	r.u.b.ah = VBE_CALL;
	r.u.b.al = VBE_SET_MODE;
	r.u.w.bx = 1<<14 | mode; // set bit 14: linear framebuffer
	r.u.b.intno = VBE_INTERRUPT;

	if( sys_int86(&r) != OK ) {
		printf("set_vbe_mode: sys_int86() failed\n");
		return NULL;
	}

	int n;
	struct mem_range mr;
	vbe_mode_info_t* vmi_p = malloc(sizeof(vbe_mode_info_t));

	if (vbe_get_mode_info(mode, vmi_p) != OK) {
		printf("vg_init(): vbe_get_mode_info failed\n");
		return NULL;
	}

	h_res = vmi_p->XResolution;
	v_res = vmi_p->YResolution;
	bits_per_pixel = vmi_p->BitsPerPixel;
	vram_size = h_res * v_res * bits_per_pixel;

	/* Allow memory mapping */
	mr.mr_base = (phys_bytes) vmi_p->PhysBasePtr;
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
