#ifndef __PLANETARY_H
#define __PLANETARY_H

#include "Bitmap.h"

#define OK		0

#define NUM_BASES		3
#define CANNON_POS_Y	530
#define LEFT_CANNON_POS_X	5	// TODO Check Both
#define RIGHT_CANNON_POS_X	795
#define CANNON_PROJECTILE_OFFSET	35

#define GROUND_Y		585

typedef enum {MENU, GAME_SINGLE, GAME_MULTI, HIGH_SCORES /* CREDITS ? */} game_state_t;
//typedef enum {START_SINGLE, START_MULTI, EPSILON /* CREDITS ? */} event_t;

int timer_handler();

#endif
