	.file	"gtkbuilder.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"Missing widget '%s'"
	.text
	.p2align 4,,15
.globl gtkbuilder_connect
	.type	gtkbuilder_connect, @function
gtkbuilder_connect:
.LFB28:
	pushq	%r14
.LCFI0:
	movq	%rsi, %r14
	pushq	%r13
.LCFI1:
	movq	%rdi, %r13
	pushq	%r12
.LCFI2:
	pushq	%rbp
.LCFI3:
	pushq	%rbx
.LCFI4:
	subq	$80, %rsp
.LCFI5:
	testq	%rdx, %rdx
	leaq	128(%rsp), %rax
	movq	%rcx, 56(%rsp)
	movq	%r8, 64(%rsp)
	movq	%r9, 72(%rsp)
	movl	$24, (%rsp)
	movq	%rax, 8(%rsp)
	leaq	32(%rsp), %rax
	movq	%rax, 16(%rsp)
	je	.L12
	movq	%rdx, %rbx
	jmp	.L11
	.p2align 4,,10
	.p2align 3
.L19:
	leal	8(%rcx), %edx
	movq	16(%rsp), %rsi
	mov	%ecx, %eax
	cmpl	$47, %edx
	movl	%edx, (%rsp)
	movq	(%rsi,%rax), %r12
	ja	.L17
	mov	%edx, %eax
	leaq	(%rsi,%rax), %rdx
	leal	16(%rcx), %eax
	movl	%eax, (%rsp)
.L6:
	movq	%rbx, %rsi
	movq	%r13, %rdi
	movq	(%rdx), %rbp
	call	gtk_builder_get_object
	testq	%rax, %rax
	je	.L18
	xorl	%r9d, %r9d
	xorl	%r8d, %r8d
	movq	%r14, %rcx
	movq	%rbp, %rdx
	movq	%r12, %rsi
	movq	%rax, %rdi
	call	g_signal_connect_data
.L8:
	movl	(%rsp), %eax
	cmpl	$48, %eax
	jae	.L9
	mov	%eax, %edx
	addq	16(%rsp), %rdx
	addl	$8, %eax
	movl	%eax, (%rsp)
	movq	(%rdx), %rbx
	testq	%rbx, %rbx
	je	.L12
.L11:
	movl	(%rsp), %ecx
	cmpl	$48, %ecx
	jb	.L19
	movq	8(%rsp), %rdx
	movq	%rdx, %rax
	addq	$8, %rdx
	movq	%rdx, 8(%rsp)
	movq	(%rax), %r12
.L4:
	leaq	8(%rdx), %rax
	movq	%rax, 8(%rsp)
	jmp	.L6
	.p2align 4,,10
	.p2align 3
.L9:
	movq	8(%rsp), %rdx
	leaq	8(%rdx), %rax
	movq	%rax, 8(%rsp)
	movq	(%rdx), %rbx
	testq	%rbx, %rbx
	jne	.L11
.L12:
	addq	$80, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	popq	%r14
	ret
	.p2align 4,,10
	.p2align 3
.L18:
	movq	%rbx, %rcx
	movl	$.LC0, %edx
	movl	$16, %esi
	xorl	%edi, %edi
	call	g_log
	jmp	.L8
.L17:
	movq	8(%rsp), %rdx
	jmp	.L4
.LFE28:
	.size	gtkbuilder_connect, .-gtkbuilder_connect
	.section	.rodata.str1.1
.LC1:
	.string	"greet-tweet-know"
.LC2:
	.string	"/usr/share"
.LC3:
	.string	"data"
.LC4:
	.string	".."
.LC5:
	.string	"XML file error: %s"
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC6:
	.string	"Widget '%s' at '%s' is missing."
	.text
	.p2align 4,,15
.globl gtkbuilder_get_file
	.type	gtkbuilder_get_file, @function
gtkbuilder_get_file:
.LFB27:
	pushq	%r13
.LCFI6:
	movq	%rdi, %r13
	pushq	%r12
.LCFI7:
	pushq	%rbp
.LCFI8:
	movq	%rsi, %rbp
	pushq	%rbx
.LCFI9:
	subq	$88, %rsp
.LCFI10:
	leaq	128(%rsp), %rax
	movq	%r8, 64(%rsp)
	movq	%r9, 72(%rsp)
	movq	%rdx, 48(%rsp)
	movq	%rcx, 56(%rsp)
	movq	%rax, 8(%rsp)
	leaq	32(%rsp), %rax
	movl	$16, (%rsp)
	movq	$0, 24(%rsp)
	movq	%rax, 16(%rsp)
	call	gtk_builder_new
	movl	$.LC1, %esi
	movq	%rax, %rdi
	movq	%rax, %r12
	call	gtk_builder_set_translation_domain
	xorl	%ecx, %ecx
	movq	%r13, %rdx
	movl	$.LC1, %esi
	movl	$.LC2, %edi
	xorl	%eax, %eax
	call	g_build_filename
	movl	$17, %esi
	movq	%rax, %rdi
	movq	%rax, %rbx
	call	g_file_test
	testl	%eax, %eax
	je	.L33
