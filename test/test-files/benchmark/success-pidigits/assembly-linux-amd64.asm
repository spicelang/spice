	.file	"source.spice"
	.text
	.globl	main                            # -- Begin function main
	.p2align	4
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
	xorl	%ebp, %ebp
	movl	$1, %r14d
	movl	$3, %eax
	xorl	%r13d, %r13d
	xorl	%r8d, %r8d
	movl	$3, %r9d
	movl	$1, %edx
	movl	$1, %r15d
	jmp	.LBB0_1
.LBB0_1:                                # %for.body.L19
                                        # =>This Inner Loop Header: Depth=1
	movq	%r14, %rcx
	shlq	$2, %rcx
	subq	%r15, %rcx
	addq	%r13, %rcx
	movq	%r15, %r12
	imulq	%rax, %r12
	cmpq	%r12, %rcx
	jge	.LBB0_5
# %bb.2:                                # %if.then.L20
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	%rdx, 8(%rsp)                   # 8-byte Spill
	movq	%r9, 16(%rsp)                   # 8-byte Spill
	leaq	.Lprintf.str.0(%rip), %rdi
	movq	%rax, (%rsp)                    # 8-byte Spill
	movq	%rax, %rsi
	movb	$0, %al
	movl	%r8d, %ebx
	callq	printf@PLT
	movl	%ebx, %r8d
	cmpl	$0, %r8d
	jne	.LBB0_4
# %bb.3:                                # %if.then.L22
                                        #   in Loop: Header=BB0_1 Depth=1
	movl	$46, %edi
	callq	putchar@PLT
	movl	%ebx, %r8d
.LBB0_4:                                # %if.exit.L22
                                        #   in Loop: Header=BB0_1 Depth=1
	addl	$1, %r8d
	imulq	$10, %r14, %rcx
	movq	%r13, %rax
	subq	%r12, %rax
	imulq	$10, %rax, %rsi
	imulq	$3, %r14, %rax
	addq	%rax, %r13
	imulq	$10, %r13, %rax
	cqto
	idivq	%r15
	imulq	$-10, (%rsp), %rdx              # 8-byte Folded Reload
	addq	%rdx, %rax
	movq	%rsi, %r13
	movq	%rcx, %r14
	movq	16(%rsp), %r9                   # 8-byte Reload
	movq	8(%rsp), %rdx                   # 8-byte Reload
	jmp	.LBB0_6
.LBB0_5:                                # %if.else.L20
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	%rdx, %rcx
	imulq	%r14, %rcx
	movq	%r14, %rax
	shlq	%rax
	movq	%r13, %rsi
	addq	%rax, %rsi
	imulq	%r9, %rsi
	imulq	%r9, %r15
	movq	%rdx, %rdi
	addq	$1, %rdi
	imulq	$7, %rdx, %rax
	addq	$2, %rax
	imulq	%r14, %rax
	imulq	%r9, %r13
	addq	%r13, %rax
	cqto
	idivq	%r15
	addq	$2, %r9
	movq	%rsi, %r13
	movq	%rdi, %rdx
	movq	%rcx, %r14
.LBB0_6:                                # %for.tail.L19
                                        #   in Loop: Header=BB0_1 Depth=1
	addl	$1, %ebp
	cmpl	$20, %ebp
	jne	.LBB0_1
# %bb.7:                                # %for.exit.L19
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
