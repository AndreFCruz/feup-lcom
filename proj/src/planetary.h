#ifndef __PLANETARY_H
#define __PLANETARY_H

#define L_BASE_X	50
#define R_BASE_X	750
#define BASE_Y		550

typedef enum {MENU, GAME_SINGLE, GAME_MULTI /* CREDITS ? */} state_t;
typedef enum {START_SINGLE, START_MULTI, EPSILON /* CREDITS ? */} event_t;

int timer_handler();

#endif
