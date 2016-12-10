#include "planetary.h"
#include "video_gr.h"
#include "Input.h"
#include "Bitmap.h"
#include "GVector.h"
#include "Missile.h"


int timer_handler() {
	static state_t gameState = GAME_SINGLE;

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



//void event_update(event_t * evt, Input * in) {
//	static state_t game_state = MENU;
//
//	switch(state_t) {
//	case MENU:
//		if (*evt == EXIT) {
//			exit();
//		}
//		break;
//
//	case GAME_SINGLE:
//		if (*evt == EXIT) {
//			state_t = MENU;
//			menu_loop();
//		}
//		break;
//
//	case GAME_MULTI:
//
//		break;
//
//	default:
//		printf("planetary::event_update Unrecognized State!");
//		break;
//	}
//}

//event_t game_loop(){
//
//	GVector * e_missiles = new_gvector(sizeof(Missile));
//	GVector * f_missiles = new_gvector(sizeof(Missile));
//
//	/* Update Input, game_state and draw current state */
//
//	/* Draw Game */
//
//	unsigned i;
//	// Draw enemy missiles
//	for (i = 0; i < gvector_get_size(e_missiles); ++i) {
//		draw_missile(gvector_at(e_missiles, i));
//	}
//
//	// Draw friendly missiles
//	for (i = 0; i < gvector_get_size(f_missiles); ++i) {
//		// TODO
//		gvector_at(f_missiles, i);
//	}
//

