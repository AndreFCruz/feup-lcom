#ifndef __PLANETARY_H
#define __PLANETARY_H

#include "Bitmap.h"

#define OK		0
#define ERROR	1

#define L_BASE_X	50
#define R_BASE_X	750
#define BASE_Y		550

typedef enum {MENU, GAME_SINGLE, GAME_MULTI, HIGH_SCORES /* CREDITS ? */} game_state_t;
//typedef enum {START_SINGLE, START_MULTI, EPSILON /* CREDITS ? */} event_t;

int timer_handler();

#endif
