#include "i8042.h"


.data
maxIter:
	.short 15

.global _keyboard_read_asm

.text
_keyboard_read_asm:
		xor %ecx, %ecx
		xor %eax, %eax
start:
		cmp %cl, maxIter		# cl := iterations count
		je	unsuccess
		in	$STAT_REG, %eax		# eax := kbd status
		test $STAT_OBF, %eax
		jz	next
		in	$OUT_BUF, %eax		# eax := data
		mov 1, %ch				# ch := flag de data
		jmp success
next:
		#invoke _micros_to_ticks, $KBD_DELAY_US
		push $KBD_DELAY_US
		call _micros_to_ticks
		push %eax
		call _tickdelay
		addb $1, %cl
		jmp	start
unsuccess:
		cmp $1, %ch
		je	success
		mov -1, %eax
success:
		leave
		ret
