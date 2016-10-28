# Makefile for lab3

COMPILER_TYPE= gnu

CC= gcc

PROG= lab3
SRCS= lab3.c test3.c keyboard.c timer.c asm_kbd_ih.S

#CFLAGS= -std=c99
CCFLAGS= -Wall

DPADD+= ${LIBDRIVER} ${LIBSYS}
LDADD+= -ldriver -lsys

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
