#include <math.h>
#include <stdlib.h>
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

	uint16_t color;			// Color in RGB 5:6:5

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
static Missile * new_missile(const unsigned * init_pos, const float * vel) {
	Missile * self = (Missile *) malloc(sizeof(Missile));

	memcpy(self->init_pos, init_pos, 2 * sizeof(unsigned));
	memcpy(self->pos, init_pos, 2 * sizeof(unsigned));

	memcpy(self->velocity, vel, 2 * sizeof(float));

	self->pending_movement[0] = 0.;
	self->pending_movement[1] = 0.;

	self->color = 0XFFFF;	// White, to be changed on enemy/friendly constructor

	return self;
}

// Constructor for Enemy Missile
Missile * new_emissile() {
	unsigned init_pos[2] = {rand() % (vg_getHorRes() - 100) + 50, 0};
	float vel[2] = { 1.0 / (1 + rand() % 2), 2.0 / (1 + rand() % 3)};
	Missile * self = new_missile(init_pos, vel);
	self->color = RED;
	self->isFriendly = FALSE;
	return self;
}

// Constructor for Friendly Missile
Missile * new_fmissile(const unsigned * init_pos, const float * vel, const unsigned * mouse_pos) {
	Missile * self = new_missile(init_pos, vel);
	self->color = YELLOW;
	self->isFriendly = TRUE;
	memcpy(self->end_pos, mouse_pos, 2 * sizeof(unsigned));
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

unsigned missile_getInitX(Missile * ptr) {
	return ptr->init_pos[0];
}

unsigned missile_getInitY(Missile * ptr) {
	return ptr->init_pos[1];
}

uint16_t missile_getColor(Missile * ptr) {
	return ptr->color;
}


/**
 * Methods for Explosion
 */

Explosion * new_explosion(const unsigned * position) {
	Explosion * self = (Explosion *) malloc(sizeof(Explosion));

//	self->pos = position;
	memcpy(self->pos, position, 2 * sizeof(unsigned));
	self->curr_frame = 0;

	// TODO
	// Initiate Bitmaps and number of frames

	return self;
}


void delete_explosion(Explosion * ptr) {
	free(ptr);
}





