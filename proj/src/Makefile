# Makefile for lab5

COMPILER_TYPE= gnu

CC= gcc

PROG= planetary
SRCS= planetary.c vbe.c video_gr.c timer.c keyboard.c read_xpm.c Input.c

CCFLAGS= -Wall

DPADD+= ${LIBDRIVER} ${LIBSYS}
LDADD+= -llm -ldriver -lsys

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
