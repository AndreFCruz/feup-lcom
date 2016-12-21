#ifndef __PLANETARY_H
#define __PLANETARY_H

#include "Bitmap.h"

#define OK		0

#define NUM_BASES		3
#define CANNON_POS_Y	550
#define LEFT_CANNON_POS_X	10	// TODO Check Both
#define RIGHT_CANNON_POS_X	790

#define GROUND_Y		590

typedef enum {MENU, GAME_SINGLE, GAME_MULTI, HIGH_SCORES /* CREDITS ? */} game_state_t;
//typedef enum {START_SINGLE, START_MULTI, EPSILON /* CREDITS ? */} event_t;

int timer_handler();

#endif
