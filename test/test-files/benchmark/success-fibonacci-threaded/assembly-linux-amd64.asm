	.att_syntax
	.file	"source.spice"
	.text
	.prefalign	4, .Lfunc_end0, nop     # -- Begin function _Z3fibi
	.type	_Z3fibi,@function
_Z3fibi:                                # @_Z3fibi
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
	callq	_Z3fibi
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
	.size	_Z3fibi, .Lfunc_end0-_Z3fibi
	.cfi_endproc
                                        # -- End function
	.globl	main                            # -- Begin function main
	.prefalign	4, .Lfunc_end1, nop
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:                                # %for.body.L13
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
	subq	$488, %rsp                      # imm = 0x1E8
	.cfi_def_cfa_offset 544
	.cfi_offset %rbx, -56
	.cfi_offset %r12, -48
	.cfi_offset %r13, -40
	.cfi_offset %r14, -32
	.cfi_offset %r15, -24
	.cfi_offset %rbp, -16
	leaq	144(%rsp), %rbx
	leaq	.Lstr(%rip), %rdi
	callq	puts@PLT
	leaq	_Z15lambda.L14C29.0v(%rip), %rsi
	movq	%rsp, %rdi
	xorl	%edx, %edx
	xorl	%ecx, %ecx
	callq	_ZN6Thread4ctorEPFvE@PLT
	movl	$336, %edx                      # imm = 0x150
	movq	%rbx, %rdi
	xorl	%esi, %esi
	callq	memset@PLT
	movups	(%rsp), %xmm0
	movups	16(%rsp), %xmm1
	movups	32(%rsp), %xmm2
	movaps	%xmm0, 96(%rsp)
	movaps	%xmm1, 112(%rsp)
	movaps	%xmm2, 128(%rsp)
	leaq	72(%rsp), %rbx
	leaq	96(%rsp), %rsi
	movq	%rbx, %rdi
	callq	_ZN6Thread3runEv@PLT
	movq	%rbx, %rdi
	callq	_ZN6ResultIbE4isOkEv@PLT
	testb	%al, %al
	je	.LBB1_17
# %bb.1:                                # %for.head.L13
	leaq	_Z15lambda.L14C29.0v(%rip), %rsi
	movq	%rsp, %rdi
	xorl	%edx, %edx
	xorl	%ecx, %ecx
	callq	_ZN6Thread4ctorEPFvE@PLT
	movups	(%rsp), %xmm0
	movups	16(%rsp), %xmm1
	movups	32(%rsp), %xmm2
	leaq	144(%rsp), %rsi
	movups	%xmm2, 32(%rsi)
	movups	%xmm1, 16(%rsi)
	movups	%xmm0, (%rsi)
	leaq	72(%rsp), %rbx
	movq	%rbx, %rdi
	callq	_ZN6Thread3runEv@PLT
	movq	%rbx, %rdi
	callq	_ZN6ResultIbE4isOkEv@PLT
	testb	%al, %al
	je	.LBB1_17
# %bb.2:                                # %for.head.L13.1
	leaq	192(%rsp), %r14
	leaq	_Z15lambda.L14C29.0v(%rip), %rsi
	movq	%rsp, %rdi
	xorl	%edx, %edx
	xorl	%ecx, %ecx
	callq	_ZN6Thread4ctorEPFvE@PLT
	movups	(%rsp), %xmm0
	movups	16(%rsp), %xmm1
	movups	32(%rsp), %xmm2
	movups	%xmm2, 32(%r14)
	movups	%xmm1, 16(%r14)
	movups	%xmm0, (%r14)
	leaq	72(%rsp), %rbx
	movq	%rbx, %rdi
	movq	%r14, %rsi
	callq	_ZN6Thread3runEv@PLT
	movq	%rbx, %rdi
	callq	_ZN6ResultIbE4isOkEv@PLT
	testb	%al, %al
	je	.LBB1_17
# %bb.3:                                # %for.head.L13.2
	leaq	240(%rsp), %r15
	leaq	_Z15lambda.L14C29.0v(%rip), %rsi
	movq	%rsp, %rdi
	xorl	%edx, %edx
	xorl	%ecx, %ecx
	callq	_ZN6Thread4ctorEPFvE@PLT
	movups	(%rsp), %xmm0
	movups	16(%rsp), %xmm1
	movups	32(%rsp), %xmm2
	movups	%xmm2, 32(%r15)
	movups	%xmm1, 16(%r15)
	movups	%xmm0, (%r15)
	leaq	72(%rsp), %rbx
	movq	%rbx, %rdi
	movq	%r15, %rsi
	callq	_ZN6Thread3runEv@PLT
	movq	%rbx, %rdi
	callq	_ZN6ResultIbE4isOkEv@PLT
	testb	%al, %al
	je	.LBB1_17
