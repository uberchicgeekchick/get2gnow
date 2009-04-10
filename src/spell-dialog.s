	.file	"spell-dialog.c"
	.text
	.p2align 4,,15
	.type	spell_dialog_destroy_cb, @function
spell_dialog_destroy_cb:
.LFB42:
	pushq	%rbx
.LCFI0:
	movq	40(%rsi), %rdi
	movq	%rsi, %rbx
	call	g_free
	movq	%rbx, %rdi
	popq	%rbx
	jmp	g_free
.LFE42:
	.size	spell_dialog_destroy_cb, .-spell_dialog_destroy_cb
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"label_word"
.LC1:
	.string	"button_replace"
.LC2:
	.string	"spell_dialog"
.LC3:
	.string	"spell-dialog.ui"
.LC4:
	.string	"treeview_words"
.LC5:
	.string	"destroy"
.LC6:
	.string	"response"
.LC7:
	.string	"Suggestions for the word"
.LC8:
	.string	"%s:\n<b>%s</b>"
.LC9:
	.string	"row-activated"
.LC10:
	.string	"changed"
.LC11:
	.string	"Word"
.LC12:
	.string	"text"
.LC13:
	.string	"column"
.LC14:
	.string	"textview != NULL"
.LC15:
	.string	"word != NULL"
	.text
	.p2align 4,,15
.globl spell_dialog_show
	.type	spell_dialog_show, @function
spell_dialog_show:
.LFB44:
	pushq	%r15
.LCFI1:
	pushq	%r14
.LCFI2:
	pushq	%r13
.LCFI3:
	pushq	%r12
.LCFI4:
	movq	%rsi, %r12
	pushq	%rbp
.LCFI5:
	pushq	%rbx
.LCFI6:
	movq	%rdi, %rbx
	subq	$216, %rsp
