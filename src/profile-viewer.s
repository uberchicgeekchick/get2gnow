	.file	"profile-viewer.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"title_vbox"
.LC1:
	.string	"loading_label"
.LC2:
	.string	"profile_dialog"
.LC3:
	.string	"profile-viewer.ui"
.LC4:
	.string	"profile_vbox"
.LC5:
	.string	"destroy"
.LC6:
	.string	"response"
.LC7:
	.string	"\t<u><b>%s:</b> @%s</u>\n"
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC8:
	.string	"\t<b>Tweets:</b> %lu\n\t<b>Friends:</b> %lu\n\t<b>Followers:</b> %lu\n\t<b>Location:</b> %s\n"
	.align 8
.LC9:
	.string	"\t<b>URL:</b>\t<a href=\"%s\">%s</a>\n"
	.section	.rodata.str1.1
.LC10:
	.string	"\t<b>Bio:</b>\n\t\t%s\n"
.LC11:
	.string	"unknown_image"
	.text
	.p2align 4,,15
.globl view_profile
	.type	view_profile, @function
view_profile:
.LFB44:
	pushq	%r15
.LCFI0:
	pushq	%r14
.LCFI1:
	pushq	%r13
.LCFI2:
	pushq	%r12
.LCFI3:
	movq	%rdi, %r12
	movl	$72, %edi
	pushq	%rbp
.LCFI4:
	pushq	%rbx
.LCFI5:
	movq	%rsi, %rbx
	subq	$40, %rsp
