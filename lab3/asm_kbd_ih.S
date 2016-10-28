#include "i8042.h"


.data
maxIter:
	.int 15

#.global _kbd_read_return
.global _keyboard_read_asm

.text
_keyboard_read_asm:
		xor %ecx, %ecx
		xor %eax, %eax
start:
		cmp %ecx, maxIter
		je	final
		inl	$STAT_REG, %eax		# eax := kbd status
		test $STAT_OBF, %eax
		jz	next
		in	$OUT_BUF, %eax		# eax := data
		jmp final
next:
		#tick_delay
		addl $1, %ecx
		jmp start
final:
		leave
		ret
