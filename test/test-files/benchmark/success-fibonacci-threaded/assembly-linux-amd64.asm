	.att_syntax
	.file	"source.spice"
	.text
	.prefalign	4, .Lfunc_end0, nop     # -- Begin function _Z3fibi
	.type	.L_Z3fibi,@function
.L_Z3fibi:                              # @_Z3fibi
	.cfi_startproc
# %bb.0:
	pushq	%r14
	.cfi_def_cfa_offset 16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	pushq	%rax
	.cfi_def_cfa_offset 32
	.cfi_offset %rbx, -24
	.cfi_offset %r14, -16
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
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	.L_Z3fibi, .Lfunc_end0-.L_Z3fibi
	.cfi_endproc
                                        # -- End function
	.globl	main                            # -- Begin function main
	.prefalign	4, .Lfunc_end1, nop
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
	subq	$440, %rsp                      # imm = 0x1B8
	.cfi_def_cfa_offset 496
	.cfi_offset %rbx, -56
	.cfi_offset %r12, -48
	.cfi_offset %r13, -40
	.cfi_offset %r14, -32
	.cfi_offset %r15, -24
	.cfi_offset %rbp, -16
	leaq	96(%rsp), %rbp
	leaq	144(%rsp), %rbx
	leaq	192(%rsp), %r12
	leaq	240(%rsp), %r13
	leaq	.L_Z15lambda.L12C29.0v(%rip), %r14
	movq	%rsp, %r15
	movq	%r15, %rdi
	movq	%r14, %rsi
	xorl	%edx, %edx
	xorl	%ecx, %ecx
	callq	_ZN6Thread4ctorEPFvE@PLT
	movl	$336, %edx                      # imm = 0x150
	movq	%rbp, %rdi
	xorl	%esi, %esi
	callq	memset@PLT
	movups	(%rsp), %xmm0
	movups	16(%rsp), %xmm1
	movups	32(%rsp), %xmm2
	movaps	%xmm0, 48(%rsp)
	movaps	%xmm1, 64(%rsp)
	movaps	%xmm2, 80(%rsp)
	leaq	48(%rsp), %rdi
	callq	_ZN6Thread3runEv@PLT
	movq	%r15, %rdi
	movq	%r14, %rsi
	xorl	%edx, %edx
	xorl	%ecx, %ecx
	callq	_ZN6Thread4ctorEPFvE@PLT
	movups	(%rsp), %xmm0
	movups	16(%rsp), %xmm1
	movups	32(%rsp), %xmm2
	movaps	%xmm2, 128(%rsp)
	movaps	%xmm1, 112(%rsp)
	movaps	%xmm0, 96(%rsp)
	movq	%rbp, %rdi
	callq	_ZN6Thread3runEv@PLT
	movq	%r15, %rdi
	movq	%r14, %rsi
	xorl	%edx, %edx
	xorl	%ecx, %ecx
	callq	_ZN6Thread4ctorEPFvE@PLT
	movups	(%rsp), %xmm0
	movups	16(%rsp), %xmm1
	movups	32(%rsp), %xmm2
	movaps	%xmm2, 176(%rsp)
	movaps	%xmm1, 160(%rsp)
	movaps	%xmm0, 144(%rsp)
	movq	%rbx, %rdi
	callq	_ZN6Thread3runEv@PLT
	movq	%r15, %rdi
	movq	%r14, %rsi
	xorl	%edx, %edx
	xorl	%ecx, %ecx
	callq	_ZN6Thread4ctorEPFvE@PLT
	movups	(%rsp), %xmm0
	movups	16(%rsp), %xmm1
	movups	32(%rsp), %xmm2
	movaps	%xmm2, 224(%rsp)
	movaps	%xmm1, 208(%rsp)
	movaps	%xmm0, 192(%rsp)
	movq	%r12, %rdi
	callq	_ZN6Thread3runEv@PLT
	movq	%r15, %rdi
	movq	%r14, %rsi
	xorl	%edx, %edx
	xorl	%ecx, %ecx
	callq	_ZN6Thread4ctorEPFvE@PLT
	movups	(%rsp), %xmm0
	movups	16(%rsp), %xmm1
	movups	32(%rsp), %xmm2
	movaps	%xmm2, 272(%rsp)
	movaps	%xmm1, 256(%rsp)
	movaps	%xmm0, 240(%rsp)
	movq	%r13, %rdi
	callq	_ZN6Thread3runEv@PLT
	movq	%r15, %rdi
	movq	%r14, %rsi
	xorl	%edx, %edx
	xorl	%ecx, %ecx
	callq	_ZN6Thread4ctorEPFvE@PLT
	movups	(%rsp), %xmm0
	movups	16(%rsp), %xmm1
	movups	32(%rsp), %xmm2
	movaps	%xmm2, 320(%rsp)
	movaps	%xmm1, 304(%rsp)
	movaps	%xmm0, 288(%rsp)
	leaq	288(%rsp), %rdi
	callq	_ZN6Thread3runEv@PLT
	movq	%r15, %rdi
	movq	%r14, %rsi
	xorl	%edx, %edx
	xorl	%ecx, %ecx
	callq	_ZN6Thread4ctorEPFvE@PLT
	movups	(%rsp), %xmm0
	movups	16(%rsp), %xmm1
	movups	32(%rsp), %xmm2
	movaps	%xmm2, 368(%rsp)
	movaps	%xmm1, 352(%rsp)
	movaps	%xmm0, 336(%rsp)
	leaq	336(%rsp), %rdi
	callq	_ZN6Thread3runEv@PLT
	movq	%r15, %rdi
	movq	%r14, %rsi
	xorl	%edx, %edx
	xorl	%ecx, %ecx
	callq	_ZN6Thread4ctorEPFvE@PLT
	movups	(%rsp), %xmm0
	movups	16(%rsp), %xmm1
	movups	32(%rsp), %xmm2
	movaps	%xmm2, 416(%rsp)
	movaps	%xmm1, 400(%rsp)
	movaps	%xmm0, 384(%rsp)
	leaq	384(%rsp), %r14
	movq	%r14, %rdi
	callq	_ZN6Thread3runEv@PLT
	leaq	.Lstr(%rip), %rdi
	callq	puts@PLT
	leaq	48(%rsp), %rdi
	callq	_ZN6Thread4joinEv@PLT
	movq	%rbp, %rdi
	callq	_ZN6Thread4joinEv@PLT
	movq	%rbx, %rdi
	callq	_ZN6Thread4joinEv@PLT
	movq	%r12, %rdi
	callq	_ZN6Thread4joinEv@PLT
	movq	%r13, %rdi
	callq	_ZN6Thread4joinEv@PLT
	leaq	288(%rsp), %rdi
	callq	_ZN6Thread4joinEv@PLT
	leaq	336(%rsp), %rdi
	callq	_ZN6Thread4joinEv@PLT
	movq	%r14, %rdi
	callq	_ZN6Thread4joinEv@PLT
	leaq	.Lprintf.str.2(%rip), %rdi
	xorl	%eax, %eax
	callq	printf@PLT
	xorl	%eax, %eax
	addq	$440, %rsp                      # imm = 0x1B8
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
	.prefalign	4, .Lfunc_end2, nop     # -- Begin function _Z15lambda.L12C29.0v
	.type	.L_Z15lambda.L12C29.0v,@function
.L_Z15lambda.L12C29.0v:                 # @_Z15lambda.L12C29.0v
	.cfi_startproc
# %bb.0:
	pushq	%rax
	.cfi_def_cfa_offset 16
	movl	$30, %edi
	callq	.L_Z3fibi
	leaq	.Lprintf.str.0(%rip), %rdi
	movl	%eax, %esi
	xorl	%eax, %eax
	popq	%rcx
	.cfi_def_cfa_offset 8
	jmp	printf@PLT                      # TAILCALL
.Lfunc_end2:
	.size	.L_Z15lambda.L12C29.0v, .Lfunc_end2-.L_Z15lambda.L12C29.0v
	.cfi_endproc
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