.LCFI7:
	testq	%rdi, %rdi
	movq	344(%rsp), %rax
	movq	424(%rsp), %r13
	movl	416(%rsp), %r14d
	movq	408(%rsp), %r15
	movq	%rax, 104(%rsp)
	movl	336(%rsp), %eax
	movl	%eax, 100(%rsp)
	movq	328(%rsp), %rax
	movq	%rax, 88(%rsp)
	movq	320(%rsp), %rax
	movq	%rax, 80(%rsp)
	movq	312(%rsp), %rax
	movq	%rax, 72(%rsp)
	movq	304(%rsp), %rax
	movq	%rax, 64(%rsp)
	movq	296(%rsp), %rax
	movq	%rax, 56(%rsp)
	movq	288(%rsp), %rax
	movq	%rax, 48(%rsp)
	movq	280(%rsp), %rax
	movq	%rax, 40(%rsp)
	movq	272(%rsp), %rax
	movq	%rax, 32(%rsp)
	movq	400(%rsp), %rax
	movq	%rax, 160(%rsp)
	movq	392(%rsp), %rax
	movq	%rax, 152(%rsp)
	movq	384(%rsp), %rax
	movq	%rax, 144(%rsp)
	movq	376(%rsp), %rax
	movq	%rax, 136(%rsp)
	movq	368(%rsp), %rax
	movq	%rax, 128(%rsp)
	movq	360(%rsp), %rax
	movq	%rax, 120(%rsp)
	movq	352(%rsp), %rax
	movq	%rax, 112(%rsp)
	je	.L4
	testq	%rsi, %rsi
	je	.L5
	movl	$208, %edi
	call	g_malloc0
	movq	%r12, %rdi
	movq	%rax, %rbp
	movq	%rbx, 32(%rax)
	call	g_strdup
	movq	%rax, 40(%rbp)
	movq	104(%rsp), %rax
	leaq	8(%rbp), %r8
	movq	%r13, 200(%rbp)
	movq	%r15, 184(%rbp)
	movl	$.LC0, %r9d
	movl	%r14d, 192(%rbp)
	movl	$.LC1, %ecx
	movq	%rbp, %rdx
	movq	%rax, 120(%rbp)
	movl	100(%rsp), %eax
	movl	$.LC2, %esi
	movl	$.LC3, %edi
	movl	%eax, 112(%rbp)
	movq	88(%rsp), %rax
	movq	%rax, 104(%rbp)
	movq	80(%rsp), %rax
	movq	%rax, 96(%rbp)
	movq	72(%rsp), %rax
	movq	%rax, 88(%rbp)
	movq	64(%rsp), %rax
	movq	%rax, 80(%rbp)
	movq	56(%rsp), %rax
	movq	%rax, 72(%rbp)
	movq	48(%rsp), %rax
	movq	%rax, 64(%rbp)
	movq	40(%rsp), %rax
	movq	%rax, 56(%rbp)
	movq	32(%rsp), %rax
	movq	%rax, 48(%rbp)
	movq	160(%rsp), %rax
	movq	%rax, 176(%rbp)
	movq	152(%rsp), %rax
	movq	%rax, 168(%rbp)
	movq	144(%rsp), %rax
	movq	%rax, 160(%rbp)
	movq	136(%rsp), %rax
	movq	%rax, 152(%rbp)
	movq	128(%rsp), %rax
	movq	%rax, 144(%rbp)
	movq	120(%rsp), %rax
	movq	$0, 24(%rsp)
	movq	$.LC4, 8(%rsp)
	movq	%rax, 136(%rbp)
	movq	112(%rsp), %rax
	movq	%rax, 128(%rbp)
	leaq	24(%rbp), %rax
	movq	%rax, 16(%rsp)
	leaq	16(%rbp), %rax
	movq	%rax, (%rsp)
	xorl	%eax, %eax
	call	gtkbuilder_get_file
	movl	$.LC2, %r9d
	movl	$spell_dialog_destroy_cb, %r8d
	movl	$.LC5, %ecx
	movq	%r9, %rdx
	movq	%rax, %rbx
	movq	%rbp, %rsi
	movq	%rax, %rdi
	xorl	%eax, %eax
	movq	$0, 16(%rsp)
	movq	$spell_dialog_response_cb, 8(%rsp)
	movq	$.LC6, (%rsp)
	call	gtkbuilder_connect
	movq	%rbx, %rdi
	call	g_object_unref
	xorl	%edi, %edi
	movl	$5, %edx
	movl	$.LC7, %esi
	call	dcgettext
	movq	%r12, %rdx
	movq	%rax, %rsi
	movl	$.LC8, %edi
	xorl	%eax, %eax
	call	g_strdup_printf
	movq	%rax, %rbx
	call	gtk_label_get_type
	movq	16(%rbp), %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	movq	%rbx, %rsi
	movq	%rax, %rdi
	call	gtk_label_set_markup
	movq	%rbx, %rdi
	call	g_free
	call	gtk_tree_view_get_type
	movq	24(%rbp), %rdi
	movq	%rax, %rsi
	movq	%rax, %r13
	call	g_type_check_instance_cast
	xorl	%r9d, %r9d
	xorl	%r8d, %r8d
	movq	%rbp, %rcx
	movl	$spell_dialog_model_row_activated_cb, %edx
	movl	$.LC9, %esi
	movq	%rax, %rdi
	movq	%rax, %rbx
	call	g_signal_connect_data
	movl	$64, %esi
	movl	$1, %edi
	xorl	%eax, %eax
	call	gtk_list_store_new
	movq	%rax, 168(%rsp)
	call	gtk_tree_model_get_type
	movq	168(%rsp), %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	movq	%rbx, %rdi
	movq	%rax, %rsi
	call	gtk_tree_view_set_model
	movq	%rbx, %rdi
	call	gtk_tree_view_get_selection
	movl	$1, %esi
	movq	%rax, %rbx
	movq	%rax, %rdi
	call	gtk_tree_selection_set_mode
	xorl	%r9d, %r9d
	xorl	%r8d, %r8d
	movq	%rbp, %rcx
	movl	$spell_dialog_model_selection_changed_cb, %edx
	movq	%rbx, %rdi
	movl	$.LC10, %esi
	call	g_signal_connect_data
	movq	24(%rbp), %rdi
	movq	%r13, %rsi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_tree_view_get_model
	call	gtk_cell_renderer_text_new
	xorl	%edi, %edi
	movl	$5, %edx
	movl	$.LC11, %esi
	movq	%rax, %r12
	call	dcgettext
	movq	24(%rbp), %rdi
	movq	%rax, %rbx
	movq	%r13, %rsi
	call	g_type_check_instance_cast
	xorl	%r9d, %r9d
	movl	$.LC12, %r8d
	movq	%r12, %rcx
	movq	%rbx, %rdx
	movq	%rax, %rdi
	movl	$-1, %esi
	xorl	%eax, %eax
	movq	$0, (%rsp)
	call	gtk_tree_view_insert_column_with_attributes
	movl	$80, %esi
	movq	%r12, %rdi
	movl	%eax, %ebx
	call	g_type_check_instance_cast
	xorl	%edx, %edx
	movq	%rax, %rdi
	movl	$.LC13, %esi
	call	g_object_set_data
	movq	24(%rbp), %rdi
	movq	%r13, %rsi
	call	g_type_check_instance_cast
	leal	-1(%rbx), %esi
	movq	%rax, %rdi
	call	gtk_tree_view_get_column
	xorl	%esi, %esi
	movq	%rax, %rbx
	movq	%rax, %rdi
	call	gtk_tree_view_column_set_sort_column_id
	xorl	%esi, %esi
	movq	%rbx, %rdi
	call	gtk_tree_view_column_set_resizable
	call	gtk_tree_view_column_get_type
	movq	%rbx, %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	movl	$1, %esi
	movq	%rax, %rdi
	call	gtk_tree_view_column_set_clickable
	movq	24(%rbp), %rdi
	movq	%r13, %rsi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_tree_view_get_model
	movq	%rax, %rbx
	call	gtk_list_store_get_type
	movq	%rbx, %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	movq	40(%rbp), %rdi
	movq	%rax, %r13
	call	spell_get_suggestions
	testq	%rax, %rax
	movq	%rax, %r15
	je	.L6
	leaq	176(%rsp), %r14
	movq	%rax, %r12
	.p2align 4,,10
	.p2align 3
