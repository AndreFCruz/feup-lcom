#ifndef __MISSILE_H
#define __MISSILE_H

#include <stdint.h>
#include "Bitmap.h"

struct missile_t;
typedef struct missile_t Missile;

struct explosion_t;
typedef struct explosion_t Explosion;

/* Missile's Methods */
Missile * new_emissile();
Missile * new_fmissile(const unsigned * init_pos, const unsigned * mouse_pos);

size_t missile_getSizeOf();
unsigned missile_getPosX(Missile * ptr);
unsigned missile_getPosY(Missile * ptr);
unsigned missile_getInitX(Missile * ptr);
unsigned missile_getInitY(Missile * ptr);
uint16_t missile_getColor(Missile * ptr);
int missile_update(Missile * ptr);
int missile_isFriendly(Missile * ptr);
Explosion * delete_missile(Missile * ptr);

//Colisions
int missile_atExplosion(Missile * ptr, Explosion * e_ptr);	// TODO MOVE THIS FUNCTION ELSEWHERE

/* Explosion's Methods */
Explosion * new_explosion(const unsigned * position);

int explosion_update(Explosion * ptr);

Bitmap * explosion_getBitmap(Explosion * ptr);

unsigned explosion_getPosX(Explosion * ptr);
unsigned explosion_getPosY(Explosion * ptr);

size_t explosion_getSizeOf();

void delete_explosion(Explosion * ptr);


#endif
