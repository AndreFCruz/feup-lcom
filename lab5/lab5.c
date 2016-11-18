#include <limits.h>
#include <string.h>
#include <errno.h>
#include "test5.h"

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

static void print_usage(char **argv)	//TODO: Re-Verificar se as chamadas são assim - principalmente a xpm
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
			printf("video card: wrong no. of arguments for test_init()\n");
			return 1;
		}
		mode = parse_ulong(argv[2], 16);						/* Parses string to unsigned long, in hex */
		if (mode > SHRT_MAX)
			return 1;

		time = parse_ulong(argv[3], 10);						/* Parses string to unsigned long, in decimal */
		if (time > SHRT_MAX)
			return 1;

		printf("video card::test_packet(%lu, %lu)\n", mode, time);
		if ( test_init(mode, time) == (void*) 0 )
			printf("TestInit Failed.\n");
	}
	else if (strncmp(argv[1], "square", strlen("square")) == 0) {
		if (argc != 6) {
			printf("video card: wrong no. of arguments for test_square()\n");
			return 1;
		}
		xbegin = parse_ulong(argv[2], 10);						/* Parsing all the arguments */
		if (xbegin > SHRT_MAX)
			return 1;

		ybegin = parse_ulong(argv[3], 10);
		if (ybegin > SHRT_MAX)
			return 1;

		size = parse_ulong(argv[4], 10);
		if (size > SHRT_MAX)
			return 1;

		color = parse_ulong(argv[5], 10);
		if (color == ULONG_MAX)
			return 1;

		printf("video card::test_async(%lu, %lu, %lu, %lu)\n", xbegin, ybegin, size, color);
		return test_square(xbegin, ybegin, size, color);
	}
	else if (strncmp(argv[1], "line", strlen("line")) == 0) {
		if (argc != 7) {
			printf("video card: wrong no of arguments for test_line()\n");
			return 1;
		}
		xbegin = parse_ulong(argv[2], 10);						/* Parsing all the arguments */
		if (xbegin > SHRT_MAX)
			return 1;

		ybegin = parse_ulong(argv[3], 10);
		if (ybegin > SHRT_MAX)
			return 1;

		xend = parse_ulong(argv[4], 10);
		if (xend > SHRT_MAX)
			return 1;

		yend = parse_ulong(argv[5], 10);
		if (yend > SHRT_MAX)
			return 1;

		color = parse_ulong(argv[6], 10);
		if (color == ULONG_MAX)
			return 1;

		printf("video card::test_config(%lu, %lu, %lu, %lu, %lu)\n", xbegin, ybegin, xend, yend, color);
		return test_line(xbegin, ybegin, xend, yend, color);
	}
	else if (strncmp(argv[1], "xpm", strlen("xpm")) == 0) {
		if (argc != 5) {
			printf("video card: wrong no of arguments for test_xpm()\n");
			return 1;
		}
		xbegin = parse_ulong(argv[2], 10);						/* Parsing all the arguments */
		if (xbegin > SHRT_MAX)
			return 1;

		ybegin = parse_ulong(argv[3], 10);
		if (ybegin > SHRT_MAX)
			return 1;

		//TODO:Falta aqui o controlo do pointer, perguntar amanha ao prof
		printf("video card::test_xpm(%lu, %lu)\n", xbegin, ybegin ,"char*"); //PLACEHOLDER CHAR*
//		return test_xpm(xbegin, ybegin, ...); // TODO CRAPPY PROGRAMMING
	}
	else if (strncmp(argv[1], "move", strlen("move")) == 0) {
		if (argc != 8) {
			printf("video card: wrong no of arguments for test_move()\n");
			return 1;
		}
		xbegin = parse_ulong(argv[2], 10);						/* Parsing all the arguments */
		if (xbegin > SHRT_MAX)
			return 1;

		ybegin = parse_ulong(argv[3], 10);
		if (ybegin > SHRT_MAX)
			return 1;

		//TODO: controlo do xpm aqui tb

		hor = parse_ulong(argv[5], 10);
		if (hor > SHRT_MAX)
			return 1;

		delta = parse_long(argv[6], 10);
		if (delta > SHRT_MAX || delta < SHRT_MIN)
			return 1;

		time = parse_ulong(argv[7], 10);
		if (time > SHRT_MAX)
			return 1;

		printf("video card::test_move(%d)\n", xbegin, ybegin, "char* PlaceHolder", hor, delta, time); //PLACEHOLDER CHAR*
//		return test_move(xbegin, ybegin, ..., hor, delta, time);	// TODO
	}
	else if (strncmp(argv[1], "controller", strlen("controller")) == 0) {
		if (argc != 2) {
			printf("video card: wrong no of arguments for test_controller()\n");
			return 1;
		}

		printf("video card::test_controller()\n");
		return test_controller();
	}
	else {
		printf("video card: %s - no valid function!\n", argv[1]);
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
		printf("video card: parse_ulong: no digits were found in %s\n", str);
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
		printf("video card: parse_long: no digits were found in %s\n", str);
		return LONG_MAX;	//Could also be LONG_MIN
	}

	/* Successful conversion */
	return val;
}
