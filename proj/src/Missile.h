#ifndef __MISSILE_H
#define __MISSILE_H

typedef struct {
	unsigned init_pos[2];	// Initial Position for Missile
	unsigned end_pos[2];

	float velocity[2];		// Velocity, in pixels PER frame
	float pending_movement[2];	// Movement left unperformed on the previous frame

} missile_t;

typedef struct {
	unsigned pos[2];
	unsigned radius;

	unsigned life_span;

} explosion_t;


#endif
