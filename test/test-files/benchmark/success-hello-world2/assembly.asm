	.text
	.def	@feat.00;
	.scl	3;
	.type	0;
	.endef
	.globl	@feat.00
.set @feat.00, 0
	.file	"source.spice"
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
	movl	$72, %ecx
	callq	putchar
	movl	$101, %ecx
	callq	putchar
	movl	$108, %ecx
	callq	putchar
	movl	$108, %ecx
	callq	putchar
	movl	$111, %ecx
	callq	putchar
	movl	$32, %ecx
	callq	putchar
	movl	$87, %ecx
	callq	putchar
	movl	$111, %ecx
	callq	putchar
	movl	$114, %ecx
	callq	putchar
	movl	$108, %ecx
	callq	putchar
	movl	$100, %ecx
	callq	putchar
	movl	$32, %ecx
	callq	putchar
	xorl	%eax, %eax
	addq	$32, %rsp
	popq	%rbp
	retq
	.seh_endproc