.L7:
	movq	(%r12), %rbx
	movq	%r14, %rsi
	movq	%r13, %rdi
	call	gtk_list_store_append
	xorl	%edx, %edx
	xorl	%eax, %eax
	movl	$-1, %r8d
	movq	%rbx, %rcx
	movq	%r14, %rsi
	movq	%r13, %rdi
	call	gtk_list_store_set
	movq	8(%r12), %r12
	testq	%r12, %r12
	jne	.L7
.L6:
	movq	%r15, %rdi
	call	spell_free_suggestions
	movq	168(%rsp), %rdi
	call	g_object_unref
	movq	(%rbp), %rdi
	call	gtk_widget_show
.L9:
	addq	$216, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	ret
	.p2align 4,,10
	.p2align 3
.L4:
	movl	$.LC14, %edx
	movl	$__PRETTY_FUNCTION__.39249, %esi
	xorl	%edi, %edi
	call	g_return_if_fail_warning
	jmp	.L9
	.p2align 4,,10
	.p2align 3
.L5:
	movl	$.LC15, %edx
	movl	$__PRETTY_FUNCTION__.39249, %esi
	xorl	%edi, %edi
	call	g_return_if_fail_warning
	jmp	.L9
.LFE44:
	.size	spell_dialog_show, .-spell_dialog_show
	.p2align 4,,15
	.type	spell_dialog_model_selection_changed_cb, @function
spell_dialog_model_selection_changed_cb:
.LFB40:
	pushq	%rbx
.LCFI8:
	movq	%rsi, %rbx
	call	gtk_tree_selection_count_selected_rows
	movq	8(%rbx), %rdi
	xorl	%esi, %esi
	cmpl	$1, %eax
	popq	%rbx
	sete	%sil
	jmp	gtk_widget_set_sensitive
.LFE40:
	.size	spell_dialog_model_selection_changed_cb, .-spell_dialog_model_selection_changed_cb
	.p2align 4,,15
	.type	spell_dialog_response_cb, @function
spell_dialog_response_cb:
.LFB43:
	movq	%rbx, -16(%rsp)
.LCFI9:
	movq	%rbp, -8(%rsp)
.LCFI10:
	subq	$232, %rsp
.LCFI11:
	cmpl	$-5, %esi
	movq	%rdx, %rbx
	je	.L18
.L15:
	movq	(%rbx), %rdi
	call	gtk_widget_destroy
