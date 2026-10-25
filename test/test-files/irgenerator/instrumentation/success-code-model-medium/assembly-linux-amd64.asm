	.att_syntax
	.file	"source.spice"
	.text
	.globl	main                            # -- Begin function main
	.prefalign	4, .Lfunc_end0, nop
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:
	pushq	%rax
	.cfi_def_cfa_offset 16
	leaq	_GLOBAL_OFFSET_TABLE_(%rip), %rdi
	movabsq	$COUNTER@GOTOFF, %rax
	movl	(%rdi,%rax), %ecx
	incl	%ecx
	movl	%ecx, (%rdi,%rax)
	movl	(%rdi,%rax), %esi
	movabsq	$.Lprintf.str.0@GOTOFF, %rax
	addq	%rax, %rdi
	xorl	%eax, %eax
                                        # kill: def $al killed $al killed $eax
	callq	printf@PLT
	xorl	%eax, %eax
	popq	%rcx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        # -- End function
	.type	COUNTER,@object                 # @COUNTER
	.section	.ldata,"awl",@progbits
	.p2align	2, 0x0
COUNTER:
	.long	1                               # 0x1
	.size	COUNTER, 4

	.type	.Lprintf.str.0,@object          # @printf.str.0
	.section	.lrodata.str1.4,"aMSl",@progbits,1
	.p2align	2, 0x0
.Lprintf.str.0:
	.asciz	"Counter: %d\n"
	.size	.Lprintf.str.0, 13

	.ident	"spice version dev (https://github.com/spicelang/spice)"
	.section	".note.GNU-stack","",@progbits
