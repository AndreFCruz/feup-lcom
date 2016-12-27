#ifndef __PLANETARY_H
#define __PLANETARY_H

#include "Bitmap.h"
#include "BMPsHolder.h"

#define OK		0

#define GROUND_Y					595

#define NUM_BASES					3
#define CANNON_POS_Y				GROUND_Y - CANNON_SIZE_Y
#define LEFT_CANNON_POS_X			5
#define RIGHT_CANNON_POS_X			795
#define CANNON_PROJECTILE_OFFSET	32


typedef enum {MENU, GAME_SINGLE, GAME_MULTI, HIGH_SCORES, END_GAME_ANIMATION} game_state_t;

int timer_handler();

#endif
