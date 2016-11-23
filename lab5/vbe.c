#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>

#include "vbe.h"
#include "lmlib.h"

#define LINEAR_MODEL_BIT 14

#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)


// Codigo baseado no pdf VESA
void paintPixel(int x,int y,int color)
{
    long addr = (long)y * bytesperline + x;
    setBank((int)(addr >> 16));
    *(screenPtr + (addr & 0xFFFF)) = (char)color;
}

int vbe_assert_error(unsigned char e) {
	switch (e) {
	case 0x01:
		printf("Function call failed\n");
		break;
	case 0x02:
		printf("Function is not supported in current HW configuration\n");
		break;
	case 0x03:
		printf("Function is invalid in current video mode\n");
		break;
	default:
		printf("Success\n");
		break;
	}
}

int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {
  
  /* To be completed */
  
  return 1;
}


