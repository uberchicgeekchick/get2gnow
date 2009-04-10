	.file	"debug.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"DEBUG"
.LC1:
	.string	":, "
.LC2:
	.string	"%s: "
.LC3:
	.string	"\n"
.LC4:
	.string	"domain != NULL"
.LC5:
	.string	"msg != NULL"
	.text
	.p2align 4,,15
.globl debug_impl
	.type	debug_impl, @function
debug_impl:
.LFB38:
	pushq	%r12
.LCFI0:
	movq	%rsi, %r12
	pushq	%rbp
.LCFI1:
	movq	%rdi, %rbp
	pushq	%rbx
.LCFI2:
	subq	$208, %rsp
.LCFI3:
	movq	%rdx, 48(%rsp)
	movzbl	%al, %edx
	movq	%rcx, 56(%rsp)
	leaq	0(,%rdx,4), %rax
	movl	$.L2, %edx
	movq	%r8, 64(%rsp)
	movq	%r9, 72(%rsp)
	subq	%rax, %rdx
	leaq	207(%rsp), %rax
	testq	%rdi, %rdi
	jmp	*%rdx
	movaps	%xmm7, -15(%rax)
	movaps	%xmm6, -31(%rax)
	movaps	%xmm5, -47(%rax)
	movaps	%xmm4, -63(%rax)
	movaps	%xmm3, -79(%rax)
	movaps	%xmm2, -95(%rax)
	movaps	%xmm1, -111(%rax)
	movaps	%xmm0, -127(%rax)
.L2:
	je	.L3
	testq	%rsi, %rsi
	je	.L4
	movl	inited.7963(%rip), %ecx
	testl	%ecx, %ecx
	je	.L19
.L5:
	movq	debug_strv(%rip), %rax
	testq	%rax, %rax
	je	.L15
	movq	(%rax), %rsi
	testq	%rsi, %rsi
	je	.L15
	movl	all_domains(%rip), %edx
	testl	%edx, %edx
	jne	.L12
	movq	%rax, %rbx
	jmp	.L14
	.p2align 4,,10
	.p2align 3
.L13:
	movq	8(%rbx), %rsi
	addq	$8, %rbx
	testq	%rsi, %rsi
	je	.L15
.L14:
	movq	%rbp, %rdi
	call	strcmp
	testl	%eax, %eax
	jne	.L13
.L12:
	movq	%rbp, %rsi
	movl	$.LC2, %edi
	xorl	%eax, %eax
	call	g_print
	leaq	240(%rsp), %rax
	movq	%r12, %rdi
	movq	%rsp, %rsi
	movl	$16, (%rsp)
	movl	$48, 4(%rsp)
	movq	%rax, 8(%rsp)
	leaq	32(%rsp), %rax
	movq	%rax, 16(%rsp)
	call	g_vprintf
	movl	$.LC3, %edi
	xorl	%eax, %eax
	call	g_print
.L15:
	addq	$208, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
	.p2align 4,,10
	.p2align 3
.L19:
	movl	$.LC0, %edi
	call	g_getenv
	testq	%rax, %rax
	je	.L6
	xorl	%edx, %edx
	movl	$.LC1, %esi
	movq	%rax, %rdi
	call	g_strsplit_set
	testq	%rax, %rax
	movq	%rax, debug_strv(%rip)
	je	.L7
	movq	(%rax), %rdx
	testq	%rdx, %rdx
	je	.L7
	movl	all_domains(%rip), %ecx
	movl	$1, %esi
	.p2align 4,,10
	.p2align 3
.L10:
	cmpb	$97, (%rdx)
	jne	.L9
	cmpb	$108, 1(%rdx)
	jne	.L9
	cmpb	$108, 2(%rdx)
	jne	.L9
	cmpb	$0, 3(%rdx)
	cmove	%esi, %ecx
	.p2align 4,,10
	.p2align 3
.L9:
	movq	8(%rax), %rdx
	addq	$8, %rax
	testq	%rdx, %rdx
	jne	.L10
	movl	%ecx, all_domains(%rip)
.L7:
	movl	$1, inited.7963(%rip)
	jmp	.L5
	.p2align 4,,10
	.p2align 3
.L3:
	movl	$.LC4, %edx
	movl	$__PRETTY_FUNCTION__.8024, %esi
	xorl	%edi, %edi
	call	g_return_if_fail_warning
	addq	$208, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
	.p2align 4,,10
	.p2align 3
.L4:
	movl	$.LC5, %edx
	movl	$__PRETTY_FUNCTION__.8024, %esi
	xorl	%edi, %edi
	call	g_return_if_fail_warning
	addq	$208, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
.L6:
	movq	$0, debug_strv(%rip)
	jmp	.L7
.LFE38:
	.size	debug_impl, .-debug_impl
	.local	all_domains
	.comm	all_domains,4,4
	.section	.rodata
	.type	__PRETTY_FUNCTION__.8024, @object
	.size	__PRETTY_FUNCTION__.8024, 11
__PRETTY_FUNCTION__.8024:
	.string	"debug_impl"
	.local	inited.7963
	.comm	inited.7963,4,4
	.local	debug_strv
	.comm	debug_strv,8,8
	.section	.eh_frame,"a",@progbits
.Lframe1:
	.long	.LECIE1-.LSCIE1
.LSCIE1:
	.long	0x0
	.byte	0x1
	.string	"zR"
	.uleb128 0x1
	.sleb128 -8
	.byte	0x10
	.uleb128 0x1
	.byte	0x3
	.byte	0xc
	.uleb128 0x7
	.uleb128 0x8
	.byte	0x90
	.uleb128 0x1
	.align 8
.LECIE1:
.LSFDE1:
	.long	.LEFDE1-.LASFDE1
.LASFDE1:
	.long	.LASFDE1-.Lframe1
	.long	.LFB38
	.long	.LFE38-.LFB38
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI0-.LFB38
	.byte	0xe
	.uleb128 0x10
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI1-.LCFI0
	.byte	0xe
	.uleb128 0x18
	.byte	0x86
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI2-.LCFI1
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI3-.LCFI2
	.byte	0xe
	.uleb128 0xf0
	.byte	0x83
	.uleb128 0x4
	.align 8
.LEFDE1:
	.ident	"GCC: (SUSE Linux) 4.3.2 [gcc-4_3-branch revision 141291]"
	.section	.comment.SUSE.OPTs,"MS",@progbits,1
	.ascii	"OspWg"
	.section	.note.GNU-stack,"",@progbits
