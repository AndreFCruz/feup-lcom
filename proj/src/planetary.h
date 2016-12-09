#ifndef __PLANETARY_H
#define __PLANETARY_H

typedef enum {MENU, GAME_SINGLE, GAME_MULTI} state_t;
typedef enum {START_SINGLE, START_MULTI, CREDITS, EXIT} event_t;

int game_loop();

#endif
