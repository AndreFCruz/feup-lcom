#include "BMPsHolder.h"
#include <stdlib.h>
#include <string.h>

static BMPsHolder_t * bmps_ptr = NULL;

// Load sequence of bitmaps numbered [00, num)
Bitmap ** load_bmps(const char * s1, unsigned num) {
	Bitmap ** array = malloc(sizeof(Bitmap *) * num);
	char * path = (char*) malloc(strlen(s1) + 2 + strlen(".bmp"));
	strcpy(path, s1);
	strcat(path, "00.bmp");

	printf("\t\tLoading Animation BMPs with Template String \"%s\"\n", path);

	unsigned i, j;
	for (i = 0; i < num; ++i) {
		char parsed_num[2]; // Parse unsigned int to string
		snprintf(parsed_num, 3, "%02d", i);

		for (j = 0; path[j] != 0 && parsed_num[j] != 0; ++j) {
			path[strlen(s1) + j] = parsed_num[j];
		}

		array[i] = loadBitmap(path);
	}

	return array;
}

static BMPsHolder_t * new_bmps_holder() {
	BMPsHolder_t * ptr = malloc(sizeof(BMPsHolder_t));

	ptr->numbers = load_bmps("/home/lcom/svn/lcom1617-t4g01/proj/res/Numbers/",
			10);
	ptr->big_numbers = load_bmps(
			"/home/lcom/svn/lcom1617-t4g01/proj/res/Numbers/big", 10);
	ptr->explosion = load_bmps(
			"/home/lcom/svn/lcom1617-t4g01/proj/res/Explosion/",
			NUM_EXPLOSION_BMPS);
	ptr->buildings = load_bmps(
			"/home/lcom/svn/lcom1617-t4g01/proj/res/Buildings/building", 3);

	ptr->game_background = loadBitmap(
			"/home/lcom/svn/lcom1617-t4g01/proj/res/background.bmp");
	ptr->HS_background = loadBitmap(
			"/home/lcom/svn/lcom1617-t4g01/proj/res/HSbackground.bmp");
	ptr->heart = loadBitmap(
			"/home/lcom/svn/lcom1617-t4g01/proj/res/8_bit_heart.bmp");

	ptr->menu_background =
			loadBitmap(
					"/home/lcom/svn/lcom1617-t4g01/proj/res/InitialMenu/InitialMenu.bmp");
	ptr->SP_button = loadBitmap(
			"/home/lcom/svn/lcom1617-t4g01/proj/res/InitialMenu/SpArea.bmp");
	ptr->MP_button = loadBitmap(
			"/home/lcom/svn/lcom1617-t4g01/proj/res/InitialMenu/MpArea.bmp");
	ptr->HS_button = loadBitmap(
			"/home/lcom/svn/lcom1617-t4g01/proj/res/InitialMenu/HsArea.bmp");
	ptr->highscore_text = loadBitmap(
			"/home/lcom/svn/lcom1617-t4g01/proj/res/highscore_text.bmp");

	printf("New BMPsHolder Instance\n");

	return ptr;
}

void delete_bmps_holder() {
	if (NULL != bmps_ptr) { // TODO Delete stuff
		deleteBitmap(bmps_ptr->menu_background);
		deleteBitmap(bmps_ptr->SP_button);
		deleteBitmap(bmps_ptr->MP_button);
		deleteBitmap(bmps_ptr->HS_button);
		deleteBitmap(bmps_ptr->HS_background);
		deleteBitmap(bmps_ptr->heart);
		deleteBitmap(bmps_ptr->highscore_text);
		//TODO: Missing the delete of the arrays of bmps
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
