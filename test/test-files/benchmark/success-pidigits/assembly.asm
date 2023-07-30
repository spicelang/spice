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
	xorl	%ebx, %ebx
	movl	$1, %esi
	movl	$3, %r11d
	xorl	%r13d, %r13d
	xorl	%r10d, %r10d
	movl	$3, %r15d
	movl	$1, %edx
	movl	$1, %r12d
	jmp	.LBB0_1
.LBB0_1:
	movq	%rsi, %rax
	shlq	$2, %rax
	movq	%r13, %rcx
	subq	%r12, %rcx
	addq	%rax, %rcx
	movq	%r15, %rdi
	imulq	%r12, %rdi
	cmpq	%rdi, %rcx
	jge	.LBB0_5
	movq	%rdx, (%rbp)
	movq	%r11, -8(%rbp)
	leaq	.Lprintf.str.0(%rip), %rcx
	movq	%r15, %rdx
	movl	%r10d, %r14d
	callq	printf
	movl	%r14d, %r10d
	cmpl	$0, %r10d
	jne	.LBB0_4
	movl	$46, %ecx
	callq	putchar
	movl	%r14d, %r10d
.LBB0_4:
	addl	$1, %r10d
	imulq	$10, %rsi, %rcx
	movq	%r13, %rax
	subq	%rdi, %rax
	imulq	$10, %rax, %r8
	imulq	$3, %rsi, %rax
	addq	%r13, %rax
	imulq	$10, %rax, %rax
	cqto
	idivq	%r12
	imulq	$-10, %r15, %rdx
	addq	%rdx, %rax
	movq	%r8, %r13
	movq	%rax, %r15
	movq	%rcx, %rsi
	movq	-8(%rbp), %r11
	movq	(%rbp), %rdx
	jmp	.LBB0_6
.LBB0_5:
	movq	%rsi, %rcx
	imulq	%rdx, %rcx
	movq	%rsi, %r8
	shlq	%r8
	addq	%r13, %r8
	imulq	%r11, %r8
	imulq	%r11, %r12
	movq	%rdx, %r9
	addq	$1, %r9
	imulq	$7, %rdx, %rax
	addq	$2, %rax
	imulq	%rax, %rsi
	movq	%r11, %rax
	imulq	%r13, %rax
	addq	%rax, %rsi
	movq	%rsi, %rax
	cqto
	idivq	%r12
	movq	%rax, %r15
	addq	$2, %r11
	movq	%r8, %r13
	movq	%r9, %rdx
	movq	%rcx, %rsi
.LBB0_6:
	addl	$1, %ebx
	cmpl	$20, %ebx
	jne	.LBB0_1
	xorl	%eax, %eax
	addq	$56, %rsp
	popq	%rbx
	popq	%rdi
	popq	%rsi
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	popq	%rbp
	retq
	.seh_endproc

	.section	.rdata,"dr"
.Lprintf.str.0:
	.asciz	"%d"

