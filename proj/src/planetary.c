#include "planetary.h"
#include "video_gr.h"
#include "Input.h"
#include "Bitmap.h"
#include "GVector.h"
#include "Missile.h"
#include <stdlib.h>	// rand()


typedef struct {
	GVector * e_missiles;	// Enemy Missiles
	GVector * f_missiles;	// Friendly Missiles

	unsigned frames;		// FRAMES survived, frames == times * 60
	unsigned enemy_spawn_fr;// FRAME in which an enemy should be spawned
    Bitmap* background;
} Game_t;

static Game_t * new_game() {
	printf("New Game Instance!!\n");

	Game_t * Game = malloc(sizeof(Game_t));

	Game->frames = 0;
	Game->enemy_spawn_fr = 120;

	Game->background = loadBitmap("/home/lcom/svn/proj/res/background.bmp");

	Game->e_missiles = new_gvector(missile_getSizeOf());
	Game->f_missiles = new_gvector(missile_getSizeOf());

	return Game;
}

static Game_t * game_ptr = NULL;

Game_t * game_instance() {
	if ( NULL == game_ptr ) {
		return (game_ptr = new_game());
	}
	else
		return game_ptr;
}

void delete_game() {
	if (NULL != game_ptr) {
		deleteBitmap(game_ptr->background);
		delete_gvector(game_ptr->e_missiles);
		delete_gvector(game_ptr->f_missiles);
		free(game_ptr);
		game_ptr = NULL;
	}
}

// Returns the frame in which an enemy should be spawned
unsigned next_spawn_frame() {// 300 * (1 + frames / 512)^(-1)
	return 300 * pow(1 + game_instance()->frames / 512, -1);
}

int timer_handler() {
//	static state_t game_state = GAME_SINGLE;
//
//	switch (game_state) {
//	case MENU:
//		if (menu_timer_handler() == EXIT_CURRENT)
//			return 1;
//		break;
//	case GAME_SINGLE:
//		if (game_timer_handler() == EXIT_CURRENT)
//			game_state = MENU;
//		break;
//	case GAME_MULTI:
//		printf("FOR THE LONG HAUL...\n");
//		break;
//	}

	game_timer_handler();

	return 0;
}

int menu_timer_handler() {
	// TODO Menu

	return 1;
}

int game_timer_handler() {

	Input_t * Input = input_instance();
	Game_t * game = game_instance();
	if (NULL == game)
		printf("THIS SHOULD NEVER HAPPEN, MAKES NO SENSE\n");
	unsigned idx;

	/** Handle Input **/
	// Keyboard
	switch (input_get_key()) {
	case ESC_BREAK:
		return 1;	// TODO proper way out?
		break;
	default:
		break;
	}

	// Mouse
	//spawn missiles on mouse clicks

	printf("Got to line 102 planetary.c\n");

	/** Spontaneous Game Events **/
	++(game->frames);
	if (game->frames == game->enemy_spawn_fr) {
		game->enemy_spawn_fr = next_spawn_frame();
		printf("Spawning New Enemy Missile\n");
		Missile * new_enemy = new_emissile();
		gvector_push_back(game->e_missiles, new_enemy);
	}

	printf("Got to line 111 planetary.c\n");

	/** Draw Game **/
	drawBitmap(vg_getBufferPtr(), game->background, 0, 0, ALIGN_LEFT);
	draw_mouse_cross(get_mouse_x(), get_mouse_y());

	printf("Drew BitMaps -- Got to line 117 planetary.c\n");

	// Draw and Update enemy missiles
	for (idx = 0; idx < gvector_get_size(game->e_missiles); ++idx) {
		draw_missile(gvector_at(game->e_missiles, idx));
		missile_update(gvector_at(game->e_missiles, idx));
	}

	printf("Got to line 125 planetary.c\n");

	// Draw and Update friendly missiles
	for (idx = 0; idx < gvector_get_size(game->f_missiles); ++idx) {
		draw_missile(gvector_at(game->f_missiles, idx));
		missile_update(gvector_at(game->f_missiles, idx));
	}

	// Draw and Update Explosions

	printf("Ended game_timer_handler -- Got to line 131 planetary.c\n");

	return 0;
}

