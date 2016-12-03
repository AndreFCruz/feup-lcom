#ifndef __MISSILE_H
#define __MISSILE_H

struct missile_t;
typedef struct missile_t Missile;

struct explosion_t;
typedef struct explosion_t Explosion;

/* Missile's Methods */
Missile * new_emissile(const unsigned * init_pos, const float * vel);
Missile * new_fmissile(const unsigned * init_pos, const float * vel, const unsigned * mouse_pos);

unsigned missile_getXPos(Missile * ptr);
unsigned missile_getYPos(Missile * ptr);
void missile_update(Missile * ptr);

Explosion * delete_missile(Missile * ptr);

/* Explosion's Methods */
Explosion * new_explosion(const unsigned * position);
void delete_explosion(Explosion * ptr);


#endif
