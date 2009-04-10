	.file	"followers-dialog.c"
	.text
	.p2align 4,,15
	.type	followers_destroy_cb, @function
followers_destroy_cb:
.LFB45:
	movq	%rsi, %rdi
	jmp	g_free
.LFE45:
	.size	followers_destroy_cb, .-followers_destroy_cb
	.p2align 4,,15
	.type	list_follower_activated_cb, @function
list_follower_activated_cb:
.LFB49:
	movq	%rbx, -32(%rsp)
.LCFI0:
	movq	%r12, -24(%rsp)
.LCFI1:
	movq	%rcx, %rbx
	movq	%r13, -16(%rsp)
.LCFI2:
	movq	%r14, -8(%rsp)
.LCFI3:
	subq	$88, %rsp
.LCFI4:
	movq	%rsi, %r14
	call	gtk_tree_model_get_type
	movq	16(%rbx), %rdi
	movq	%rax, %rsi
	movq	%rax, %r12
	call	g_type_check_instance_cast
	movq	%r14, %rdx
	movq	%rsp, %rsi
	movq	%rax, %rdi
	call	gtk_tree_model_get_iter
	movq	16(%rbx), %rdi
	movq	%r12, %rsi
	call	g_type_check_instance_cast
	leaq	40(%rsp), %rcx
	movq	%rsp, %rsi
	movl	$-1, %r8d
	xorl	%edx, %edx
	movq	%rax, %rdi
	xorl	%eax, %eax
	call	gtk_tree_model_get
	movq	40(%rsp), %rdi
	call	network_get_user
	movq	40(%rsp), %rdi
	call	g_free
	movq	(%rbx), %rdi
	call	gtk_widget_destroy
	movq	%rbx, %rdi
	call	g_free
	movq	56(%rsp), %rbx
	movq	64(%rsp), %r12
	movq	72(%rsp), %r13
	movq	80(%rsp), %r14
	addq	$88, %rsp
	ret
.LFE49:
	.size	list_follower_activated_cb, .-list_follower_activated_cb
	.p2align 4,,15
	.type	followers_response_cb, @function
followers_response_cb:
.LFB44:
	jmp	gtk_widget_destroy
.LFE44:
	.size	followers_response_cb, .-followers_response_cb
	.p2align 4,,15
	.type	followers_view_profile, @function
followers_view_profile:
.LFB48:
	pushq	%rbp
.LCFI5:
	pushq	%rbx
.LCFI6:
	movq	%rsi, %rbx
	subq	$56, %rsp
.LCFI7:
	movq	8(%rsi), %rdi
	call	gtk_tree_view_get_selection
	xorl	%esi, %esi
	movq	%rax, %rdi
	movq	%rsp, %rdx
	call	gtk_tree_selection_get_selected
	testl	%eax, %eax
	jne	.L11
.L9:
	addq	$56, %rsp
	popq	%rbx
	popq	%rbp
	ret
	.p2align 4,,10
	.p2align 3
.L11:
	.p2align 4,,6
	.p2align 3
	call	gtk_tree_model_get_type
	movq	8(%rbx), %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	leaq	40(%rsp), %rcx
	movq	%rax, %rdi
	xorl	%edx, %edx
	xorl	%eax, %eax
	movl	$-1, %r8d
	movq	%rsp, %rsi
	call	gtk_tree_model_get
	cmpq	$0, 40(%rsp)
	je	.L9
	call	gtk_window_get_type
	movq	(%rbx), %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	movq	40(%rsp), %rdi
	movq	%rax, %rsi
	call	view_profile
	movq	40(%rsp), %rdi
	call	g_free
	addq	$56, %rsp
	popq	%rbx
	popq	%rbp
	ret
.LFE48:
	.size	followers_view_profile, .-followers_view_profile
	.p2align 4,,15
	.type	followers_block_response_cb, @function
followers_block_response_cb:
.LFB47:
	pushq	%rbp
.LCFI8:
	pushq	%rbx
.LCFI9:
	movq	%rsi, %rbx
	subq	$56, %rsp
.LCFI10:
	movq	8(%rsi), %rdi
	movq	$0, 40(%rsp)
	call	gtk_tree_view_get_selection
	xorl	%esi, %esi
	movq	%rax, %rdi
	movq	%rsp, %rdx
	call	gtk_tree_selection_get_selected
	testl	%eax, %eax
	je	.L14
	movq	16(%rbx), %rdi
	leaq	40(%rsp), %rcx
	movl	$-1, %r8d
	xorl	%edx, %edx
	movq	%rsp, %rsi
	xorl	%eax, %eax
	call	gtk_tree_model_get
	call	gtk_list_store_get_type
	movq	16(%rbx), %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	movq	%rsp, %rsi
	movq	%rax, %rdi
	call	gtk_list_store_remove
	movq	40(%rsp), %rdi
	call	network_block_user
