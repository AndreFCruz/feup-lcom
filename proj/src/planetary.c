#include "planetary.h"
#include "video_gr.h"
#include "timer.h"
#include "keyboard.h"
#include "i8042.h"
#include "read_xpm.h"
#include "Input.h"
#include "vbe.h"
#include "pixmap.h"
#include "stddef.h"
#include "mouse.h"
#include "Bitmap.h"
#include "GVector.h"
#include "Missile.h"

static char * v_mem = NULL;	// TODO Mudar para o video_gr ??

void event_update(event_t * evt, Input * in) {
	static state_t game_state = MENU;

	switch(state_t) {
	case MENU:
		if (*evt == EXIT) {
			exit();
		}
		break;

	case GAME_SINGLE:
		if (*evt == EXIT) {
			state_t = MENU;
			menu_loop();
		}
		break;

	case GAME_MULTI:

		break;

	default:
		printf("planetary::event_update Unrecognized State!");
		break;
	}
}
// TODO Mudar V_MEM ptr para o video_gr.c
int start() {
	sef_startup();

	if ( (v_mem = vg_init(MODE_800X600_64k)) == NULL ) {
		printf("test_square failed VRAM map in vg_init\n");
		return 1;
	}

	// Iniciar Menu?
}

void exit() {
	vg_exit();
}

//event_t menu_loop(char * v_mem) {
//	if (NULL == v_mem) {
//		start();
//	}
//
//	// Loop (com teclado e rato) que retorna o evento correto quando é premido o botão START_GAME etc.
//}

event_t game_loop(){
	if (NULL == v_mem) {
		start();
	}

	GVector * e_missiles = new_gvector(sizeof(Missile));
	GVector * f_missiles = new_gvector(sizeof(Missile));

	int r; int esc_flag = 0;
	while( ! esc_flag ) { // While ESC BreakCode not detected
		/* Get a request message. */
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			 printf("driver_receive failed with: %d\n", r);
			 continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & keyboard_irq_set) { /* keyboard interrupt */

				}

				if (msg.NOTIFY_ARG & timer_irq_set) { /* timer interrupt */
					/* Update Input, game_state and draw current state */



					/* Draw Game */

					unsigned i;
					// Draw enemy missiles
					for (i = 0; i < gvector_get_size(e_missiles); ++i) {
						draw_missile(gvector_at(e_missiles, i));
					}

					// Draw friendly missiles
					for (i = 0; i < gvector_get_size(f_missiles); ++i) {
						// TODO
						gvector_at(f_missiles, i);
					}
				}

				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
				/* no standard messages expected: do nothing */
			printf("Error: received unexpected standard message.\n");
			return 1;
		}
	}

	if ( kbd_unsubscribe_int() < 0 ) {
		printf("game_loop -> FAILED kbd_unsubscribe_int()\n");
		return 1;
	}
	if ( timer_unsubscribe_int() < 0 ) {
		printf("game_loop -> FAILED timer_unsubscribe_int()\n");
		return 1;
	}


//	event_t evt = EXIT;
//	return evt;
	return event_t(EXIT);
}
