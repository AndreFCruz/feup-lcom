#ifndef __MISSILE_H
#define __MISSILE_H

#include <stdint.h>

struct missile_t;
typedef struct missile_t Missile;

struct explosion_t;
typedef struct explosion_t Explosion;

/* Missile's Methods */
Missile * new_emissile();
Missile * new_fmissile(const unsigned * init_pos, const int * mouse_pos);

size_t missile_getSizeOf();
unsigned missile_getPosX(Missile * ptr);
unsigned missile_getPosY(Missile * ptr);
unsigned missile_getInitX(Missile * ptr);
unsigned missile_getInitY(Missile * ptr);
uint16_t missile_getColor(Missile * ptr);
void missile_update(Missile * ptr);
int missile_isFriendly(Missile * ptr);
Explosion * delete_missile(Missile * ptr);

/* Explosion's Methods */
Explosion * new_explosion(const unsigned * position);
void delete_explosion(Explosion * ptr);


#endif
