#include <math.h>
#include <stdlib.h>
#include <sys/types.h>
#include "Missile.h"
#include "video_gr.h"
#include "BMPsHolder.h"

/**
 * Structs
 */

/**
 * boolean type
 */
typedef enum {
	TRUE, FALSE
} bool;

/**
 * Structure used to define a missile. Common to friendly and enemy missiles.
 */
struct missile_t {
	int init_pos[2]; ///> Initial Position for Missile Trail
	int pos[2];	///> Current Position

	float velocity[2]; ///> Velocity, in pixels PER frame
	float pending_movement[2]; ///> Movement left unperformed on the previous frame

	uint16_t color; ///> Color in RGB 5:6:5

	/* Attributes for Friendly Missile */
	bool isFriendly;
	int end_pos[2];

};

/**
 * Structure used to define explosions
 */
struct explosion_t {
	int pos[2]; ///> Center position of Explosion (x,y)
	int radius; ///> Radius of Explosion

	Bitmap ** bmps; ///> Explosion Animation
	unsigned curr_bmp_index; ///> current bitmap being displayed
	unsigned no_bmps; ///> Explosion ended
	unsigned frame_count;
	unsigned frames_per_bmp;
};

/**
 * END of Structs
 */

/**
 * Rounds float to nearest integer
 */
int round_float(float f) {
	return (f > (floor(f) + 0.5)) ? ceil(f) : floor(f);
}

/**
 * Methods for Missile
 */

/**
 * Constructor for Generic Missile
 */
static Missile * new_missile(const int * init_pos, const float * vel) {
	Missile * m_ptr = (Missile *) malloc(sizeof(Missile));

	memmove(m_ptr->init_pos, init_pos, 2 * sizeof(int));
	memmove(m_ptr->pos, init_pos, 2 * sizeof(int));

	memmove(m_ptr->velocity, vel, 2 * sizeof(float));

	m_ptr->pending_movement[0] = 0.;
	m_ptr->pending_movement[1] = 0.;

	m_ptr->color = WHITE; // White, to be changed on enemy/friendly constructor

	return m_ptr;
}

/**
 * Constructor for Enemy Missile
 */
Missile * new_emissile(const unsigned * bases_pos, const unsigned * bases_hp) {
	int init_pos[2] = { rand() % (vg_getHorRes() - 100) + 50, 0 };
	unsigned base_to_attack = rand() % 3;

	while (bases_hp[base_to_attack] == 0) // if dead select another
		base_to_attack = (base_to_attack + 1) % 3;

	// target random position around the base
	float end_pos[2] = { bases_pos[base_to_attack]
			+ 0.9 * BUILDING_SIZE_X
					/ (rand() % 2 ? rand() % 9 - 10 : rand() % 9 + 2),
			vg_getVerRes() };

	float vel[2];
	float magnitude = sqrt(
			(((float) end_pos[0] - (float) init_pos[0])
					* ((float) end_pos[0] - (float) init_pos[0]))
					+ (((float) end_pos[0] - (float) init_pos[1])
							* ((float) end_pos[1] - (float) init_pos[1])));

	float rand_multiplier = (10. + (float) (rand() % 10)) / 10.;
	vel[0] = (rand_multiplier / magnitude)
			* ((float) end_pos[0] - (float) init_pos[0]);
	vel[1] = (rand_multiplier / magnitude)
			* ((float) end_pos[1] - (float) init_pos[1]);

	printf("New Enemy Missile Vel: %d, %d\n", (int) vel[0], (int) vel[1]);

	Missile * m_ptr = new_missile(init_pos, vel);
	m_ptr->color = RED;
	m_ptr->isFriendly = FALSE;
	return m_ptr;
}

/**
 * Constructor for Friendly Missile
 */
Missile * new_fmissile(const int * init_pos, const int * mouse_pos) {
	float vel[2];
	float magnitude = sqrt(
			(((float) mouse_pos[0] - (float) init_pos[0])
					* ((float) mouse_pos[0] - (float) init_pos[0]))
					+ (((float) mouse_pos[0] - (float) init_pos[1])
							* ((float) mouse_pos[1] - (float) init_pos[1])));
	vel[0] = (4. / magnitude) * ((float) mouse_pos[0] - (float) init_pos[0]);
	vel[1] = (4. / magnitude) * ((float) mouse_pos[1] - (float) init_pos[1]);
	// Velocity for Friendly Missiles is constant -- 4 pixels per frame

	Missile * m_ptr = new_missile(init_pos, vel);
	m_ptr->color = YELLOW;
	m_ptr->isFriendly = TRUE;
	memmove(m_ptr->end_pos, mouse_pos, 2 * sizeof(int));

	return m_ptr;
}