.L14:
	addq	$56, %rsp
	popq	%rbx
	popq	%rbp
	ret
.LFE47:
	.size	followers_block_response_cb, .-followers_block_response_cb
	.p2align 4,,15
	.type	followers_rem_response_cb, @function
followers_rem_response_cb:
.LFB46:
	pushq	%rbp
.LCFI11:
	pushq	%rbx
.LCFI12:
	movq	%rsi, %rbx
	subq	$56, %rsp
.LCFI13:
	movq	8(%rsi), %rdi
	movq	$0, 40(%rsp)
	call	gtk_tree_view_get_selection
	xorl	%esi, %esi
	movq	%rax, %rdi
	movq	%rsp, %rdx
	call	gtk_tree_selection_get_selected
	testl	%eax, %eax
	je	.L17
	movq	16(%rbx), %rdi
	leaq	40(%rsp), %rcx
	movl	$-1, %r8d
	xorl	%edx, %edx
	movq	%rsp, %rsi
	xorl	%eax, %eax
	call	gtk_tree_model_get
	call	gtk_list_store_get_type
	movq	16(%rbx), %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	movq	%rsp, %rsi
	movq	%rax, %rdi
	call	gtk_list_store_remove
	movq	40(%rsp), %rdi
	call	network_unfollow_user
.L17:
	addq	$56, %rsp
	popq	%rbx
	popq	%rbp
	ret
.LFE46:
	.size	followers_rem_response_cb, .-followers_rem_response_cb
	.p2align 4,,15
.globl followers_dialog_load_lists
	.type	followers_dialog_load_lists, @function
followers_dialog_load_lists:
.LFB50:
	pushq	%r15
.LCFI14:
	pushq	%r14
.LCFI15:
	pushq	%r13
.LCFI16:
	pushq	%r12
.LCFI17:
	pushq	%rbp
.LCFI18:
	pushq	%rbx
.LCFI19:
	movq	%rdi, %rbx
	subq	$72, %rsp
.LCFI20:
	testq	%rdi, %rdi
	je	.L21
	leaq	32(%rsp), %r15
	movq	%rbx, %rbp
	call	gtk_list_store_get_type
	movq	%rax, %r14
	.p2align 4,,10
	.p2align 3
