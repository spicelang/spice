	.att_syntax
	.file	"source.spice"
	.text
	.prefalign	4, .Lfunc_end0, nop     # -- Begin function _Z5addUpii
	.type	_Z5addUpii,@function
_Z5addUpii:                             # @_Z5addUpii
	.cfi_startproc
# %bb.0:
	pushq	%rax
	.cfi_def_cfa_offset 16
	movl	%edi, 4(%rsp)
	movl	%esi, (%rsp)
	movl	(%rsp), %eax
	addl	4(%rsp), %eax
	popq	%rcx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	_Z5addUpii, .Lfunc_end0-_Z5addUpii
	.cfi_endproc
                                        # -- End function
	.globl	main                            # -- Begin function main
	.prefalign	4, .Lfunc_end1, nop
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:
	pushq	%rax
	.cfi_def_cfa_offset 16
	movl	$2, %edi
	movl	$3, %esi
	callq	_Z5addUpii
	movl	%eax, %esi
	leaq	.Lprintf.str.0(%rip), %rdi
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
	.asciz	"Sum: %d\n"
	.size	.Lprintf.str.0, 9

	.ident	"spice version dev (https://github.com/spicelang/spice)"
	.section	".note.GNU-stack","",@progbits
