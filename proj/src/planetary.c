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
	state_t state;


} Game_t;

static Game_t * new_game() {
	Game_t * Game = malloc(sizeof(Game_t));

	Game->state = GAME_SINGLE;
	Game->elapsed = 0;
	Game->frames_till_enemy = 120;

	Game->background = loadBitmap("/home/lcom/svn/proj/res/background.bmp");

	e_missiles = new_gvector(sizeof(Missile));
	f_missiles = new_gvector(sizeof(Missile));

}

static Game_t * game_ptr = NULL;

Game_t * game_instance() {
	if ( NULL == Game ) {
		Game = new_game();
		return Game;
	}
	else
		return Game;
}

void delete_game() {
	free(game_ptr);
	game_ptr = NULL;
}

// Returns the frame in which an enemy should be spawned
unsigned next_spawn_frame() {// 300 * (1 + frames / 512)^(-1)
	return 300 * pow(1 + game_instance()->frames / 512, -1);
}

int timer_handler() {
	switch (gameState) {
	case MENU:
		if (menu_timer_handler() == EXIT)
			return 1;
		break;
	case GAME_SINGLE:
		if (game_timer_handler() == EXIT)
			gameState = MENU;
		break;
	case GAME_MULTI:
		printf("FOR THE LONG HAUL...\n");
		break;
	}

	return OK;
}

event_t menu_timer_handler() {

	event_t tmp = NONE;
	return tmp;
}

event_t game_timer_handler() {

	Input_t * Input = input_instance();
	Game_t * game = game_instance();
	event_t evt = NONE;
	unsigned idx;

	/** Handle Input **/
	// Keyboard
	switch (input_get_key()) {
	case ESC_BREAK:
		evt = EXIT;
		break;
	default:
		break;
	}

	// Mouse
	//spawn missiles on mouse clicks

	/** Spontaneous Game Events **/
	game->frames++;
	if (game->frames == game->enemy_spawn_fr) {
		game->enemy_spawn_fr = next_spawn_frame();
		Missile * new_enemy = new_emissile();//TODO
		gvector_push_back(game->e_missiles, new_enemy);
	}


	/** Draw Game **/
	drawBitmap(vg_getBufferPtr(),game->background,0,0,ALIGN_LEFT);
	draw_mouse_cross(get_mouse_x(), get_mouse_y());

	// Draw and Update enemy missiles
	for (idx = 0; idx < gvector_get_size(game->e_missiles); ++idx) {
		draw_missile(gvector_at((game->e_missiles, idx));
		missile_update(gvector_at((game->e_missiles, idx));
	}

	// Draw and Update friendly missiles
	for (idx = 0; idx < gvector_get_size(game->f_missiles); ++idx) {
		draw_missile(gvector_at((game->f_missiles, idx));
		missile_update(gvector_at((game->f_missiles, idx));
	}

	// Draw and Update Explosions



	return tmp;
}

