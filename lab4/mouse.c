#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <minix/sysutil.h>
#include "mouse.h"
#include "i8042.h"

typedef int bool;

static int mouse_hook_id = MOUSE_INITIAL_HOOK_ID;
static const unsigned maxIter = 15;         // Maximum Iterations when retrieving data

int mouse_subscribe_int(void)
{
        if ( sys_irqsetpolicy (MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id) != OK ) {
                printf("mouse_subscribe_int() -> FAILED sys_irqsetpolicy()\n");
                return -1;
        }
        if ( sys_irqenable (&mouse_hook_id) != OK ) {
                printf("mouse_subscribe_int() -> FAILED sys_irqenable()\n");
                return -1;
        }

        return MOUSE_INITIAL_HOOK_ID;
}

int mouse_unsubscribe_int(void)
{
        if ( sys_irqdisable (&mouse_hook_id) != OK ) {
                printf("mouse_unsubscribe_int() -> FAILED sys_irqdisable()\n");
                return -1;
        }
        if ( sys_irqrmpolicy (&mouse_hook_id) != OK ) {
                printf("mouse_unsubscribe_int() -> FAILED sys_irqrmpolicy()\n");
                return -1;
        }

        return MOUSE_INITIAL_HOOK_ID;
}


//TODO: funcao generica mouse_write_command que escreve um comando no mouse. Serve para funcoes 1,2 e 3! Usa kbd_read e kbd_write!

//Draft
int mouse_write_command()
{

	if (keyboard_write (WRITE_B_MOUSE) != OK)
		return 1;		//Print of error is done in keyboard_write()

	if ( (kbdResponse = keyboard_read ()) == -1 ) {
		printf ("keyboard_write_command() -> FAILED sys_inb()\n");
		return 1;
	}

	//Talvez nao de para usar as kbd_write e kbd_read. Pela necessidade de verificar o bit 5.... VER isto
}