Explosion * delete_missile(Missile * m_ptr) {
	Explosion * exp = new_explosion(m_ptr->pos);

	free(m_ptr);

	return exp;
}

// Returns 1 if missile is to be deleted
int missile_update(Missile * m_ptr) {

	//Update Pending Movement
	m_ptr->pending_movement[0] += m_ptr->velocity[0];
	m_ptr->pending_movement[1] += m_ptr->velocity[1];

	//Update Position
	int tmp;
	if (abs(tmp = round_float(m_ptr->pending_movement[0])) > 1) {
		m_ptr->pos[0] += tmp;
		m_ptr->pending_movement[0] -= tmp;
	}
	if (abs(tmp = round_float(m_ptr->pending_movement[1])) > 1) {
		m_ptr->pos[1] += tmp;
		m_ptr->pending_movement[1] -= tmp;
	}

	// IF is Friendly Missile
	if (m_ptr->isFriendly == TRUE) {
		if (abs(m_ptr->pos[0] - m_ptr->end_pos[0]) < 4
				&& abs(m_ptr->pos[1] - m_ptr->end_pos[1]) < 4) {
			printf("\tFriendly Missile Reached End-Pos\n");
			return 1; // Reached End Pos -- Should Explode
		}
	}

	return 0;
}

int missile_isFriendly(Missile * m_ptr) {
	return (m_ptr->isFriendly == TRUE ? 1 : 0);
}

int missile_getPosX(Missile * m_ptr) {
	return m_ptr->pos[0];
}

int missile_getPosY(Missile * m_ptr) {
	return m_ptr->pos[1];
}

int missile_getInitX(Missile * m_ptr) {
	return m_ptr->init_pos[0];
}

int missile_getInitY(Missile * m_ptr) {
	return m_ptr->init_pos[1];
}

uint16_t missile_getColor(Missile * m_ptr) {
	return m_ptr->color;
}

size_t missile_getSizeOf() {
	return sizeof(Missile);
}

/**
 * Methods for Explosion
 */

Explosion * new_explosion(const int * position) {
	Explosion * self = (Explosion *) malloc(sizeof(Explosion));

	memmove(self->pos, position, 2 * sizeof(int));
	self->frame_count = 0;
	self->curr_bmp_index = 0;
	self->frames_per_bmp = 5;
	self->no_bmps = 16;
	self->radius = 28;

	self->bmps = BMPsHolder()->explosion;

	return self;
}

void delete_explosion(Explosion * e_ptr) {
	free(e_ptr);
}

int explosion_update(Explosion * e_ptr) {
	if (++(e_ptr->frame_count) > e_ptr->frames_per_bmp) {
		e_ptr->frame_count = 0;
		--(e_ptr->radius);	// Update radius (!)
							// Radius diminishes roughly proportionally to the increase of bmp idx
		if (++(e_ptr->curr_bmp_index) >= e_ptr->no_bmps) {
			return 1; // return 1 indicates the animation finished, e_ptr should be deleted
		}
	}

	return 0;
}

Bitmap * explosion_getBitmap(Explosion * e_ptr) {
	return e_ptr->bmps[e_ptr->curr_bmp_index];
}

int explosion_getPosX(Explosion * e_ptr) {
	return e_ptr->pos[0];
}

int explosion_getPosY(Explosion * e_ptr) {
	return e_ptr->pos[1];
}

size_t explosion_getSizeOf() {
	return sizeof(Explosion);
}

/* Collisions */

int missile_collidedWithExplosion(Missile * m_ptr, Explosion * e_ptr) {
	int x_var = (m_ptr->pos[0] - e_ptr->pos[0]);
	int y_var = (m_ptr->pos[1] - e_ptr->pos[1]);

	if (x_var * x_var + y_var * y_var <= e_ptr->radius * e_ptr->radius)	//x²+y² <= r²
		return 1;
	else
		return 0;
}

// (posX, posY) indicates the lower-left point of the rectangle
int missile_collidedWithRect(Missile * ptr, unsigned posX, unsigned posY,
		unsigned sizeX, unsigned sizeY) {
	if (ptr->pos[0] > posX && ptr->pos[0] < (posX + sizeX) && ptr->pos[1] < posY
			&& ptr->pos[1] > posY - sizeY) {
		return 1;
	} else {
		return 0;
	}
}

