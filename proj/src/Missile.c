#include "Missile.h"

/**
 * Structs
 */
typedef enum {TRUE, FALSE} bool;

struct missile_t {
	unsigned init_pos[2];	// Initial Position for Missile Trail
	unsigned pos[2];		// Current Position

	float velocity[2];		// Velocity, in pixels PER frame
	float pending_movement[2];	// Movement left unperformed on the previous frame

	unsigned color;

	/* Attributes for Friendly Missile */
	bool isFriendly;
	unsigned end_pos[2];

};


struct explosion_t {
	unsigned pos[2];
	unsigned radius;

	unsigned life_span;

//	Bitmap ** sprites;
	unsigned curr_frame;
	unsigned frames_per_sprite;

};

/**
 * END of Structs
 */


// Rounds float to nearest integer
int round_float(float f) {
	return (f > (floor(f) + 0.5)) ? ceil(f) : floor(f);
}


/**
 * Methods for Missile
 */

// Constructor for Generic Missile
static Missile * new_missile(const unsigned * init_pos, float * vel) {
	Missile * self = (Missile *) malloc(sizeof(Missile));

//	self->init_pos = init_pos;
	memcpy(self->init_pos, init_pos, 2 * sizeof(unsigned));	// TODO Update other array assignments
	self->pos = init_pos;

	self->velocity = vel;

	self->color = 3;	// FOR DEBUGGING -- should never be displayed with this color

	return self;
}

// Constructor for Enemy Missile
Missile * new_emissile(const unsigned * init_pos, float * vel) {
	Missile * self = new_missile(init_pos, vel);
	self->color = 4;
	self->isFriendly = FALSE;
	return self;
}

// Constructor for Friendly Missile
Missile * new_fmissile(unsigned * init_pos, float * vel, unsigned * mouse_pos) {
	Missile * self = new_missile(init_pos, vel);
	self->color = 6;
	self->isFriendly = TRUE;
	self->end_pos = mouse_pos;
	return self;
}


Explosion * delete_missile(Missile * ptr) {
	Explosion * exp = new_explosion(ptr->pos);

	free(ptr);

	return exp;
}

void missile_update(Missile * ptr) {

	//Update Pending Movement
	ptr->pending_movement[0] += ptr->velocity[0];
	ptr->pending_movement[1] += ptr->velocity[1];

	//Update Position
	int tmp;
	if ( (tmp = round_float(ptr->pending_movement[0])) > 1 ) {
		ptr->pos[0] += tmp;
		ptr->pending_movement[0] -= tmp;
	}
	if ( (tmp = round_float(ptr->pending_movement[1])) > 1 ) {
		ptr->pos[1] += tmp;
		ptr->pending_movement[1] -= tmp;
	}
}

int missile_isFriendly(Missile * ptr) {
	return (ptr->isFriendly == TRUE ? 1 : 0);
}

unsigned missile_getXPos(Missile * ptr) {
	return ptr->pos[0];
}

unsigned missile_getYPos(Missile * ptr) {
	return ptr->pos[1];
}

/**
 * Methods for Explosion
 */

Explosion * new_explosion(unsigned * position) {
	Explosion * self = (Explosion *) malloc(sizeof(Explosion));

	self->pos = position;
	self->curr_frame = 0;
	// Initiate Bitmaps and number of frames

	return self;
}


void delete_explosion(Explosion * ptr) {
	free(ptr);
}





