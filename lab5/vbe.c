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
		return OK;
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

void * vbe_get_controller_info (vbe_info_block *vbe_info_p) {

	mmap_t mem_map;
	struct reg86u r;

	void * lma_p = lm_init();	//Need to save this only for check's
	if (lma_p == NULL) {
		printf("vbe_get_controller_info: Failed to initialize low memory area.\n");
		return NULL;
	}

	lm_alloc(sizeof(vbe_info_block), &mem_map);
	if ( & mem_map == NULL) {	//check this condition
		printf("vbe_get_controller_info: Failed to allocate a memory block.\n");
		return NULL;
	}

	//can't change him directly, otherwise lm_free doesnt work. mem_map.phys is the real mode adress
	phys_bytes mem_copy = mem_map.phys;

	//Block gotten from VESA pdf
	r.u.b.ah = VBE_CALL;
	r.u.b.al = GET_VBE_CONTROLLER_INFO;
	r.u.w.es = PB2BASE(mem_copy);
	r.u.w.di = PB2OFF(mem_copy);
	r.u.b.intno = VBE_INTERRUPT;

	sys_int86(&r);
	if ( !(OK == vbe_assert_error(r.u.b.ah)))
		return NULL;

	//== mem.cpy
	*vbe_info_p = *(vbe_info_block*) mem_map.virtual;	//Saving on the allocated memory pointer
	lm_free(&mem_map);

	//If lma_p still points to somewhere but NULL, it means lm_free failed
	if ( lma_p == NULL) {
		printf("vbe_get_controller_info: Failed to free the memory block in the low area.\n");
		return NULL;
	}

	return lma_p;
}


void* vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vbe_mode_p) {

	mmap_t mem_map;
	struct reg86u r;

	void * lma_p = lm_init();	//Need to save this only for check's
	if (lm_init() == NULL) {
		printf("vbe_get_mode_info: Failed to initialize low memory area.\n");
		return NULL;
	}

	lm_alloc(sizeof(vbe_mode_info_t), &mem_map);
	if (& mem_map == NULL) {	//check this condition
		printf("vbe_get_controller_info: Failed to allocate a memory block.\n");
		return NULL;
	}

	//can't change him directly, otherwise lm_free doesnt work. mem_map.phys is the real mode adress
	phys_bytes mem_copy = mem_map.phys;

	r.u.b.ah = VBE_CALL;
	r.u.b.al = GET_VBE_MODE_INFO;
	r.u.w.es = PB2BASE(mem_copy);
	r.u.w.di = PB2OFF(mem_copy);
	r.u.w.cx = mode;
	r.u.b.intno = VBE_INTERRUPT;

	sys_int86(&r);
	if ( !(OK == vbe_assert_error(r.u.b.ah)))
		return NULL;

	//== mem.cpy
	*vbe_mode_p = *(vbe_mode_info_t*) mem_map.virtual;	//Saving on the allocated memory pointer
	lm_free(&mem_map);

	// Need to return lma_p (virtual pointer).
	return OK;
}


