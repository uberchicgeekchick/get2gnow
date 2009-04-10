	.file	"geometry.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"Loading window geometry..."
.LC1:
	.string	"Geometry"
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC2:
	.string	"/apps/greet-tweet-know/ui/main_window_height"
	.align 8
.LC3:
	.string	"/apps/greet-tweet-know/ui/main_window_width"
	.align 8
.LC4:
	.string	"/apps/greet-tweet-know/ui/main_window_pos_x"
	.align 8
.LC5:
	.string	"/apps/greet-tweet-know/ui/main_window_pos_y"
	.align 8
.LC6:
	.string	"Configuring window default size w:%d, h: %d"
	.align 8
.LC7:
	.string	"Configuring window default position x:%d, y:%d"
	.text
	.p2align 4,,15
.globl geometry_load_for_main_window
	.type	geometry_load_for_main_window, @function
geometry_load_for_main_window:
.LFB26:
	pushq	%r12
.LCFI0:
	movl	$.LC0, %esi
	xorl	%eax, %eax
	pushq	%rbp
.LCFI1:
	movq	%rdi, %rbp
	movl	$.LC1, %edi
	pushq	%rbx
.LCFI2:
	subq	$16, %rsp
.LCFI3:
	call	debug_impl
	call	conf_get
	movq	%rsp, %rdx
	movl	$.LC2, %esi
	movq	%rax, %rdi
	movq	%rax, %rbx
	call	conf_get_int
	leaq	4(%rsp), %rdx
	movl	$.LC3, %esi
	movq	%rbx, %rdi
	call	conf_get_int
	leaq	12(%rsp), %rdx
	movl	$.LC4, %esi
	movq	%rbx, %rdi
	call	conf_get_int
	leaq	8(%rsp), %rdx
	movl	$.LC5, %esi
	movq	%rbx, %rdi
	call	conf_get_int
	movl	4(%rsp), %edx
	testl	%edx, %edx
	jle	.L2
	movl	(%rsp), %ecx
	testl	%ecx, %ecx
	jle	.L2
	movl	$.LC6, %esi
	movl	$.LC1, %edi
	xorl	%eax, %eax
	call	debug_impl
	movl	(%rsp), %ebx
	movl	4(%rsp), %r12d
	call	gtk_window_get_type
	movq	%rbp, %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	movl	%ebx, %edx
	movq	%rax, %rdi
	movl	%r12d, %esi
	call	gtk_window_resize
.L2:
	movl	12(%rsp), %edx
	testl	%edx, %edx
	js	.L4
	movl	8(%rsp), %ecx
	testl	%ecx, %ecx
	js	.L4
	movl	$.LC7, %esi
	movl	$.LC1, %edi
	xorl	%eax, %eax
	call	debug_impl
	movl	8(%rsp), %ebx
	movl	12(%rsp), %r12d
	call	gtk_window_get_type
	movq	%rbp, %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	movl	%ebx, %edx
	movq	%rax, %rdi
	movl	%r12d, %esi
	call	gtk_window_move
.L4:
	addq	$16, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
.LFE26:
	.size	geometry_load_for_main_window, .-geometry_load_for_main_window
	.section	.rodata.str1.8
	.align 8
.LC8:
	.string	"Saving for main window: x:%d, y:%d, w:%d, h:%d"
	.text
	.p2align 4,,15
.globl geometry_save_for_main_window
	.type	geometry_save_for_main_window, @function
geometry_save_for_main_window:
.LFB25:
	movl	%ecx, %r9d
	movl	%edx, %r8d
	movq	%rbx, -40(%rsp)
.LCFI4:
	movq	%r12, -32(%rsp)
.LCFI5:
	movq	%r13, -24(%rsp)
.LCFI6:
	movl	%ecx, %r12d
	movq	%r14, -16(%rsp)
.LCFI7:
	movl	%esi, %ecx
	movq	%r15, -8(%rsp)
.LCFI8:
	movl	%edi, %r14d
	subq	$40, %rsp
.LCFI9:
	movl	%esi, %r15d
	movl	%edx, %r13d
	movl	$.LC8, %esi
	movl	%edi, %edx
	xorl	%eax, %eax
	movl	$.LC1, %edi
	call	debug_impl
	call	conf_get
	movl	%r12d, %edx
	movq	%rax, %rbx
	movl	$.LC2, %esi
	movq	%rax, %rdi
	call	conf_set_int
	movl	%r13d, %edx
	movq	%rbx, %rdi
	movl	$.LC3, %esi
	call	conf_set_int
	movl	%r14d, %edx
	movq	%rbx, %rdi
	movl	$.LC4, %esi
	call	conf_set_int
	movl	%r15d, %edx
	movq	%rbx, %rdi
	movq	8(%rsp), %r12
	movq	(%rsp), %rbx
	movq	16(%rsp), %r13
	movl	$.LC5, %esi
	movq	24(%rsp), %r14
	movq	32(%rsp), %r15
	addq	$40, %rsp
	jmp	conf_set_int
.LFE25:
	.size	geometry_save_for_main_window, .-geometry_save_for_main_window
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
	.long	.LFB26
	.long	.LFE26-.LFB26
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI0-.LFB26
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI1-.LCFI0
	.byte	0xe
	.uleb128 0x18
	.byte	0x86
	.uleb128 0x3
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI2-.LCFI1
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI3-.LCFI2
	.byte	0xe
	.uleb128 0x30
	.byte	0x83
	.uleb128 0x4
	.align 8
.LEFDE1:
.LSFDE3:
	.long	.LEFDE3-.LASFDE3
.LASFDE3:
	.long	.LASFDE3-.Lframe1
	.long	.LFB25
	.long	.LFE25-.LFB25
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI6-.LFB25
	.byte	0x8d
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x5
	.byte	0x83
	.uleb128 0x6
	.byte	0x4
	.long	.LCFI8-.LCFI6
	.byte	0x8f
	.uleb128 0x2
	.byte	0x8e
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI9-.LCFI8
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE3:
	.ident	"GCC: (SUSE Linux) 4.3.2 [gcc-4_3-branch revision 141291]"
	.section	.comment.SUSE.OPTs,"MS",@progbits,1
	.ascii	"OspWg"
	.section	.note.GNU-stack,"",@progbits
