	.text
	.def	@feat.00;
	.scl	3;
	.type	0;
	.endef
	.globl	@feat.00
.set @feat.00, 0
	.file	"source.spice"
	.def	.L_f__void__int__ack__int_int;
	.scl	3;
	.type	32;
	.endef
	.p2align	4, 0x90
.L_f__void__int__ack__int_int:
	pushq	%rsi
	subq	$32, %rsp
	movl	%edx, %eax
	testl	%ecx, %ecx
	je	.LBB0_5
	movl	%ecx, %esi
	jmp	.LBB0_2
	.p2align	4, 0x90
.LBB0_3:
	movl	$1, %eax
	decl	%esi
	je	.LBB0_5
.LBB0_2:
	testl	%eax, %eax
	je	.LBB0_3
	decl	%eax
	movl	%esi, %ecx
	movl	%eax, %edx
	callq	.L_f__void__int__ack__int_int
	decl	%esi
	jne	.LBB0_2
.LBB0_5:
	incl	%eax
	addq	$32, %rsp
	popq	%rsi
	retq

	.def	main;
	.scl	2;
	.type	32;
	.endef
	.globl	main
	.p2align	4, 0x90
main:
.seh_proc main
	pushq	%rbp
	.seh_pushreg %rbp
	subq	$32, %rsp
	.seh_stackalloc 32
	leaq	32(%rsp), %rbp
	.seh_setframe %rbp, 32
	.seh_endprologue
	callq	__main
	movl	$3, %ecx
	movl	$12, %edx
	callq	.L_f__void__int__ack__int_int
	leaq	.Lprintf.str.0(%rip), %rcx
	movl	$3, %edx
	movl	$12, %r8d
	movl	%eax, %r9d
	callq	printf
	xorl	%eax, %eax
	addq	$32, %rsp
	popq	%rbp
	retq
	.seh_endproc

	.section	.rdata,"dr"
.Lprintf.str.0:
	.asciz	"Ackermann of base m=%d and n=%d: %d"

