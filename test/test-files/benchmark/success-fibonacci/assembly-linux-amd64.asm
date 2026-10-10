	.att_syntax
	.file	"source.spice"
	.text
	.prefalign	4, .Lfunc_end0, nop     # -- Begin function _Z4fiboi
	.type	_Z4fiboi,@function
_Z4fiboi:                               # @_Z4fiboi
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
	movl	%edi, %r14d
	xorl	%ebx, %ebx
	cmpl	$2, %edi
	jge	.LBB0_3
# %bb.1:
	movl	%r14d, %ecx
	jmp	.LBB0_2
.LBB0_3:                                # %if.exit.L2.preheader
	xorl	%ebx, %ebx
	.p2align	4
.LBB0_4:                                # %if.exit.L2
                                        # =>This Inner Loop Header: Depth=1
	leal	-1(%r14), %edi
	callq	_Z4fiboi
	leal	-2(%r14), %ecx
	addl	%eax, %ebx
	cmpl	$4, %r14d
	movl	%ecx, %r14d
	jae	.LBB0_4
.LBB0_2:                                # %common.ret
	addl	%ecx, %ebx
	movl	%ebx, %eax
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	_Z4fiboi, .Lfunc_end0-_Z4fiboi
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
	movl	$30, %edi
	callq	_Z4fiboi
	leaq	.Lprintf.str.0(%rip), %rdi
	movl	%eax, %esi
	xorl	%eax, %eax
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
	.asciz	"Result: %d"
	.size	.Lprintf.str.0, 11

	.ident	"spice version dev (https://github.com/spicelang/spice)"
	.section	".note.GNU-stack","",@progbits
