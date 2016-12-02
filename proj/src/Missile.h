#ifndef __MISSILE_H
#define __MISSILE_H

struct missile_t;
typedef struct missile_t Missile;

struct explosion_t;
typedef struct explosion_t Explosion;

/* Missile's Methods */
Missile * new_emissile(unsigned * init_pos, float * vel);
Missile * new_fmissile(unsigned * init_pos, float * vel, unsigned * mouse_pos);

unsigned missile_getXPos(Missile * ptr);
unsigned missile_getYPos(Missile * ptr);
void missile_update(Missile * ptr);

Explosion * delete_missile(Missile * ptr);

/* Explosion's Methods */



#endif
