	.file	"accounts-dialog.c"
	.text
	.p2align 4,,15
	.type	accounts_destroy_cb, @function
accounts_destroy_cb:
.LFB26:
	movq	%rsi, %rdi
	jmp	g_free
.LFE26:
	.size	accounts_destroy_cb, .-accounts_destroy_cb
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"password_entry"
.LC1:
	.string	"username_entry"
.LC2:
	.string	"accounts_dialog"
.LC3:
	.string	"accounts-dialog.ui"
.LC4:
	.string	"show_password_checkbutton"
.LC5:
	.string	"destroy"
.LC6:
	.string	"toggled"
.LC7:
	.string	"response"
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC8:
	.string	"/apps/greet-tweet-know/auth/user_id"
	.section	.rodata.str1.1
.LC9:
	.string	""
	.text
	.p2align 4,,15
.globl accounts_dialog_show
	.type	accounts_dialog_show, @function
accounts_dialog_show:
.LFB28:
	pushq	%rbp
.LCFI0:
	movq	%rdi, %rbp
	pushq	%rbx
.LCFI1:
	subq	$72, %rsp
.LCFI2:
	movq	act.38741(%rip), %rbx
	testq	%rbx, %rbx
	je	.L4
	call	gtk_window_get_type
	movq	(%rbx), %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_window_present
	addq	$72, %rsp
	popq	%rbx
	popq	%rbp
	ret
	.p2align 4,,10
	.p2align 3
