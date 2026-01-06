	.file	"source.spice"
	.text
	.p2align	4                               # -- Begin function _Z3fibi
	.type	.L_Z3fibi,@function
.L_Z3fibi:                              # @_Z3fibi
# %bb.0:
	pushq	%r14
	pushq	%rbx
	pushq	%rax
	movl	$1, %ebx
	cmpl	$3, %edi
	jl	.LBB0_4
# %bb.1:                                # %if.exit.L4.preheader
	movl	%edi, %r14d
	decl	%r14d
	xorl	%ebx, %ebx
	.p2align	4
.LBB0_2:                                # %if.exit.L4
                                        # =>This Inner Loop Header: Depth=1
	movl	%r14d, %edi
	callq	.L_Z3fibi
	addl	%eax, %ebx
	leal	-2(%r14), %eax
	incl	%r14d
	cmpl	$5, %r14d
	movl	%eax, %r14d
	jae	.LBB0_2
# %bb.3:                                # %common.ret.loopexit
	incl	%ebx
.LBB0_4:                                # %common.ret
	movl	%ebx, %eax
	addq	$8, %rsp
	popq	%rbx
	popq	%r14
	retq
.Lfunc_end0:
	.size	.L_Z3fibi, .Lfunc_end0-.L_Z3fibi
                                        # -- End function
	.globl	main                            # -- Begin function main
	.p2align	4
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:                                # %for.body.L11
	pushq	%rbp
	.cfi_def_cfa_offset 16
	pushq	%r15
	.cfi_def_cfa_offset 24
	pushq	%r14
	.cfi_def_cfa_offset 32
	pushq	%r13
	.cfi_def_cfa_offset 40
	pushq	%r12
	.cfi_def_cfa_offset 48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	subq	$232, %rsp
	.cfi_def_cfa_offset 288
	.cfi_offset %rbx, -56
	.cfi_offset %r12, -48
	.cfi_offset %r13, -40
	.cfi_offset %r14, -32
	.cfi_offset %r15, -24
	.cfi_offset %rbp, -16
	leaq	40(%rsp), %rbx
	addq	$24, %rbx
	leaq	40(%rsp), %rbp
	addq	$48, %rbp
	leaq	40(%rsp), %r13
	addq	$72, %r13
	leaq	40(%rsp), %r12
	addq	$96, %r12
	leaq	40(%rsp), %r15
	addq	$120, %r15
	movq	%r15, 24(%rsp)                  # 8-byte Spill
	leaq	40(%rsp), %r14
	addq	$144, %r14
	leaq	40(%rsp), %rax
	addq	$168, %rax
	movq	%rax, 32(%rsp)                  # 8-byte Spill
	leaq	.L_Z15lambda.L12C29.0v(%rip), %rsi
	movq	%rsp, %rdi
	callq	_ZN6Thread4ctorEPFvE@PLT
	leaq	40(%rsp), %rdi
	addq	$24, %rdi
	xorl	%esi, %esi
	movl	$168, %edx
	callq	memset@PLT
	movq	(%rsp), %rax
	movq	%rax, 40(%rsp)
	movq	8(%rsp), %rax
	movq	%rax, 48(%rsp)
	movq	16(%rsp), %rax
	movq	%rax, 56(%rsp)
	leaq	40(%rsp), %rdi
	callq	_ZN6Thread3runEv@PLT
	leaq	.L_Z15lambda.L12C29.0v(%rip), %rsi
	movq	%rsp, %rdi
	callq	_ZN6Thread4ctorEPFvE@PLT
	movq	(%rsp), %rax
	movq	%rax, 64(%rsp)
	movq	8(%rsp), %rax
	movq	%rax, 72(%rsp)
	movq	16(%rsp), %rax
	movq	%rax, 80(%rsp)
	movq	%rbx, %rdi
	callq	_ZN6Thread3runEv@PLT
	leaq	.L_Z15lambda.L12C29.0v(%rip), %rsi
	movq	%rsp, %rdi
	callq	_ZN6Thread4ctorEPFvE@PLT
	movq	(%rsp), %rax
	movq	%rax, 88(%rsp)
	movq	8(%rsp), %rax
	movq	%rax, 96(%rsp)
	movq	16(%rsp), %rax
	movq	%rax, 104(%rsp)
	movq	%rbp, %rdi
	callq	_ZN6Thread3runEv@PLT
	leaq	.L_Z15lambda.L12C29.0v(%rip), %rsi
	movq	%rsp, %rdi
	callq	_ZN6Thread4ctorEPFvE@PLT
	movq	(%rsp), %rax
	movq	%rax, 112(%rsp)
	movq	8(%rsp), %rax
	movq	%rax, 120(%rsp)
	movq	16(%rsp), %rax
	movq	%rax, 128(%rsp)
	movq	%r13, %rdi
	callq	_ZN6Thread3runEv@PLT
	leaq	.L_Z15lambda.L12C29.0v(%rip), %rsi
	movq	%rsp, %rdi
	callq	_ZN6Thread4ctorEPFvE@PLT
	movq	(%rsp), %rax
	movq	%rax, 136(%rsp)
	movq	8(%rsp), %rax
	movq	%rax, 144(%rsp)
	movq	16(%rsp), %rax
	movq	%rax, 152(%rsp)
	movq	%r12, %rdi
	callq	_ZN6Thread3runEv@PLT
	leaq	.L_Z15lambda.L12C29.0v(%rip), %rsi
	movq	%rsp, %rdi
	callq	_ZN6Thread4ctorEPFvE@PLT
	movq	(%rsp), %rax
	movq	%rax, 160(%rsp)
	movq	8(%rsp), %rax
	movq	%rax, 168(%rsp)
	movq	16(%rsp), %rax
	movq	%rax, 176(%rsp)
	movq	%r15, %rdi
	callq	_ZN6Thread3runEv@PLT
	leaq	.L_Z15lambda.L12C29.0v(%rip), %rsi
	movq	%rsp, %rdi
	callq	_ZN6Thread4ctorEPFvE@PLT
	movq	(%rsp), %rax
	movq	%rax, 184(%rsp)
	movq	8(%rsp), %rax
	movq	%rax, 192(%rsp)
	movq	16(%rsp), %rax
	movq	%rax, 200(%rsp)
	movq	%r14, %rdi
	movq	%r14, %r15
	callq	_ZN6Thread3runEv@PLT
	leaq	.L_Z15lambda.L12C29.0v(%rip), %rsi
	movq	%rsp, %rdi
	callq	_ZN6Thread4ctorEPFvE@PLT
	movq	(%rsp), %rax
	movq	%rax, 208(%rsp)
	movq	8(%rsp), %rax
	movq	%rax, 216(%rsp)
	movq	16(%rsp), %rax
	movq	%rax, 224(%rsp)
	movq	32(%rsp), %r14                  # 8-byte Reload
	movq	%r14, %rdi
	callq	_ZN6Thread3runEv@PLT
	leaq	.Lstr(%rip), %rdi
	callq	puts@PLT
	leaq	40(%rsp), %rdi
	callq	_ZN6Thread4joinEv@PLT
	movq	%rbx, %rdi
	callq	_ZN6Thread4joinEv@PLT
	movq	%rbp, %rdi
	callq	_ZN6Thread4joinEv@PLT
	movq	%r13, %rdi
	callq	_ZN6Thread4joinEv@PLT
	movq	%r12, %rdi
	callq	_ZN6Thread4joinEv@PLT
	movq	24(%rsp), %rdi                  # 8-byte Reload
	callq	_ZN6Thread4joinEv@PLT
	movq	%r15, %rdi
	callq	_ZN6Thread4joinEv@PLT
	movq	%r14, %rdi
	callq	_ZN6Thread4joinEv@PLT
	leaq	.Lprintf.str.2(%rip), %rdi
	movb	$0, %al
	callq	printf@PLT
	xorl	%eax, %eax
	addq	$232, %rsp
	.cfi_def_cfa_offset 56
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%r12
	.cfi_def_cfa_offset 40
	popq	%r13
	.cfi_def_cfa_offset 32
	popq	%r14
	.cfi_def_cfa_offset 24
	popq	%r15
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc
                                        # -- End function
	.p2align	4                               # -- Begin function _Z15lambda.L12C29.0v
	.type	.L_Z15lambda.L12C29.0v,@function
