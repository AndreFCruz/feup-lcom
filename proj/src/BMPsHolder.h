#ifndef __BMPS_HOLDER_H
#define __BMPS_HOLDER_H

/** @defgroup BMPsHolder BMPsHolder
 * @{
 * Functions for manipulating a structure holding Bitmaps.
 */

#include "Bitmap.h"

#define NUM_EXPLOSION_BMPS	16		/**< @brief Number of Bitmaps in explosion animation */
#define NUM_BUILDINGS_BMPS	3		/**< @brief Number of Bitmaps in Buildings destruction Animation */
#define NUM_NUMBERS_BMPS	10		/**< @brief Number of Bitmaps for the numbers graphic representation */

#define EXPLOSION_SIZE_X	64		/**< @brief Visual space occupied by explosions in the horizontal axis */
#define EXPLOSION_SIZE_Y	64		/**< @brief Visual space occupied by explosions in the vertical axis */

#define NUMBER_SIZE_X		30		/**< @brief Visual space occupied by a number in the horizontal axis */
#define NUMBER_SIZE_Y		45		/**< @brief Visual space occupied by a number in the vertical axis */

#define BIG_NUMBER_SIZE_X	68		/**< @brief Visual space occupied by a big number in the horizontal axis */
#define BIG_NUMBER_SIZE_Y	102		/**< @brief Visual space occupied by a big number in the vertical axis */

#define BUILDING_SIZE_X		160		/**< @brief Visual space occupied by a building in the horizontal axis */
#define BUILDING0_SIZE_Y	16		/**< @brief Visual space occupied by a destroyed building in the vertical axis */
#define BUILDING1_SIZE_Y	48		/**< @brief Visual space occupied by a partially destroyed building in the vertical axis */
#define BUILDING2_SIZE_Y	76		/**< @brief Visual space occupied by a building in the vertical axis */
#define BUILDING_SIZE_Y		76		/**< @brief Visual space occupied by a building in the vertical axis */

#define HEART_SIZE_X		48		/**< @brief Visual space occupied by a heart in the horizontal axis */
#define HEART_SIZE_Y		48		/**< @brief Visual space occupied by a heart in the vertical axis */

#define CANNON_SIZE_X		50		/**< @brief Visual space occupied by a missile launcher in the horizontal axis */
#define CANNON_SIZE_Y		62		/**< @brief Visual space occupied by a missile launcher in the vertical axis */

#define BUTTONS_X			211		/**< @brief Initial position of menu buttons in the horizontal axis */
#define SINGLEP_Y			231		/**< @brief Initial position of single player button in the vertical axis */
#define MULTIP_Y			334		/**< @brief Initial position of multi player button in the vertical axis */
#define HIGHS_Y				445		/**< @brief Initial position of highscores button in the vertical axis */
#define	BUTTONS_WIDTH		376		/**< @brief Width of the menu buttons*/
#define BUTTONS_HEIGHT		82		/**< @brief Height of the menu buttons*/
#define EXIT_X				759		/**< @brief Center position of exit button in the horizontal axis */
#define EXIT_Y				566		/**< @brief Center position of exit button in the vertical axis */
#define EXIT_RADIUS			17		/**< @brief Radius  of the circle representing the exit button */

#define HIGHSCORE_SIZE_X	410		/**< @brief Visual space occupied by the highscore text in the horizontal axis */
#define HIGHSCORE_SIZE_Y	50		/**< @brief Visual space occupied by the highscore text in the vertical axis */

/**
 * @brief A structure created to hold all the Bitmaps used in the game.
 */
typedef struct {
	Bitmap ** numbers; ///> Array containing the pointers to the numbers bitmaps
	Bitmap ** big_numbers; ///> Array containing the pointers to the big numbers bitmaps
	Bitmap ** explosion; ///> Array containing the pointers to the explosion bitmaps
	//Bitmap ** ground_explosion; // TODO ?
	Bitmap ** buildings; ///> Array containing the pointers to the building destruction bitmaps

	Bitmap * game_background; ///> Pointer to the bitmap of the game background
	Bitmap * HS_background; ///> Pointer to the bitmap of the highscores background
	Bitmap * heart; ///> Pointer to the bitmap of the hearts representing the remaining lives of the player

	Bitmap * menu_background; ///> Pointer to the bitmap of the menu background
	Bitmap * SP_button; ///> Pointer to the bitmap of the single player button
	Bitmap * MP_button; ///> Pointer to the bitmap of the multi player button
	Bitmap * HS_button; ///> Pointer to the bitmap of the highscores button
	Bitmap * highscore_text; ///> Pointer to the bitmap of the highscore text showed in the end of a game
	Bitmap * waiting_MP; ///> Pointer to the bitmap showed when waiting for other player (multiplayer mode)
} BMPsHolder_t;

/*
 * @brief Creates a new instance (BMPsHolder_t) if there is none, otherwise returns the existing one (Singleton like behaviour).
 *
 * @return Pointer to a BMPsHolder instance.
 */
BMPsHolder_t * BMPsHolder();

/*
 * @brief Destroys the BMPsHolder instance, freeing all the allocated memory
 */
void delete_bmps_holder();

/*
 * @brief Loads sequence of bitmaps numbered [00, num);
 *
 * @param base Base path of the location of the Bitmaps
 * @param num Number of Bitmaps to be loaded
 *
 * @return Array of bitmaps, of size num, containing the loaded sequence
 */
Bitmap ** load_bmps(const char * base, unsigned num);

/*
 * @brief Deletes num Bitmap* and frees the dynamically allocated array
 *
 * @param arr Array of Bitmap*, of size num
 * @param num Size of the array
 */
void delete_bmps(Bitmap ** arr, unsigned num);

/**@}*/

#endif /* __BMPS_HOLDER_H */
