	.file	"source.spice"
	.text
	.p2align	4                               # -- Begin function _Z7facultyi
	.type	.L_Z7facultyi,@function
.L_Z7facultyi:                          # @_Z7facultyi
# %bb.0:
                                        # kill: def $edi killed $edi def $rdi
	movl	$1, %eax
	cmpl	$2, %edi
	jb	.LBB0_7
# %bb.1:                                # %if.exit.L2.preheader
	leal	-1(%rdi), %edx
	leal	-2(%rdi), %esi
	movl	%edx, %ecx
	andl	$7, %ecx
	cmpl	$7, %esi
	jb	.LBB0_6
# %bb.2:                                # %if.exit.L2.preheader.new
	movl	%edx, %r8d
	andl	$-8, %r8d
	negl	%r8d
	xorl	%esi, %esi
	.p2align	4
.LBB0_3:                                # %if.exit.L2
                                        # =>This Inner Loop Header: Depth=1
	leal	(%rdi,%rsi), %r9d
	imull	%eax, %r9d
	leal	(%rdx,%rsi), %eax
	leal	(%rdx,%rsi), %r10d
	decl	%r10d
	imull	%eax, %r10d
	imull	%r9d, %r10d
	leal	(%rdx,%rsi), %eax
	addl	$-2, %eax
	leal	(%rdx,%rsi), %r9d
	addl	$-3, %r9d
	imull	%eax, %r9d
	leal	(%rdx,%rsi), %r11d
	addl	$-4, %r11d
	imull	%r9d, %r11d
	imull	%r10d, %r11d
	leal	(%rdx,%rsi), %r9d
	addl	$-5, %r9d
	leal	(%rdx,%rsi), %eax
	addl	$-6, %eax
	imull	%r9d, %eax
	imull	%r11d, %eax
	addl	$-8, %esi
	cmpl	%esi, %r8d
	jne	.LBB0_3
# %bb.4:                                # %common.ret.loopexit.unr-lcssa
	testl	%ecx, %ecx
	je	.LBB0_7
# %bb.5:
	addl	%esi, %edi
	.p2align	4
.LBB0_6:                                # %if.exit.L2.epil
                                        # =>This Inner Loop Header: Depth=1
	imull	%edi, %eax
	decl	%edi
	decl	%ecx
	jne	.LBB0_6
.LBB0_7:                                # %common.ret
	retq
.Lfunc_end0:
	.size	.L_Z7facultyi, .Lfunc_end0-.L_Z7facultyi
                                        # -- End function
	.globl	main                            # -- Begin function main
	.p2align	4
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:
	pushq	%rax
	.cfi_def_cfa_offset 16
	movl	$10, %edi
	callq	.L_Z7facultyi
	leaq	.Lprintf.str.0(%rip), %rdi
	movl	$10, %esi
	movl	%eax, %edx
	movb	$0, %al
	callq	printf@PLT
	xorl	%eax, %eax
	popq	%rcx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc
                                        # -- End function
	.type	.Lprintf.str.0,@object          # @printf.str.0
	.section	.rodata.str1.4,"aMS",@progbits,1
	.p2align	2, 0x0
.Lprintf.str.0:
	.asciz	"Faculty of %d is: %d"
	.size	.Lprintf.str.0, 21

	.ident	"spice version dev (https://github.com/spicelang/spice)"
	.section	".note.GNU-stack","",@progbits
