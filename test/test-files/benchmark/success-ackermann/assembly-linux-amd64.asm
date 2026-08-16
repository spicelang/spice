	.att_syntax
	.file	"source.spice"
	.text
	.prefalign	4, .Lfunc_end0, nop     # -- Begin function _Z3ackii
	.type	_Z3ackii,@function
_Z3ackii:                               # @_Z3ackii
	.cfi_startproc
# %bb.0:
	movl	%esi, %eax
	testl	%edi, %edi
	je	.LBB0_6
# %bb.1:
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset %rbx, -16
	movl	%edi, %ebx
	jmp	.LBB0_2
	.p2align	4
.LBB0_3:                                #   in Loop: Header=BB0_2 Depth=1
	movl	$1, %eax
	decl	%ebx
	je	.LBB0_5
.LBB0_2:                                # %if.exit.L2
                                        # =>This Inner Loop Header: Depth=1
	testl	%eax, %eax
	je	.LBB0_3
# %bb.7:                                # %if.exit.L3
                                        #   in Loop: Header=BB0_2 Depth=1
	decl	%eax
	movl	%ebx, %edi
	movl	%eax, %esi
	callq	_Z3ackii
	decl	%ebx
	jne	.LBB0_2
.LBB0_5:
	popq	%rbx
	.cfi_def_cfa_offset 8
	.cfi_restore %rbx
.LBB0_6:                                # %if.then.L2
	incl	%eax
	retq
.Lfunc_end0:
	.size	_Z3ackii, .Lfunc_end0-_Z3ackii
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
	movl	$3, %edi
	movl	$10, %esi
	callq	_Z3ackii
	leaq	.Lprintf.str.0(%rip), %rdi
	movl	$3, %esi
	movl	$10, %edx
	movl	%eax, %ecx
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
	.asciz	"Ackermann of base m=%d and n=%d: %d"
	.size	.Lprintf.str.0, 36

	.ident	"spice version dev (https://github.com/spicelang/spice)"
	.section	".note.GNU-stack","",@progbits
