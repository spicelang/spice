	.text
	.def	@feat.00;
	.scl	3;
	.type	0;
	.endef
	.globl	@feat.00
.set @feat.00, 0
	.file	"source.spice"
	.def	.L_f__void__int__faculty__int;
	.scl	3;
	.type	32;
	.endef
	.section	.rdata,"dr"
	.p2align	4, 0x0
.LCPI0_0:
	.long	0
	.long	4294967295
	.long	4294967294
	.long	4294967293
.LCPI0_1:
	.long	1
	.long	1
	.long	1
	.long	1
.LCPI0_2:
	.long	4294967292
	.long	4294967292
	.long	4294967292
	.long	4294967292
.LCPI0_3:
	.long	4294967288
	.long	4294967288
	.long	4294967288
	.long	4294967288
	.text
	.p2align	4, 0x90
.L_f__void__int__faculty__int:
	subq	$40, %rsp
	movdqa	%xmm7, 16(%rsp)
	movdqa	%xmm6, (%rsp)
	movl	$1, %eax
	cmpl	$2, %ecx
	jl	.LBB0_6
	xorl	%eax, %eax
	movl	%ecx, %edx
	subl	$2, %edx
	cmovbl	%eax, %edx
	movl	$1, %eax
	cmpl	$7, %edx
	jb	.LBB0_5
	incl	%edx
	movl	%edx, %r8d
	andl	$-8, %r8d
	movd	%ecx, %xmm0
	subl	%r8d, %ecx
	pshufd	$0, %xmm0, %xmm0
	paddd	.LCPI0_0(%rip), %xmm0
	movl	%r8d, %eax
	negl	%eax
	movdqa	.LCPI0_1(%rip), %xmm1
	movdqa	.LCPI0_2(%rip), %xmm3
	movdqa	.LCPI0_3(%rip), %xmm4
	movdqa	%xmm1, %xmm2
	.p2align	4, 0x90
.LBB0_3:
	movdqa	%xmm0, %xmm5
	paddd	%xmm3, %xmm5
	movdqa	%xmm0, %xmm6
	pmuludq	%xmm1, %xmm6
	pshufd	$245, %xmm1, %xmm1
	pshufd	$232, %xmm6, %xmm6
	pshufd	$245, %xmm0, %xmm7
	pmuludq	%xmm1, %xmm7
	pshufd	$232, %xmm7, %xmm7
	movdqa	%xmm6, %xmm1
	punpckldq	%xmm7, %xmm1
	pshufd	$245, %xmm5, %xmm6
	pmuludq	%xmm2, %xmm5
	pshufd	$245, %xmm2, %xmm2
	pshufd	$232, %xmm5, %xmm5
	pmuludq	%xmm6, %xmm2
	pshufd	$232, %xmm2, %xmm6
	movdqa	%xmm5, %xmm2
	punpckldq	%xmm6, %xmm2
	paddd	%xmm4, %xmm0
	addl	$8, %eax
	jne	.LBB0_3
	pshufd	$245, %xmm1, %xmm0
	pshufd	$245, %xmm2, %xmm3
	pmuludq	%xmm0, %xmm3
	pmuludq	%xmm1, %xmm2
	pshufd	$238, %xmm2, %xmm0
	pmuludq	%xmm2, %xmm0
	pshufd	$170, %xmm3, %xmm1
	pmuludq	%xmm3, %xmm1
	pmuludq	%xmm0, %xmm1
	movd	%xmm1, %eax
	cmpl	%r8d, %edx
	je	.LBB0_6
	.p2align	4, 0x90
.LBB0_5:
	imull	%ecx, %eax
	leal	-1(%rcx), %edx
	cmpl	$3, %ecx
	movl	%edx, %ecx
	jae	.LBB0_5
.LBB0_6:
	movaps	(%rsp), %xmm6
	movaps	16(%rsp), %xmm7
	addq	$40, %rsp
	retq

	.def	main;
	.scl	2;
	.type	32;
	.endef
	.globl	main
	.p2align	4, 0x90
main:
.seh_proc main
	pushq	%rbp
	.seh_pushreg %rbp
	subq	$32, %rsp
	.seh_stackalloc 32
	leaq	32(%rsp), %rbp
	.seh_setframe %rbp, 32
	.seh_endprologue
	callq	__main
	movl	$10, %ecx
	callq	.L_f__void__int__faculty__int
	leaq	.Lprintf.str.0(%rip), %rcx
	movl	$10, %edx
	movl	%eax, %r8d
	callq	printf
	xorl	%eax, %eax
	addq	$32, %rsp
	popq	%rbp
	retq
	.seh_endproc

	.section	.rdata,"dr"
.Lprintf.str.0:
	.asciz	"Faculty of %d is: %d"

