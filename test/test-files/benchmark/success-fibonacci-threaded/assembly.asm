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
	.p2align	4, 0x90                         # -- Begin function _Z3fibi
.L_Z3fibi:                              # @_Z3fibi
# %bb.0:
	pushq	%rsi
	pushq	%rdi
	subq	$40, %rsp
	movl	$1, %esi
	cmpl	$3, %ecx
	jl	.LBB0_4
# %bb.1:                                # %if.exit.L4.preheader
	movl	%ecx, %edi
	decl	%edi
	xorl	%esi, %esi
	.p2align	4, 0x90
.LBB0_2:                                # %if.exit.L4
                                        # =>This Inner Loop Header: Depth=1
	movl	%edi, %ecx
	callq	.L_Z3fibi
	addl	%eax, %esi
	leal	-2(%rdi), %eax
	incl	%edi
	cmpl	$5, %edi
	movl	%eax, %edi
	jae	.LBB0_2
# %bb.3:                                # %common.ret.loopexit
	incl	%esi
.LBB0_4:                                # %common.ret
	movl	%esi, %eax
	addq	$40, %rsp
	popq	%rdi
	popq	%rsi
	retq
                                        # -- End function
	.def	main;
	.scl	2;
	.type	32;
	.endef
	.globl	main                            # -- Begin function main
	.p2align	4, 0x90
main:                                   # @main
.seh_proc main
# %bb.0:                                # %for.body.L11
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
	subq	$248, %rsp
	.seh_stackalloc 248
	leaq	128(%rsp), %rbp
	.seh_setframe %rbp, 128
	.seh_endprologue
	callq	__main
	leaq	-96(%rbp), %r13
	addq	$24, %r13
	leaq	-96(%rbp), %r12
	addq	$48, %r12
	leaq	-96(%rbp), %r15
	addq	$72, %r15
	leaq	-96(%rbp), %r14
	addq	$96, %r14
	leaq	-96(%rbp), %rbx
	addq	$120, %rbx
	leaq	-96(%rbp), %rdi
	addq	$144, %rdi
	leaq	-96(%rbp), %rsi
	addq	$168, %rsi
	leaq	-96(%rbp), %rcx
	addq	$24, %rcx
	xorl	%edx, %edx
	movl	$168, %r8d
	callq	memset
	leaq	.L_Z15lambda.L12C29.0v(%rip), %rdx
	leaq	96(%rbp), %rcx
	callq	_ZN6Thread4ctorEPFvE
	movq	96(%rbp), %rax
	movq	104(%rbp), %rcx
	movq	112(%rbp), %rdx
	movq	%rax, -96(%rbp)
	movq	%rcx, -88(%rbp)
	movq	%rdx, -80(%rbp)
	leaq	-96(%rbp), %rcx
	callq	_ZN6Thread3runEv
	leaq	.L_Z15lambda.L12C29.0v(%rip), %rdx
	leaq	96(%rbp), %rcx
	callq	_ZN6Thread4ctorEPFvE
	movq	96(%rbp), %rax
	movq	104(%rbp), %rcx
	movq	112(%rbp), %rdx
	movq	%rax, -72(%rbp)
	movq	%rcx, -64(%rbp)
	movq	%rdx, -56(%rbp)
	movq	%r13, %rcx
	callq	_ZN6Thread3runEv
	leaq	.L_Z15lambda.L12C29.0v(%rip), %rdx
	leaq	96(%rbp), %rcx
	callq	_ZN6Thread4ctorEPFvE
	movq	96(%rbp), %rax
	movq	104(%rbp), %rcx
	movq	112(%rbp), %rdx
	movq	%rax, -48(%rbp)
	movq	%rcx, -40(%rbp)
	movq	%rdx, -32(%rbp)
	movq	%r12, %rcx
	callq	_ZN6Thread3runEv
	leaq	.L_Z15lambda.L12C29.0v(%rip), %rdx
	leaq	96(%rbp), %rcx
	callq	_ZN6Thread4ctorEPFvE
	movq	96(%rbp), %rax
	movq	104(%rbp), %rcx
	movq	112(%rbp), %rdx
	movq	%rax, -24(%rbp)
	movq	%rcx, -16(%rbp)
	movq	%rdx, -8(%rbp)
	movq	%r15, %rcx
	callq	_ZN6Thread3runEv
	leaq	.L_Z15lambda.L12C29.0v(%rip), %rdx
	leaq	96(%rbp), %rcx
	callq	_ZN6Thread4ctorEPFvE
	movq	96(%rbp), %rax
	movq	104(%rbp), %rcx
	movq	112(%rbp), %rdx
	movq	%rax, (%rbp)
	movq	%rcx, 8(%rbp)
	movq	%rdx, 16(%rbp)
	movq	%r14, %rcx
	callq	_ZN6Thread3runEv
	leaq	.L_Z15lambda.L12C29.0v(%rip), %rdx
	leaq	96(%rbp), %rcx
	callq	_ZN6Thread4ctorEPFvE
	movq	96(%rbp), %rax
	movq	104(%rbp), %rcx
	movq	112(%rbp), %rdx
	movq	%rax, 24(%rbp)
	movq	%rcx, 32(%rbp)
	movq	%rdx, 40(%rbp)
	movq	%rbx, %rcx
	callq	_ZN6Thread3runEv
	leaq	.L_Z15lambda.L12C29.0v(%rip), %rdx
	leaq	96(%rbp), %rcx
	callq	_ZN6Thread4ctorEPFvE
	movq	96(%rbp), %rax
	movq	104(%rbp), %rcx
	movq	112(%rbp), %rdx
	movq	%rax, 48(%rbp)
	movq	%rcx, 56(%rbp)
	movq	%rdx, 64(%rbp)
	movq	%rdi, %rcx
	callq	_ZN6Thread3runEv
	leaq	.L_Z15lambda.L12C29.0v(%rip), %rdx
	leaq	96(%rbp), %rcx
	callq	_ZN6Thread4ctorEPFvE
	movq	96(%rbp), %rax
	movq	104(%rbp), %rcx
	movq	112(%rbp), %rdx
	movq	%rax, 72(%rbp)
	movq	%rcx, 80(%rbp)
	movq	%rdx, 88(%rbp)
	movq	%rsi, %rcx
	callq	_ZN6Thread3runEv
	leaq	.Lstr(%rip), %rcx
	callq	puts
	leaq	-96(%rbp), %rcx
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
	addq	$248, %rsp
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
                                        # -- End function
	.def	.L_Z15lambda.L12C29.0v;
	.scl	3;
	.type	32;
	.endef
	.p2align	4, 0x90                         # -- Begin function _Z15lambda.L12C29.0v
.L_Z15lambda.L12C29.0v:                 # @_Z15lambda.L12C29.0v
# %bb.0:
	subq	$40, %rsp
	movl	$30, %ecx
	callq	.L_Z3fibi
	leaq	.Lprintf.str.0(%rip), %rcx
	movl	%eax, %edx
	callq	printf
	addq	$40, %rsp
	retq
                                        # -- End function
	.section	.rdata,"dr"
.Lprintf.str.0:                         # @printf.str.0
	.asciz	"Thread returned with result: %d\n"

.Lprintf.str.2:                         # @printf.str.2
	.asciz	"Program finished"

.Lstr:                                  # @str
	.asciz	"Started all threads. Waiting for results ..."

