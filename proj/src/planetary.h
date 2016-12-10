#ifndef __PLANETARY_H
#define __PLANETARY_H

typedef enum {MENU, GAME_SINGLE, GAME_MULTI /* CREDITS ?? */} state_t;
typedef enum {START_SINGLE, START_MULTI, EPSILON /* CREDITS ?? */} event_t;


int timer_handler();

#endif
