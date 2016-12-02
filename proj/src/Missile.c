#include "Missile.h"

/**
 * Structs
 */

typedef struct {
	unsigned init_pos[2];	// Initial Position for Missile Trail
	unsigned pos[2];		// Current Position

	float velocity[2];		// Velocity, in pixels PER frame
	float pending_movement[2];	// Movement left unperformed on the previous frame

	unsigned color;

} missile_t;


// Additional Functionality
typedef struct {
	missile_t * missile;	// "Inherits" from missile_t

	unsigned end_pos[2];	// End position (provided by a mouse click)

} friendly_missile_t;


typedef struct {
	unsigned pos[2];
	unsigned radius;

	unsigned life_span;

	Bitmap ** sprites;
	unsigned num_frames;
	unsigned frames_per_sprite;

} explosion_t;

/**
 * END of Structs
 */


// Rounds float to nearest integer
int round_float(float f) {
	return (f > (floor(f) + 0.5)) ? ceil(f) : floor(f);
}


/**
 * Methods for missile_t
 */

missile_t * new_missile(unsigned * init_pos, float * vel) {
	missile_t * self = malloc(sizeof(missile_t));

	self->trail_pos = init_pos;
	self->pos = init_pos;

	self->velocity = vel;

	self->color = 4;

	return self;
}

explosion_t * delete_missile(missile_t * ptr) {
	explosion_t * exp = new_explosion(ptr->pos);

	free(ptr);

	return exp;
}

void missile_update(missile_t * ptr) {

	//Update Pending Movement
	ptr->pending_movement[0] += ptr->velocity[0];
	ptr->pending_movement[1] += ptr->velocity[1];

	//Update Position
	int tmp;
	if ( (tmp = round_float(ptr->pending_update[0])) > 1 ) {
		ptr->pos[0] += tmp;
		ptr->pending_movement[0] -= tmp;
	}
	if ( (tmp = round_float(ptr->pending_update[1])) > 1 ) {
		ptr->pos[1] += tmp;
		ptr->pending_movement[1] -= tmp;
	}
}



/**
 * Methods for explosion_t
 */

explosion_t * new_explosion(unsigned * position) {
	explosion_t * self = malloc(sizeof(explosion_t));

	self->pos = position;


}







