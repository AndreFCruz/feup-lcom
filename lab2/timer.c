#include <minix/syslib.h>
#include <minix/drivers.h>
#include <i8254.h>

int timer_set_square(unsigned long timer, unsigned long freq) {

	return 1;
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
	sys_inb(port, st);

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

	unsigned int timer_mode = conf & 0x001F;	// Compatible with future intel products
	printf("Timer in mode %d\n", timer_mode);

	return 0;
}

int timer_test_square(unsigned long freq) {
	
	return 1;
}

int timer_test_int(unsigned long time) {
	
	return 1;
}

int timer_test_config(unsigned long timer) {
	
	return 1;
}
