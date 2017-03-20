#include <stdlib.h>
#include <string.h>
#include "planetary.h"
#include "video_gr.h"
#include "Input.h"
#include "Bitmap.h"
#include "GVector.h"
#include "Missile.h"
#include "BMPsHolder.h"
#include "RTC.h"
#include "Highscores.h"
#include "Serial.h"
#include "Communication.h"

static int menu_timer_handler();
static int game_timer_handler();
static int end_game_timer_handler(int highscore_flag);
static int highscores_timer_handler();
static int multiplayer_timer_handler();
static int multiplayer_end_animation(int winner_flag);

/**
 * Menu Struct and Methods
 */
typedef struct {
	unsigned SP_pos[2];		//(x,y) position of Single Player Button
	unsigned MP_pos[2];		//(x,y) position of Multi Player Button
	unsigned HS_pos[2];		//(x,y) position of High Scores Button

	unsigned options_size[2];	// Height and Width of Menu Buttons

	unsigned exit_pos[2];	// (x,y) position of Exit button center
	unsigned exit_radius;	// Height and width of Exit Button

} Menu_t;

static Menu_t * new_menu() {
	Menu_t * Menu = malloc(sizeof(Menu_t));

	Menu->SP_pos[0] = BUTTONS_X;
	Menu->SP_pos[1] = SINGLEP_Y;

	Menu->MP_pos[0] = BUTTONS_X;
	Menu->MP_pos[1] = MULTIP_Y;

	Menu->HS_pos[0] = BUTTONS_X;
	Menu->HS_pos[1] = HIGHS_Y;

	Menu->options_size[0] = BUTTONS_WIDTH;
	Menu->options_size[1] = BUTTONS_HEIGHT;

	Menu->exit_pos[0] = EXIT_X;
	Menu->exit_pos[1] = EXIT_Y;
	Menu->exit_radius = EXIT_RADIUS;

	return Menu;
}

static Menu_t * menu_ptr = NULL;

static Menu_t * menu_instance() {
	if (NULL == menu_ptr) {
		return (menu_ptr = new_menu());
	} else
		return menu_ptr;
}

