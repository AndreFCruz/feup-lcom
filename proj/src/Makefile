# Makefile for Planetary Defense (game project)

COMPILER_TYPE= gnu

CC= gcc

PROG= planetary
SRCS= main.c planetary.c vbe.c video_gr.c timer.c keyboard.c mouse.c GVector.c Input.c Missile.c Bitmap.c

CCFLAGS= -Wall

DPADD+= ${LIBDRIVER} ${LIBSYS}
LDADD+= -llm -ldriver -lsys

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
