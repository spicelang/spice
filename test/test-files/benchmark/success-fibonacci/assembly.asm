	.text
	.def	@feat.00;
	.scl	3;
	.type	0;
	.endef
	.globl	@feat.00
.set @feat.00, 0
	.file	"source.spice"
	.def	.L_f__void__int__fibo__int;
	.scl	3;
	.type	32;
	.endef
	.p2align	4, 0x90
.L_f__void__int__fibo__int:
	pushq	%rsi
	pushq	%rdi
	subq	$40, %rsp
	movl	%ecx, %edi
	xorl	%esi, %esi
	cmpl	$2, %ecx
	jge	.LBB0_3
	movl	%edi, %ecx
	jmp	.LBB0_2
.LBB0_3:
	xorl	%esi, %esi
	.p2align	4, 0x90
.LBB0_4:
	leal	-1(%rdi), %ecx
	callq	.L_f__void__int__fibo__int
	leal	-2(%rdi), %ecx
	addl	%eax, %esi
	cmpl	$4, %edi
	movl	%ecx, %edi
	jae	.LBB0_4
.LBB0_2:
	addl	%ecx, %esi
	movl	%esi, %eax
	addq	$40, %rsp
	popq	%rdi
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
	movl	$30, %ecx
	callq	.L_f__void__int__fibo__int
	leaq	.Lprintf.str.0(%rip), %rcx
	movl	%eax, %edx
	callq	printf
	xorl	%eax, %eax
	addq	$32, %rsp
	popq	%rbp
	retq