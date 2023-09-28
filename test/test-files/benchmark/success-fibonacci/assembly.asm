	.text
	.def	@feat.00;
	.scl	3;
	.type	0;
	.endef
	.globl	@feat.00
.set @feat.00, 0
	.file	"source.spice"
	.def	.L_Z4fiboi;
	.scl	3;
	.type	32;
	.endef
	.p2align	4, 0x90                         # -- Begin function _Z4fiboi
.L_Z4fiboi:                             # @_Z4fiboi
# %bb.0:
	pushq	%rsi
	pushq	%rdi
	subq	$40, %rsp
	movl	%ecx, %edi
	xorl	%esi, %esi
	cmpl	$2, %ecx
	jge	.LBB0_3
# %bb.1:
	movl	%edi, %ecx
	jmp	.LBB0_2
.LBB0_3:                                # %if.exit.L2.preheader
	xorl	%esi, %esi
	.p2align	4, 0x90
.LBB0_4:                                # %if.exit.L2
                                        # =>This Inner Loop Header: Depth=1
	leal	-1(%rdi), %ecx
	callq	.L_Z4fiboi
	leal	-2(%rdi), %ecx
	addl	%eax, %esi
	cmpl	$4, %edi
	movl	%ecx, %edi
	jae	.LBB0_4
.LBB0_2:                                # %common.ret
	addl	%ecx, %esi
	movl	%esi, %eax
	addq	$40, %rsp
	popq	%rdi
	popq	%rsi
	retq
                                        # -- End function
	.def	main;
	.scl	2;
	.type	32;
	.endef
	.globl	main                            # -- Begin function main
	.p2align	4, 0x90
main:                                   # @main
.seh_proc main
# %bb.0:
	pushq	%rbp
	.seh_pushreg %rbp
	subq	$32, %rsp
	.seh_stackalloc 32
	leaq	32(%rsp), %rbp
	.seh_setframe %rbp, 32
	.seh_endprologue
	callq	__main
	movl	$30, %ecx
	callq	.L_Z4fiboi
	leaq	.Lprintf.str.0(%rip), %rcx
	movl	%eax, %edx
	callq	printf
	xorl	%eax, %eax
	addq	$32, %rsp
	popq	%rbp
	retq
	.seh_endproc
                                        # -- End function
	.section	.rdata,"dr"
.Lprintf.str.0:                         # @printf.str.0
	.asciz	"Result: %d"

