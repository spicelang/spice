	.att_syntax
	.file	"source.spice"
	.text
	.globl	_Z9incrementv                   # -- Begin function _Z9incrementv
	.prefalign	4, .Lfunc_end0, nop
	.type	_Z9incrementv,@function
_Z9incrementv:                          # @_Z9incrementv
	.cfi_startproc
# %bb.0:
	movl	COUNTER(%rip), %eax
	incl	%eax
	movl	%eax, COUNTER(%rip)
	retq
.Lfunc_end0:
	.size	_Z9incrementv, .Lfunc_end0-_Z9incrementv
	.cfi_endproc
                                        # -- End function
	.type	COUNTER,@object                 # @COUNTER
	.data
	.p2align	2, 0x0
COUNTER:
	.long	1                               # 0x1
	.size	COUNTER, 4

	.ident	"spice version dev (https://github.com/spicelang/spice)"
	.section	".note.GNU-stack","",@progbits
