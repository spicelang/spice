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
	.long	4294967291                      # 0xfffffffb
	.long	4294967290                      # 0xfffffffa
	.long	4294967289                      # 0xfffffff9
.LCPI0_3:
	.long	4294967288                      # 0xfffffff8
	.long	4294967287                      # 0xfffffff7
	.long	4294967286                      # 0xfffffff6
	.long	4294967285                      # 0xfffffff5
.LCPI0_4:
	.long	2147483648                      # 0x80000000
	.long	2147483648                      # 0x80000000
	.long	2147483648                      # 0x80000000
	.long	2147483648                      # 0x80000000
.LCPI0_5:
	.long	2147483648                      # 0x80000000
	.long	2147483649                      # 0x80000001
	.long	2147483650                      # 0x80000002
	.long	2147483651                      # 0x80000003
	.text
	.p2align	4
	.type	.L_Z7facultyi,@function
.L_Z7facultyi:                          # @_Z7facultyi
# %bb.0:
                                        # kill: def $edi killed $edi def $rdi
	movl	$1, %eax
	cmpl	$2, %edi
	jb	.LBB0_7
# %bb.1:                                # %vector.ph
	leal	2(%rdi), %eax
	andl	$28, %eax
	movd	%edi, %xmm0
	addl	$-2, %edi
	pshufd	$0, %xmm0, %xmm2                # xmm2 = xmm0[0,0,0,0]
	movdqa	.LCPI0_0(%rip), %xmm0           # xmm0 = [0,4294967295,4294967294,4294967293]
	paddd	%xmm2, %xmm0
	movd	%edi, %xmm1
	pshufd	$0, %xmm1, %xmm1                # xmm1 = xmm1[0,0,0,0]
	cmpl	$4, %eax
	jne	.LBB0_3
# %bb.2:
	movdqa	.LCPI0_1(%rip), %xmm2           # xmm2 = [1,1,1,1]
	xorl	%ecx, %ecx
	jmp	.LBB0_6
.LBB0_3:                                # %vector.body.1
	movdqa	.LCPI0_2(%rip), %xmm3           # xmm3 = [4294967292,4294967291,4294967290,4294967289]
	paddd	%xmm2, %xmm3
	pshufd	$245, %xmm3, %xmm4              # xmm4 = xmm3[1,1,3,3]
	pmuludq	%xmm0, %xmm3
	pshufd	$232, %xmm3, %xmm3              # xmm3 = xmm3[0,2,2,3]
	pshufd	$245, %xmm0, %xmm5              # xmm5 = xmm0[1,1,3,3]
	pmuludq	%xmm4, %xmm5
	pshufd	$232, %xmm5, %xmm4              # xmm4 = xmm5[0,2,2,3]
	punpckldq	%xmm4, %xmm3            # xmm3 = xmm3[0],xmm4[0],xmm3[1],xmm4[1]
	movl	$4, %ecx
	cmpl	$8, %eax
	jne	.LBB0_5
# %bb.4:
	movdqa	%xmm0, %xmm2
	movdqa	%xmm3, %xmm0
	jmp	.LBB0_6
.LBB0_5:                                # %vector.body.2
	paddd	.LCPI0_3(%rip), %xmm2
	movdqa	%xmm2, %xmm0
	pmuludq	%xmm3, %xmm0
	pshufd	$232, %xmm0, %xmm0              # xmm0 = xmm0[0,2,2,3]
	pshufd	$245, %xmm3, %xmm4              # xmm4 = xmm3[1,1,3,3]
	pshufd	$245, %xmm2, %xmm2              # xmm2 = xmm2[1,1,3,3]
	pmuludq	%xmm4, %xmm2
	pshufd	$232, %xmm2, %xmm2              # xmm2 = xmm2[0,2,2,3]
	punpckldq	%xmm2, %xmm0            # xmm0 = xmm0[0],xmm2[0],xmm0[1],xmm2[1]
	movl	$8, %ecx
	movdqa	%xmm3, %xmm2
.LBB0_6:                                # %middle.block
	movd	%ecx, %xmm3
	pshufd	$0, %xmm3, %xmm3                # xmm3 = xmm3[0,0,0,0]
	pxor	.LCPI0_4(%rip), %xmm1
	por	.LCPI0_5(%rip), %xmm3
	pcmpgtd	%xmm1, %xmm3
	pand	%xmm3, %xmm2
	pandn	%xmm0, %xmm3
	por	%xmm2, %xmm3
	pshufd	$238, %xmm3, %xmm0              # xmm0 = xmm3[2,3,2,3]
	pshufd	$255, %xmm3, %xmm1              # xmm1 = xmm3[3,3,3,3]
	pshufd	$245, %xmm3, %xmm2              # xmm2 = xmm3[1,1,3,3]
	pmuludq	%xmm1, %xmm2
	pmuludq	%xmm3, %xmm0
	pmuludq	%xmm2, %xmm0
	movd	%xmm0, %eax
.LBB0_7:                                # %common.ret
	retq
.Lfunc_end0:
	.size	.L_Z7facultyi, .Lfunc_end0-.L_Z7facultyi
                                        # -- End function
	.globl	main                            # -- Begin function main
	.p2align	4
	.type	main,@function
main:                                   # @main
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
	.cfi_endproc
                                        # -- End function
	.type	.Lprintf.str.0,@object          # @printf.str.0
	.section	.rodata.str1.1,"aMS",@progbits,1
.Lprintf.str.0:
	.asciz	"Faculty of %d is: %d"
	.size	.Lprintf.str.0, 21

	.ident	"spice version dev (https://github.com/spicelang/spice)"
	.section	".note.GNU-stack","",@progbits