.L_Z15lambda.L12C29.0v:                 # @_Z15lambda.L12C29.0v
# %bb.0:
	pushq	%rax
	movl	$30, %edi
	callq	.L_Z3fibi
	leaq	.Lprintf.str.0(%rip), %rdi
	movl	%eax, %esi
	xorl	%eax, %eax
	popq	%rcx
	jmp	printf@PLT                      # TAILCALL
.Lfunc_end2:
	.size	.L_Z15lambda.L12C29.0v, .Lfunc_end2-.L_Z15lambda.L12C29.0v
                                        # -- End function
	.type	.Lprintf.str.0,@object          # @printf.str.0
	.section	.rodata.str1.4,"aMS",@progbits,1
	.p2align	2, 0x0
.Lprintf.str.0:
	.asciz	"Thread returned with result: %d\n"
	.size	.Lprintf.str.0, 33

	.type	.Lprintf.str.2,@object          # @printf.str.2
	.p2align	2, 0x0
.Lprintf.str.2:
	.asciz	"Program finished"
	.size	.Lprintf.str.2, 17

	.type	.Lstr,@object                   # @str
	.section	.rodata.str1.1,"aMS",@progbits,1
.Lstr:
	.asciz	"Started all threads. Waiting for results ..."
	.size	.Lstr, 45

	.ident	"spice version dev (https://github.com/spicelang/spice)"
	.section	".note.GNU-stack","",@progbits
