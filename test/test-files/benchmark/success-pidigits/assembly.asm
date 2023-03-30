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
	pushq	%r15
	.seh_pushreg %r15
	pushq	%r14
	.seh_pushreg %r14
	pushq	%r13
	.seh_pushreg %r13
	pushq	%r12
	.seh_pushreg %r12
	pushq	%rsi
	.seh_pushreg %rsi
	pushq	%rdi
	.seh_pushreg %rdi
	pushq	%rbx
	.seh_pushreg %rbx
	subq	$56, %rsp
	.seh_stackalloc 56
	leaq	48(%rsp), %rbp
	.seh_setframe %rbp, 48
	.seh_endprologue
	callq	__main
	xorl	%edi, %edi
	movl	$1, %r14d
	movl	$3, %r10d
	xorl	%r12d, %r12d
	xorl	%r11d, %r11d
	movl	$3, %esi
	movl	$1, %edx
	movl	$1, %ebx
	jmp	.LBB0_1
.LBB0_1:
	movq	%r14, %rax
	shlq	$2, %rax
	movq	%r12, %rcx
	addq	%rax, %rcx
	subq	%rbx, %rcx
	movq	%rbx, %r15
	imulq	%rsi, %r15
	cmpq	%r15, %rcx
	jge	.LBB0_5
	movq	%rdx, (%rbp)
	movq	%r10, -8(%rbp)
	leaq	.Lprintf.str.0(%rip), %rcx
	movq	%rsi, %rdx
	movl	%r11d, %r13d
	callq	printf
	movl	%r13d, %r11d
	cmpl	$0, %r11d
	jne	.LBB0_4
	movl	$46, %ecx
	callq	putchar
	movl	%r13d, %r11d
.LBB0_4:
	addl	$1, %r11d
	imulq	$10, %r14, %rcx
	movq	%r12, %rax
	subq	%r15, %rax
	imulq	$10, %rax, %r8
	imulq	$3, %r14, %rax
	addq	%rax, %r12
	imulq	$10, %r12, %rax
	cqto
	idivq	%rbx
	imulq	$-10, %rsi, %rdx
	addq	%rdx, %rax
	movq	%rax, %rsi
	movq	%r8, %r12
	movq	%rcx, %r14
	movq	-8(%rbp), %r10
	movq	(%rbp), %rdx
	jmp	.LBB0_6
.LBB0_5:
	movq	%rdx, %rcx
	imulq	%r14, %rcx
	movq	%r14, %rax
	shlq	$1, %rax
	movq	%r12, %r8
	addq	%rax, %r8
	imulq	%r10, %r8
	