	.text
	.file	"source.spice"
	.p2align	4, 0x90                         # -- Begin function _Z4fiboi
	.type	.L_Z4fiboi,@function
.L_Z4fiboi:                             # @_Z4fiboi
# %bb.0:
	pushq	%r14
	pushq	%rbx
	pushq	%rax
	movl	%edi, %r14d
	xorl	%ebx, %ebx
	cmpl	$2, %edi
	jge	.LBB0_3
# %bb.1:
	movl	%r14d, %ecx
	jmp	.LBB0_2
.LBB0_3:                                # %if.exit.L2.preheader
	xorl	%ebx, %ebx
	.p2align	4, 0x90
.LBB0_4:                                # %if.exit.L2
                                        # =>This Inner Loop Header: Depth=1
	leal	-1(%r14), %edi
	callq	.L_Z4fiboi
	leal	-2(%r14), %ecx
	addl	%eax, %ebx
	cmpl	$4, %r14d
	movl	%ecx, %r14d
	jae	.LBB0_4
.LBB0_2:                                # %common.ret
	addl	%ecx, %ebx
	movl	%ebx, %eax
	addq	$8, %rsp
	popq	%rbx
	popq	%r14
	retq
.Lfunc_end0:
	.size	.L_Z4fiboi, .Lfunc_end0-.L_Z4fiboi
                                        # -- End function
	.globl	main                            # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:
	pushq	%rax
	.cfi_def_cfa_offset 16
	movl	$30, %edi
	callq	.L_Z4fiboi
	leaq	.Lprintf.str.0(%rip), %rdi
	movl	%eax, %esi
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
	.section	.rodata.str1.1,"aMS",@progbits,1
.Lprintf.str.0:
	.asciz	"Result: %d"
	.size	.Lprintf.str.0, 11

	.ident	"spice version dev (https://github.com/spicelang/spice)"
	.section	".note.GNU-stack","",@progbits
