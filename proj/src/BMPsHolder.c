#include "BMPsHolder.h"
#include <stdlib.h>
#include <string.h>

static BMPsHolder_t * bmps_ptr = NULL;

// Load sequence of bitmaps numbered [00, num)
Bitmap ** load_bmps(const char * base, unsigned num) {
	Bitmap ** array = malloc(sizeof(Bitmap *) * num);
	char * path = (char*) malloc(strlen(base) + 2 + strlen(".bmp"));
	strcpy(path, base);
	strcat(path, "00.bmp");

	unsigned i, j;
	for (i = 0; i < num; ++i) {
		char parsed_num[2]; // Parse unsigned int to string
		snprintf(parsed_num, 3, "%02d", i);

		for (j = 0; path[j] != 0 && parsed_num[j] != 0; ++j) {
			path[strlen(base) + j] = parsed_num[j];
		}

		array[i] = loadBitmap(path);
	}

	return array;
}

void delete_bmps(Bitmap ** arr, unsigned num) {
	unsigned i;
	for (i = 0; i < num; ++i)
		deleteBitmap(arr[i]);
	free(arr);
}

static BMPsHolder_t * new_bmps_holder() {
	BMPsHolder_t * ptr = malloc(sizeof(BMPsHolder_t));

	ptr->numbers = load_bmps("/home/planetary_defense/res/Numbers/",
	NUM_NUMBERS_BMPS);
	ptr->big_numbers = load_bmps(
			"/home/planetary_defense/res/Numbers/big",
			NUM_NUMBERS_BMPS);
	ptr->explosion = load_bmps(
			"/home/planetary_defense/res/Explosion/",
			NUM_EXPLOSION_BMPS);
	ptr->buildings = load_bmps(
			"/home/planetary_defense/res/Buildings/building",
			NUM_BUILDINGS_BMPS);

	ptr->game_background = loadBitmap(
			"/home/planetary_defense/res/background.bmp");
	ptr->HS_background = loadBitmap(
			"/home/planetary_defense/res/HSbackground.bmp");
	ptr->heart = loadBitmap(
			"/home/planetary_defense/res/8_bit_heart.bmp");

	ptr->menu_background =
			loadBitmap(
					"/home/planetary_defense/res/InitialMenu/InitialMenu.bmp");
	ptr->SP_button = loadBitmap(
			"/home/planetary_defense/res/InitialMenu/SpArea.bmp");
	ptr->MP_button = loadBitmap(
			"/home/planetary_defense/res/InitialMenu/MpArea.bmp");
	ptr->HS_button = loadBitmap(
			"/home/planetary_defense/res/InitialMenu/HsArea.bmp");
	ptr->highscore_text = loadBitmap(
			"/home/planetary_defense/res/highscore_text.bmp");
	ptr->waiting_MP = loadBitmap(
			"/home/planetary_defense/res/waitingMP.bmp");
	ptr->win = loadBitmap(
			"/home/planetary_defense/res/win.bmp");
	ptr->lost = loadBitmap(
			"/home/planetary_defense/res/lost.bmp");

	return ptr;
}

void delete_bmps_holder() {
	if (NULL != bmps_ptr) {
		deleteBitmap(bmps_ptr->menu_background);
		deleteBitmap(bmps_ptr->SP_button);
		deleteBitmap(bmps_ptr->MP_button);
		deleteBitmap(bmps_ptr->HS_button);
		deleteBitmap(bmps_ptr->HS_background);
		deleteBitmap(bmps_ptr->heart);
		deleteBitmap(bmps_ptr->highscore_text);

		delete_bmps(bmps_ptr->numbers, NUM_NUMBERS_BMPS);
		delete_bmps(bmps_ptr->big_numbers, NUM_NUMBERS_BMPS);
		delete_bmps(bmps_ptr->explosion, NUM_EXPLOSION_BMPS);
		delete_bmps(bmps_ptr->buildings, NUM_BUILDINGS_BMPS);

		bmps_ptr = NULL;
	}
}

BMPsHolder_t * BMPsHolder() {
	if (NULL == bmps_ptr) {
		return (bmps_ptr = new_bmps_holder());
	} else {
		return bmps_ptr;
	}
}
