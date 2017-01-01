#define RTC_ADDR_REG			0x70
#define RTC_DATA_REG			0x71

# For use in C code
.global _parserBCD
.global _rtc_read_register

.text

_parserBCD: # 10 * (val >> 4) + val & 0x0F
	xor 	%eax, %eax
	pushl	%ebp			# prologue
	movl	%esp, %ebp		#
	cmpl	$10, 8(%ebp)	# arg > 10 ?
	jle		BCDready		# arg < 10 -> already in BCD
	movl	8(%ebp), %eax
	sarl	$4, %eax
    imull   $10, %eax, %eax
	movl	8(%ebp), %edx
    andl    $0x0F, %edx
    addl    %edx, %eax
	jmp		BCDend
BCDready:
	movl	8(%ebp), %eax
BCDend:
#	popl	%ebp
	leave
	ret


_rtc_read_register:
	xor		%eax, %eax
	pushl	%ebp			# save ebp
	movl	%esp, %ebp
	subl	$0x20, %esp		# reserve space
	movl	8(%ebp), %eax
	movl	$0x10, 8(%esp)
	movl	%eax, 4(%esp)	# push args for sys_call
	movl	$RTC_ADDR_REG, (%esp)	#
	call	_sys_out		# call sys_out
	testl	%eax, %eax		# return is in eax
	jz		OK1				# sys_out returned OK
	movl	$-1, %eax
	jmp		read_end
OK1:
#	inb		$RTC_DATA_REG	# result in eax
	movl	$0x10, 8(%esp)
	leal	-12(%ebp), %eax	# load ptr to eax
	movl	%eax, 4(%esp)
	movl	$RTC_DATA_REG, (%esp)
	call	_sys_in
	testl	%eax, %eax
	jz		OK2				# sys_in returned  OK
	movl	$-1, %eax
	jmp		read_end
OK2:
	movl	-12(%ebp), %eax
read_end:
	leave
	ret

