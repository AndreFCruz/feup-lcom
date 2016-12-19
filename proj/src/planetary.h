#ifndef __PLANETARY_H
#define __PLANETARY_H

#include "Bitmap.h"

#define OK		0
#define ERROR	1

#define NUM_EXPLOSION_BMPS	16

#define L_BASE_X	50
#define R_BASE_X	750
#define BASE_Y		550

#define BUTTONS_X		211
#define SINGLEP_Y		231
#define MULTIP_Y		334
#define HIGHS_Y			445
#define	BUTTONS_WIDTH	376
#define BUTTONS_HEIGHT	82
#define EXIT_X			759
#define EXIT_Y			566
#define EXIT_RADIUS		17

typedef enum {MENU, GAME_SINGLE, GAME_MULTI, HIGH_SCORES /* CREDITS ? */} game_state_t;
//typedef enum {START_SINGLE, START_MULTI, EPSILON /* CREDITS ? */} event_t;

Bitmap ** game_getExplosionBmps();

int timer_handler();

#endif
