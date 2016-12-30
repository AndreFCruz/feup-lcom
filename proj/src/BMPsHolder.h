#ifndef __BMPS_HOLDER_H
#define __BMPS_HOLDER_H

/** @defgroup BMPsHolder BMPsHolder
 * @{
 * Functions for manipulating a structure holding Bitmaps.
 */

#include "Bitmap.h"

#define NUM_EXPLOSION_BMPS	16

#define EXPLOSION_SIZE_X	64
#define EXPLOSION_SIZE_Y	64

#define NUMBER_SIZE_X		30
#define NUMBER_SIZE_Y		45

#define BIG_NUMBER_SIZE_X	68
#define BIG_NUMBER_SIZE_Y	102

#define BUILDING_SIZE_X		160
#define BUILDING0_SIZE_Y	16
#define BUILDING1_SIZE_Y	48
#define BUILDING2_SIZE_Y	76
#define BUILDING_SIZE_Y		76

#define HEART_SIZE_X		48
#define HEART_SIZE_Y		48

#define CANNON_SIZE_X		50
#define CANNON_SIZE_Y		62

#define BUTTONS_X			211
#define SINGLEP_Y			231
#define MULTIP_Y			334
#define HIGHS_Y				445
#define	BUTTONS_WIDTH		376
#define BUTTONS_HEIGHT		82
#define EXIT_X				759
#define EXIT_Y				566
#define EXIT_RADIUS			17

#define HIGHSCORE_SIZE_X	410
#define HIGHSCORE_SIZE_Y	50

typedef struct {
	Bitmap ** numbers;
	Bitmap ** big_numbers;
	Bitmap ** explosion;
	//Bitmap ** ground_explosion; // TODO
	Bitmap ** buildings;

	Bitmap * game_background;
	Bitmap * HS_background;
	Bitmap * heart;

	Bitmap * menu_background;
	Bitmap * SP_button;
	Bitmap * MP_button;
	Bitmap * HS_button;
	Bitmap * highscore_text;
} BMPsHolder_t;

/*
 * @brief Creates a new instance (BMPsHolder_t) if there is none, otherwise returns the existing one (Singleton lie behaviour).
 *
 * @return Pointer to a BMPsHolder instance.
 */
BMPsHolder_t * BMPsHolder();

/*
 * @brief Destroys the BMPsHolder instance, freeing all the resources used by it
 */
void delete_bmps_holder();

/*
 * @brief Load sequence of bitmaps numbered [00, num];
 *
 * @param s1 path of the location of the Bitmaps
 * @param num number of Bitmaps to be loaded
 *
 * @return Array of bitmaps, of size num, containing the loaded sequence
 */
Bitmap ** load_bmps(const char * s1, unsigned num);

#endif
