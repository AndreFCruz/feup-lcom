#include <limits.h>
#include <string.h>
#include <errno.h>
#include "test5.h"
#include "pixmap.h"

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
			"\t service run %s -args \"init <decimal no.- mode> <decimal no.- delay>\"\n"
			"\t service run %s -args \"square <decimal no. - x_begin> <decimal no. - y_begin> <decimal no. - size> <decimal no. - color>\"\n"
			"\t service run %s -args \"line <decimal no. - x_begin> <decimal no. - y_begin> <decimal no. - x_end> <decimal no. - y_end> <decimal no. - color>\"\n"
			"\t service run %s -args \"xpm <decimal no. - x_begin> <decimal no. - y_begin> <char no. - xpm_pointer>\"\n"
			"\t service run %s -args \"move <decimal no. - x_begin> <decimal no. - y_begin > <char no. - xpm_pointer> <decimal no. - direction> <decimal no. - number of pixels> <decimal no. - time>\"\n"
			"\t service run %s -args \"controller\"\n",
			argv[0], argv[0], argv[0], argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char **argv)
{
	unsigned long mode, time, color, xbegin, ybegin, size, xend, yend, hor;
	long delta;

	if (strncmp(argv[1], "init", strlen("init")) == 0) {
		if (argc != 4) {
			printf("lab5: wrong no. of arguments for test_init()\n");
			return 1;
		}
		mode = parse_ulong(argv[2], 16);
		if (mode > USHRT_MAX) {
			printf("lab5: mode exceeds USHRT_MAX, was %u", mode);
			return 1;
		}

		time = parse_ulong(argv[3], 10);
		if (time > USHRT_MAX) {
			printf("lab5: time exceeds USHRT_MAX, was %u", time);
			return 1;
		}

		printf("lab5::test_init(%X, %lu)\n", mode, time);
		if ( test_init(mode, time) == NULL )
			printf("TestInit Returned NULL.\n");
	}
	else if (strncmp(argv[1], "square", strlen("square")) == 0) {
		if (argc != 6) {
			printf("lab5: wrong no. of arguments for test_square()\n");
			return 1;
		}

		if ( (xbegin = parse_ulong(argv[2], 10)) > USHRT_MAX )
			return 1;

		if ( (ybegin = parse_ulong(argv[3], 10)) > USHRT_MAX )
			return 1;

		if ( (size = parse_ulong(argv[4], 10)) > USHRT_MAX )
			return 1;

		if ( (color = parse_ulong(argv[5], 16)) == ULONG_MAX )
			return 1;

		printf("lab5::test_square(%lu, %lu, %lu, %x)\n", xbegin, ybegin, size, color);
		return test_square(xbegin, ybegin, size, color);
	}
	else if (strncmp(argv[1], "line", strlen("line")) == 0) {
		if (argc != 7) {
			printf("lab5: wrong no of arguments for test_line()\n");
			return 1;
		}
		if ( (xbegin = parse_ulong(argv[2], 10)) > USHRT_MAX )
			return 1;

		if ( (ybegin = parse_ulong(argv[3], 10)) > USHRT_MAX )
			return 1;

		if ( (xend = parse_ulong(argv[4], 10)) > USHRT_MAX )
			return 1;

		if ( (yend = parse_ulong(argv[5], 10)) > USHRT_MAX )
			return 1;

		if ( (color = parse_ulong(argv[6], 10)) == ULONG_MAX )
			return 1;

		printf("lab5::test_line(%lu, %lu, %lu, %lu, %lu)\n", xbegin, ybegin, xend, yend, color);
		return test_line(xbegin, ybegin, xend, yend, color);
	}
	else if (strncmp(argv[1], "xpm", strlen("xpm")) == 0) {
		if (argc != 5) {
			printf("lab5: wrong no of arguments for test_xpm()\n");
			return 1;
		}

		if ( (xbegin = parse_ulong(argv[2], 10)) > USHRT_MAX)
			return 1;

		if ( (ybegin = parse_ulong(argv[3], 10)) > USHRT_MAX)
			return 1;

		char ** image;
		// Comparar argc[4] com o nome das varias imagens
		if ( strncmp(argv[4], "pic1", strlen("pic1")) == 0 ) {
			image = pic1;
		} else if ( strncmp(argv[4], "pic2", strlen("pic2")) == 0 ) {
			image = pic2;
		} else if ( strncmp(argv[4], "cross", strlen("cross")) == 0 ) {
			image = cross;
		} else if ( strncmp(argv[4], "pic3", strlen("pic3")) == 0 ) {
			image = pic3;
		} else if ( strncmp(argv[4], "penguin", strlen("penguin")) == 0 ) {
			image = penguin;
		} else {
			printf("lab5: there's no such image.\n");
			return 1;
		}

		printf("lab5::test_xpm(%lu, %lu, %s)\n", xbegin, ybegin , argv[4]);
		return test_xpm(xbegin, ybegin, image);
	}
	else if (strncmp(argv[1], "move", strlen("move")) == 0) {
		if (argc != 8) {
			printf("lab5: wrong no of arguments for test_move()\n");
			return 1;
		}

		if ( (xbegin = parse_ulong(argv[2], 10)) > USHRT_MAX)
			return 1;
		if ( (ybegin = parse_ulong(argv[3], 10)) > USHRT_MAX)
			return 1;
		if ( (hor = parse_ulong(argv[5], 10)) > USHRT_MAX)
			return 1;
		if ( (delta = parse_long(argv[6], 10)) > SHRT_MAX || delta < SHRT_MIN)
			return 1;
		if ( (time = parse_ulong(argv[7], 10)) > USHRT_MAX)
			return 1;

		char ** image;
		// Comparar argc[4] com o nome das varias imagens
		if ( strncmp(argv[4], "pic1", strlen("pic1")) == 0 ) {
			image = pic1;
		} else if ( strncmp(argv[4], "pic2", strlen("pic2")) == 0 ) {
			image = pic2;
		} else if ( strncmp(argv[4], "cross", strlen("cross")) == 0 ) {
			image = cross;
		} else if ( strncmp(argv[4], "pic3", strlen("pic3")) == 0 ) {
			image = pic3;
		} else if ( strncmp(argv[4], "penguin", strlen("penguin")) == 0 ) {
			image = penguin;
		} else {
			printf("lab5: there's no such image.\n");
			return 1;
		}

		printf("lab5::test_move(%lu, %lu, %s, %lu, %lu, %lu)\n", xbegin, ybegin, argv[4], hor, delta, time);
		return test_move(xbegin, ybegin, image, hor, delta, time);
	}
	else if (strncmp(argv[1], "controller", strlen("controller")) == 0) {
		if (argc != 2) {
			printf("lab5: wrong no of arguments for test_controller()\n");
			return 1;
		}

		printf("lab5::test_controller()\n");
		return test_controller();
	}
	else {
		printf("lab5: %s - no valid function!\n", argv[1]);
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
		printf("lab5: parse_ulong: no digits were found in %s\n", str);
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
		printf("lab5: parse_long: no digits were found in %s\n", str);
		return LONG_MAX;	//Could also be LONG_MIN
	}

	/* Successful conversion */
	return val;
}
