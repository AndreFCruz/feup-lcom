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
	helper = helper | BIT(1) | BIT(2) | BIT(3) | BIT(5);

	if (timer == 0) {
		sys_outb (TIMER_CTRL, helper);
		sys_outb (TIMER_0, msb);
	}
	else if (timer == 1) {
		helper = helper | BIT(6);
		sys_outb (TIMER_CTRL, helper);
		sys_outb (TIMER_1, msb);
	}
	else if (timer == 2) {
		helper = helper | BIT(7);
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
	u8_t config = TIMER_RB_CMD;
	config |= TIMER_RB_COUNT_;	// 0 value is active!
	// deactivated COUNT, meaning only STATUS command is active

	if (timer == 0) {
		port = TIMER_0;
		config |= TIMER_RB_SEL(0);
	} else if (timer == 1) {
		port = TIMER_1;
		config |= TIMER_RB_SEL(1);
	} else if (timer == 2) {
		port = TIMER_2;
		config |= TIMER_RB_SEL(2);
	}	else {
		fprintf(stderr, "Error: %s.\n", "timer must be an integer in range [0, 2]");
		return 1;	// not successful
	}

	// Send ReadBack command to the control register
	if (sys_outb(TIMER_CTRL, config)) {
		fprintf(stderr, "Error: %s.\n", "kernel call returned non-zero value");
		return 1;
	}

	// Read TIMER status register
	if (sys_inb(port, (unsigned long int *) st)) {
		fprintf(stderr, "Error: %s\n", "kernel call returned non-zero value");
		return 1;
	}

	return 0;
}

int timer_display_conf(unsigned char conf)
{
	// OutPin Flag
	printf("Out Pin is: %d.\n", conf & BIT(7) ? "1" : "0");

	// Null Count Flag
	if (conf & BIT(6))
		printf("Null Count.\n");
	else
		printf("Count available for reading.\n");

	//Type of Access
	printf("Timer access type: ");
	if (conf & (BIT(5) | BIT(4)) == TIMER_LSB_MSB) {
		printf("LSB and MSB.\n");
	} else if (conf & (BIT(5) | BIT(4)) == TIMER_LSB) {
		printf("LSB.\n");
	} else if (conf & (BIT(5) | BIT(4)) == TIMER_MSB) {
		printf("MSB.\n");
	} else {
		fprintf(stderr, "%s\n", );
		return 1;	// Unsuccessful
	}

	// Programmed Mode
	printf("Timer in mode %d.\n", (conf & (BIT(1) | BIT(2) | BIT(3)) >> 1));

	// BCD Flag
	printf("BCD status: %d.", conf & BIT(0));

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
