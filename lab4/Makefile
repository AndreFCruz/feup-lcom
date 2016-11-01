# Makefile for lab3

COMPILER_TYPE= gnu

CC= gcc

PROG= lab4
SRCS= lab4.c test4.c mouse.c

#CFLAGS= -std=c99
CCFLAGS= -Wall

DPADD+= ${LIBDRIVER} ${LIBSYS}
LDADD+= -ldriver -lsys

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