# %bb.4:                                # %for.head.L13.3
	leaq	288(%rsp), %r12
	leaq	_Z15lambda.L14C29.0v(%rip), %rsi
	movq	%rsp, %rdi
	xorl	%edx, %edx
	xorl	%ecx, %ecx
	callq	_ZN6Thread4ctorEPFvE@PLT
	movups	(%rsp), %xmm0
	movups	16(%rsp), %xmm1
	movups	32(%rsp), %xmm2
	movups	%xmm2, 32(%r12)
	movups	%xmm1, 16(%r12)
	movups	%xmm0, (%r12)
	leaq	72(%rsp), %rbx
	movq	%rbx, %rdi
	movq	%r12, %rsi
	callq	_ZN6Thread3runEv@PLT
	movq	%rbx, %rdi
	callq	_ZN6ResultIbE4isOkEv@PLT
	testb	%al, %al
	je	.LBB1_17
# %bb.5:                                # %for.head.L13.4
	leaq	336(%rsp), %r13
	leaq	_Z15lambda.L14C29.0v(%rip), %rsi
	movq	%rsp, %rdi
	xorl	%edx, %edx
	xorl	%ecx, %ecx
	callq	_ZN6Thread4ctorEPFvE@PLT
	movups	(%rsp), %xmm0
	movups	16(%rsp), %xmm1
	movups	32(%rsp), %xmm2
	movups	%xmm2, 32(%r13)
	movups	%xmm1, 16(%r13)
	movups	%xmm0, (%r13)
	leaq	72(%rsp), %rbx
	movq	%rbx, %rdi
	movq	%r13, %rsi
	callq	_ZN6Thread3runEv@PLT
	movq	%rbx, %rdi
	callq	_ZN6ResultIbE4isOkEv@PLT
	testb	%al, %al
	je	.LBB1_17
# %bb.6:                                # %for.head.L13.5
	leaq	384(%rsp), %rbp
	leaq	_Z15lambda.L14C29.0v(%rip), %rsi
	movq	%rsp, %rdi
	xorl	%edx, %edx
	xorl	%ecx, %ecx
	callq	_ZN6Thread4ctorEPFvE@PLT
	movups	(%rsp), %xmm0
	movups	16(%rsp), %xmm1
	movups	32(%rsp), %xmm2
	movups	%xmm2, 32(%rbp)
	movups	%xmm1, 16(%rbp)
	movups	%xmm0, (%rbp)
	leaq	72(%rsp), %rbx
	movq	%rbx, %rdi
	movq	%rbp, %rsi
	callq	_ZN6Thread3runEv@PLT
	movq	%rbx, %rdi
	callq	_ZN6ResultIbE4isOkEv@PLT
	testb	%al, %al
	je	.LBB1_17
# %bb.7:                                # %for.head.L13.6
	leaq	432(%rsp), %rbx
	leaq	_Z15lambda.L14C29.0v(%rip), %rsi
	movq	%rsp, %rdi
	xorl	%edx, %edx
	xorl	%ecx, %ecx
	callq	_ZN6Thread4ctorEPFvE@PLT
	movups	(%rsp), %xmm0
	movups	16(%rsp), %xmm1
	movups	32(%rsp), %xmm2
	movups	%xmm2, 32(%rbx)
	movups	%xmm1, 16(%rbx)
	movups	%xmm0, (%rbx)
	leaq	72(%rsp), %r14
	movq	%r14, %rdi
	movq	%rbx, %rsi
	callq	_ZN6Thread3runEv@PLT
	movq	%r14, %rdi
	callq	_ZN6ResultIbE4isOkEv@PLT
	testb	%al, %al
	je	.LBB1_17
# %bb.8:                                # %for.head.L13.7
	leaq	48(%rsp), %r14
	leaq	96(%rsp), %rsi
	movq	%r14, %rdi
	callq	_ZN6Thread4joinEv@PLT
	movq	%r14, %rdi
	callq	_ZN6ResultIbE4isOkEv@PLT
	testb	%al, %al
	je	.LBB1_19
# %bb.9:                                # %for.head.L22
	leaq	48(%rsp), %r14
	movq	%r14, %rdi
	leaq	144(%rsp), %rsi
	callq	_ZN6Thread4joinEv@PLT
	movq	%r14, %rdi
	callq	_ZN6ResultIbE4isOkEv@PLT
	testb	%al, %al
	je	.LBB1_19
# %bb.10:                               # %for.head.L22.1
	leaq	48(%rsp), %r14
	movq	%r14, %rdi
	leaq	192(%rsp), %rsi
	callq	_ZN6Thread4joinEv@PLT
	movq	%r14, %rdi
	callq	_ZN6ResultIbE4isOkEv@PLT
	testb	%al, %al
	je	.LBB1_19
# %bb.11:                               # %for.head.L22.2
	leaq	48(%rsp), %r14
	movq	%r14, %rdi
	movq	%r15, %rsi
	callq	_ZN6Thread4joinEv@PLT
	movq	%r14, %rdi
	callq	_ZN6ResultIbE4isOkEv@PLT
	testb	%al, %al
	je	.LBB1_19
