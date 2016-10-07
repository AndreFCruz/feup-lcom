#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8254.h"

int timer_set_square(unsigned long timer, unsigned long freq) {

	if (timer != 0 && timer != 1 && timer != 2)
		return 1;

	unsigned long Div = TIMER_FREQ/freq;
	unsigned long lsb = Div & 0x00FF;
	unsigned long msb = (Div >> 8) & 0xFF;

	unsigned long helper = 0;
	helper = helper | Bit(1) | Bit(2) | Bit(3) | Bit(5);

	if (timer == 0) {
		sys_outb (TIMER_CTRL, helper);
		sys_outb (TIMER_0, msb);
	}
	else if (timer == 1) {
		helper = helper | Bit(6);
		sys_outb (TIMER_CTRL, helper);
		sys_outb (TIMER_1, msb);
	}
	else if (timer == 2) {
		helper = helper | Bit(7);
		sys_outb (TIMER_CTRL, helper);
		sys_outb (TIMER_2, msb);
	}

	return 0;
}

int timer_subscribe_int(void ) {

	return 1;
}

int timer_unsubscribe_int() {

	return 1;
}

void timer_int_handler() {

}

int timer_get_conf(unsigned long timer, unsigned char *st) {
	
	port_t port;
	unsigned long config = TIMER_RB_CMD | TIMER_RB_STATUS_;

	if (timer == 0) {
		port = TIMER_0;
		config |= timer;
	} else if (timer == 1) {
		port = TIMER_1;
		config |= timer;
	} else if (timer == 2) {
		port = TIMER_2;
		config |= timer;
	}
	else
		return 1;	// not successful

	sys_outb(TIMER_CTRL, config);
	sys_inb(port, (unsigned long int *) st);

	return 0;
}

int timer_display_conf(unsigned char conf) {
	
	unsigned int outPinFlag;
	if (conf & BIT(7))	// 7th bit is 1
		outPinFlag = 1;
	else	// 7th bit is 0
		outPinFlag = 0;
	printf("Out Pin is: %d.\n", outPinFlag);

	unsigned int nullCountFlag;
	if (conf & BIT(6))	// 6th bit is 1
		nullCountFlag = 1;
	else	// 6th bit is 0
		nullCountFlag = 0;
	printf("Null Count is: %d.\n", nullCountFlag);

	unsigned int bit_mask_5lsb = 0x001F;	// bit mask to get 5 least significant bits
	unsigned int timer_mode = conf & bit_mask_5lsb;
	printf("Timer in mode %d.\n", timer_mode);

	return 0;
}

int timer_test_square(unsigned long freq) {
	
	if(timer_set_square (0,freq) == 0)
		return 1;
	else
		return 0;
}

int timer_test_int(unsigned long time) {
	
	return 1;
}

int timer_test_config(unsigned long timer) {
	
	return 1;
}
