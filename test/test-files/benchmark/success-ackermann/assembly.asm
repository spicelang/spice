	.text
	.def	@feat.00;
	.scl	3;
	.type	0;
	.endef
	.globl	@feat.00
.set @feat.00, 0
	.file	"source.spice"
	.def	.L_Z3ackii;
	.scl	3;
	.type	32;
	.endef
	.p2align	4, 0x90                         # -- Begin function _Z3ackii
.L_Z3ackii:                             # @_Z3ackii
# %bb.0:
	pushq	%rsi
	subq	$32, %rsp
	movl	%edx, %eax
	testl	%ecx, %ecx
	je	.LBB0_5
# %bb.1:
	movl	%ecx, %esi
	jmp	.LBB0_2
	.p2align	4, 0x90
.LBB0_3:                                #   in Loop: Header=BB0_2 Depth=1
	movl	$1, %eax
	decl	%esi
	je	.LBB0_5
.LBB0_2:                                # %if.exit.L2
                                        # =>This Inner Loop Header: Depth=1
	testl	%eax, %eax
	je	.LBB0_3
# %bb.6:                                # %if.exit.L3
                                        #   in Loop: Header=BB0_2 Depth=1
	decl	%eax
	movl	%esi, %ecx
	movl	%eax, %edx
	callq	.L_Z3ackii
	decl	%esi
	jne	.LBB0_2
.LBB0_5:                                # %if.then.L2
	incl	%eax
	addq	$32, %rsp
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
	movl	$3, %ecx
	movl	$10, %edx
	callq	.L_Z3ackii
	leaq	.Lprintf.str.0(%rip), %rcx
	movl	$3, %edx
	movl	$10, %r8d
	movl	%eax, %r9d
	callq	printf
	xorl	%eax, %eax
	addq	$32, %rsp
	popq	%rbp
	retq
	.seh_endproc
                                        # -- End function
	.section	.rdata,"dr"
.Lprintf.str.0:                         # @printf.str.0
	.asciz	"Ackermann of base m=%d and n=%d: %d"

