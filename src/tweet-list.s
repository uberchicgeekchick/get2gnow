	.file	"tweet-list.c"
	.text
	.p2align 4,,15
.globl tweet_list_get_store
	.type	tweet_list_get_store, @function
tweet_list_get_store:
.LFB42:
	movq	list_priv(%rip), %rax
	movq	(%rax), %rax
	ret
.LFE42:
	.size	tweet_list_get_store, .-tweet_list_get_store
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"TweetList"
	.text
	.p2align 4,,15
.globl tweet_list_get_type
	.type	tweet_list_get_type, @function
tweet_list_get_type:
.LFB33:
	pushq	%rbx
.LCFI0:
	subq	$16, %rsp
.LCFI1:
	movq	g_define_type_id__volatile.43497(%rip), %rax
	testq	%rax, %rax
	je	.L10
.L4:
	movq	g_define_type_id__volatile.43497(%rip), %rax
	addq	$16, %rsp
	popq	%rbx
	ret
	.p2align 4,,10
	.p2align 3
.L10:
	movl	$g_define_type_id__volatile.43497, %edi
	call	g_once_init_enter_impl
	testl	%eax, %eax
	je	.L4
	movl	$.LC0, %edi
	call	g_intern_static_string
	movq	%rax, %rbx
	call	gtk_tree_view_get_type
	movq	%rbx, %rsi
	movq	%rax, %rdi
	movl	$tweet_list_init, %r9d
	movl	$120, %r8d
	movl	$tweet_list_class_intern_init, %ecx
	movl	$992, %edx
	movl	$0, (%rsp)
	call	g_type_register_static_simple
	movl	$g_define_type_id__volatile.43497, %edi
	movq	%rax, %rsi
	call	g_once_init_leave
	movq	g_define_type_id__volatile.43497(%rip), %rax
	addq	$16, %rsp
	popq	%rbx
	ret
.LFE33:
	.size	tweet_list_get_type, .-tweet_list_get_type
	.p2align 4,,15
	.type	tweet_list_class_intern_init, @function
tweet_list_class_intern_init:
.LFB32:
	pushq	%rbx
.LCFI2:
	movq	%rdi, %rbx
	call	g_type_class_peek_parent
	movq	%rbx, %rdi
	movl	$80, %esi
	movq	%rax, tweet_list_parent_class(%rip)
	call	g_type_check_class_cast
	movq	$tweet_list_finalize, 48(%rax)
	movq	%rax, %rdi
	movl	$24, %esi
	popq	%rbx
	jmp	g_type_class_add_private
.LFE32:
	.size	tweet_list_class_intern_init, .-tweet_list_class_intern_init
	.p2align 4,,15
	.type	tweet_list_finalize, @function
tweet_list_finalize:
.LFB36:
	movq	list_priv(%rip), %rax
	pushq	%rbx
.LCFI3:
	movq	%rdi, %rbx
	movq	(%rax), %rdi
	testq	%rdi, %rdi
	je	.L14
	call	g_object_unref
.L14:
	movq	tweet_list_parent_class(%rip), %rdi
	movl	$80, %esi
	call	g_type_check_class_cast
	movq	%rbx, %rdi
	movq	48(%rax), %r11
	popq	%rbx
	jmp	*%r11
.LFE36:
	.size	tweet_list_finalize, .-tweet_list_finalize
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC1:
	.string	"HotKeys: press [Return] and '@' to reply, '>' to re-tweet, [Ctrl+N] to tweet, and/or [Ctrl+D] or <Shift>+[Return] to DM."
	.align 8
.LC2:
	.string	"/apps/greet-tweet-know/ui/expand_messages"
	.text
	.p2align 4,,15
	.type	tweet_list_changed_cb, @function
tweet_list_changed_cb:
.LFB39:
	pushq	%rbx
.LCFI4:
	movq	%rdi, %rbx
	subq	$176, %rsp