static void delete_menu() {
	if (NULL != menu_ptr) {
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

	unsigned long frames;		// FRAMES survived, frames == times * FRAME_RATE
	unsigned long enemy_spawn_fr;	// FRAME in which an enemy should be spawned

	unsigned cannon_pos[2];	// x position of the left and right cannons
	unsigned health_points;	// number of bases left

	unsigned bases_hp[3];	// 0 (destroyed), 1 (damaged) or 2 (intact)
	unsigned bases_pos[3];	// x position of the three bases

	unsigned buildings_size_y[3];

	Score_t * highscores;	// Array containing HighScores

} Game_t;

static Game_t * new_game() {
	printf("Game Instance called!\n");

	Game_t * Game = malloc(sizeof(Game_t));

	Game->frames = 0;
	Game->enemy_spawn_fr = 120;

	Game->e_missiles = new_gvector(sizeof(void*));
	Game->f_missiles = new_gvector(sizeof(void*));
	Game->explosions = new_gvector(sizeof(void*));

	Game->health_points = 3;

	Game->cannon_pos[0] = LEFT_CANNON_POS_X;
	Game->cannon_pos[1] = RIGHT_CANNON_POS_X;

	unsigned idx;
	for (idx = 0; idx < NUM_BASES; ++idx) {
		Game->bases_hp[idx] = 2;
	}
	for (idx = 0; idx < NUM_BASES; ++idx) {
		Game->bases_pos[idx] = 200
				+ idx * ((float) vg_getHorRes() - 200.) / NUM_BASES;
	}

	Game->buildings_size_y[0] = BUILDING0_SIZE_Y;
	Game->buildings_size_y[1] = BUILDING1_SIZE_Y;
	Game->buildings_size_y[2] = BUILDING2_SIZE_Y;

	Game->highscores = loadScores(
	SCORES_TXT_PATH);

	printf("Game Instance was successfully created\n");

	return Game;
}

static Game_t * game_ptr = NULL;

static Game_t * game_instance() {
	if (NULL == game_ptr) {
		return (game_ptr = new_game());
	} else
		return game_ptr;
}

static void delete_game() {
	if (NULL != game_ptr) {
		unsigned idx;
		for (idx = 0; idx < gvector_get_size(game_ptr->e_missiles); ++idx) {
			free(*(Missile **) gvector_at(game_ptr->e_missiles, idx));
		}
		delete_gvector(game_ptr->e_missiles);

		for (idx = 0; idx < gvector_get_size(game_ptr->f_missiles); ++idx) {
			free(*(Missile **) gvector_at(game_ptr->f_missiles, idx));
		}
		delete_gvector(game_ptr->f_missiles);

		for (idx = 0; idx < gvector_get_size(game_ptr->explosions); ++idx) {
			free(*(Explosion **) gvector_at(game_ptr->explosions, idx));
		}
		delete_gvector(game_ptr->explosions);

		free(game_ptr);
		game_ptr = NULL;

		printf("\tGame Instance was successfully deleted!\n");
	}
}

/** **/

// Returns the frame in which an enemy should be spawned
unsigned long next_spawn_frame() {	// 500 / (1 + frames / 512)
	return game_instance()->frames + 256000. / (game_instance()->frames + 512);
}

int timer_handler() {
	static game_state_t game_state = MENU;
	static int highscore_flag = 0, winner_flag = 0;

	int ret;

	switch (game_state) {
	case MENU:
		if ( OK != menu_timer_handler(&game_state)) {
			delete_bmps_holder();
			delete_menu();
			return 1;
		}
		break;
	case GAME_SINGLE:
		ret = game_timer_handler();
		if (OK != ret) {
			game_state = END_GAME_ANIMATION;
			printf("\tEND OF GAME -- \n");
			// highscore
			if (2 == ret) {
				highscore_flag = 1;
				printf("\t\tHIGHSCORE DETECTED!!\n");
			}
		}
		break;
	case GAME_MULTI:
		ret = multiplayer_timer_handler();
		if (OK != ret ) {
			// Fetch winning status
			printf("ENDED MULTIPLAYER!\n");
			game_state = MP_END_ANIMATION;

			if (2 == ret)
				winner_flag = 1;
		}
		break;
	case MP_END_ANIMATION:
		if ( OK != multiplayer_end_animation(winner_flag) ) {
			delete_game();
			game_state = MENU;
			winner_flag = 0;
			setComState(NONE);
		}
	case END_GAME_ANIMATION:
		if ( OK != end_game_timer_handler(highscore_flag)) {
			delete_game();
			game_state = MENU;
			highscore_flag = 0;
		}
		break;
	case HIGH_SCORES:
		if ( OK != highscores_timer_handler()) {
			game_state = MENU;
		}
		break;
	}

	return OK;
}

static int multiplayer_timer_handler() {
	static unsigned long count;

	switch(getComState()) {
	case MP_WAITING:
		// draw bitmap waiting for connection
		drawBitmap(vg_getBufferPtr(), BMPsHolder()->waiting_MP, 0, 0,
				ALIGN_LEFT);
		draw_mouse_cross(get_mouse_pos(), WHITE);

		if (count % (FRAME_RATE / 2) == 0 ) {
			serial_write(MP_WAITING);
			count = 0;
		}
		++count;
		break;
	case MP_ONGOING: // You Lost
		if (game_timer_handler() != OK) {
			setComState(MP_ENDED);
			return 1;
		}
		break;
	case MP_ENDED: // You Won!
		return 2;
		break;
	}

	return OK;
}

static int menu_timer_handler(game_state_t * game_state) {
	static int selected = 0; // Button highlighted. 0 indicates none

	Input_t * Input = input_instance();
	Menu_t * Menu = menu_instance();

	int enter_flag = 0; // Indicates whether enter was pressed

	/** Handle Keyboard Input **/
	switch (input_get_key()) {
	case ESC_BREAK:
		printf("ESC BREAK_CODE DETECTED: 0x%X\n", ESC_BREAK);
		return 1;
		break;
	case ENTER_BREAK:
		printf("ENTER BREAK_CODE DETECTED: 0x%X\n", ENTER_BREAK);
		enter_flag = 1;
		break;
	case UP_MAKE:
		printf("UP_ARROW MAKE CODE DETECTED: 0x%X\n", UP_MAKE);
		selected = selected - 1 < 0 ? 3 : selected - 1; // 3 is the number of buttons
		break;
	case DOWN_MAKE:
		printf("DOWN_ARROW MAKE CODE DETECTED: 0x%X\n", DOWN_MAKE);
		selected = selected + 1 > 3 ? 0 : selected + 1;
		break;
	default:
		// Un-mapped key_code received, do nothing
		break;
	}

	drawBitmap(vg_getBufferPtr(), BMPsHolder()->menu_background, 0, 0,
			ALIGN_LEFT);

	if (mouse_inside_rect(Menu->SP_pos[0], Menu->SP_pos[1],
			Menu->SP_pos[0] + Menu->options_size[0],
			Menu->SP_pos[1] + Menu->options_size[1])) {
		selected = 1;

		if (get_mouseRMB()) {
			*game_state = GAME_SINGLE;
			selected = 0;
		}
	} else if (mouse_inside_rect(Menu->MP_pos[0], Menu->MP_pos[1],
			Menu->MP_pos[0] + Menu->options_size[0],
			Menu->MP_pos[1] + Menu->options_size[1])) {
		selected = 2;

		if (get_mouseRMB()) {
			serial_enable_interrupts();
			setComState(MP_WAITING);
			*game_state = GAME_MULTI;
			selected = 0;
		}
	} else if (mouse_inside_rect(Menu->HS_pos[0], Menu->HS_pos[1],
			Menu->HS_pos[0] + Menu->options_size[0],
			Menu->HS_pos[1] + Menu->options_size[1])) {
		selected = 3;

		if (get_mouseRMB()) {
			*game_state = HIGH_SCORES;
			selected = 0;
		}
	} else if (mouse_inside_circle(Menu->exit_pos[0], Menu->exit_pos[1],
			Menu->exit_radius) && get_mouseRMB()) {
		return 1;
	}

	// Draw Selection Highlight
	switch (selected) {
	case 0:
		// Nothing is selected
		break;
	case 1:
		drawBitmap(vg_getBufferPtr(), BMPsHolder()->SP_button, Menu->SP_pos[0],
				Menu->SP_pos[1], ALIGN_LEFT);
		if (enter_flag)
			*game_state = GAME_SINGLE;
		break;
	case 2:
		drawBitmap(vg_getBufferPtr(), BMPsHolder()->MP_button, Menu->MP_pos[0],
				Menu->MP_pos[1], ALIGN_LEFT);
		if (enter_flag)
			*game_state = GAME_MULTI;
		break;
	case 3:
		drawBitmap(vg_getBufferPtr(), BMPsHolder()->HS_button, Menu->HS_pos[0],
				Menu->HS_pos[1], ALIGN_LEFT);
		if (enter_flag)
			*game_state = HIGH_SCORES;
		break;
	default:
		printf("Menu Button Selection Went Out of Range! Was %x.\n", selected);
	}

	// Draw mouse cross last, so it is in the top layer
	draw_mouse_cross(get_mouse_pos(), WHITE);

	return OK;
}

static int game_timer_handler() {

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
	if ( get_mouseRMB() && gvector_get_size(self->f_missiles) < MAX_NUM_MISSILES
			&& get_mouse_pos()[1] < CANNON_POS_Y ) {
		int tmp_pos[2] = { self->cannon_pos[0] + CANNON_PROJECTILE_OFFSET,
		CANNON_POS_Y };
		Missile * tmp = new_fmissile(tmp_pos, get_mouse_pos());
		gvector_push_back(self->f_missiles, &tmp);
	}
	if ( get_mouseLMB() && gvector_get_size(self->f_missiles) < MAX_NUM_MISSILES
			&& get_mouse_pos()[1] < CANNON_POS_Y ) {
		int tmp_pos[2] = { self->cannon_pos[1] - CANNON_PROJECTILE_OFFSET,
		CANNON_POS_Y };
		Missile * tmp = new_fmissile(tmp_pos, get_mouse_pos());
		gvector_push_back(self->f_missiles, &tmp);
	}

	/** Spontaneous self Events **/
	++(self->frames);
	if (self->frames == self->enemy_spawn_fr) {
		self->enemy_spawn_fr = next_spawn_frame();
		printf("Spawning New Enemy Missile\n");
		Missile * new_enemy = new_emissile(self->bases_pos, self->bases_hp);
		gvector_push_back(self->e_missiles, &new_enemy);
	}

	/** Draw self **/
	drawBitmap(vg_getBufferPtr(), BMPsHolder()->game_background, 0, 0,
			ALIGN_LEFT);

	// Draw Bases/Houses
	for (idx = 0; idx < NUM_BASES; ++idx) {
		unsigned base_hp = self->bases_hp[idx];

		drawBitmap(vg_getBufferPtr(), BMPsHolder()->buildings[base_hp],
				self->bases_pos[idx],
				GROUND_Y - self->buildings_size_y[base_hp], ALIGN_CENTER);
	}

	// Draw and Update enemy missiles
	for (idx = 0; idx < gvector_get_size(self->e_missiles); ++idx) {
		draw_missile(*(Missile **) gvector_at(self->e_missiles, idx));
		missile_update(*(Missile **) gvector_at(self->e_missiles, idx));
	}

	// Draw and Update friendly missiles
	for (idx = 0; idx < gvector_get_size(self->f_missiles); ++idx) {
		Missile * current = *(Missile **) gvector_at(self->f_missiles, idx);
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
		Explosion * current = *(Explosion **) gvector_at(self->explosions, idx);
		draw_explosion(current);
		if (explosion_update(current)) { // Animation ended ?
			gvector_erase(self->explosions, idx);
			--idx;
			delete_explosion(current);
			printf("\t\t\tExplosion deleted\n");
		}
	}

	/** Collision Detection **/

	// Check Collisions e_missiles with ground
	for (idx = 0; idx < gvector_get_size(self->e_missiles); ++idx) {
		Missile * current = *(Missile **) gvector_at(self->e_missiles, idx);
		if (missile_getPosY(current) > GROUND_Y) {
			gvector_erase(self->e_missiles, idx);
			--idx;

			Explosion * tmp = delete_missile(current);
			gvector_push_back(self->explosions, &tmp);
		}
	}

	// Check Collisions missiles with explosions
	unsigned j;
	for (idx = 0; idx < gvector_get_size(self->explosions); ++idx) {
		Explosion * exp_ptr = *(Explosion **) gvector_at(self->explosions, idx);

		// Check Enemy Missiles
		for (j = 0; j < gvector_get_size(self->e_missiles); ++j) {
			Missile * missile_ptr = *(Missile **) gvector_at(self->e_missiles,
					j);

			if (missile_collidedWithExplosion(missile_ptr, exp_ptr)) {
				gvector_erase(self->e_missiles, j);
				--j;

				Explosion * tmp = delete_missile(missile_ptr);
				gvector_push_back(self->explosions, &tmp);
			}
		}

		// Check Friendly Missiles
		for (j = 0; j < gvector_get_size(self->f_missiles); ++j) {
			Missile * missile_ptr = *(Missile **) gvector_at(self->f_missiles,
					j);

			if (missile_collidedWithExplosion(missile_ptr, exp_ptr)) {
				gvector_erase(self->f_missiles, j);
				--j;

				Explosion * tmp = delete_missile(missile_ptr);
				gvector_push_back(self->explosions, &tmp);
			}
		}
	}

	// Note! Bases aren't destroyed on collisions with explosions by design!
	// Check Collisions missiles with bases, update bases
	for (idx = 0; idx < NUM_BASES; ++idx) {

		// Enemy Missiles
		for (j = 0; j < gvector_get_size(self->e_missiles); ++j) {
			Missile * missile_ptr = *(Missile **) gvector_at(self->e_missiles,
					j);

			if (missile_collidedWithRect(missile_ptr,
					self->bases_pos[idx] - BUILDING_SIZE_X / 2.0,
					GROUND_Y, BUILDING_SIZE_X,
					self->buildings_size_y[self->bases_hp[idx]])) {
				printf("\tCollision Detected! Enemy Missile with base %d!\n",
						idx);

				gvector_erase(self->e_missiles, j);
				--j;

				Explosion * tmp = delete_missile(missile_ptr);
				gvector_push_back(self->explosions, &tmp);

				self->bases_hp[idx] =
						self->bases_hp[idx] > 0 ? self->bases_hp[idx] - 1 : 0;
			}
		}

	}

	/** **/

	// Draw Score - Upper Right Corner
	draw_score(self->frames / FRAME_RATE, vg_getHorRes() - 10, 10);

	// Calculate HP -- Game ends if it's zero
	unsigned health_points = 0;
	for (idx = 0; idx < NUM_BASES; ++idx) {
		if (self->bases_hp[idx] > 0)
			++health_points;
	}

	// Draw Lives - Upper Left Corner
	for (idx = 0; idx < health_points; ++idx) {
		drawBitmap(vg_getBufferPtr(), BMPsHolder()->heart,
				10 + (idx * HEART_SIZE_X + 10), 10, ALIGN_LEFT);
	}

	// Draw mouse cross last, so it is in the top layer
	draw_mouse_cross(get_mouse_pos(), BLACK);

	if (0 == health_points) { // Everything Explodes in the End x)

		// Delete Enemy Missiles
		while (0 != gvector_get_size(self->e_missiles)) {
			Missile * missile_ptr = *(Missile **) gvector_at(self->e_missiles,
					0);
			gvector_erase(self->e_missiles, 0);

			Explosion * tmp = delete_missile(missile_ptr);
			gvector_push_back(self->explosions, &tmp);
		}

		// Delete Friendly Missiles
		while (0 != gvector_get_size(self->f_missiles)) {
			Missile * missile_ptr = *(Missile **) gvector_at(self->f_missiles,
					0);
			gvector_erase(self->f_missiles, 0);

			Explosion * tmp = delete_missile(missile_ptr);
			gvector_push_back(self->explosions, &tmp);
		}

		/* Update Scores */
		//creating a new Score
		Score_t endgame;
		endgame.score = (self->frames / FRAME_RATE);

		//Assembling Date and Hour
		Date_t * date = rtc_read_date();
		endgame.hour = date->hour;
		endgame.minute = date->minute;
		endgame.day = date->day;
		endgame.month = date->month;
		endgame.year = 2000 + date->year;
		free(date);

		if (updateScores(self->highscores, endgame)) {
			writeScores(SCORES_TXT_PATH,
					self->highscores);
			printf("END_OF_GAME->HIGHSCORE\n");
			return 2; // return highscore flag
		}

		return 1;
	}

	return OK;
}

// Handles Timer Interrupts in the End of Game Animation State
static int end_game_timer_handler(int highscore_flag) {
	static unsigned count = 0;	// For blinking animation

	Game_t * self = game_instance();
	unsigned idx;

	/** Handle Input **/
	// Keyboard
	switch (input_get_key()) {
	case ESC_BREAK:
		count = 0;
		return 1;
		break;
	case ENTER_BREAK:
		count = 0;
		return 1;
	default:
		break;
	}

	// Exit after 10 seconds
	if (count / 60 == 10) {
		count = 0;
		return 1;
	}

	// Draw Background
	drawBitmap(vg_getBufferPtr(), BMPsHolder()->game_background, 0, 0,
			ALIGN_LEFT);

	// Draw Highscore Text ?
	if (highscore_flag) {
		drawBitmap(vg_getBufferPtr(), BMPsHolder()->highscore_text,
				vg_getHorRes() / 2, 100, ALIGN_CENTER);
	}

	// Spawn Random Explosions every 0.5 seconds
	if (count % (FRAME_RATE / 2) == 0) {
		int rand_pos[2] = { EXPLOSION_SIZE_X
				+ rand() % (vg_getHorRes() - 2 * EXPLOSION_SIZE_X),
		EXPLOSION_SIZE_Y + rand() % (vg_getVerRes() - 2 * EXPLOSION_SIZE_Y) };
		Explosion * new = new_explosion(rand_pos);
		gvector_push_back(self->explosions, &new);
	}

	// Draw and Update Explosions
	for (idx = 0; idx < gvector_get_size(self->explosions); ++idx) {
		Explosion * current = *(Explosion **) gvector_at(self->explosions, idx);
		draw_explosion(current);
		if (explosion_update(current)) { // Animation ended ?
			gvector_erase(self->explosions, idx);
			--idx;
			delete_explosion(current);
			printf("\t\t\tExplosion deleted\n");
		}
	}

	++count;
	// Draw Blinking Score -- Center of Screen
	if ((count / (FRAME_RATE / 2)) % 2)
		draw_score(self->frames / FRAME_RATE,
				vg_getHorRes() / 2 + NUMBER_SIZE_X / 2,
				vg_getVerRes() / 2 + NUMBER_SIZE_Y / 2);

	return OK;
}

static int highscores_timer_handler() {
	static Score_t * scores = NULL;

	// Fetch Highscores if not loaded
	if (NULL == scores)
		scores = loadScores(SCORES_TXT_PATH);

	/** Handle Keyboard Input **/
	switch (input_get_key()) {
	case ESC_BREAK:
		printf("ESC BREAK_CODE DETECTED: 0x%X\n", ESC_BREAK);
		free(scores);
		scores = NULL;
		return 1;
		break;
	default:
		break;
	}

	//Checking if Exit Button clicked
	if (mouse_inside_circle(EXIT_X, EXIT_Y, EXIT_RADIUS) && get_mouseRMB()) {
		free(scores);
		scores = NULL;
		return 1;
	}

	// Draw Background
	drawBitmap(vg_getBufferPtr(), BMPsHolder()->HS_background, 0, 0,
			ALIGN_LEFT);

	unsigned i;
	for (i = 0; i < HIGHSCORE_NUMBER; ++i) {
		draw_score(scores[i].score, SCORE_SCORE_X, SCORE_Y + i * SCORE_Y_INC);
		draw_score(scores[i].hour, SCORE_HOUR_X, SCORE_Y + i * SCORE_Y_INC);
		draw_score(scores[i].minute, SCORE_MINUTE_X, SCORE_Y + i * SCORE_Y_INC);
		draw_score(scores[i].day, SCORE_DAY_X, SCORE_Y + i * SCORE_Y_INC);
		draw_score(scores[i].month, SCORE_MONTH_X, SCORE_Y + i * SCORE_Y_INC);
		draw_score(scores[i].year, SCORE_YEAR_X, SCORE_Y + i * SCORE_Y_INC);
	}

	// Draw mouse cross last, so it is in the top layer
	draw_mouse_cross(get_mouse_pos(), WHITE);

	return OK;
}

// TODO Improve animation ? explosions maybe
static int multiplayer_end_animation(int winner_flag) {

	/** Handle Keyboard Input **/
	switch (input_get_key()) {
	case ESC_BREAK:
		printf("ESC BREAK_CODE DETECTED: 0x%X\n", ESC_BREAK);
		return 1;
		break;
	default:
		break;
	}

	// Draw Background
	drawBitmap(vg_getBufferPtr(), BMPsHolder()->HS_background, 0, 0,
			ALIGN_LEFT);

	// Draw appropriate Bitmap
	if (winner_flag)
		drawBitmap(vg_getBufferPtr(), BMPsHolder()->win, vg_getHorRes() / 2, vg_getVerRes() / 2, ALIGN_CENTER);
	else
		drawBitmap(vg_getBufferPtr(), BMPsHolder()->lost, vg_getHorRes() / 2, vg_getVerRes() / 2, ALIGN_CENTER);

	draw_mouse_cross(get_mouse_pos(), BLACK);

	return OK;
}
