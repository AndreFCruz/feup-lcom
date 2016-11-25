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
		return 0;
		break;
	case 0x01:
		printf("Function call failed\n");
		return 1;
		break;
	case 0x02:
		printf("Function is not supported in current HW configuration\n");
		return 1;
		break;
	case 0x03:
		printf("Function is invalid in current video mode\n");
		return 1;
		break;
	default:
		printf("General failure condition\n");
		return 1;
		break;
	}
}

int vbe_get_controller_info (vbe_info_block *vbe_info_p) {

	mmap_t mem_map;
	struct reg86u reg86;

	void * lma_p = lm.init();	//Contém apontador para onde o 1º Mib foi mapped - Há necessidade de guardar?
	if (lma_p = NULL) {
		printf("vbe_get_controller_info: Failed to initialize low memory area.\n");
		return 1;
	}

	lm_alloc(sizeof(vbe_info_block), &mem_map);
	if ( & mem = NULL) {	//check this condition
		printf("vbe_get_controller_info: Failed to allocate a memory block.");
		return1;
	}

	//Para não alterarmos a original
	physbytes mem_copy = mem.phys;

	r.u.b.ah = VBE_CALL;
	r.u.b.al = GET_VBE_CONTROLLER_INFO;
	reg86.u.w.es = PB2BASE(mem_copy);
	reg86.u.w.di = PB2OFF(mem.copy);
	reg86.u.b.intno = VIDEO_INTERRUPT;

	sys_int86(&reg86);
	if ( !(OK == vbe_assert_error(reg86.u.b.ah)))
		return 1;

	*vbe_info_p = *(vbe_info_block*) mem.virtual;	//Saving on the allocated memory pointer
	lm_free(&mem_map);

//	if (lma_p == NULL)
//		return 1;

	return 0;
}

// TODO CHECK
//int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {
//	struct reg86u r;
//	mmap_t mem;
//
//	if (lm_init() == NULL) {
//		printf("vbe_get_mode_info() -> initialization failed.\n");
//		return 1;
//	}
//
//	lm_alloc(sizeof(vbe_mode_info_t), &mem);
//	phys_bytes buf = mem.phys;
//
//	r.u.b.ah = VBE_CALL;
//	r.u.b.al = GET_VBE_MODE_INFO;
//	r.u.w.es = PB2BASE(buf);
//	r.u.w.di = PB2OFF(buf);
//	r.u.w.cx = mode;
//	r.u.b.intno = VBE_INTERRUPT;
//
//	if (sys_int86(&r) != OK) {
//		printf("vbe_get_mode_info() -> sys call returned non-zero\n");
//		return 1;
//	}
//
//    *vmi_p = *(vbe_mode_info_t*) mem.virtual;
//	lm_free(&mem);
//	return OK;
//
//}


