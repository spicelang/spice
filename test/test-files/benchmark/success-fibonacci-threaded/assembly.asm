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

	.def	.L_Z9calcFib30v;
	.scl	3;
	.type	32;
	.endef
	.p2align	4, 0x90
.L_Z9calcFib30v:
	subq	$40, %rsp
	movl	$30, %ecx
	callq	.L_Z3fibi
	leaq	.Lprintf.str.0(%rip), %rcx
	movl	%eax, %edx
	callq	printf
	addq	$40, %rsp
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
	subq	$104, %rsp
	.seh_stackalloc 104
	leaq	96(%rsp), %rbp
	.seh_setframe %rbp, 96
	.seh_endprologue
	callq	__main
	leaq	-64(%rbp), %r13
	addq	$8, %r13
	leaq	-64(%rbp), %r12
	addq	$16, %r12
	leaq	-64(%rbp), %r15
	addq	$24, %r15
	leaq	-64(%rbp), %r14
	addq	$32, %r14
	leaq	-64(%rbp), %rbx
	addq	$40, %rbx
	leaq	-64(%rbp), %rdi
	addq	$48, %rdi
	leaq	-64(%rbp), %rsi
	addq	$56, %rsi
	movq	%rbp, %rcx
	leaq	.L_Z9calcFib30v(%rip), %rdx
	callq	_ZN6Thread4ctorEPFvE
	movq	(%rbp), %rax
	movq	%rax, -64(%rbp)
	movq	%rbp, %rcx
	leaq	.L_Z9calcFib30v(%rip), %rdx
	callq	_ZN6Thread4ctorEPFvE
	movq	(%rbp), %rax
	movq	%rax, -56(%rbp)
	movq	%rbp, %rcx
	leaq	.L_Z9calcFib30v(%rip), %rdx
	callq	_ZN6Thread4ctorEPFvE
	movq	(%rbp), %rax
	movq	%rax, -48(%rbp)
	movq	%rbp, %rcx
	leaq	.L_Z9calcFib30v(%rip), %rdx
	callq	_ZN6Thread4ctorEPFvE
	movq	(%rbp), %rax
	movq	%rax, -40(%rbp)
	movq	%rbp, %rcx
	leaq	.L_Z9calcFib30v(%rip), %rdx
	callq	_ZN6Thread4ctorEPFvE
	movq	(%rbp), %rax
	movq	%rax, -32(%rbp)
	movq	%rbp, %rcx
	leaq	.L_Z9calcFib30v(%rip), %rdx
	callq	_ZN6Thread4ctorEPFvE
	movq	(%rbp), %rax
	movq	%rax, -24(%rbp)
	movq	%rbp, %rcx
	leaq	.L_Z9calcFib30v(%rip), %rdx
	callq	_ZN6Thread4ctorEPFvE
	movq	(%rbp), %rax
	movq	%rax, -16(%rbp)
	movq	%rbp, %rcx
	leaq	.L_Z9calcFib30v(%rip), %rdx
	callq	_ZN6Thread4ctorEPFvE
	movq	(%rbp), %rax
	movq	%rax, -8(%rbp)
	leaq	.Lstr(%rip), %rcx
	callq	puts
	leaq	-64(%rbp), %rcx
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
	addq	$104, %rsp
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
	.asciz	"Thread returned with result: %d\n"

.Lprintf.str.2:
	.asciz	"Program finished"

.Lstr:
	.asciz	"Started all threads. Waiting for results ..."

