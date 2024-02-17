	.text
	.def	@feat.00;
	.scl	3;
	.type	0;
	.endef
	.globl	@feat.00
.set @feat.00, 0
	.file	"source.spice"
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
	pushq	%r15
	.seh_pushreg %r15
	pushq	%r14
	.seh_pushreg %r14
	pushq	%r13
	.seh_pushreg %r13
	pushq	%r12
	.seh_pushreg %r12
	pushq	%rsi
	.seh_pushreg %rsi
	pushq	%rdi
	.seh_pushreg %rdi
	pushq	%rbx
	.seh_pushreg %rbx
	subq	$56, %rsp
	.seh_stackalloc 56
	leaq	48(%rsp), %rbp
	.seh_setframe %rbp, 48
	.seh_endprologue
	callq	__main
	xorl	%edi, %edi
	movl	$1, %r14d
	movl	$3, %r10d
	xorl	%r12d, %r12d
	xorl	%r11d, %r11d
	movl	$3, %esi
	movl	$1, %edx
	movl	$1, %ebx
	jmp	.LBB0_1
.LBB0_1:                                # %for.body.L19
                                        # =>This Inner Loop Header: Depth=1
	movq	%r14, %rax
	shlq	$2, %rax
	subq	%rbx, %rax
	addq	%r12, %rax
	movq	%rbx, %r15
	imulq	%rsi, %r15
	cmpq	%r15, %rax
	jge	.LBB0_5
# %bb.2:                                # %if.then.L20
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	%rdx, (%rbp)                    # 8-byte Spill
	movq	%r10, -8(%rbp)                  # 8-byte Spill
	leaq	.Lprintf.str.0(%rip), %rcx
	movq	%rsi, %rdx
	movl	%r11d, %r13d
	callq	printf
	movl	%r13d, %r11d
	cmpl	$0, %r11d
	jne	.LBB0_4
# %bb.3:                                # %if.then.L22
                                        #   in Loop: Header=BB0_1 Depth=1
	movl	$46, %ecx
	callq	putchar
	movl	%r13d, %r11d
.LBB0_4:                                # %if.exit.L22
                                        #   in Loop: Header=BB0_1 Depth=1
	addl	$1, %r11d
	imulq	$10, %r14, %rcx
	movq	%r12, %rax
	subq	%r15, %rax
	imulq	$10, %rax, %r8
	imulq	$3, %r14, %rax
	addq	%rax, %r12
	imulq	$10, %r12, %rax
	cqto
	idivq	%rbx
	imulq	$-10, %rsi, %rdx
	addq	%rdx, %rax
	movq	%r8, %r12
	movq	%rax, %rsi
	movq	%rcx, %r14
	movq	-8(%rbp), %r10                  # 8-byte Reload
	movq	(%rbp), %rdx                    # 8-byte Reload
	jmp	.LBB0_6
.LBB0_5:                                # %if.else.L20
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	%rdx, %rcx
	imulq	%r14, %rcx
	movq	%r14, %rax
	shlq	%rax
	movq	%r12, %r8
	addq	%rax, %r8
	imulq	%r10, %r8
	imulq	%r10, %rbx
	movq	%rdx, %r9
	addq	$1, %r9
	imulq	$7, %rdx, %rax
	addq	$2, %rax
	imulq	%r14, %rax
	imulq	%r10, %r12
	addq	%r12, %rax
	cqto
	idivq	%rbx
	movq	%rax, %rsi
	addq	$2, %r10
	movq	%r8, %r12
	movq	%r9, %rdx
	movq	%rcx, %r14
.LBB0_6:                                # %for.tail.L19
                                        #   in Loop: Header=BB0_1 Depth=1
	addl	$1, %edi
	cmpl	$20, %edi
	jne	.LBB0_1
# %bb.7:                                # %for.exit.L19
	xorl	%eax, %eax
	addq	$56, %rsp
	popq	%rbx
	popq	%rdi
	popq	%rsi
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	popq	%rbp
	retq
	.seh_endproc
                                        # -- End function
	.section	.rdata,"dr"
.Lprintf.str.0:                         # @printf.str.0
	.asciz	"%d"

