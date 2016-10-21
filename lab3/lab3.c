#include "test3.h"
#include "i8042.h"
#include <limits.h>
#include <string.h>
#include <errno.h>


// ALTERAR PARA TER EM CONTA LAB3 !!

static int proc_args(int argc, char **argv);
static unsigned long parse_ulong(char *str, int base);
static void print_usage(char **argv);

int main(int argc, char **argv)
{
	sef_startup();	// synchronize with RS

	if (argc == 1) {					/* Prints usage of the program if no arguments are passed */
		print_usage(argv);
		return 0;
	}
	else return proc_args(argc, argv);
}

static void print_usage(char **argv)
{
	printf("Usage: one of the following:\n"
			//ALTERAR
			"\t service run %s -args \"scan <decimal no.- mode>\"\n"
			"\t service run %s -args \"leds <decimal no. - elements>\"\n" /
			"\t service run %s -args \"timed scan <decimal no. - time>\"\n",
			/// /// ///
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
		mode = parse_ulong(argv[2], 10);						/* Parses string to unsigned long */
		if (mode == ULONG_MAX)
			return 1;
		printf("keyboard::kbd_test_scan(%lu)\n", mode);
		return timer_test_config(mode);
	}
	else if (strncmp(argv[1], "leds", strlen("leds")) == 0) {
		if (argc < 3) {
			printf("keyboard: wrong no. of arguments for kbd_test_leds()\n");
			return 1;
		}
		unsigned nElements = argc - 2;							/* Number of arguments in the function*/
		int toggles[nElements];

		for (int i = 0; i < nElements; i++) {					/* Getting the arguments into an array*/
			toggles[i] = parse_ulong (argv[i+2], 10);
			if (toggles[i] < 0 || toggles[i] > 2)				/* There are only 3 leds (0, 1, 2)*/
			{
				printf("keyboard: arguments for kbd_test_leds() must be 0, 1 or 2\n");
				return 1;
			}
		}
		printf("keyboard::kbd_test_leds(%lu)\n", nElements, & toggles);
		return kbd_test_leds(nElems, & toggles);
	}
	else if (strncmp(argv[1], "time", strlen("time")) == 0) {
		if (argc != 3) {
			printf("keyboard: wrong no of arguments for kbd_test_timed_scan()\n");
			return 1;
		}
		time = parse_ulong(argv[2], 10);						/* Parses string to unsigned long */
		if (time == ULONG_MAX)
			return 1;
		printf("keyboard::kbd_test_timed_scan(%lu)\n", time);
		return kbd_test_timed_scan(time);
	}
	else {
		printf("keyboard: %s - no valid function!\n", argv[1]);
		return 1;
	}
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
