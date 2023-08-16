	.text
	.def	@feat.00;
	.scl	3;
	.type	0;
	.endef
	.globl	@feat.00
.set @feat.00, 0
	.file	"source.spice"
	.def	.L_Z3fibi;
	.scl	3;
	.type	32;
	.endef
	.p2align	4, 0x90
.L_Z3fibi:
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
	callq	.L_Z3fibi
	addl	%eax, %esi
	leal	-2(%rdi), %eax
	incl	%edi
	cmpl	$5, %edi
	movl	%eax, %edi
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
	subq	$184, %rsp
	.seh_stackalloc 184
	leaq	128(%rsp), %rbp
	.seh_setframe %rbp, 128
	.seh_endprologue
	callq	__main
	leaq	-88(%rbp), %r13
	addq	$16, %r13
	leaq	-88(%rbp), %r12
	addq	$32, %r12
	leaq	-88(%rbp), %r15
	addq	$48, %r15
	leaq	-88(%rbp), %r14
	addq	$64, %r14
	leaq	-88(%rbp), %rbx
	addq	$80, %rbx
	leaq	-88(%rbp), %rdi
	addq	$96, %rdi
	leaq	-88(%rbp), %rsi
	addq	$112, %rsi
	leaq	-88(%rbp), %rcx
	addq	$16, %rcx
	xorl	%edx, %edx
	movl	$112, %r8d
	callq	memset
	leaq	40(%rbp), %rcx
	leaq	.L_Z15lambda.L12C29.0v(%rip), %rdx
	callq	_ZN6Thread4ctorEPFvE
	movq	40(%rbp), %rax
	movq	%rax, -88(%rbp)
	movq	48(%rbp), %rax
	movq	%rax, -80(%rbp)
	leaq	-88(%rbp), %rcx
	callq	_ZN6Thread3runEv
	leaq	40(%rbp), %rcx
	leaq	.L_Z15lambda.L12C29.0v(%rip), %rdx
	callq	_ZN6Thread4ctorEPFvE
	movq	40(%rbp), %rax
	movq	%rax, -72(%rbp)
	movq	48(%rbp), %rax
	movq	%rax, -64(%rbp)
	movq	%r13, %rcx
	callq	_ZN6Thread3runEv
	leaq	40(%rbp), %rcx
	leaq	.L_Z15lambda.L12C29.0v(%rip), %rdx
	callq	_ZN6Thread4ctorEPFvE
	movq	40(%rbp), %rax
	movq	%rax, -56(%rbp)
	movq	48(%rbp), %rax
	movq	%rax, -48(%rbp)
	movq	%r12, %rcx
	callq	_ZN6Thread3runEv
	leaq	40(%rbp), %rcx
	leaq	.L_Z15lambda.L12C29.0v(%rip), %rdx
	callq	_ZN6Thread4ctorEPFvE
	movq	40(%rbp), %rax
	movq	%rax, -40(%rbp)
	movq	48(%rbp), %rax
	movq	%rax, -32(%rbp)
	movq	%r15, %rcx
	callq	_ZN6Thread3runEv
	leaq	40(%rbp), %rcx
	leaq	.L_Z15lambda.L12C29.0v(%rip), %rdx
	callq	_ZN6Thread4ctorEPFvE
	movq	40(%rbp), %rax
	movq	%rax, -24(%rbp)
	movq	48(%rbp), %rax
	movq	%rax, -16(%rbp)
	movq	%r14, %rcx
	callq	_ZN6Thread3runEv
	leaq	40(%rbp), %rcx
	leaq	.L_Z15lambda.L12C29.0v(%rip), %rdx
	callq	_ZN6Thread4ctorEPFvE
	movq	40(%rbp), %rax
	movq	%rax, -8(%rbp)
	movq	48(%rbp), %rax
	movq	%rax, (%rbp)
	movq	%rbx, %rcx
	callq	_ZN6Thread3runEv
	leaq	40(%rbp), %rcx
	leaq	.L_Z15lambda.L12C29.0v(%rip), %rdx
	callq	_ZN6Thread4ctorEPFvE
	movq	40(%rbp), %rax
	movq	%rax, 8(%rbp)
	movq	48(%rbp), %rax
	movq	%rax, 16(%rbp)
	movq	%rdi, %rcx
	callq	_ZN6Thread3runEv
	leaq	40(%rbp), %rcx
	leaq	.L_Z15lambda.L12C29.0v(%rip), %rdx
	callq	_ZN6Thread4ctorEPFvE
	movq	40(%rbp), %rax
	movq	%rax, 24(%rbp)
	movq	48(%rbp), %rax
	movq	%rax, 32(%rbp)
	movq	%rsi, %rcx
	callq	_ZN6Thread3runEv
	leaq	.Lstr(%rip), %rcx
	callq	puts
	leaq	-88(%rbp), %rcx
	callq	_ZN6Thread4joinEv
	movq	%r13, %rcx
	callq	_ZN6Thread4joinEv
	movq	%r12, %rcx
	callq	_ZN6Thread4joinEv
	movq	%r15, %rcx
	callq	_ZN6Thread4joinEv
	movq	%r14, %rcx
	callq	_ZN6Thread4joinEv
	movq	%rbx, %rcx
	callq	_ZN6Thread4joinEv
	movq	%rdi, %rcx
	callq	_ZN6Thread4joinEv
	movq	%rsi, %rcx
	callq	_ZN6Thread4joinEv
	leaq	.Lprintf.str.2(%rip), %rcx
	callq	printf
	xorl	%eax, %eax
	addq	$184, %rsp
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

	.def	.L_Z15lambda.L12C29.0v;
	.scl	3;
	.type	32;
	.endef
	.p2align	4, 0x90
.L_Z15lambda.L12C29.0v:
	subq	$40, %rsp
	movl	$30, %ecx
	callq	.L_Z3fibi
	leaq	.Lprintf.str.0(%rip), %rcx
	movl	%eax, %edx
	callq	printf
	addq	$40, %rsp
	retq

	.section	.rdata,"dr"
.Lprintf.str.0:
	.asciz	"Thread returned with result: %d\n"

.Lprintf.str.2:
	.asciz	"Program finished"

.Lstr:
	.asciz	"Started all threads. Waiting for results ..."