.LCFI5:
	call	gtk_tree_view_get_type
	movq	%rbx, %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_tree_view_get_selection
	testq	%rax, %rax
	je	.L17
	leaq	80(%rsp), %rbx
	xorl	%esi, %esi
	movq	%rax, %rdi
	movq	%rbx, %rdx
	call	gtk_tree_selection_get_selected
	testl	%eax, %eax
	jne	.L18
.L17:
	xorl	%edi, %edi
	call	show_tweets_submenu_entries
.L21:
	addq	$176, %rsp
	popq	%rbx
	.p2align 4,,1
	.p2align 3
	ret
	.p2align 4,,10
	.p2align 3
.L18:
	movl	$1, %edi
	call	show_tweets_submenu_entries
	movl	$.LC1, %edi
	call	app_set_statusbar_msg
	call	conf_get
	leaq	172(%rsp), %rdx
	movq	%rax, %rdi
	movl	$.LC2, %esi
	call	conf_get_bool
	call	gtk_tree_model_get_type
	movq	%rax, %rsi
	movq	list_priv(%rip), %rax
	movq	(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	leaq	160(%rsp), %rax
	leaq	136(%rsp), %rcx
	leaq	120(%rsp), %r9
	movl	$4, %r8d
	movl	$2, %edx
	movq	%rax, 56(%rsp)
	leaq	152(%rsp), %rax
	movq	%rbx, %rsi
	movl	$-1, 64(%rsp)
	movl	$6, 48(%rsp)
	movq	%rax, 40(%rsp)
	leaq	144(%rsp), %rax
	movl	$0, 32(%rsp)
	movl	$5, 16(%rsp)
	movl	$3, (%rsp)
	movq	%rax, 24(%rsp)
	leaq	128(%rsp), %rax
	movq	%rax, 8(%rsp)
	xorl	%eax, %eax
	call	gtk_tree_model_get
	movq	120(%rsp), %rdx
	movq	144(%rsp), %rsi
	movq	160(%rsp), %rdi
	call	set_selected_tweet
	movl	172(%rsp), %eax
	testl	%eax, %eax
	jne	.L22
.L20:
	movq	144(%rsp), %rdi
	call	g_free
	movq	120(%rsp), %rdi
	call	g_free
	movq	128(%rsp), %rdi
	call	g_free
	movq	136(%rsp), %rdi
	call	g_free
	movq	152(%rsp), %rdi
	testq	%rdi, %rdi
	je	.L21
	call	g_object_unref
	addq	$176, %rsp
	popq	%rbx
	ret
	.p2align 4,,10
	.p2align 3
.L22:
	movq	120(%rsp), %rcx
	movq	128(%rsp), %rdx
	movq	136(%rsp), %rsi
	movq	144(%rsp), %rdi
	movq	152(%rsp), %r8
	call	app_expand_message
	jmp	.L20
.LFE39:
	.size	tweet_list_changed_cb, .-tweet_list_changed_cb
	.section	.rodata.str1.1
.LC3:
	.string	"wrap-width"
	.text
	.p2align 4,,15
	.type	tweet_list_size_cb, @function
tweet_list_size_cb:
.LFB40:
	pushq	%rbp
.LCFI6:
	movq	%rdx, %rbp
	pushq	%rbx
.LCFI7:
	subq	$8, %rsp
.LCFI8:
	movq	g_define_type_id__volatile.43497(%rip), %rax
	testq	%rax, %rax
	je	.L29
.L24:
	movq	g_define_type_id__volatile.43497(%rip), %rsi
	movq	%rbp, %rdi
	call	g_type_check_instance_cast
	movq	list_priv(%rip), %rax
	movq	8(%rax), %rdi
	call	gtk_tree_view_column_get_width
	leal	-10(%rax), %edx
	movq	list_priv(%rip), %rax
	xorl	%ecx, %ecx
	movl	$.LC3, %esi
	movq	16(%rax), %rdi
	addq	$8, %rsp
	xorl	%eax, %eax
	popq	%rbx
	popq	%rbp
	jmp	g_object_set
	.p2align 4,,10
	.p2align 3
.L29:
	movl	$g_define_type_id__volatile.43497, %edi
	call	g_once_init_enter_impl
	testl	%eax, %eax
	je	.L24
	movl	$.LC0, %edi
	call	g_intern_static_string
	movq	%rax, %rbx
	call	gtk_tree_view_get_type
	movq	%rbx, %rsi
	movq	%rax, %rdi
	movl	$tweet_list_init, %r9d
	movl	$120, %r8d
	movl	$tweet_list_class_intern_init, %ecx
	movl	$992, %edx
	movl	$0, (%rsp)
	call	g_type_register_static_simple
	movl	$g_define_type_id__volatile.43497, %edi
	movq	%rax, %rsi
	call	g_once_init_leave
	jmp	.L24
.LFE40:
	.size	tweet_list_size_cb, .-tweet_list_size_cb
	.p2align 4,,15
.globl tweet_list_new
	.type	tweet_list_new, @function
tweet_list_new:
.LFB41:
	pushq	%rbx
.LCFI9:
	subq	$16, %rsp
.LCFI10:
	movq	g_define_type_id__volatile.43497(%rip), %rax
	testq	%rax, %rax
	je	.L36
.L31:
	movq	g_define_type_id__volatile.43497(%rip), %rdi
	addq	$16, %rsp
	xorl	%esi, %esi
	popq	%rbx
	xorl	%eax, %eax
	jmp	g_object_new
	.p2align 4,,10
	.p2align 3
.L36:
	movl	$g_define_type_id__volatile.43497, %edi
	call	g_once_init_enter_impl
	testl	%eax, %eax
	je	.L31
	movl	$.LC0, %edi
	call	g_intern_static_string
	movq	%rax, %rbx
	call	gtk_tree_view_get_type
	movq	%rbx, %rsi
	movq	%rax, %rdi
	movl	$tweet_list_init, %r9d
	movl	$120, %r8d
	movl	$tweet_list_class_intern_init, %ecx
	movl	$992, %edx
	movl	$0, (%rsp)
	call	g_type_register_static_simple
	movl	$g_define_type_id__volatile.43497, %edi
	movq	%rax, %rsi
	call	g_once_init_leave
	jmp	.L31
.LFE41:
	.size	tweet_list_new, .-tweet_list_new
	.section	.rodata.str1.1
.LC4:
	.string	"headers-visible"
.LC5:
	.string	"reorderable"
.LC6:
	.string	"rules-hint"
.LC7:
	.string	"pixbuf"
.LC8:
	.string	"markup"
.LC9:
	.string	"yalign"
.LC10:
	.string	"xpad"
.LC11:
	.string	"ypad"
.LC12:
	.string	"wrap-mode"
.LC14:
	.string	"size_allocate"
.LC15:
	.string	"cursor-changed"
.LC16:
	.string	"row-activated"
	.text
	.p2align 4,,15
	.type	tweet_list_init, @function
tweet_list_init:
.LFB35:
	pushq	%r13
.LCFI11:
	pushq	%r12
.LCFI12:
	pushq	%rbp
.LCFI13:
	pushq	%rbx
.LCFI14:
	subq	$24, %rsp
.LCFI15:
	movq	g_define_type_id__volatile.43497(%rip), %rax
	movq	%rdi, list(%rip)
	testq	%rax, %rax
	je	.L38
.L43:
	call	gtk_tree_view_get_type
	movq	%rax, %rbp
.L39:
	movq	list(%rip), %rdi
	movq	g_define_type_id__volatile.43497(%rip), %rsi
	call	g_type_instance_get_private
	movq	(%rax), %rdi
	movq	%rax, list_priv(%rip)
	movq	list(%rip), %r12
	testq	%rdi, %rdi
	je	.L40
	call	g_object_unref
.L40:
	movq	list_priv(%rip), %rbx
	call	gdk_pixbuf_get_type
	movl	$64, %r9d
	movl	$64, %r8d
	movl	$64, %ecx
	movl	$64, %edx
	movq	%rax, %rsi
	movl	$7, %edi
	xorl	%eax, %eax
	movq	$36, 8(%rsp)
	movq	$64, (%rsp)
	call	gtk_list_store_new
	movq	%rax, (%rbx)
	call	gtk_tree_model_get_type
	movq	%rax, %rsi
	movq	list_priv(%rip), %rax
	movq	(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%rbp, %rsi
	movq	%rax, %rbx
	movq	%r12, %rdi
	call	g_type_check_instance_cast
	movq	%rbx, %rsi
	movq	%rax, %rdi
	call	gtk_tree_view_set_model
	movq	list(%rip), %r13
	movl	$.LC4, %r9d
	xorl	%r8d, %r8d
	movl	$.LC5, %ecx
	movl	$1, %edx
	movl	$.LC6, %esi
	xorl	%eax, %eax
	movq	$0, 8(%rsp)
	movl	$0, (%rsp)
	movq	%r13, %rdi
	call	g_object_set
	call	gtk_cell_renderer_pixbuf_new
	movl	$48, %edx
	movq	%rax, %rbx
	movq	%rax, %rdi
	movl	$48, %esi
	call	gtk_cell_renderer_set_fixed_size
	xorl	%r8d, %r8d
	xorl	%ecx, %ecx
	movl	$.LC7, %edx
	movq	%rbx, %rsi
	xorl	%edi, %edi
	xorl	%eax, %eax
	call	gtk_tree_view_column_new_with_attributes
	movl	$2, %esi
	movq	%rax, %rbx
	movq	%rax, %rdi
	call	gtk_tree_view_column_set_sizing
	movq	%rbx, %rdi
	movl	$48, %esi
	call	gtk_tree_view_column_set_min_width
	movq	%rbx, %rdi
	movl	$48, %esi
	call	gtk_tree_view_column_set_fixed_width
	movq	%rbp, %rsi
	movq	%r13, %rdi
	call	g_type_check_instance_cast
	movq	%rbx, %rsi
	movq	%rax, %rdi
	call	gtk_tree_view_append_column
	call	gtk_cell_renderer_text_new
	xorl	%r8d, %r8d
	movq	%rax, %r12
	movl	$1, %ecx
	movl	$.LC8, %edx
	movq	%rax, %rsi
	xorl	%edi, %edi
	xorl	%eax, %eax
	call	gtk_tree_view_column_new_with_attributes
	movl	$2, %esi
	movq	%rax, %rdi
	movq	%rax, %rbx
	call	gtk_tree_view_column_set_sizing
	xorpd	%xmm0, %xmm0
	xorl	%edx, %edx
	movl	$.LC9, %r9d
	movl	$5, %r8d
	movl	$.LC10, %ecx
	movq	%r12, %rdi
	movl	$.LC11, %esi
	movl	$1, %eax
	movq	$0, 16(%rsp)
	movl	$2, 8(%rsp)
	movq	$.LC12, (%rsp)
	call	g_object_set
	movq	%rbp, %rsi
	movq	%r13, %rdi
	call	g_type_check_instance_cast
	movq	%rbx, %rsi
	movq	%rax, %rdi
	call	gtk_tree_view_append_column
	movq	list_priv(%rip), %rax
	movq	list(%rip), %rdi
	xorl	%r9d, %r9d
	movl	$tweet_list_size_cb, %edx
	xorl	%r8d, %r8d
	movl	$.LC14, %esi
	movq	%rdi, %rcx
	movq	%rbx, 8(%rax)
	movq	%r12, 16(%rax)
	call	g_signal_connect_data
	movq	list(%rip), %rdi
	xorl	%r9d, %r9d
	xorl	%r8d, %r8d
	movl	$tweet_list_changed_cb, %edx
	movl	$.LC15, %esi
	movq	%rdi, %rcx
	call	g_signal_connect_data
	movq	list(%rip), %rdi
	addq	$24, %rsp
	xorl	%r9d, %r9d
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	xorl	%r8d, %r8d
	movq	%rdi, %rcx
	movl	$tweets_reply, %edx
	movl	$.LC16, %esi
	jmp	g_signal_connect_data
	.p2align 4,,10
	.p2align 3
.L38:
	movl	$g_define_type_id__volatile.43497, %edi
	call	g_once_init_enter_impl
	testl	%eax, %eax
	je	.L43
	movl	$.LC0, %edi
	call	g_intern_static_string
	movq	%rax, %rbx
	call	gtk_tree_view_get_type
	movq	%rbx, %rsi
	movl	$tweet_list_init, %r9d
	movl	$120, %r8d
	movl	$tweet_list_class_intern_init, %ecx
	movl	$992, %edx
	movq	%rax, %rdi
	movq	%rax, %rbp
	movl	$0, (%rsp)
	call	g_type_register_static_simple
	movl	$g_define_type_id__volatile.43497, %edi
	movq	%rax, %rsi
	call	g_once_init_leave
	jmp	.L39
.LFE35:
	.size	tweet_list_init, .-tweet_list_init
	.local	list_priv
	.comm	list_priv,8,8
	.local	g_define_type_id__volatile.43497
	.comm	g_define_type_id__volatile.43497,8,8
	.local	tweet_list_parent_class
	.comm	tweet_list_parent_class,8,8
	.local	list
	.comm	list,8,8
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
	.long	.LFB42
	.long	.LFE42-.LFB42
	.uleb128 0x0
	.align 8
.LEFDE1:
.LSFDE3:
	.long	.LEFDE3-.LASFDE3
.LASFDE3:
	.long	.LASFDE3-.Lframe1
	.long	.LFB33
	.long	.LFE33-.LFB33
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI0-.LFB33
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI1-.LCFI0
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE3:
.LSFDE5:
	.long	.LEFDE5-.LASFDE5
.LASFDE5:
	.long	.LASFDE5-.Lframe1
	.long	.LFB32
	.long	.LFE32-.LFB32
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI2-.LFB32
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE5:
.LSFDE7:
	.long	.LEFDE7-.LASFDE7
.LASFDE7:
	.long	.LASFDE7-.Lframe1
	.long	.LFB36
	.long	.LFE36-.LFB36
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI3-.LFB36
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE7:
.LSFDE9:
	.long	.LEFDE9-.LASFDE9
.LASFDE9:
	.long	.LASFDE9-.Lframe1
	.long	.LFB39
	.long	.LFE39-.LFB39
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI4-.LFB39
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI5-.LCFI4
	.byte	0xe
	.uleb128 0xc0
	.align 8
.LEFDE9:
.LSFDE11:
	.long	.LEFDE11-.LASFDE11
.LASFDE11:
	.long	.LASFDE11-.Lframe1
	.long	.LFB40
	.long	.LFE40-.LFB40
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI6-.LFB40
	.byte	0xe
	.uleb128 0x10
	.byte	0x86
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI7-.LCFI6
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI8-.LCFI7
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x3
	.align 8
.LEFDE11:
.LSFDE13:
	.long	.LEFDE13-.LASFDE13
.LASFDE13:
	.long	.LASFDE13-.Lframe1
	.long	.LFB41
	.long	.LFE41-.LFB41
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI9-.LFB41
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI10-.LCFI9
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE13:
.LSFDE15:
	.long	.LEFDE15-.LASFDE15
.LASFDE15:
	.long	.LASFDE15-.Lframe1
	.long	.LFB35
	.long	.LFE35-.LFB35
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI11-.LFB35
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI12-.LCFI11
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI13-.LCFI12
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI14-.LCFI13
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.long	.LCFI15-.LCFI14
	.byte	0xe
	.uleb128 0x40
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x8d
	.uleb128 0x2
	.align 8
.LEFDE15:
	.ident	"GCC: (SUSE Linux) 4.3.2 [gcc-4_3-branch revision 141291]"
	.section	.comment.SUSE.OPTs,"MS",@progbits,1
	.ascii	"OspWg"
	.section	.note.GNU-stack,"",@progbits
