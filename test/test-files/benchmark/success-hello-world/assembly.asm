	.text
	.file	"source.spice"
	.globl	main                            # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:
	pushq	%rax
	.cfi_def_cfa_offset 16
	leaq	.Lprintf.str.0(%rip), %rdi
	movb	$0, %al
	callq	printf@PLT
	xorl	%eax, %eax
	popq	%rcx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        # -- End function
	.type	.Lprintf.str.0,@object          # @printf.str.0
	.section	.rodata.str1.1,"aMS",@progbits,1
.Lprintf.str.0:
	.asciz	"Hello World!"
	.size	.Lprintf.str.0, 13

	.ident	"spice version dev (https://github.com/spicelang/spice)"
	.section	".note.GNU-stack","",@progbits
