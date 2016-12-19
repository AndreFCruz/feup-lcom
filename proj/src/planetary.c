#include "planetary.h"
#include "video_gr.h"
#include "Input.h"
#include "Bitmap.h"
#include "GVector.h"
#include "Missile.h"
#include <stdlib.h>
#include <string.h>


// Load sequence of bitmaps numbered [00, num)
Bitmap ** load_bmps(const char * s1, unsigned num) {
	Bitmap ** array = malloc(sizeof(Bitmap *) * num);
	char * path = (char*) malloc(strlen(s1) + 2 + strlen(".bmp"));
	strcpy(path, s1);
	strcat(path, "00.bmp");

	printf("Loading Animation BMPs with Template String \"%s\"\n", path);

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


// TODO Mudar Structs de Menu e Game para outro ficheiro?

/**
 * Menu Struct and Methods
 */
typedef struct {
	Bitmap * background;

	Bitmap * SParea;
	unsigned SP_pos[2];		//(x,y) position of Single Player Button

	Bitmap * MParea;		//(x,y) position of Multi Player Button
	unsigned MP_pos[2];

	Bitmap * HSarea;		//(x,y) position of High Scores Button
	unsigned HS_pos[2];

	unsigned options_size[2];	// Height and Width of Menu Buttons

	unsigned exit_pos[2];	// (x,y) position of Exit button center
	unsigned exit_radius;	// Height and width of Exit Button

} Menu_t;

static Menu_t * new_menu() {

	Menu_t * Menu = malloc(sizeof(Menu_t));

	Menu->background = loadBitmap("/home/lcom/svn/lcom1617-t4g01/proj/res/InitialMenu/InitialMenu.bmp");

	Menu-> SParea = loadBitmap("/home/lcom/svn/lcom1617-t4g01/proj/res/InitialMenu/SpArea.bmp");
	Menu-> SP_pos[0] = BUTTONS_X;
	Menu-> SP_pos[1] = SINGLEP_Y;

	Menu-> MParea = loadBitmap("/home/lcom/svn/lcom1617-t4g01/proj/res/InitialMenu/MpArea.bmp");
	Menu-> MP_pos[0] = BUTTONS_X;
	Menu-> MP_pos[1] = MULTIP_Y;

	Menu-> HSarea = loadBitmap("/home/lcom/svn/lcom1617-t4g01/proj/res/InitialMenu/HsArea.bmp");
	Menu->HS_pos[0] = BUTTONS_X;
	Menu->HS_pos[1] = HIGHS_Y;

	Menu-> options_size[0] = BUTTONS_WIDTH;
	Menu-> options_size[1] = BUTTONS_HEIGHT;

	Menu->exit_pos[0] = EXIT_X;
	Menu->exit_pos[1] = EXIT_Y;
	Menu-> exit_radius = EXIT_RADIUS;

	return Menu;
}

static Menu_t * menu_ptr = NULL;

static Menu_t * menu_instance() {
	if ( NULL == menu_ptr ) {
		return (menu_ptr = new_menu());
	}
	else
		return menu_ptr;
}

void delete_menu () { // TODO Delete all missiles/explosions ?
	if (NULL != menu_ptr) {
		deleteBitmap(menu_ptr->background);
		deleteBitmap(menu_ptr->SParea);
		deleteBitmap(menu_ptr->MParea);
		deleteBitmap(menu_ptr->HSarea);
		free(menu_ptr);
		menu_ptr = NULL;
	}
}
/** **/

/**
 * Game Struct and Methods
 */
typedef struct {
	GVector * e_missiles;	// Enemy Missiles
	GVector * f_missiles;	// Friendly Missiles
	GVector * explosions;	// Explosions on Screen

	unsigned long frames;		// FRAMES survived, frames == times * 60
	unsigned long enemy_spawn_fr;// FRAME in which an enemy should be spawned

    Bitmap * background;
	Bitmap ** explosion_bmps;

    unsigned base_pos[2];	// (x,y) position of missile origin (base)


} Game_t;

static Game_t * new_game() {
	printf("New Game Instance!!\n");

	Game_t * Game = malloc(sizeof(Game_t));

	Game->frames = 0;
	Game->enemy_spawn_fr = 120;

	Game->background = loadBitmap("/home/lcom/svn/lcom1617-t4g01/proj/res/background.bmp");

	// Load Explosion BitMaps
	Game->explosion_bmps = load_bmps("/home/lcom/svn/lcom1617-t4g01/proj/res/Explosion/" , NUM_EXPLOSION_BMPS);

	Game->e_missiles = new_gvector(sizeof(void*));
	Game->f_missiles = new_gvector(sizeof(void*));
	Game->explosions = new_gvector(sizeof(void*));

	Game->base_pos[0] = L_BASE_X;
	Game->base_pos[1] = BASE_Y;

	printf("Game Instance was successfully created\n");

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

void delete_game() { // TODO Delete all missiles/explosions ?
	if (NULL != game_ptr) {
		deleteBitmap(game_ptr->background);
		delete_gvector(game_ptr->e_missiles);
		delete_gvector(game_ptr->f_missiles);
		delete_gvector(game_ptr->explosions);
		free(game_ptr->explosion_bmps);
		free(game_ptr);
		game_ptr = NULL;
	}
}

Bitmap ** game_getExplosionBmps() {
	return (game_instance()->explosion_bmps);
}
/** **/


// Returns the frame in which an enemy should be spawned
unsigned long next_spawn_frame() {// 500 * (1 + frames / 512)^(-1)
	return game_instance()->frames + 256000. / (game_instance()->frames + 512);
}

int timer_handler() {
	static game_state_t game_state = MENU;

	switch (game_state) {
	case MENU:
		if ( menu_timer_handler(&game_state) != OK )
			return 1;
		break;
	case GAME_SINGLE:
		if ( game_timer_handler() != OK ) {
			delete_game();
			game_state = MENU;
		}
		break;
	case GAME_MULTI:
		printf("FOR THE LONG HAUL...\n");
		game_state = MENU;
		break;
	case HIGH_SCORES:
		printf("ACCESS SCORES FILE AND SHOW BEST ON SCREEN\n");
		game_state = MENU;
		break;
	}

	return OK;
}

int menu_timer_handler(game_state_t * game_state) {
	Input_t * Input = input_instance();
	Menu_t * Menu = menu_instance();

	/** Handle Keyboard Input **/
	switch (input_get_key()) {
	case ESC_BREAK:
		printf("ESC BREAK_CODE DETECTED\n");
		return 1;
		break;
	default:
		break;
	}

	drawBitmap(vg_getBufferPtr(), Menu->background, 0, 0, ALIGN_LEFT);

	if (mouse_inside_rect(Menu->SP_pos[0], Menu->SP_pos[1], Menu->SP_pos[0] + Menu->options_size[0], Menu->SP_pos[1] + Menu->options_size[1])) {
		drawBitmap(vg_getBufferPtr(), Menu->SParea, Menu->SP_pos[0], Menu->SP_pos[1], ALIGN_LEFT);

		if (get_mouseRMB()) {
			* game_state = GAME_SINGLE;
		}
	}
	else if (mouse_inside_rect(Menu->MP_pos[0], Menu->MP_pos[1], Menu->MP_pos[0] + Menu->options_size[0], Menu->MP_pos[1] + Menu->options_size[1])) {
		drawBitmap(vg_getBufferPtr(), Menu->MParea, Menu->MP_pos[0], Menu->MP_pos[1], ALIGN_LEFT);

		if (get_mouseRMB()) {
			* game_state = GAME_MULTI;
		}
	}
	else if (mouse_inside_rect(Menu->HS_pos[0], Menu->HS_pos[1], Menu->HS_pos[0] + Menu->options_size[0], Menu->HS_pos[1] + Menu->options_size[1])) {
		drawBitmap(vg_getBufferPtr(), Menu->HSarea, Menu->HS_pos[0], Menu->HS_pos[1], ALIGN_LEFT);

		if (get_mouseRMB()) {
			* game_state = HIGH_SCORES;
		}
	}
	else if (mouse_inside_circle(Menu->exit_pos[0], Menu->exit_pos[1], Menu->exit_radius) && get_mouseRMB()) {
		return 1;
	}

	draw_mouse_cross(get_mouse_pos());

	return OK;
}

int game_timer_handler() {

	Input_t * Input = input_instance();
	Game_t * self = game_instance();
	if (NULL == self)
		printf("THIS SHOULD NEVER HAPPEN\n");
	unsigned idx;

	/** Handle Input **/
	// Keyboard
	switch (input_get_key()) {
	case ESC_BREAK:
		printf("ESC BREAK_CODE DETECTED\n");
		return 1;
		break;
	default:
		break;
	}

	// Mouse
	//spawn missiles on mouse clicks
	if (get_mouseRMB()) {
		Missile * tmp = new_fmissile(self->base_pos, get_mouse_pos());
		gvector_push_back(self->f_missiles, &tmp);
	}


	/** Spontaneous self Events **/
	++(self->frames);
	if (self->frames == self->enemy_spawn_fr) {
		self->enemy_spawn_fr = next_spawn_frame();
		printf("Spawning New Enemy Missile\n");
		Missile * new_enemy = new_emissile();
		gvector_push_back(self->e_missiles, &new_enemy);
	}

	/** Draw self **/
	drawBitmap(vg_getBufferPtr(), self->background, 0, 0, ALIGN_LEFT);
	draw_mouse_cross(get_mouse_pos());

	// Draw and Update enemy missiles
	for (idx = 0; idx < gvector_get_size(self->e_missiles); ++idx) {
		draw_missile(* (Missile **) gvector_at(self->e_missiles, idx));
		missile_update(* (Missile **) gvector_at(self->e_missiles, idx));
	}

	// Draw and Update friendly missiles
	for (idx = 0; idx < gvector_get_size(self->f_missiles); ++idx) {
		Missile * current = * (Missile **) gvector_at(self->f_missiles, idx);
		draw_missile(current);
		if (missile_update(current)) { // Reached End-Pos
			gvector_erase(self->f_missiles, idx);
			--idx;

			Explosion * tmp = (Explosion *) delete_missile(current);
			gvector_push_back(self->explosions, &tmp);
		}
	}

	// Draw and Update Explosions
	for (idx = 0; idx < gvector_get_size(self->explosions); ++idx) {
		Explosion * current = * (Explosion **) gvector_at(self->explosions, idx);
		draw_explosion(current);
		if (explosion_update(current)) { // Animation ended ?
			printf("\t\t\tExplosion Animation Ended\n");
			gvector_erase(self->explosions, idx);
			--idx;
			delete_explosion(current);
			printf("\t\t\tExplosion deleted\n");
		}
	}

	// Check Collisions e_missiles with sky-line
	for (idx = 0; idx < gvector_get_size(self->e_missiles); ++idx) {
		Missile * current = * (Missile **) gvector_at(self->e_missiles, idx);
		if (missile_getPosY(current) > BASE_Y) {
			gvector_erase(self->e_missiles, idx);
			--idx;

			Explosion * tmp = delete_missile(current);
			gvector_push_back(self->explosions, &tmp);
		}
	}

	// Check Collisions missiles with explosions
	unsigned j;
	for (idx = 0; idx < gvector_get_size(self->explosions); ++idx) {
		Explosion * exp_ptr = * (Explosion **) gvector_at(self->explosions, idx);

		for (j = 0; j < gvector_get_size(self->e_missiles); ++j) {
			Missile * missile_ptr = * (Missile **) gvector_at(self->e_missiles, j);

			if (missile_collidedWithExplosion(missile_ptr, exp_ptr)) {
				gvector_erase(self->e_missiles,j);
				--j;

				Explosion * tmp = delete_missile(missile_ptr);
				gvector_push_back(self->explosions, &tmp);
			}
		}
	}

	return OK;
}

