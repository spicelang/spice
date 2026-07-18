	.att_syntax
	.file	"source.spice"
	.text
	.prefalign	4, .Lfunc_end0, nop     # -- Begin function _Z3fibi
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
	subq	$456, %rsp                      # imm = 0x1C8
	.cfi_def_cfa_offset 512
	.cfi_offset %rbx, -56
	.cfi_offset %r12, -48
	.cfi_offset %r13, -40
	.cfi_offset %r14, -32
	.cfi_offset %r15, -24
	.cfi_offset %rbp, -16
	leaq	72(%rsp), %rbx
	addq	$48, %rbx
	leaq	72(%rsp), %rbp
	addq	$96, %rbp
	leaq	72(%rsp), %r13
	addq	$144, %r13
	leaq	72(%rsp), %r12
	addq	$192, %r12
	leaq	72(%rsp), %rax
	addq	$240, %rax
	movq	%rax, %r15
	leaq	72(%rsp), %rax
	addq	$288, %rax                      # imm = 0x120
	movq	%rax, %r14
	leaq	72(%rsp), %rax
	addq	$336, %rax                      # imm = 0x150
	movq	%rax, 64(%rsp)                  # 8-byte Spill
	leaq	.L_Z15lambda.L12C29.0v(%rip), %rsi
	leaq	8(%rsp), %rdi
	xorl	%edx, %edx
	xorl	%ecx, %ecx
	callq	_ZN6Thread4ctorEPFvE@PLT
	leaq	72(%rsp), %rdi
	addq	$48, %rdi
	xorl	%esi, %esi
	movl	$336, %edx                      # imm = 0x150
	callq	memset@PLT
	leaq	72(%rsp), %rdi
	leaq	8(%rsp), %rsi
	movl	$48, %edx
	callq	memcpy@PLT
	leaq	72(%rsp), %rdi
	callq	_ZN6Thread3runEv@PLT
	leaq	.L_Z15lambda.L12C29.0v(%rip), %rsi
	leaq	8(%rsp), %rdi
	xorl	%edx, %edx
	xorl	%ecx, %ecx
	callq	_ZN6Thread4ctorEPFvE@PLT
	leaq	8(%rsp), %rsi
	movq	%rbx, %rdi
	movl	$48, %edx
	callq	memcpy@PLT
	movq	%rbx, %rdi
	callq	_ZN6Thread3runEv@PLT
	leaq	.L_Z15lambda.L12C29.0v(%rip), %rsi
	leaq	8(%rsp), %rdi
	xorl	%edx, %edx
	xorl	%ecx, %ecx
	callq	_ZN6Thread4ctorEPFvE@PLT
	leaq	8(%rsp), %rsi
	movq	%rbp, %rdi
	movl	$48, %edx
	callq	memcpy@PLT
	movq	%rbp, %rdi
	callq	_ZN6Thread3runEv@PLT
	leaq	.L_Z15lambda.L12C29.0v(%rip), %rsi
	leaq	8(%rsp), %rdi
	xorl	%edx, %edx
	xorl	%ecx, %ecx
	callq	_ZN6Thread4ctorEPFvE@PLT
	leaq	8(%rsp), %rsi
	movq	%r13, %rdi
	movl	$48, %edx
	callq	memcpy@PLT
	movq	%r13, %rdi
	callq	_ZN6Thread3runEv@PLT
	leaq	.L_Z15lambda.L12C29.0v(%rip), %rsi
	leaq	8(%rsp), %rdi
	xorl	%edx, %edx
	xorl	%ecx, %ecx
	callq	_ZN6Thread4ctorEPFvE@PLT
	leaq	8(%rsp), %rsi
	movq	%r12, %rdi
	movl	$48, %edx
	callq	memcpy@PLT
	movq	%r12, %rdi
	callq	_ZN6Thread3runEv@PLT
	leaq	.L_Z15lambda.L12C29.0v(%rip), %rsi
	leaq	8(%rsp), %rdi
	xorl	%edx, %edx
	xorl	%ecx, %ecx
	callq	_ZN6Thread4ctorEPFvE@PLT
	leaq	8(%rsp), %rsi
	movq	%r15, 56(%rsp)                  # 8-byte Spill
	movq	%r15, %rdi
	movl	$48, %edx
	callq	memcpy@PLT
	movq	%r15, %rdi
	callq	_ZN6Thread3runEv@PLT
	leaq	.L_Z15lambda.L12C29.0v(%rip), %rsi
	leaq	8(%rsp), %rdi
	xorl	%edx, %edx
	xorl	%ecx, %ecx
	callq	_ZN6Thread4ctorEPFvE@PLT
	leaq	8(%rsp), %rsi
	movq	%r14, %r15
	movq	%r15, %rdi
	movl	$48, %edx
	callq	memcpy@PLT
	movq	%r15, %rdi
	callq	_ZN6Thread3runEv@PLT
	leaq	.L_Z15lambda.L12C29.0v(%rip), %rsi
	leaq	8(%rsp), %rdi
	xorl	%edx, %edx
	xorl	%ecx, %ecx
	callq	_ZN6Thread4ctorEPFvE@PLT
	leaq	8(%rsp), %rsi
	movq	64(%rsp), %r14                  # 8-byte Reload
	movq	%r14, %rdi
	movl	$48, %edx
	callq	memcpy@PLT
	movq	%r14, %rdi
	callq	_ZN6Thread3runEv@PLT
	leaq	.Lstr(%rip), %rdi
	callq	puts@PLT
	leaq	72(%rsp), %rdi
	callq	_ZN6Thread4joinEv@PLT
	movq	%rbx, %rdi
	callq	_ZN6Thread4joinEv@PLT
	movq	%rbp, %rdi
	callq	_ZN6Thread4joinEv@PLT
	movq	%r13, %rdi
	callq	_ZN6Thread4joinEv@PLT
	movq	%r12, %rdi
	callq	_ZN6Thread4joinEv@PLT
	movq	56(%rsp), %rdi                  # 8-byte Reload
	callq	_ZN6Thread4joinEv@PLT
	movq	%r15, %rdi
	callq	_ZN6Thread4joinEv@PLT
	movq	%r14, %rdi
	callq	_ZN6Thread4joinEv@PLT
	leaq	.Lprintf.str.2(%rip), %rdi
	movb	$0, %al
	callq	printf@PLT
	xorl	%eax, %eax
	addq	$456, %rsp                      # imm = 0x1C8
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
