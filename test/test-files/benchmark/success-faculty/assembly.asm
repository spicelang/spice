	.text
	.file	"source.spice"
	.section	.rodata.cst16,"aM",@progbits,16
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
	.type	.L_Z7facultyi,@function
.L_Z7facultyi:                          # @_Z7facultyi
# %bb.0:
                                        # kill: def $edi killed $edi def $rdi
	movl	$1, %eax
	cmpl	$2, %edi
	jl	.LBB0_6
# %bb.1:                                # %if.exit.L2.preheader
	cmpl	$8, %edi
	jbe	.LBB0_5
# %bb.2:                                # %vector.ph
	leal	-1(%rdi), %ecx
	movl	%ecx, %edx
	andl	$-8, %edx
	movd	%edi, %xmm0
	subl	%edx, %edi
	pshufd	$0, %xmm0, %xmm0                # xmm0 = xmm0[0,0,0,0]
	paddd	.LCPI0_0(%rip), %xmm0
	movl	%edx, %eax
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
	cmpl	%edx, %ecx
	je	.LBB0_6
	.p2align	4, 0x90
.LBB0_5:                                # %if.exit.L2
                                        # =>This Inner Loop Header: Depth=1
	imull	%edi, %eax
	leal	-1(%rdi), %ecx
	cmpl	$3, %edi
	movl	%ecx, %edi
	jae	.LBB0_5
.LBB0_6:                                # %common.ret
	retq
.Lfunc_end0:
	.size	.L_Z7facultyi, .Lfunc_end0-.L_Z7facultyi
                                        # -- End function
	.globl	main                            # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
.Lmain$local:
	.type	.Lmain$local,@function
	.cfi_startproc
# %bb.0:
	pushq	%rax
	.cfi_def_cfa_offset 16
	movl	$10, %edi
	callq	.L_Z7facultyi
	leaq	.Lprintf.str.0(%rip), %rdi
	movl	$10, %esi
	movl	%eax, %edx
	movb	$0, %al
	callq	printf@PLT
	xorl	%eax, %eax
	popq	%rcx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.size	.Lmain$local, .Lfunc_end1-main
	.cfi_endproc
                                        # -- End function
	.type	.Lprintf.str.0,@object          # @printf.str.0
	.section	.rodata.str1.1,"aMS",@progbits,1
.Lprintf.str.0:
	.asciz	"Faculty of %d is: %d"
	.size	.Lprintf.str.0, 21

	.section	".note.GNU-stack","",@progbits
