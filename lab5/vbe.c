#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>

#include "vbe.h"
#include "lmlib.h"

#define LINEAR_MODEL_BIT 14

#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)


// To be called on register AH
int vbe_assert_error(unsigned char e) {
	switch (e) {
	case 0x00:
		printf("Function call succeeded\n");
		break;
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
		printf("General failure condition\n");
		break;
	}
}


// TODO CHECK
int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {
	struct reg86u r;
	mmap_t mem;

	if (lm_init() == NULL) {
		printf("vbe_get_mode_info() -> initialization failed.\n");
		return 1;
	}

	lm_alloc(sizeof(vbe_mode_info_t), &mem);
	phys_bytes buf = mem.phys;

	r.u.b.ah = VBE_CALL;
	r.u.b.al = VBE_GET_MODE_INFO;
	r.u.w.es = PB2BASE(buf);
	r.u.w.di = PB2OFF(buf);
	r.u.w.cx = mode;
	r.u.b.intno = VIDEO_INTERRUPT;

	if (sys_int86(&r) != OK) {
		printf("vbe_get_mode_info() -> sys call returned non-zero\n");
		return 1;
	}

    *vmi_p = *(vbe_mode_info_t*) mem.virtual;
	lm_free(&mem);
	return OK;

}


