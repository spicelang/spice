	.att_syntax
	.file	"source.spice"
	.text
	.globl	main                            # -- Begin function main
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:
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
	subq	$24, %rsp
	.cfi_def_cfa_offset 80
	.cfi_offset %rbx, -56
	.cfi_offset %r12, -48
	.cfi_offset %r13, -40
	.cfi_offset %r14, -32
	.cfi_offset %r15, -24
	.cfi_offset %rbp, -16
	pushq	$3
	.cfi_adjust_cfa_offset 8
	popq	%rbx
	.cfi_adjust_cfa_offset -8
	pushq	$1
	.cfi_adjust_cfa_offset 8
	popq	%r13
	.cfi_adjust_cfa_offset -8
	pushq	$20
	.cfi_adjust_cfa_offset 8
	popq	%r10
	.cfi_adjust_cfa_offset -8
	xorl	%r15d, %r15d
	movq	%r13, %rbp
	movq	%r13, %rdi
	movq	%rbx, %r8
	xorl	%r9d, %r9d
.LBB0_1:                                # %for.head.L19
                                        # =>This Inner Loop Header: Depth=1
	cmpl	$1, %r10d
	jb	.LBB0_8
# %bb.2:                                # %for.body.L19
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	%r15, %rax
	subq	%rbp, %rax
	leaq	(%rax,%r13,4), %rax
	movq	%rbx, %r14
	imulq	%rbp, %r14
	cmpq	%r14, %rax
	jge	.LBB0_6
# %bb.3:                                # %if.then.L20
                                        #   in Loop: Header=BB0_1 Depth=1
	movl	%r10d, 4(%rsp)                  # 4-byte Spill
	movq	%r8, 8(%rsp)                    # 8-byte Spill
	movq	%rdi, 16(%rsp)                  # 8-byte Spill
	leaq	.Lprintf.str.0(%rip), %rdi
	movq	%rbx, %rsi
	xorl	%eax, %eax
	movl	%r9d, %r12d
	callq	printf@PLT
	movl	%r12d, %r9d
	testl	%r12d, %r12d
	jne	.LBB0_5
# %bb.4:                                # %if.then.L22
                                        #   in Loop: Header=BB0_1 Depth=1
	pushq	$46
	.cfi_adjust_cfa_offset 8
	popq	%rdi
	.cfi_adjust_cfa_offset -8
	callq	putchar@PLT
	movl	%r12d, %r9d
.LBB0_5:                                # %if.exit.L22
                                        #   in Loop: Header=BB0_1 Depth=1
	incl	%r9d
	imulq	$10, %r13, %rcx
	movq	%r15, %rax
	subq	%r14, %rax
	imulq	$10, %rax, %rsi
	leaq	(,%r13,2), %rax
	addq	%r13, %rax
	addq	%r15, %rax
	imulq	$10, %rax, %rax
	cqto
	idivq	%rbp
	imulq	$-10, %rbx, %rbx
	addq	%rax, %rbx
	movq	16(%rsp), %rdi                  # 8-byte Reload
	movq	8(%rsp), %r8                    # 8-byte Reload
	movl	4(%rsp), %r10d                  # 4-byte Reload
	jmp	.LBB0_7
.LBB0_6:                                # %if.else.L20
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	%r13, %rcx
	imulq	%rdi, %rcx
	leaq	(%r15,%r13,2), %rsi
	imulq	%r8, %rsi
	imulq	%r8, %rbp
	imulq	$7, %rdi, %rax
	incq	%rdi
	addq	$2, %rax
	imulq	%r13, %rax
	imulq	%r8, %r15
	addq	%rax, %r15
	movq	%r15, %rax
	cqto
	idivq	%rbp
	movq	%rax, %rbx
	addq	$2, %r8
.LBB0_7:                                # %for.tail.L19
                                        #   in Loop: Header=BB0_1 Depth=1
	decl	%r10d
	movq	%rsi, %r15
	movq	%rcx, %r13
	jmp	.LBB0_1
.LBB0_8:                                # %for.exit.L19
	xorl	%eax, %eax
	addq	$24, %rsp
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
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        # -- End function
	.type	.Lprintf.str.0,@object          # @printf.str.0
	.section	.rodata.str1.4,"aMS",@progbits,1
	.p2align	2, 0x0
.Lprintf.str.0:
	.asciz	"%d"
	.size	.Lprintf.str.0, 3

	.ident	"spice version dev (https://github.com/spicelang/spice)"
	.section	".note.GNU-stack","",@progbits
