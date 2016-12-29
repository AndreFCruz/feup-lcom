#define RTC_ADDR_REG			0x70
#define RTC_DATA_REG			0x71

# For use in C code
.global _parserBCD
.global _rtc_read_register

.text
_parserBCD:
	xor 	%eax, %eax
	pushl	%ebp
	movl	%esp, %ebp	# copy stack pointer
	cmpl	$10, 8(%ebp)	# arg > 10 ?
	jle		BCDready	# arg < 10, already in BCD
	movl	8(%ebp), %eax
	movl	%eax, %edx
	sarl	$4, %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	addl	%eax, %eax
	movl	8(%ebp), %edx
	andl	$0x0F, %edx
	addl	%edx, %eax
	jmp		BCDend
BCDready:
	movl	8(%ebp), %eax
BCDend:
	popl	%ebp
	ret


_rtc_read_register:
	xor		%eax, %eax
	pushl	%ebp		# save ebp
	movl	%esp, %ebp
	subl	$40, %esp
	movl	8(%ebp), %eax
	movl	$16, 8(%esp)
	movl	%eax, 4(%esp)
	movl	$RTC_ADDR_REG, (%esp)
	call	_sys_out
	testl	%eax, %eax
	jz		OK1		# sys_out returned OK
	movl	$-1, %eax
	jmp		read_end
OK1:
	movl	$16, 8(%esp)
	leal	-12(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	$RTC_DATA_REG, (%esp)
	call	_sys_in
	testl	%eax, %eax
	jz		OK2			# sys_in returned  OK
	movl	$-1, %eax
	jmp		read_end
OK2:
	movl	-12(%ebp), %eax
read_end:
	leave
	ret