.L4:
	movl	$32, %edi
	call	g_malloc0
	leaq	24(%rax), %rdx
	leaq	8(%rax), %r8
	movq	%rax, act.38741(%rip)
	movl	$.LC0, %r9d
	movl	$.LC1, %ecx
	movl	$.LC2, %esi
	movq	%rdx, 16(%rsp)
	leaq	16(%rax), %rdx
	movl	$.LC3, %edi
	movq	$0, 24(%rsp)
	movq	$.LC4, 8(%rsp)
	movq	%rdx, (%rsp)
	movq	%rax, %rdx
	xorl	%eax, %eax
	call	gtkbuilder_get_file
	movq	act.38741(%rip), %rsi
	movl	$.LC2, %r9d
	movl	$accounts_destroy_cb, %r8d
	movq	%r9, %rdx
	movl	$.LC5, %ecx
	movq	%rax, %rbx
	movq	%rax, %rdi
	xorl	%eax, %eax
	movq	$0, 40(%rsp)
	movq	$accounts_show_password_cb, 32(%rsp)
	movq	$.LC6, 24(%rsp)
	movq	$.LC4, 16(%rsp)
	movq	$accounts_response_cb, 8(%rsp)
	movq	$.LC7, (%rsp)
	call	gtkbuilder_connect
	movq	%rbx, %rdi
	movl	$.LC9, %ebx
	call	g_object_unref
	movq	act.38741(%rip), %rax
	movl	$80, %esi
	movq	(%rax), %rdi
	call	g_type_check_instance_cast
	movl	$act.38741, %esi
	movq	%rax, %rdi
	call	g_object_add_weak_pointer
	call	gtk_window_get_type
	movq	%rax, %rsi
	movq	act.38741(%rip), %rax
	movq	(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%rbp, %rsi
	movq	%rax, %rdi
	call	gtk_window_set_transient_for
	call	conf_get
	leaq	64(%rsp), %rdx
	movq	%rax, %rdi
	movl	$.LC8, %esi
	call	conf_get_string
	movq	64(%rsp), %rax
	testq	%rax, %rax
	cmovne	%rax, %rbx
	call	gtk_entry_get_type
	movq	%rax, %rbp
	movq	act.38741(%rip), %rax
	movq	%rbp, %rsi
	movq	8(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%rbx, %rsi
	movq	%rax, %rdi
	call	gtk_entry_set_text
	movq	64(%rsp), %rdi
	testq	%rdi, %rdi
	je	.L8
	cmpb	$0, (%rdi)
	jne	.L9
.L8:
	movq	$0, 64(%rsp)
	movq	$0, 56(%rsp)
.L10:
	movl	$.LC9, %ebx
.L12:
	movq	act.38741(%rip), %rax
	movq	%rbp, %rsi
	movq	16(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%rbx, %rsi
	movq	%rax, %rdi
	call	gtk_entry_set_text
	movq	64(%rsp), %rdi
	call	g_free
	movq	56(%rsp), %rdi
	call	g_free
	movq	act.38741(%rip), %rax
	movq	(%rax), %rdi
	call	gtk_widget_show
	addq	$72, %rsp
	popq	%rbx
	popq	%rbp
	ret
	.p2align 4,,10
	.p2align 3
.L9:
	leaq	56(%rsp), %rsi
	call	keyring_get_password
	testl	%eax, %eax
	jne	.L11
	movq	$0, 56(%rsp)
	jmp	.L10
	.p2align 4,,10
	.p2align 3
.L11:
	movq	56(%rsp), %rsi
	testq	%rsi, %rsi
	movq	%rsi, %rbx
	jne	.L12
	jmp	.L10
.LFE28:
	.size	accounts_dialog_show, .-accounts_dialog_show
	.p2align 4,,15
	.type	accounts_show_password_cb, @function
accounts_show_password_cb:
.LFB27:
	pushq	%r12
.LCFI3:
	movq	%rsi, %r12
	pushq	%rbx
.LCFI4:
	subq	$8, %rsp
.LCFI5:
	call	gtk_toggle_button_get_type
	movq	24(%r12), %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_toggle_button_get_active
	movl	%eax, %ebx
	call	gtk_entry_get_type
	movq	16(%r12), %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	addq	$8, %rsp
	movl	%ebx, %esi
	movq	%rax, %rdi
	popq	%rbx
	popq	%r12
	jmp	gtk_entry_set_visibility
.LFE27:
	.size	accounts_show_password_cb, .-accounts_show_password_cb
	.p2align 4,,15
	.type	accounts_response_cb, @function
accounts_response_cb:
.LFB25:
	movq	%rbp, -24(%rsp)
.LCFI6:
	movq	%r13, -8(%rsp)
.LCFI7:
	movq	%rdx, %rbp
	movq	%rbx, -32(%rsp)
.LCFI8:
	movq	%r12, -16(%rsp)
.LCFI9:
	subq	$40, %rsp
.LCFI10:
	cmpl	$-5, %esi
	movq	%rdi, %r13
	je	.L20
.L18:
	movq	%r13, %rdi
	movq	8(%rsp), %rbx
	movq	16(%rsp), %rbp
	movq	24(%rsp), %r12
	movq	32(%rsp), %r13
	addq	$40, %rsp
	jmp	gtk_widget_destroy
	.p2align 4,,10
	.p2align 3
.L20:
	call	conf_get
	movq	%rax, %r12
	call	gtk_entry_get_type
	movq	8(%rbp), %rdi
	movq	%rax, %rsi
	movq	%rax, %rbx
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_entry_get_text
	movq	%r12, %rdi
	movq	%rax, %rdx
	movl	$.LC8, %esi
	call	conf_set_string
	movq	16(%rbp), %rdi
	movq	%rbx, %rsi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_entry_get_text
	movq	8(%rbp), %rdi
	movq	%rbx, %rsi
	movq	%rax, %r12
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_entry_get_text
	movq	%r12, %rsi
	movq	%rax, %rdi
	call	keyring_set_password
	jmp	.L18
.LFE25:
	.size	accounts_response_cb, .-accounts_response_cb
	.local	act.38741
	.comm	act.38741,8,8
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
	.align 8
.LEFDE1:
.LSFDE3:
	.long	.LEFDE3-.LASFDE3
.LASFDE3:
	.long	.LASFDE3-.Lframe1
	.long	.LFB28
	.long	.LFE28-.LFB28
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI0-.LFB28
	.byte	0xe
	.uleb128 0x10
	.byte	0x86
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI1-.LCFI0
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI2-.LCFI1
	.byte	0xe
	.uleb128 0x60
	.byte	0x83
	.uleb128 0x3
	.align 8
.LEFDE3:
.LSFDE5:
	.long	.LEFDE5-.LASFDE5
.LASFDE5:
	.long	.LASFDE5-.Lframe1
	.long	.LFB27
	.long	.LFE27-.LFB27
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI3-.LFB27
	.byte	0xe
	.uleb128 0x10
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI4-.LCFI3
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI5-.LCFI4
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x3
	.align 8
.LEFDE5:
.LSFDE7:
	.long	.LEFDE7-.LASFDE7
.LASFDE7:
	.long	.LASFDE7-.Lframe1
	.long	.LFB25
	.long	.LFE25-.LFB25
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI7-.LFB25
	.byte	0x8d
	.uleb128 0x2
	.byte	0x86
	.uleb128 0x4
	.byte	0x4
	.long	.LCFI10-.LCFI7
	.byte	0xe
	.uleb128 0x30
	.byte	0x8c
	.uleb128 0x3
	.byte	0x83
	.uleb128 0x5
	.align 8
.LEFDE7:
	.ident	"GCC: (SUSE Linux) 4.3.2 [gcc-4_3-branch revision 141291]"
	.section	.comment.SUSE.OPTs,"MS",@progbits,1
	.ascii	"OspWg"
	.section	.note.GNU-stack,"",@progbits
