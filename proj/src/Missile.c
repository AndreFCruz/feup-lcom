#include <math.h>
#include <stdlib.h>
#include <sys/types.h>
#include "Missile.h"
#include "video_gr.h"

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

	Bitmap ** bmps;
	unsigned curr_bmp_index;
	unsigned no_bmps;
	unsigned curr_frame;
	unsigned frames_per_bmp;
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
	printf("Missile Constructor Called\n");

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
	float vel[2] = { 1.0 / (rand() % 2 ? rand() % 8 + 1 : rand() % 8 - 8), 4.0 / (1 + rand() % 8)};
	//TODO find function that shoots left if missile is on the right of screen, and vice-verca (with rand() variations)

	Missile * self = new_missile(init_pos, vel);
	self->color = RED;
	self->isFriendly = FALSE;
	return self;
}

// Constructor for Friendly Missile
Missile * new_fmissile(const unsigned * init_pos, const unsigned * mouse_pos) {
	printf("new_fmissile Constructor Called\n");
	float vel[2] = {((int)mouse_pos[0] - (int)init_pos[0]) / 300., ((int)mouse_pos[1] - (int)init_pos[1]) / 300.};	//TODO Vel should not be time/frame based
	printf("new_fmissile vel calculated\n");
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

unsigned missile_getPosX(Missile * ptr) {
	return ptr->pos[0];
}

unsigned missile_getPosY(Missile * ptr) {
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

size_t missile_getSizeOf() {
	return sizeof(Missile);
}

/**
 * Methods for Explosion
 */

Explosion * new_explosion(const unsigned * position) {
	Explosion * self = (Explosion *) malloc(sizeof(Explosion));

	memcpy(self->pos, position, 2 * sizeof(unsigned));
	self->curr_frame = 0;
	self->curr_bmp_index = 0;
	self->frames_per_bmp = 5;
	self->no_bmps = 16;
	self->radius = 28;	// TODO Check

	self->bmps = game_getExplosionBmps();

	return self;
}

void delete_explosion(Explosion * ptr) {
	free(ptr);
}

int explosion_update(Explosion * ptr) { // TODO Check!
	if (++(ptr->curr_frame) > ptr->frames_per_bmp) {
		ptr->curr_frame = 0;
		if (++(ptr->curr_bmp_index) >= ptr->no_bmps) {
			return 1; // return 1 indicates the animation finished, ptr should be deleted
		}
		// TODO update radius
	}

	return 0;
}

Bitmap * explosion_getBitmap(Explosion * ptr) {
	return ptr->bmps[ptr->curr_bmp_index];
}

size_t explosion_getSizeOf() {
	return sizeof(Explosion);
}

//Collisions
int missile_atCity(Missile * ptr) {
	if (ptr->pos[1] > 545)
		return 1;
	else return 0;
}

int missile_atExplosion(Missile * ptr, Explosion * e_ptr) {
	int x_var = (ptr->pos[0] - e_ptr->pos[0]);
	int y_var = (ptr->pos[1] - e_ptr->pos[1]);

	if (x_var*x_var+y_var*y_var <= e_ptr->radius)	//x²+y² <= r²
		return 1;
	else return 0;
}