.L20:
	movq	followers(%rip), %rax
	movq	%r14, %rsi
	movq	(%rbp), %rbx
	movq	16(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%r15, %rsi
	movq	%rax, %rdi
	call	gtk_list_store_append
	movq	followers(%rip), %rax
	movq	16(%rbx), %r12
	movq	%r14, %rsi
	movq	8(%rbx), %r13
	movq	16(%rax), %rdi
	call	g_type_check_instance_cast
	xorl	%edx, %edx
	movq	%rax, %rdi
	movq	%r12, %r9
	xorl	%eax, %eax
	movl	$1, %r8d
	movq	%r13, %rcx
	movq	%r15, %rsi
	movl	$-1, 16(%rsp)
	movq	%rbx, 8(%rsp)
	movl	$2, (%rsp)
	call	gtk_list_store_set
	movq	8(%rbp), %rbp
	testq	%rbp, %rbp
	jne	.L20
.L21:
	addq	$72, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	ret
.LFE50:
	.size	followers_dialog_load_lists, .-followers_dialog_load_lists
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"user_unfollow"
.LC1:
	.string	"following_list"
.LC2:
	.string	"followers_dialog"
.LC3:
	.string	"followers-dialog.ui"
.LC4:
	.string	"view_profile"
.LC5:
	.string	"user_block"
.LC6:
	.string	"destroy"
.LC7:
	.string	"row-activated"
.LC8:
	.string	"clicked"
.LC9:
	.string	"response"
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC10:
	.string	"Please wait while the list of every one you're following is loaded."
	.section	.rodata.str1.1
.LC11:
	.string	""
	.text
	.p2align 4,,15
.globl followers_dialog_show
	.type	followers_dialog_show, @function
followers_dialog_show:
.LFB51:
	movq	%rbx, -48(%rsp)
.LCFI21:
	movq	%rbp, -40(%rsp)
.LCFI22:
	movq	%rdi, %rbp
	movq	%r12, -32(%rsp)
.LCFI23:
	movq	%r13, -24(%rsp)
.LCFI24:
	movq	%r14, -16(%rsp)
.LCFI25:
	movq	%r15, -8(%rsp)
.LCFI26:
	subq	$232, %rsp
.LCFI27:
	movq	followers(%rip), %rbx
	testq	%rbx, %rbx
	je	.L24
	call	gtk_window_get_type
	movq	(%rbx), %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_window_present
.L28:
	movq	184(%rsp), %rbx
	movq	192(%rsp), %rbp
	movq	200(%rsp), %r12
	movq	208(%rsp), %r13
	movq	216(%rsp), %r14
	movq	224(%rsp), %r15
	addq	$232, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L24:
	movl	$48, %edi
	call	g_malloc0
	leaq	40(%rax), %rdx
	leaq	8(%rax), %r8
	movl	$.LC0, %r9d
	movl	$.LC1, %ecx
	movl	$.LC2, %esi
	movq	%rax, followers(%rip)
	movq	%rdx, 32(%rsp)
	leaq	32(%rax), %rdx
	movl	$.LC3, %edi
	movq	$0, 40(%rsp)
	movq	$.LC4, 24(%rsp)
	movq	%rdx, 16(%rsp)
	leaq	24(%rax), %rdx
	movq	$.LC5, 8(%rsp)
	movq	%rdx, (%rsp)
	movq	%rax, %rdx
	xorl	%eax, %eax
	call	gtkbuilder_get_file
	movq	followers(%rip), %rbx
	movq	%rax, %r12
	movq	8(%rbx), %rdi
	call	gtk_tree_view_get_model
	movq	followers(%rip), %rsi
	movl	$.LC2, %r9d
	movq	%rax, 16(%rbx)
	movq	%r9, %rdx
	movl	$followers_destroy_cb, %r8d
	movl	$.LC6, %ecx
	xorl	%eax, %eax
	movq	%r12, %rdi
	movq	$0, 112(%rsp)
	movq	$list_follower_activated_cb, 104(%rsp)
	movq	$.LC7, 96(%rsp)
	movq	$.LC1, 88(%rsp)
	movq	$followers_view_profile, 80(%rsp)
	movq	$.LC8, 72(%rsp)
	movq	$.LC4, 64(%rsp)
	movq	$followers_block_response_cb, 56(%rsp)
	movq	$.LC8, 48(%rsp)
	movq	$.LC5, 40(%rsp)
	movq	$followers_rem_response_cb, 32(%rsp)
	movq	$.LC8, 24(%rsp)
	movq	$.LC0, 16(%rsp)
	movq	$followers_response_cb, 8(%rsp)
	movq	$.LC9, (%rsp)
	call	gtkbuilder_connect
	movq	%r12, %rdi
	call	g_object_unref
	movq	followers(%rip), %rax
	movl	$80, %esi
	movq	(%rax), %rdi
	call	g_type_check_instance_cast
	movl	$followers, %esi
	movq	%rax, %rdi
	call	g_object_add_weak_pointer
	call	gtk_window_get_type
	movq	%rax, %rsi
	movq	followers(%rip), %rax
	movq	(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%rbp, %rsi
	movq	%rax, %rdi
	call	gtk_window_set_transient_for
	movq	followers(%rip), %rax
	movq	(%rax), %rdi
	call	gtk_widget_show
	movl	$5, %edx
	movl	$.LC10, %esi
	xorl	%edi, %edi
	call	dcgettext
	movq	%rax, %rdi
	call	app_set_statusbar_msg
	movl	$150, %edi
	call	gdk_cursor_new
	movq	%rax, %rbx
	call	gtk_widget_get_type
	movq	%rax, %rsi
	movq	%rax, 136(%rsp)
	movq	followers(%rip), %rax
	movq	(%rax), %rdi
	call	g_type_check_instance_cast
	movq	80(%rax), %rdi
	movq	%rbx, %rsi
	call	gdk_window_set_cursor
	movq	followers(%rip), %rax
	xorl	%esi, %esi
	movq	(%rax), %rdi
	call	gtk_widget_set_sensitive
	call	user_get_friends
	testq	%rax, %rax
	movq	%rax, %rbp
	je	.L26
	leaq	144(%rsp), %r15
	call	gtk_list_store_get_type
	movq	%rax, %r14
	.p2align 4,,10
	.p2align 3
.L27:
	movq	followers(%rip), %rax
	movq	%r14, %rsi
	movq	(%rbp), %rbx
	movq	16(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%r15, %rsi
	movq	%rax, %rdi
	call	gtk_list_store_append
	movq	followers(%rip), %rax
	movq	16(%rbx), %r12
	movq	%r14, %rsi
	movq	8(%rbx), %r13
	movq	16(%rax), %rdi
	call	g_type_check_instance_cast
	xorl	%edx, %edx
	movq	%rax, %rdi
	movq	%r12, %r9
	xorl	%eax, %eax
	movl	$1, %r8d
	movq	%r13, %rcx
	movq	%r15, %rsi
	movl	$-1, 16(%rsp)
	movq	%rbx, 8(%rsp)
	movl	$2, (%rsp)
	call	gtk_list_store_set
	movq	8(%rbp), %rbp
	testq	%rbp, %rbp
	jne	.L27
.L26:
	movq	followers(%rip), %rax
	movq	136(%rsp), %rsi
	movq	(%rax), %rdi
	call	g_type_check_instance_cast
	movq	80(%rax), %rdi
	xorl	%esi, %esi
	call	gdk_window_set_cursor
	movq	followers(%rip), %rax
	movl	$1, %esi
	movq	(%rax), %rdi
	call	gtk_widget_set_sensitive
	movl	$.LC11, %edi
	call	app_set_statusbar_msg
	jmp	.L28
.LFE51:
	.size	followers_dialog_show, .-followers_dialog_show
	.local	followers
	.comm	followers,8,8
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
	.long	.LFB45
	.long	.LFE45-.LFB45
	.uleb128 0x0
	.align 8
.LEFDE1:
.LSFDE3:
	.long	.LEFDE3-.LASFDE3
.LASFDE3:
	.long	.LASFDE3-.Lframe1
	.long	.LFB49
	.long	.LFE49-.LFB49
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI1-.LFB49
	.byte	0x8c
	.uleb128 0x4
	.byte	0x83
	.uleb128 0x5
	.byte	0x4
	.long	.LCFI4-.LCFI1
	.byte	0xe
	.uleb128 0x60
	.byte	0x8e
	.uleb128 0x2
	.byte	0x8d
	.uleb128 0x3
	.align 8
.LEFDE3:
.LSFDE5:
	.long	.LEFDE5-.LASFDE5
.LASFDE5:
	.long	.LASFDE5-.Lframe1
	.long	.LFB44
	.long	.LFE44-.LFB44
	.uleb128 0x0
	.align 8
.LEFDE5:
.LSFDE7:
	.long	.LEFDE7-.LASFDE7
.LASFDE7:
	.long	.LASFDE7-.Lframe1
	.long	.LFB48
	.long	.LFE48-.LFB48
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI5-.LFB48
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI6-.LCFI5
	.byte	0xe
	.uleb128 0x18
	.byte	0x83
	.uleb128 0x3
	.byte	0x86
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI7-.LCFI6
	.byte	0xe
	.uleb128 0x50
	.align 8
.LEFDE7:
.LSFDE9:
	.long	.LEFDE9-.LASFDE9
.LASFDE9:
	.long	.LASFDE9-.Lframe1
	.long	.LFB47
	.long	.LFE47-.LFB47
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI8-.LFB47
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI9-.LCFI8
	.byte	0xe
	.uleb128 0x18
	.byte	0x83
	.uleb128 0x3
	.byte	0x86
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI10-.LCFI9
	.byte	0xe
	.uleb128 0x50
	.align 8
.LEFDE9:
.LSFDE11:
	.long	.LEFDE11-.LASFDE11
.LASFDE11:
	.long	.LASFDE11-.Lframe1
	.long	.LFB46
	.long	.LFE46-.LFB46
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI11-.LFB46
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI12-.LCFI11
	.byte	0xe
	.uleb128 0x18
	.byte	0x83
	.uleb128 0x3
	.byte	0x86
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI13-.LCFI12
	.byte	0xe
	.uleb128 0x50
	.align 8
.LEFDE11:
.LSFDE13:
	.long	.LEFDE13-.LASFDE13
.LASFDE13:
	.long	.LASFDE13-.Lframe1
	.long	.LFB50
	.long	.LFE50-.LFB50
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI14-.LFB50
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI15-.LCFI14
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI16-.LCFI15
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI17-.LCFI16
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.long	.LCFI18-.LCFI17
	.byte	0xe
	.uleb128 0x30
	.byte	0x4
	.long	.LCFI19-.LCFI18
	.byte	0xe
	.uleb128 0x38
	.byte	0x83
	.uleb128 0x7
	.byte	0x86
	.uleb128 0x6
	.byte	0x8c
	.uleb128 0x5
	.byte	0x8d
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x3
	.byte	0x8f
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI20-.LCFI19
	.byte	0xe
	.uleb128 0x80
	.align 8
.LEFDE13:
.LSFDE15:
	.long	.LEFDE15-.LASFDE15
.LASFDE15:
	.long	.LASFDE15-.Lframe1
	.long	.LFB51
	.long	.LFE51-.LFB51
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI22-.LFB51
	.byte	0x86
	.uleb128 0x6
	.byte	0x83
	.uleb128 0x7
	.byte	0x4
	.long	.LCFI27-.LCFI22
	.byte	0xe
	.uleb128 0xf0
	.byte	0x8f
	.uleb128 0x2
	.byte	0x8e
	.uleb128 0x3
	.byte	0x8d
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x5
	.align 8
.LEFDE15:
	.ident	"GCC: (SUSE Linux) 4.3.2 [gcc-4_3-branch revision 141291]"
	.section	.comment.SUSE.OPTs,"MS",@progbits,1
	.ascii	"OspWg"
	.section	.note.GNU-stack,"",@progbits