.L17:
	movq	216(%rsp), %rbx
	movq	224(%rsp), %rbp
	addq	$232, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L18:
	call	gtk_tree_view_get_type
	movq	24(%rbx), %rdi
	movq	%rax, %rsi
	leaq	160(%rsp), %rbp
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_tree_view_get_selection
	leaq	200(%rsp), %rsi
	movq	%rax, %rdi
	movq	%rbp, %rdx
	call	gtk_tree_selection_get_selected
	testl	%eax, %eax
	je	.L17
	movq	200(%rsp), %rdi
	leaq	192(%rsp), %rcx
	movl	$-1, %r8d
	xorl	%edx, %edx
	movq	%rbp, %rsi
	xorl	%eax, %eax
	call	gtk_tree_model_get
	movq	128(%rbx), %rax
	movq	32(%rbx), %rdi
	movq	192(%rsp), %rsi
	movq	%rax, 80(%rsp)
	movq	136(%rbx), %rax
	movq	%rax, 88(%rsp)
	movq	144(%rbx), %rax
	movq	%rax, 96(%rsp)
	movq	152(%rbx), %rax
	movq	%rax, 104(%rsp)
	movq	160(%rbx), %rax
	movq	%rax, 112(%rsp)
	movq	168(%rbx), %rax
	movq	%rax, 120(%rsp)
	movq	176(%rbx), %rax
	movq	%rax, 128(%rsp)
	movq	184(%rbx), %rax
	movq	%rax, 136(%rsp)
	movq	192(%rbx), %rax
	movq	%rax, 144(%rsp)
	movq	200(%rbx), %rax
	movq	%rax, 152(%rsp)
	movq	48(%rbx), %rax
	movq	%rax, (%rsp)
	movq	56(%rbx), %rax
	movq	%rax, 8(%rsp)
	movq	64(%rbx), %rax
	movq	%rax, 16(%rsp)
	movq	72(%rbx), %rax
	movq	%rax, 24(%rsp)
	movq	80(%rbx), %rax
	movq	%rax, 32(%rsp)
	movq	88(%rbx), %rax
	movq	%rax, 40(%rsp)
	movq	96(%rbx), %rax
	movq	%rax, 48(%rsp)
	movq	104(%rbx), %rax
	movq	%rax, 56(%rsp)
	movq	112(%rbx), %rax
	movq	%rax, 64(%rsp)
	movq	120(%rbx), %rax
	movq	%rax, 72(%rsp)
	call	message_correct_word
	movq	192(%rsp), %rdi
	call	g_free
	jmp	.L15
.LFE43:
	.size	spell_dialog_response_cb, .-spell_dialog_response_cb
	.p2align 4,,15
	.type	spell_dialog_model_row_activated_cb, @function
spell_dialog_model_row_activated_cb:
.LFB39:
	movq	(%rcx), %rdi
	movq	%rcx, %rdx
	movl	$-5, %esi
	jmp	spell_dialog_response_cb
.LFE39:
	.size	spell_dialog_model_row_activated_cb, .-spell_dialog_model_row_activated_cb
	.section	.rodata
	.align 16
	.type	__PRETTY_FUNCTION__.39249, @object
	.size	__PRETTY_FUNCTION__.39249, 18
__PRETTY_FUNCTION__.39249:
	.string	"spell_dialog_show"
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
	.byte	0x4
	.long	.LCFI0-.LFB42
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE1:
.LSFDE3:
	.long	.LEFDE3-.LASFDE3
.LASFDE3:
	.long	.LASFDE3-.Lframe1
	.long	.LFB44
	.long	.LFE44-.LFB44
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI1-.LFB44
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI2-.LCFI1
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI3-.LCFI2
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI4-.LCFI3
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
	.long	.LCFI5-.LCFI4
	.byte	0xe
	.uleb128 0x30
	.byte	0x4
	.long	.LCFI6-.LCFI5
	.byte	0xe
	.uleb128 0x38
	.byte	0x83
	.uleb128 0x7
	.byte	0x86
	.uleb128 0x6
	.byte	0x4
	.long	.LCFI7-.LCFI6
	.byte	0xe
	.uleb128 0x110
	.align 8
.LEFDE3:
.LSFDE5:
	.long	.LEFDE5-.LASFDE5
.LASFDE5:
	.long	.LASFDE5-.Lframe1
	.long	.LFB40
	.long	.LFE40-.LFB40
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI8-.LFB40
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
	.long	.LFB43
	.long	.LFE43-.LFB43
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI11-.LFB43
	.byte	0xe
	.uleb128 0xf0
	.byte	0x86
	.uleb128 0x2
	.byte	0x83
	.uleb128 0x3
	.align 8
.LEFDE7:
.LSFDE9:
	.long	.LEFDE9-.LASFDE9
.LASFDE9:
	.long	.LASFDE9-.Lframe1
	.long	.LFB39
	.long	.LFE39-.LFB39
	.uleb128 0x0
	.align 8
.LEFDE9:
	.ident	"GCC: (SUSE Linux) 4.3.2 [gcc-4_3-branch revision 141291]"
	.section	.comment.SUSE.OPTs,"MS",@progbits,1
	.ascii	"OspWg"
	.section	.note.GNU-stack,"",@progbits
