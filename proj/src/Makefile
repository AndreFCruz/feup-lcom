# Makefile for Planetary Defense (game project)

COMPILER_TYPE= gnu

CC= gcc

PROG= planetary_defense
SRCS= main.c planetary.c vbe.c video_gr.c timer.c keyboard.c mouse.c GVector.c Input.c Missile.c Bitmap.c BMPsHolder.c RTC.c Highscores.c Serial.c rtc_asm.S

CCFLAGS= -Wall

DPADD+= ${LIBDRIVER} ${LIBSYS}
LDADD+= -llm -ldriver -lsys

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