.L21:
	leaq	24(%rsp), %rdx
	movq	%rbx, %rsi
	movq	%r12, %rdi
	call	gtk_builder_add_from_file
	testl	%eax, %eax
	je	.L34
	movq	%rbx, %rdi
	call	g_free
	testq	%rbp, %rbp
	.p2align 4,,2
	.p2align 3
	jne	.L31
	.p2align 4,,4
	.p2align 3
	jmp	.L23
	.p2align 4,,10
	.p2align 3
.L36:
	mov	%eax, %edx
	addq	16(%rsp), %rdx
	addl	$8, %eax
	movl	%eax, (%rsp)
.L25:
	movq	(%rdx), %rbx
	movq	%rbp, %rsi
	movq	%r12, %rdi
	call	gtk_builder_get_object
	testq	%rax, %rax
	movq	%rax, (%rbx)
	je	.L35
.L26:
	movl	(%rsp), %eax
	cmpl	$47, %eax
	ja	.L27
	mov	%eax, %edx
	addq	16(%rsp), %rdx
	addl	$8, %eax
	movl	%eax, (%rsp)
	movq	(%rdx), %rbp
	testq	%rbp, %rbp
	je	.L23
.L31:
	movl	(%rsp), %eax
	cmpl	$47, %eax
	jbe	.L36
	movq	8(%rsp), %rdx
	leaq	8(%rdx), %rax
	movq	%rax, 8(%rsp)
	jmp	.L25
	.p2align 4,,10
	.p2align 3
.L27:
	movq	8(%rsp), %rdx
	leaq	8(%rdx), %rax
	movq	%rax, 8(%rsp)
	movq	(%rdx), %rbp
	testq	%rbp, %rbp
	jne	.L31
.L23:
	addq	$88, %rsp
	movq	%r12, %rax
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
	.p2align 4,,10
	.p2align 3
.L35:
	movq	%r13, %r8
	movq	%rbp, %rcx
	movl	$.LC6, %edx
	movl	$16, %esi
	xorl	%edi, %edi
	call	g_log
	jmp	.L26
	.p2align 4,,10
	.p2align 3
.L33:
	movq	%rbx, %rdi
	call	g_free
	xorl	%ecx, %ecx
	movq	%r13, %rdx
	movl	$.LC3, %esi
	movl	$.LC4, %edi
	xorl	%eax, %eax
	call	g_build_filename
	movq	%rax, %rbx
	jmp	.L21
.L34:
	movq	24(%rsp), %rax
	movl	$.LC5, %edx
	movl	$16, %esi
	xorl	%edi, %edi
	xorl	%r12d, %r12d
	movq	8(%rax), %rcx
	xorl	%eax, %eax
	call	g_log
	movq	24(%rsp), %rdi
	call	g_error_free
	movq	%rbx, %rdi
	call	g_free
	jmp	.L23
.LFE27:
	.size	gtkbuilder_get_file, .-gtkbuilder_get_file
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
	.long	.LFB28
	.long	.LFE28-.LFB28
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI0-.LFB28
	.byte	0xe
	.uleb128 0x10
	.byte	0x8e
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI1-.LCFI0
	.byte	0xe
	.uleb128 0x18
	.byte	0x8d
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI2-.LCFI1
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI3-.LCFI2
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.long	.LCFI4-.LCFI3
	.byte	0xe
	.uleb128 0x30
	.byte	0x4
	.long	.LCFI5-.LCFI4
	.byte	0xe
	.uleb128 0x80
	.byte	0x83
	.uleb128 0x6
	.byte	0x86
	.uleb128 0x5
	.byte	0x8c
	.uleb128 0x4
	.align 8
.LEFDE1:
.LSFDE3:
	.long	.LEFDE3-.LASFDE3
.LASFDE3:
	.long	.LASFDE3-.Lframe1
	.long	.LFB27
	.long	.LFE27-.LFB27
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI6-.LFB27
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI7-.LCFI6
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI8-.LCFI7
	.byte	0xe
	.uleb128 0x20
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI9-.LCFI8
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.long	.LCFI10-.LCFI9
	.byte	0xe
	.uleb128 0x80
	.byte	0x83
	.uleb128 0x5
	.align 8
.LEFDE3:
	.ident	"GCC: (SUSE Linux) 4.3.2 [gcc-4_3-branch revision 141291]"
	.section	.comment.SUSE.OPTs,"MS",@progbits,1
	.ascii	"OspWg"
	.section	.note.GNU-stack,"",@progbits
