#include <limits.h>
#include <string.h>
#include <errno.h>
#include "test3.h"



static int proc_args(int argc, char **argv);
static unsigned long parse_ulong(char *str, int base);
static void print_usage(char **argv);

int main(int argc, char **argv)
{
	sef_startup();	// synchronize with RS

	if (argc == 1) {				/* Prints usage of the program if no arguments are passed */
		print_usage(argv);
		return 0;
	}
	else return proc_args(argc, argv);
}

static void print_usage(char **argv)
{
	printf("Usage: one of the following:\n"
			"\t service run %s -args \"scan <decimal no.- mode>\"\n"
			"\t service run %s -args \"leds <decimal no. - elements>\"\n"
			"\t service run %s -args \"timed scan <decimal no. - time>\"\n",
			argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char **argv)
{
	unsigned short mode, elements, time;

	if (strncmp(argv[1], "scan", strlen("scan")) == 0) {
		if (argc != 3) {
			printf("keyboard: wrong no. of arguments for kbd_test_scan()\n");
			return 1;
		}
		mode = parse_ulong(argv[2], 10);
		if (mode == ULONG_MAX || mode > 1)
			return 1;
		printf("keyboard::kbd_test_scan(%lu)\n", mode);
		kbd_test_scan(mode);
	}
	else if (strncmp(argv[1], "leds", strlen("leds")) == 0) {
		if (argc < 3) {
			printf("keyboard: wrong no. of arguments for kbd_test_leds()\n");
			return 1;
		}
		unsigned short nElements = argc - 2;
		unsigned short toggles[nElements];

		unsigned short i;
		for (i = 0; i < nElements; i++) {			/* Getting command line arguments into an array */
			toggles[i] = parse_ulong (argv[i+2], 10);
		}
		printf("keyboard::kbd_test_leds(%d, toggles)\n", nElements);
		kbd_test_leds(nElements, toggles);
	}
	else if (strncmp(argv[1], "time", strlen("time")) == 0) {
		if (argc != 4) {
			printf("keyboard: wrong no of arguments for kbd_test_timed_scan()\n");
			return 1;
		}
		time = parse_ulong(argv[3], 10);
		if (time == ULONG_MAX)
			return 1;
		printf("keyboard::kbd_test_timed_scan(%lu)\n", time);
		kbd_test_timed_scan(time);
	}
	else {
		printf("keyboard: %s - no valid function!\n", argv[1]);
		return 1;
	}
	return 0;	//OK
}

static unsigned long parse_ulong(char *str, int base)
{
	char *endptr;
	unsigned long val;

	/* Convert string to unsigned long */
	val = strtoul(str, &endptr, base);			

	/* Check for conversion errors */
	if ((errno == ERANGE && val == ULONG_MAX) || (errno != 0 && val == 0)) {
		perror("strtoul");
		return ULONG_MAX;
	}

	if (endptr == str) {
		printf("keyboard: parse_ulong: no digits were found in %s\n", str);
		return ULONG_MAX;
	}

	/* Successful conversion */
	return val;
}
