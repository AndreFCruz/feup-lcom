#ifndef __MISSILE_H
#define __MISSILE_H

#include <stdint.h>
#include "Bitmap.h"

struct missile_t;
typedef struct missile_t Missile;

struct explosion_t;
typedef struct explosion_t Explosion;

/* Missile's Methods */
Missile * new_emissile(const unsigned * bases_pos);
Missile * new_fmissile(const int * init_pos, const int * mouse_pos);

size_t missile_getSizeOf();
int missile_getPosX(Missile * ptr);
int missile_getPosY(Missile * ptr);
int missile_getInitX(Missile * ptr);
int missile_getInitY(Missile * ptr);
uint16_t missile_getColor(Missile * ptr);
int missile_update(Missile * ptr);
int missile_isFriendly(Missile * ptr);
Explosion * delete_missile(Missile * ptr);


/* Explosion's Methods */
Explosion * new_explosion(const int * position);

int explosion_update(Explosion * ptr);

Bitmap * explosion_getBitmap(Explosion * ptr);

int explosion_getPosX(Explosion * ptr);
int explosion_getPosY(Explosion * ptr);

size_t explosion_getSizeOf();

void delete_explosion(Explosion * ptr);


//Collisions
int missile_collidedWithExplosion(Missile * ptr, Explosion * e_ptr);
int missile_collidedWithRect(Missile * ptr, unsigned posX, unsigned posY, unsigned sizeX, unsigned sizeY);

#endif
