	.att_syntax
	.file	"source.spice"
	.text
	.globl	main                            # -- Begin function main
	.prefalign	4, .Lfunc_end0, nop
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:                                # %_Z7facultyi.exit
	pushq	%rax
	.cfi_def_cfa_offset 16
	leaq	.Lprintf.str.0(%rip), %rdi
	movl	$10, %esi
	movl	$3628800, %edx                  # imm = 0x375F00
	xorl	%eax, %eax
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
	.section	.rodata.str1.4,"aMS",@progbits,1
	.p2align	2, 0x0
.Lprintf.str.0:
	.asciz	"Faculty of %d is: %d"
	.size	.Lprintf.str.0, 21

	.ident	"spice version dev (https://github.com/spicelang/spice)"
	.section	".note.GNU-stack","",@progbits
