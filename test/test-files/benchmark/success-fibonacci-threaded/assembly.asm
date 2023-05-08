	.text
	.def	@feat.00;
	.scl	3;
	.type	0;
	.endef
	.globl	@feat.00
.set @feat.00, 0
	.file	"source.spice"
	.def	.L_f__void__int__fib__int;
	.scl	3;
	.type	32;
	.endef
	.p2align	4, 0x90
.L_f__void__int__fib__int:
	pushq	%rsi
	pushq	%rdi
	subq	$40, %rsp
	movl	$1, %esi
	cmpl	$3, %ecx
	jl	.LBB0_4
	movl	%ecx, %edi
	decl	%edi
	xorl	%esi, %esi
	.p2align	4, 0x90
.LBB0_2:
	movl	%edi, %ecx
	callq	.L_f__void__int__fib__int
	leal	-2(%rdi), %ecx
	addl	%eax, %esi
	incl	%edi
	cmpl	$5, %edi
	movl	%ecx, %edi
	jae	.LBB0_2
	incl	%esi
.LBB0_4:
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
	subq	$120, %rsp
	.seh_stackalloc 120
	leaq	112(%rsp), %rbp
	.seh_setframe %rbp, 112
	.seh_endprologue
	callq	__main
	leaq	-72(%rbp), %rcx
	leaq	.L_thread0(%rip), %r8
	movq	%rbp, %r9
	xorl	%edx, %edx
	callq	pthread_create
	movq	-72(%rbp), %rax
	movq	%rax, -8(%rbp)
	leaq	-64(%rbp), %rcx
	leaq	.L_thread0(%rip), %r8
	movq	%rbp, %r9
	xorl	%edx, %edx
	callq	pthread_create
	movq	-64(%rbp), %rdi
	leaq	-56(%rbp), %rcx
	leaq	.L_thread0(%rip), %r8
	movq	%rbp, %r9
	xorl	%edx, %edx
	callq	pthread_create
	movq	-56(%rbp), %rbx
	leaq	-48(%rbp), %rcx
	leaq	.L_thread0(%rip), %r8
	movq	%rbp, %r9
	xorl	%edx, %edx
	callq	pthread_create
	movq	-48(%rbp), %r14
	leaq	-40(%rbp), %rcx
	leaq	.L_thread0(%rip), %r8
	movq	%rbp, %r9
	xorl	%edx, %edx
	callq	pthread_create
	movq	-40(%rbp), %r15
	leaq	-32(%rbp), %rcx
	leaq	.L_thread0(%rip), %r8
	movq	%rbp, %r9
	xorl	%edx, %edx
	callq	pthread_create
	movq	-32(%rbp), %r12
	leaq	-24(%rbp), %rcx
	leaq	.L_thread0(%rip), %r8
	movq	%rbp, %r9
	xorl	%edx, %edx
	callq	pthread_create
	movq	-24(%rbp), %r13
	leaq	-16(%rbp), %rcx
	leaq	.L_thread0(%rip), %r8
	movq	%rbp, %r9
	xorl	%edx, %edx
	callq	pthread_create
	movq	-16(%rbp), %rsi
	leaq	.Lstr(%rip), %rcx
	callq	puts
	movq	-8(%rbp), %rcx
	xorl	%edx, %edx
	callq	pthread_join
	movq	%rdi, %rcx
	xorl	%edx, %edx
	callq	pthread_join
	movq	%rbx, %rcx
	xorl	%edx, %edx
	callq	pthread_join
	movq	%r14, %rcx
	xorl	%edx, %edx
	callq	pthread_join
	movq	%r15, %rcx
	xorl	%edx, %edx
	callq	pthread_join
	movq	%r12, %rcx
	xorl	%edx, %edx
	callq	pthread_join
	movq	%r13, %rcx
	xorl	%edx, %edx
	callq	pthread_join
	movq	%rsi, %rcx
	xorl	%edx, %edx
	callq	pthread_join
	leaq	.Lprintf.str.2(%rip), %rcx
	callq	printf
	xorl	%eax, %eax
	addq	$120, %rsp
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

	.def	.L_thread0;
	.scl	3;
	.type	32;
	.endef
	.p2align	4, 0x90
.L_thread0:
	subq	$40, %rsp
	movl	$30, %ecx
	callq	.L_f__void__int__fib__int
	leaq	.Lprintf.str.0(%rip), %rcx
	movl	%eax, %edx
	callq	printf
	xorl	%eax, %eax
	addq	$40, %rsp
	retq

	.section	.rdata,"dr"
.Lprintf.str.0:
	.asciz	"Thread returned with result: %d\n"

.Lprintf.str.2:
	.asciz	"Program finished"

.Lstr:
	.asciz	"Started all threads. Waiting for results ..."

