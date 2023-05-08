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
	.p2align	5, 0x0
.LCPI0_0:
	.long	0
	.long	4294967295
	.long	4294967294
	.long	4294967293
	.long	4294967292
	.long	4294967291
	.long	4294967290
	.long	4294967289
.LCPI0_1:
	.long	1
.LCPI0_2:
	.long	4294967288
.LCPI0_3:
	.long	4294967280
.LCPI0_4:
	.long	4294967272
.LCPI0_5:
	.long	4294967264
	.text
	.p2align	4, 0x90
.L_f__void__int__faculty__int:
	subq	$72, %rsp
	movl	$1, %eax
	vmovdqa	%xmm9, 48(%rsp)
	vmovdqa	%xmm8, 32(%rsp)
	vmovdqa	%xmm7, 16(%rsp)
	vmovdqa	%xmm6, (%rsp)
	cmpl	$2, %ecx
	jl	.LBB0_6
	xorl	%eax, %eax
	movl	%ecx, %edx
	subl	$2, %edx
	cmovbl	%eax, %edx
	movl	$1, %eax
	cmpl	$31, %edx
	jb	.LBB0_5
	vmovd	%ecx, %xmm0
	vpbroadcastd	.LCPI0_1(%rip), %ymm1
	vpbroadcastd	.LCPI0_2(%rip), %ymm2
	vpbroadcastd	.LCPI0_3(%rip), %ymm3
	vpbroadcastd	.LCPI0_4(%rip), %ymm4
	vpbroadcastd	.LCPI0_5(%rip), %ymm5
	incl	%edx
	vpbroadcastd	%xmm0, %ymm0
	vpaddd	.LCPI0_0(%rip), %ymm0, %ymm0
	movl	%edx, %r8d
	andl	$-32, %r8d
	movl	%r8d, %eax
	subl	%r8d, %ecx
	negl	%eax
	vmovdqa	%ymm1, %ymm6
	vmovdqa	%ymm1, %ymm7
	vmovdqa	%ymm1, %ymm8
	.p2align	4, 0x90
.LBB0_3:
	vpaddd	%ymm2, %ymm0, %ymm9
	vpmulld	%ymm1, %ymm0, %ymm1
	addl	$32, %eax
	vpmulld	%ymm6, %ymm9, %ymm6
	vpaddd	%ymm3, %ymm0, %ymm9
	vpmulld	%ymm7, %ymm9, %ymm7
	vpaddd	%ymm4, %ymm0, %ymm9
	vpaddd	%ymm5, %ymm0, %ymm0
	vpmulld	%ymm8, %ymm9, %ymm8
	jne	.LBB0_3
	vpmulld	%ymm1, %ymm6, %ymm0
	vpmulld	%ymm0, %ymm7, %ymm0
	vpmulld	%ymm0, %ymm8, %ymm0
	vextracti128	$1, %ymm0, %xmm1
	vpmulld	%xmm1, %xmm0, %xmm0
	vpshufd	$238, %xmm0, %xmm1
	vpmulld	%xmm1, %xmm0, %xmm0
	vpshufd	$85, %xmm0, %xmm1
	vpmulld	%xmm1, %xmm0, %xmm0
	vmovd	%xmm0, %eax
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
	vmovaps	(%rsp), %xmm6
	vmovaps	16(%rsp), %xmm7
	vmovaps	32(%rsp), %xmm8
	vmovaps	48(%rsp), %xmm9
	addq	$72, %rsp
	vzeroupper
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