.LCFI6:
	call	g_malloc
	movq	%rax, %rbp
	leaq	8(%rax), %rdx
	leaq	48(%rax), %rax
	leaq	24(%rbp), %r8
	movl	$.LC0, %r9d
	movl	$.LC1, %ecx
	movq	%rax, 16(%rsp)
	leaq	32(%rbp), %rax
	movl	$.LC2, %esi
	movl	$.LC3, %edi
	movq	$0, 24(%rsp)
	movq	$.LC4, 8(%rsp)
	movq	%rax, (%rsp)
	xorl	%eax, %eax
	call	gtkbuilder_get_file
	movl	$.LC2, %r9d
	movl	$gtk_widget_destroy, %r8d
	movl	$.LC5, %ecx
	movq	%r9, %rdx
	movq	%rax, %rdi
	movq	%rbp, %rsi
	xorl	%eax, %eax
	movq	$0, 16(%rsp)
	movq	$gtk_widget_destroy, 8(%rsp)
	movq	$.LC6, (%rsp)
	call	gtkbuilder_connect
	call	gtk_window_get_type
	movq	8(%rbp), %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	movq	%rbx, %rsi
	movq	%rax, %rdi
	call	gtk_window_set_transient_for
	call	gtk_widget_get_type
	movq	8(%rbp), %rdi
	movq	%rax, %rsi
	movq	%rax, %r15
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_widget_show_all
	movq	%r12, %rdi
	call	network_fetch_profile
	movq	8(%rax), %rdx
	movq	16(%rax), %rsi
	movl	$.LC7, %edi
	movq	%rax, (%rbp)
	xorl	%eax, %eax
	call	g_strdup_printf
	movq	%rax, %r12
	call	label_new
	movq	%r15, %rsi
	movq	%rax, %rdi
	movq	%rax, 40(%rbp)
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_widget_show
	movq	40(%rbp), %rdi
	movq	%r15, %rsi
	call	g_type_check_instance_cast
	movq	%rax, %rbx
	call	gtk_box_get_type
	movq	32(%rbp), %rdi
	movq	%rax, %rsi
	movq	%rax, %r14
	call	g_type_check_instance_cast
	xorl	%r8d, %r8d
	movl	$1, %ecx
	movl	$1, %edx
	movq	%rbx, %rsi
	movq	%rax, %rdi
	call	gtk_box_pack_start
	call	label_get_type
	movq	40(%rbp), %rdi
	movq	%rax, %rsi
	movq	%rax, %r13
	call	g_type_check_instance_cast
	movq	%r12, %rsi
	movq	%rax, %rdi
	call	label_set_text
	movq	%r12, %rdi
	call	g_free
	movq	(%rbp), %rax
	movl	$.LC8, %edi
	movq	80(%rax), %rcx
	movq	24(%rax), %r8
	movq	72(%rax), %rdx
	movq	64(%rax), %rsi
	xorl	%eax, %eax
	call	g_strdup_printf
	movq	8(%rbp), %rdi
	movq	%rax, %rbx
	movq	%rax, %rsi
	call	gtk_message_dialog_set_markup
	movq	%rbx, %rdi
	call	g_free
	movq	(%rbp), %rax
	movl	$.LC9, %edi
	movq	40(%rax), %rsi
	xorl	%eax, %eax
	movq	%rsi, %rdx
	call	g_strdup_printf
	movq	%rax, %r12
	call	label_new
	movq	%r15, %rsi
	movq	%rax, %rdi
	movq	%rax, 56(%rbp)
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_widget_show
	movq	56(%rbp), %rdi
	movq	%r15, %rsi
	call	g_type_check_instance_cast
	movq	48(%rbp), %rdi
	movq	%rax, %rbx
	movq	%r14, %rsi
	call	g_type_check_instance_cast
	xorl	%r8d, %r8d
	movl	$1, %ecx
	movl	$1, %edx
	movq	%rax, %rdi
	movq	%rbx, %rsi
	call	gtk_box_pack_start
	movq	56(%rbp), %rdi
	movq	%r13, %rsi
	call	g_type_check_instance_cast
	movq	%r12, %rsi
	movq	%rax, %rdi
	call	label_set_text
	movq	%r12, %rdi
	call	g_free
	movq	(%rbp), %rax
	movl	$.LC10, %edi
	movq	32(%rax), %rsi
	xorl	%eax, %eax
	call	g_strdup_printf
	movq	%rax, %r12
	call	label_new
	movq	%r15, %rsi
	movq	%rax, %rdi
	movq	%rax, 64(%rbp)
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_widget_show
	movq	64(%rbp), %rdi
	movq	%r15, %rsi
	call	g_type_check_instance_cast
	movq	48(%rbp), %rdi
	movq	%rax, %rbx
	movq	%r14, %rsi
	call	g_type_check_instance_cast
	xorl	%r8d, %r8d
	movl	$1, %ecx
	movl	$1, %edx
	movq	%rax, %rdi
	movq	%rbx, %rsi
	call	gtk_box_pack_end
	movq	64(%rbp), %rdi
	movq	%r13, %rsi
	call	g_type_check_instance_cast
	movq	%r12, %rsi
	movq	%rax, %rdi
	call	label_set_text
	movq	%r12, %rdi
	call	g_free
	movq	(%rbp), %rax
	movq	48(%rax), %rdi
	call	network_download_avatar
	movq	(%rbp), %rax
	movl	$.LC11, %edi
	movq	56(%rax), %rsi
	call	g_str_equal
	testl	%eax, %eax
	jne	.L2
	movq	(%rbp), %rax
	movq	56(%rax), %rdi
	call	images_get_maximized_image_from_filename
	movq	%r15, %rsi
	movq	%rax, %rdi
	movq	%rax, 16(%rbp)
	call	g_type_check_instance_cast
	movq	8(%rbp), %rdi
	movq	%rax, %rsi
	call	gtk_message_dialog_set_image
.L2:
	movq	8(%rbp), %rdi
	movq	%r15, %rsi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_widget_show_all
	movq	24(%rbp), %rdi
	movq	%r15, %rsi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_widget_hide
	movq	(%rbp), %rdi
	addq	$40, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	jmp	user_free
.LFE44:
	.size	view_profile, .-view_profile
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
	.long	.LFB44
	.long	.LFE44-.LFB44
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI0-.LFB44
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI1-.LCFI0
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI2-.LCFI1
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI3-.LCFI2
	.byte	0xe
	.uleb128 0x28
	.byte	0x8c
	.uleb128 0x5
	.byte	0x8d
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x3
	.byte	0x8f
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI4-.LCFI3
	.byte	0xe
	.uleb128 0x30
	.byte	0x4
	.long	.LCFI5-.LCFI4
	.byte	0xe
	.uleb128 0x38
	.byte	0x83
	.uleb128 0x7
	.byte	0x86
	.uleb128 0x6
	.byte	0x4
	.long	.LCFI6-.LCFI5
	.byte	0xe
	.uleb128 0x60
	.align 8
.LEFDE1:
	.ident	"GCC: (SUSE Linux) 4.3.2 [gcc-4_3-branch revision 141291]"
	.section	.comment.SUSE.OPTs,"MS",@progbits,1
	.ascii	"OspWg"
	.section	.note.GNU-stack,"",@progbits
