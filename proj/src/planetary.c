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
	GVector * explosions;	// Explosions on Screen

	unsigned frames;		// FRAMES survived, frames == times * 60
	unsigned enemy_spawn_fr;// FRAME in which an enemy should be spawned
    Bitmap * background;
	Bitmap ** explosion_bmps;

    unsigned base_pos[2];	// (x,y) position of missile origin (base)

} Game_t;


//// TODO NOT WORKING
//char * explosion_bmp_extension(const char * s1, unsigned i) {
//	char * new_string = malloc(strlen(s1) + 3 + strlen(".bmp"));
//	strcpy(new_string, s1);
//	strcat(new_string, ".bmp");
//
//	return new_string;
//}

static Game_t * new_game() {
	printf("New Game Instance!!\n");

	Game_t * Game = malloc(sizeof(Game_t));

	Game->frames = 0;
	Game->enemy_spawn_fr = 120;

	Game->background = loadBitmap("/home/lcom/svn/lcom1617-t4g01/proj/res/background.bmp");
//	Game->background = loadBitmap("/home/lcom/svn/proj/res/background.bmp");

	// Load Explosion BitMaps
	Game->explosion_bmps = malloc(NUM_EXPLOSION_BMPS * sizeof(Bitmap*));

//	int i;	// GOD DAMN IT
//	for (i = 0; i < NUM_EXPLOSION_BMPS; ++i) {
//		//filename : base + i
//		char * tmp = explosion_bmp_extension("/home/lcom/svn/lcom1617-t4g01/proj/res/Explosion/", i);
//		printf("    LOAD EXPLOSIONS : %s", tmp);
//		Game->explosion_bmps[i] = loadBitmap(tmp);
//	}

	Game->explosion_bmps[0] = loadBitmap("/home/lcom/svn/lcom1617-t4g01/proj/res/Explosion/00.bmp");
	Game->explosion_bmps[1] = loadBitmap("/home/lcom/svn/lcom1617-t4g01/proj/res/Explosion/01.bmp");
	Game->explosion_bmps[2] = loadBitmap("/home/lcom/svn/lcom1617-t4g01/proj/res/Explosion/02.bmp");
	Game->explosion_bmps[3] = loadBitmap("/home/lcom/svn/lcom1617-t4g01/proj/res/Explosion/03.bmp");
	Game->explosion_bmps[4] = loadBitmap("/home/lcom/svn/lcom1617-t4g01/proj/res/Explosion/04.bmp");
	Game->explosion_bmps[5] = loadBitmap("/home/lcom/svn/lcom1617-t4g01/proj/res/Explosion/05.bmp");
	Game->explosion_bmps[6] = loadBitmap("/home/lcom/svn/lcom1617-t4g01/proj/res/Explosion/06.bmp");
	Game->explosion_bmps[7] = loadBitmap("/home/lcom/svn/lcom1617-t4g01/proj/res/Explosion/07.bmp");
	Game->explosion_bmps[8] = loadBitmap("/home/lcom/svn/lcom1617-t4g01/proj/res/Explosion/08.bmp");
	Game->explosion_bmps[9] = loadBitmap("/home/lcom/svn/lcom1617-t4g01/proj/res/Explosion/09.bmp");
	Game->explosion_bmps[10] = loadBitmap("/home/lcom/svn/lcom1617-t4g01/proj/res/Explosion/10.bmp");
	Game->explosion_bmps[11] = loadBitmap("/home/lcom/svn/lcom1617-t4g01/proj/res/Explosion/11.bmp");
	Game->explosion_bmps[12] = loadBitmap("/home/lcom/svn/lcom1617-t4g01/proj/res/Explosion/12.bmp");
	Game->explosion_bmps[13] = loadBitmap("/home/lcom/svn/lcom1617-t4g01/proj/res/Explosion/13.bmp");
	Game->explosion_bmps[14] = loadBitmap("/home/lcom/svn/lcom1617-t4g01/proj/res/Explosion/14.bmp");
	Game->explosion_bmps[15] = loadBitmap("/home/lcom/svn/lcom1617-t4g01/proj/res/Explosion/15.bmp");

//	Game->explosion_bmps[0] = loadBitmap("/home/lcom/svn/proj/res/Explosion/00.bmp");
//	Game->explosion_bmps[1] = loadBitmap("/home/lcom/svn/proj/res/Explosion/01.bmp");
//	Game->explosion_bmps[2] = loadBitmap("/home/lcom/svn/proj/res/Explosion/02.bmp");
//	Game->explosion_bmps[3] = loadBitmap("/home/lcom/svn/proj/res/Explosion/03.bmp");
//	Game->explosion_bmps[4] = loadBitmap("/home/lcom/svn/proj/res/Explosion/04.bmp");
//	Game->explosion_bmps[5] = loadBitmap("/home/lcom/svn/proj/res/Explosion/05.bmp");
//	Game->explosion_bmps[6] = loadBitmap("/home/lcom/svn/proj/res/Explosion/06.bmp");
//	Game->explosion_bmps[7] = loadBitmap("/home/lcom/svn/proj/res/Explosion/07.bmp");
//	Game->explosion_bmps[8] = loadBitmap("/home/lcom/svn/proj/res/Explosion/08.bmp");
//	Game->explosion_bmps[9] = loadBitmap("/home/lcom/svn/proj/res/Explosion/09.bmp");
//	Game->explosion_bmps[10] = loadBitmap("/home/lcom/svn/proj/res/Explosion/10.bmp");
//	Game->explosion_bmps[11] = loadBitmap("/home/lcom/svn/proj/res/Explosion/11.bmp");
//	Game->explosion_bmps[12] = loadBitmap("/home/lcom/svn/proj/res/Explosion/12.bmp");
//	Game->explosion_bmps[13] = loadBitmap("/home/lcom/svn/proj/res/Explosion/13.bmp");
//	Game->explosion_bmps[14] = loadBitmap("/home/lcom/svn/proj/res/Explosion/14.bmp");
//	Game->explosion_bmps[15] = loadBitmap("/home/lcom/svn/proj/res/Explosion/15.bmp");


	Game->e_missiles = new_gvector(missile_getSizeOf());
	Game->f_missiles = new_gvector(missile_getSizeOf());
	Game->explosions = new_gvector(explosion_getSizeOf());

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

void delete_game() {
	if (NULL != game_ptr) {
		deleteBitmap(game_ptr->background);
		delete_gvector(game_ptr->e_missiles);
		delete_gvector(game_ptr->f_missiles);
		free(game_ptr);
		game_ptr = NULL;
	}
}

Bitmap ** game_getExplosionBmps() {
	return game_instance()->explosion_bmps;
}


// Returns the frame in which an enemy should be spawned
unsigned next_spawn_frame() {// 300 * (1 + frames / 512)^(-1)
	return game_instance()->frames + 300 * pow(1 + game_instance()->frames / 512, -1);
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

	return game_timer_handler();
}

int menu_timer_handler() {
	// TODO Menu

	return 1;
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
		return 1;	// TODO Does not Exit...
		break;
	default:
		break;
	}

//	printf("Checking Mouse Input\n");

	// Mouse
	//spawn missiles on mouse clicks
	if (get_mouseRMB()) {
		Missile * tmp = new_fmissile(self->base_pos, get_mouse_pos());
		gvector_push_back(self->f_missiles, tmp);
	}


	/** Spontaneous self Events **/
	++(self->frames);
	if (self->frames == self->enemy_spawn_fr) {
		self->enemy_spawn_fr = next_spawn_frame();
		printf("Spawning New Enemy Missile\n");
		Missile * new_enemy = new_emissile();
		gvector_push_back(self->e_missiles, new_enemy);
	}

//	printf("Drawing Game\n");

	/** Draw self **/
	drawBitmap(vg_getBufferPtr(), self->background, 0, 0, ALIGN_LEFT);
	draw_mouse_cross(get_mouse_pos());

//	printf("Drew BitMaps, Drawing and Updating Missiles\n");

	// Draw and Update enemy missiles
	for (idx = 0; idx < gvector_get_size(self->e_missiles); ++idx) {
		draw_missile(gvector_at(self->e_missiles, idx));
		missile_update(gvector_at(self->e_missiles, idx));
	}

//	printf("Enemy missiles handled, handling friendly missiles\n");

	// Draw and Update friendly missiles
	for (idx = 0; idx < gvector_get_size(self->f_missiles); ++idx) {
		Missile * current = gvector_at(self->f_missiles, idx);
		draw_missile(current);
		if (missile_update(current)) { // Reached End-Pos
			gvector_erase(self->f_missiles, idx);
			--idx;

			gvector_push_back(self->explosions, delete_missile(current));
		}
	}

	// Draw and Update Explosions
	for (idx = 0; idx < gvector_get_size(self->explosions); ++idx) {
		Explosion * current = gvector_at(self->explosions, idx);
		drawBitmap(vg_getBufferPtr(), explosion_getBitmap(current), explosion_getPosX(current), explosion_getPosY(current) - 32, ALIGN_CENTER);
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
		Missile * current = gvector_at(self->e_missiles, idx);
		if (missile_getPosY(current) > BASE_Y) {
			gvector_erase(self->e_missiles, idx);	//TODO: Erases all the missiles on the screen, why?
			--idx;

			gvector_push_back(self->explosions, delete_missile(current));
		}
	}

	// Check Collisions missiles with explosions
	unsigned j;
	for (idx = 0; idx < gvector_get_size(self->explosions); ++idx) {
		for (j = 0; j < gvector_get_size(self->e_missiles); ++j) {
			if (missile_collidedWithExplosion(gvector_at(self->e_missiles, j),gvector_at(self->explosions, idx))) {
				Missile * helper = gvector_at(self->e_missiles, j);
				gvector_erase(self->e_missiles,idx);	//TODO: Erases all the missiles on the screen, why?
				--j;

				delete_missile(helper);
			}
		}
	}


//	printf("-- Ended game_timer_handler --\n");

	return 0;
}

