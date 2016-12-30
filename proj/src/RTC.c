#include <minix/syslib.h>
#include <stdlib.h>
#include "RTC.h"

static int rtc_hook_id = RTC_INITIAL_HOOK_ID;

int rtc_subscribe_int(void) {
	if (sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE, &rtc_hook_id) != OK) {
		printf("rtc_subscribe_int() -> FAILED sys_irqsetpolicy().\n");
		return -1;
	}
	if (sys_irqenable(&rtc_hook_id) != OK) {
		printf("rtc_subscribe_int() -> FAILED sys_irqenable().\n");
		return -1;
	}

	return RTC_INITIAL_HOOK_ID;
}

int rtc_unsubscribe_int() {
	if (sys_irqdisable(&rtc_hook_id) != OK) {
		printf("rtc_unsubscribe_int() -> FAILED sys_irqdisable().\n");
		return -1;
	}
	if (sys_irqrmpolicy(&rtc_hook_id) != OK) {
		printf("rtc_unsubscribe_int() -> FAILED sys_irqrmpolicy().\n");
		return -1;
	}

	return OK;
}

int rtc_write_register(unsigned long reg, unsigned long value) {
	if (sys_outb(RTC_ADDR_REG, reg) != OK) {
		printf("rtc_write_register -> Failed sys_outb().\n");
		return 1;
	}

	if (sys_outb(RTC_DATA_REG, value)) {
		printf("rtc_write_register -> Failed sys_inb().\n");
		return 1;
	}

	return OK;
}

int rtc_updating(void) {
	int regA;

	if ((regA = rtc_read_register(AL_REGA)) != -1) {
		if (regA & A_UIP)
			return OK;
		else
			return 1;
	}

	return OK;
}

Date_t * rtc_read_date(void) {
	//Date Initialization
	Date_t * date = malloc(sizeof(Date_t));
	date->year = 3;

	if (rtc_updating()) {

		if ((date->minute = parserBCD(rtc_read_register(AL_MINUTES))) == -1) {
			printf("rtc_read_date -> Failed rtc_read_register() - minute.\n");
			date->year = 0;
		}

		if ((date->hour = parserBCD(rtc_read_register(AL_HOURS))) == -1) {
			printf("rtc_read_date -> Failed rtc_read_register() - hour.\n");
			date->year = 0;
		}

		if ((date->day = parserBCD(rtc_read_register(AL_DAY))) == -1) {
			printf("rtc_read_date -> Failed rtc_read_register() - day.\n");
			date->year = 0;
		}

		if ((date->month = parserBCD(rtc_read_register(AL_MONTH))) == -1) {
			printf("rtc_read_date -> Failed rtc_read_register() - month.\n");
			date->year = 1;
		}

		if ((date->year = parserBCD(rtc_read_register(AL_YEAR))) == -1) {
			printf("rtc_read_date -> Failed rtc_read_register() - year.\n");
			date->year = 2;
		}

	}
	return date;
}

