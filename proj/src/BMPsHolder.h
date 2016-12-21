#ifndef __BMPS_HOLDER_H
#define __BMPS_HOLDER_H

#include "Bitmap.h"

#define NUM_EXPLOSION_BMPS	16

#define EXPLOSION_SIZE_X	64
#define EXPLOSION_SIZE_Y	64

#define NUMBER_SIZE_X		30
#define NUMBER_SIZE_Y		45

#define HEART_SIZE_X		64
#define HEART_SIZE_Y		56

#define BUTTONS_X		211
#define SINGLEP_Y		231
#define MULTIP_Y		334
#define HIGHS_Y			445
#define	BUTTONS_WIDTH	376
#define BUTTONS_HEIGHT	82
#define EXIT_X			759
#define EXIT_Y			566
#define EXIT_RADIUS		17

typedef struct {
	Bitmap ** numbers;
	Bitmap ** big_numbers;
	Bitmap ** explosion;
	Bitmap ** ground_explosion; // TODO

	Bitmap * game_background;
	Bitmap * heart;

	Bitmap * menu_background;
	Bitmap * SP_button;
	Bitmap * MP_button;
	Bitmap * HS_button;
} BMPsHolder_t;

BMPsHolder_t * BMPsHolder();

void delete_bmps_holder();

Bitmap ** load_bmps(const char * s1, unsigned num);

#endif
