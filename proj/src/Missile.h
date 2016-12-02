#ifndef __MISSILE_H
#define __MISSILE_H

struct Missile;
typedef struct Missile missile_t;

unsigned missile_getXPos(Missile * ptr);
unsigned missile_getYPos(Missile * ptr);
void missile_update(Missile * ptr);


#endif
