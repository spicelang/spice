	.text
	.file	"source.spice"
	.globl	main                            # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:                                # %while.body.L5
	pushq	%rax
	.cfi_def_cfa_offset 16
	movl	$72, %edi
	callq	putchar@PLT
	movl	$101, %edi
	callq	putchar@PLT
	movl	$108, %edi
	callq	putchar@PLT
	movl	$108, %edi
	callq	putchar@PLT
	movl	$111, %edi
	callq	putchar@PLT
	movl	$32, %edi
	callq	putchar@PLT
	movl	$87, %edi
	callq	putchar@PLT
	movl	$111, %edi
	callq	putchar@PLT
	movl	$114, %edi
	callq	putchar@PLT
	movl	$108, %edi
	callq	putchar@PLT
	movl	$100, %edi
	callq	putchar@PLT
	movl	$32, %edi
	callq	putchar@PLT
	xorl	%eax, %eax
	popq	%rcx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        # -- End function
	.ident	"spice version dev (https://github.com/spicelang/spice)"
	.section	".note.GNU-stack","",@progbits
