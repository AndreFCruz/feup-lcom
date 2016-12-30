#ifndef __MISSILE_H
#define __MISSILE_H

/** @defgroup Missile Missile
 * @{
 * Functions for manipulating both the Missiles and the Explosions
 */

#include <stdint.h>
#include "Bitmap.h"

struct missile_t;
typedef struct missile_t Missile;

struct explosion_t;
typedef struct explosion_t Explosion;

/* Missile's Methods */

/**
 * @brief Generates a new enemy missile
 *
 * @param bases_pos Array containing the position of the bases, so the missile can randomly target one
 *
 * @return Pointer to the the newly created enemy missile
 */
Missile * new_emissile(const unsigned * bases_pos);

/**
 * @brief Generates a new friendly missile
 *
 * @param init_pos Array containing the initial position of the missile (x,y), associated to the cannon that fired it.
 * @param mouse_pos Array containing the mouse position where the friendly missile will explode
 *
 * @return Pointer to the newly created friendly missile
 */
Missile * new_fmissile(const int * init_pos, const int * mouse_pos);

/**
 * @brief Size of the space of memory occupied by a Missile	TODO: Recheck
 *
 * @return Size of the space of memory occupied by a Missile
 */
size_t missile_getSizeOf();

/**
 * @brief Gets the position in the horizontal axis of the Missile, on the screen
 *
 * @param ptr Pointer to the Missile in question
 *
 * @return position in the horizontal axis of the Missile, on the screen
 */
int missile_getPosX(Missile * ptr);

/**
 * @brief Gets the position in the vertical axis of the Missile, on the screen
 *
 * @param ptr Pointer to the Missile in question
 *
 * @return position in the vertical axis of the Missile, on the screen
 */
int missile_getPosY(Missile * ptr);

/**
 * @brief Gets the initial position in the horizontal axis of the Missile, on the screen
 *
 * @param ptr Pointer to the Missile in question
 *
 * @return intial position in the horizontal axis of the Missile, on the screen
 */
int missile_getInitX(Missile * ptr);

/**
 * @brief Gets the initial position in the vertical axis of the Missile, on the screen
 *
 * @param ptr Pointer to the Missile in question
 *
 * @return intial position in the vertical axis of the Missile, on the screen
 */
int missile_getInitY(Missile * ptr);

/**
 * @brief Gets the color of the Missile
 *
 * @param ptr Pointer to the Missile in question
 *
 * @return Color of the Missile
 */
uint16_t missile_getColor(Missile * ptr);

/**
 * @brief Updates the Missile,  more specifically its position on the screen
 *
 * @param ptr Pointer to the Missile in question
 *
 * @return In case a friendly missile has reached it's end position returns 1, otherwise returns 0;
 */
int missile_update(Missile * ptr);

/**
 * @brief Checks if a Missile is friendly
 *
 * @param ptr Pointer to the Missile in question
 *
 * @return Return 1 upon success, 0 otherwise
 */
int missile_isFriendly(Missile * ptr);

/**
 * @brief Destroys a missile, freeing all the resources used by it
 *
 * @param ptr Pointer to the Missile in question
 *
 * @return Pointer to an explosion that will start at the last position of the missile
 */
Explosion * delete_missile(Missile * ptr);

/* Explosion's Methods */

/**
 * @brief Creates an explosion at a given position
 *
 * @param position Array containing the position where the explosion starts (x,y)
 *
 * @return Pointer to the newly created explosion
 */
Explosion * new_explosion(const int * position);

/**
 * @brief Updates the Explosion, more specifically the radius of the explosion its animation
 *
 * @param ptr Pointer to the Explosion in question
 *
 * @return Return 1 if explosion has ended, 0 otherwise
 */
int explosion_update(Explosion * ptr);

/**
 * @brief Gets the current Bitmap of the Explosion
 *
 * @param ptr Pointer to the Explosion in question
 *
 * @return Return the current Bitmap of the Explosion
 */
Bitmap * explosion_getBitmap(Explosion * ptr);

/**
 * @brief Gets the position in the horizontal axis of the Explosion, on the screen
 *
 * @param ptr Pointer to the Explosion in question
 *
 * @return position in the horizontal axis of the Explosion, on the screen
 */
int explosion_getPosX(Explosion * ptr);

/**
 * @brief Gets the position in the vertical axis of the Explosion, on the screen
 *
 * @param ptr Pointer to the Explosion in question
 *
 * @return position in the vertical axis of the Explosion, on the screen
 */
int explosion_getPosY(Explosion * ptr);

/**
 * @brief Size of the space of memory occupied by a Explosion	TODO: Recheck
 *
 * @return Size of the space of memory occupied by a Explosion
 */
size_t explosion_getSizeOf();

/**
 * @brief Destroys a Explosion, freeing all the resources used by it
 *
 * @param ptr Pointer to the Explosion in question
 */
void delete_explosion(Explosion * ptr);

/* Collision */

/**
 * @brief Check if a Missile collided with a certain Explosion
 *
 * @param ptr Pointer to the Missile in question
 * @param e_ptr Pointer to the Explosion in question
 *
 * @return Return 1 if Collision happened, 0 otherwise
 */
int missile_collidedWithExplosion(Missile * ptr, Explosion * e_ptr);

/**
 * @brief Check if a Missile collided with a Rectangle
 *
 * @param ptr Pointer to the Missile in question
 * @param posX Initial position of the rectangle in the horizontal axis
 * @param posY Initial position of the rectangle in the vertical axis
 * @param sizeX Width of the rectangle
 * @param sizeY Height of the rectangle
 *
 * @return Return 1 if Collision happened, 0 otherwise
 */
int missile_collidedWithRect(Missile * ptr, unsigned posX, unsigned posY,
		unsigned sizeX, unsigned sizeY);

#endif