# %bb.12:                               # %for.head.L22.3
	leaq	48(%rsp), %r14
	movq	%r14, %rdi
	movq	%r12, %rsi
	callq	_ZN6Thread4joinEv@PLT
	movq	%r14, %rdi
	callq	_ZN6ResultIbE4isOkEv@PLT
	testb	%al, %al
	je	.LBB1_19
# %bb.13:                               # %for.head.L22.4
	leaq	48(%rsp), %r14
	movq	%r14, %rdi
	movq	%r13, %rsi
	callq	_ZN6Thread4joinEv@PLT
	movq	%r14, %rdi
	callq	_ZN6ResultIbE4isOkEv@PLT
	testb	%al, %al
	je	.LBB1_19
# %bb.14:                               # %for.head.L22.5
	leaq	48(%rsp), %r14
	movq	%r14, %rdi
	movq	%rbp, %rsi
	callq	_ZN6Thread4joinEv@PLT
	movq	%r14, %rdi
	callq	_ZN6ResultIbE4isOkEv@PLT
	testb	%al, %al
	je	.LBB1_19
# %bb.15:                               # %for.head.L22.6
	leaq	48(%rsp), %r14
	movq	%r14, %rdi
	movq	%rbx, %rsi
	callq	_ZN6Thread4joinEv@PLT
	movq	%r14, %rdi
	callq	_ZN6ResultIbE4isOkEv@PLT
	testb	%al, %al
	je	.LBB1_19
# %bb.16:                               # %for.head.L22.7
	leaq	.Lprintf.str.2(%rip), %rdi
	xorl	%eax, %eax
	callq	printf@PLT
	xorl	%eax, %eax
	addq	$488, %rsp                      # imm = 0x1E8
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
.LBB1_17:                               # %assert.then.L20
	.cfi_def_cfa_offset 544
	movq	stderr@GOTPCREL(%rip), %rax
	movq	(%rax), %rcx
	leaq	.Lanon.string.0(%rip), %rdi
	movl	$65, %esi
	movl	$1, %edx
	callq	fwrite@PLT
	movl	$1, %edi
	callq	exit@PLT
.LBB1_19:                               # %assert.then.L25
	movq	stderr@GOTPCREL(%rip), %rax
	movq	(%rax), %rcx
	leaq	.Lanon.string.1(%rip), %rdi
	movl	$64, %esi
	movl	$1, %edx
	callq	fwrite@PLT
	movl	$1, %edi
	callq	exit@PLT
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc
                                        # -- End function
	.prefalign	4, .Lfunc_end2, nop     # -- Begin function _Z15lambda.L14C29.0v
	.type	_Z15lambda.L14C29.0v,@function
_Z15lambda.L14C29.0v:                   # @_Z15lambda.L14C29.0v
	.cfi_startproc
# %bb.0:
	pushq	%rax
	.cfi_def_cfa_offset 16
	movl	$30, %edi
	callq	_Z3fibi
	leaq	.Lprintf.str.1(%rip), %rdi
	movl	%eax, %esi
	xorl	%eax, %eax
	popq	%rcx
	.cfi_def_cfa_offset 8
	jmp	printf@PLT                      # TAILCALL
.Lfunc_end2:
	.size	_Z15lambda.L14C29.0v, .Lfunc_end2-_Z15lambda.L14C29.0v
	.cfi_endproc
                                        # -- End function
	.type	.Lprintf.str.1,@object          # @printf.str.1
	.section	.rodata.str1.4,"aMS",@progbits,1
	.p2align	2, 0x0
.Lprintf.str.1:
	.asciz	"Thread returned with result: %d\n"
	.size	.Lprintf.str.1, 33

	.type	.Lanon.string.0,@object         # @anon.string.0
	.p2align	2, 0x0
.Lanon.string.0:
	.asciz	"Assertion failed: Condition 'started.isOk()' evaluated to false.\n"
	.size	.Lanon.string.0, 66

	.type	.Lanon.string.1,@object         # @anon.string.1
	.p2align	2, 0x0
.Lanon.string.1:
	.asciz	"Assertion failed: Condition 'joined.isOk()' evaluated to false.\n"
	.size	.Lanon.string.1, 65

	.type	.Lprintf.str.2,@object          # @printf.str.2
	.p2align	2, 0x0
.Lprintf.str.2:
	.asciz	"Program finished"
	.size	.Lprintf.str.2, 17

	.type	.Lstr,@object                   # @str
	.section	.rodata.str1.1,"aMS",@progbits,1
.Lstr:
	.asciz	"Starting all threads ..."
	.size	.Lstr, 25

	.ident	"spice version dev (https://github.com/spicelang/spice)"
	.section	".note.GNU-stack","",@progbits
