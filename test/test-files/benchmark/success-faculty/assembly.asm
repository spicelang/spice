	.text
	.def	@feat.00;
	.scl	3;
	.type	0;
	.endef
	.globl	@feat.00
.set @feat.00, 0
	.file	"source.spice"
	.def	.L_Z7facultyi;
	.scl	3;
	.type	32;
	.endef
	.section	.rdata,"dr"
	.p2align	4, 0x0                          # -- Begin function _Z7facultyi
.LCPI0_0:
	.long	0                               # 0x0
	.long	4294967295                      # 0xffffffff
	.long	4294967294                      # 0xfffffffe
	.long	4294967293                      # 0xfffffffd
.LCPI0_1:
	.long	1                               # 0x1
	.long	1                               # 0x1
	.long	1                               # 0x1
	.long	1                               # 0x1
.LCPI0_2:
	.long	4294967292                      # 0xfffffffc
	.long	4294967292                      # 0xfffffffc
	.long	4294967292                      # 0xfffffffc
	.long	4294967292                      # 0xfffffffc
.LCPI0_3:
	.long	4294967288                      # 0xfffffff8
	.long	4294967288                      # 0xfffffff8
	.long	4294967288                      # 0xfffffff8
	.long	4294967288                      # 0xfffffff8
	.text
	.p2align	4, 0x90
.L_Z7facultyi:                          # @_Z7facultyi
# %bb.0:
	subq	$40, %rsp
	movdqa	%xmm7, 16(%rsp)                 # 16-byte Spill
	movdqa	%xmm6, (%rsp)                   # 16-byte Spill
                                        # kill: def $ecx killed $ecx def $rcx
	movl	$1, %eax
	cmpl	$2, %ecx
	jl	.LBB0_6
# %bb.1:                                # %if.exit.L2.preheader
	cmpl	$9, %ecx
	jb	.LBB0_5
# %bb.2:                                # %vector.ph
	leal	-1(%rcx), %edx
	movl	%edx, %r8d
	andl	$-8, %r8d
	movd	%ecx, %xmm0
	subl	%r8d, %ecx
	pshufd	$0, %xmm0, %xmm0                # xmm0 = xmm0[0,0,0,0]
	paddd	.LCPI0_0(%rip), %xmm0
	movl	%r8d, %eax
	negl	%eax
	movdqa	.LCPI0_1(%rip), %xmm1           # xmm1 = [1,1,1,1]
	movdqa	.LCPI0_2(%rip), %xmm2           # xmm2 = [4294967292,4294967292,4294967292,4294967292]
	movdqa	.LCPI0_3(%rip), %xmm4           # xmm4 = [4294967288,4294967288,4294967288,4294967288]
	movdqa	%xmm1, %xmm3
	.p2align	4, 0x90
.LBB0_3:                                # %vector.body
                                        # =>This Inner Loop Header: Depth=1
	movdqa	%xmm0, %xmm5
	paddd	%xmm2, %xmm5
	movdqa	%xmm0, %xmm6
	pmuludq	%xmm1, %xmm6
	pshufd	$245, %xmm1, %xmm1              # xmm1 = xmm1[1,1,3,3]
	pshufd	$232, %xmm6, %xmm6              # xmm6 = xmm6[0,2,2,3]
	pshufd	$245, %xmm0, %xmm7              # xmm7 = xmm0[1,1,3,3]
	pmuludq	%xmm1, %xmm7
	pshufd	$232, %xmm7, %xmm7              # xmm7 = xmm7[0,2,2,3]
	movdqa	%xmm6, %xmm1
	punpckldq	%xmm7, %xmm1            # xmm1 = xmm1[0],xmm7[0],xmm1[1],xmm7[1]
	pshufd	$245, %xmm5, %xmm6              # xmm6 = xmm5[1,1,3,3]
	pmuludq	%xmm3, %xmm5
	pshufd	$245, %xmm3, %xmm3              # xmm3 = xmm3[1,1,3,3]
	pshufd	$232, %xmm5, %xmm5              # xmm5 = xmm5[0,2,2,3]
	pmuludq	%xmm6, %xmm3
	pshufd	$232, %xmm3, %xmm6              # xmm6 = xmm3[0,2,2,3]
	movdqa	%xmm5, %xmm3
	punpckldq	%xmm6, %xmm3            # xmm3 = xmm3[0],xmm6[0],xmm3[1],xmm6[1]
	paddd	%xmm4, %xmm0
	addl	$8, %eax
	jne	.LBB0_3
# %bb.4:                                # %middle.block
	pshufd	$245, %xmm1, %xmm0              # xmm0 = xmm1[1,1,3,3]
	pshufd	$245, %xmm3, %xmm2              # xmm2 = xmm3[1,1,3,3]
	pmuludq	%xmm0, %xmm2
	pmuludq	%xmm1, %xmm3
	pshufd	$238, %xmm3, %xmm0              # xmm0 = xmm3[2,3,2,3]
	pmuludq	%xmm3, %xmm0
	pshufd	$170, %xmm2, %xmm1              # xmm1 = xmm2[2,2,2,2]
	pmuludq	%xmm2, %xmm1
	pmuludq	%xmm0, %xmm1
	movd	%xmm1, %eax
	cmpl	%r8d, %edx
	je	.LBB0_6
	.p2align	4, 0x90
.LBB0_5:                                # %if.exit.L2
                                        # =>This Inner Loop Header: Depth=1
	imull	%ecx, %eax
	leal	-1(%rcx), %edx
	cmpl	$3, %ecx
	movl	%edx, %ecx
	jae	.LBB0_5
.LBB0_6:                                # %common.ret
	movaps	(%rsp), %xmm6                   # 16-byte Reload
	movaps	16(%rsp), %xmm7                 # 16-byte Reload
	addq	$40, %rsp
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
	movl	$10, %ecx
	callq	.L_Z7facultyi
	leaq	.Lprintf.str.0(%rip), %rcx
	movl	$10, %edx
	movl	%eax, %r8d
	callq	printf
	xorl	%eax, %eax
	addq	$32, %rsp
	popq	%rbp
	retq
	.seh_endproc
                                        # -- End function
	.section	.rdata,"dr"
.Lprintf.str.0:                         # @printf.str.0
	.asciz	"Faculty of %d is: %d"

