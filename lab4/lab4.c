#include <limits.h>
#include <string.h>
#include <errno.h>
#include "test4.h"

static int proc_args(int argc, char **argv);
static unsigned long parse_ulong(char *str, int base);
static signed long parse_long(char *str, int base);
static void print_usage(char **argv);

int main(int argc, char **argv)
{
	sef_startup();

	if (argc == 1) {					/* Prints usage of the program if no arguments are passed */
		print_usage(argv);
		return 0;
	}
	else return proc_args(argc, argv);
}

static void print_usage(char **argv)
{
	printf("Usage: one of the following:\n"
			"\t service run %s -args \"packet <decimal no.- number of packets>\"\n"
			"\t service run %s -args \"async <decimal no. - time>\"\n"
			"\t service run %s -args \"config\"\n"
			"\t service run %s -args \"gesture <decimal no. - length>\"\n",
			argv[0], argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char **argv)
{
	unsigned long nPackets, time;
	long length;

	if (strncmp(argv[1], "packet", strlen("packet")) == 0) {
		if (argc != 3) {
			printf("mouse: wrong no. of arguments for test_packet()\n");
			return 1;
		}
		nPackets = parse_ulong(argv[2], 10);						/* Parses string to unsigned long */
		if (nPackets == ULONG_MAX)
			return 1;
		printf("mouse::test_packet(%lu)\n", nPackets);
		return test_packet(nPackets);
	}
	else if (strncmp(argv[1], "async", strlen("async")) == 0) {
		if (argc != 3) {
			printf("mouse: wrong no. of arguments for test_async()\n");
			return 1;
		}
		time = parse_ulong(argv[2], 10);						/* Parses string to unsigned long */
		if (time == ULONG_MAX)
			return 1;
		printf("mouse::test_async(%lu)\n", time);
		return test_async(time);
	}
	else if (strncmp(argv[1], "config", strlen("config")) == 0) {
		if (argc != 2) {
			printf("mouse: wrong no of arguments for test_config()\n");
			return 1;
		}
		printf("mouse::test_config()\n");
		return test_config();
	}
	else if (strncmp(argv[1], "gesture", strlen("gesture")) == 0) {
		if (argc != 3) {
			printf("mouse: wrong no of arguments for test_gesture()\n");
			return 1;
		}
		length = parse_long(argv[2], 10);					/* Parses string to signed long */
		if (length == LONG_MAX || length == LONG_MIN)
			return 1;
		if (length < SHRT_MIN || length > SHRT_MAX) {
			printf("Invalid length. Must be within short int range [-32768, +32767]. Was %d.\n", length);
			return 1;
		}
		printf("mouse::test_geture(%d)\n", length);
		return test_gesture(length);
	}
	else {
		printf("mouse: %s - no valid function!\n", argv[1]);
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
		printf("mouse: parse_ulong: no digits were found in %s\n", str);
		return ULONG_MAX;
	}

	/* Successful conversion */
	return val;
}

static signed long parse_long(char *str, int base)
{
	char *endptr;
	unsigned long val;

	/*Convert string to signed long*/
	val = strtol(str, &endptr, base);

	/* Check for conversion errors */
	if ((errno == ERANGE && val == LONG_MAX) || (errno != 0 && val == 0)) {
		perror("strtol");
		return LONG_MAX;
	}
	if ((errno == ERANGE && val == LONG_MIN) || (errno != 0 && val == 0)) {
		perror("strtol");
		return LONG_MIN;
	}

	if (endptr == str) {
		printf("mouse: parse_long: no digits were found in %s\n", str);
		return LONG_MAX;	//Could also be LONG_MIN
	}

	/* Successful conversion */
	return val;
}
