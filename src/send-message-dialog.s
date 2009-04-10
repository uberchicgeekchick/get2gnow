	.file	"send-message-dialog.c"
	.text
	.p2align 4,,15
.globl message_set_message
	.type	message_set_message, @function
message_set_message:
.LFB54:
	pushq	%r12
.LCFI0:
	pushq	%rbx
.LCFI1:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI2:
	call	gtk_text_view_get_type
	movq	%rax, %rsi
	movq	dialog_priv(%rip), %rax
	movq	8(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_text_view_get_buffer
	movq	%rbx, %rsi
	movq	%rax, %rdi
	movl	$-1, %edx
	call	gtk_text_buffer_set_text
	movq	dialog_priv(%rip), %rbx
	call	gtk_window_get_type
	movq	%rax, %rsi
	movq	(%rbx), %rdi
	movq	8(%rbx), %r12
	call	g_type_check_instance_cast
	addq	$8, %rsp
	movq	%r12, %rsi
	movq	%rax, %rdi
	popq	%rbx
	popq	%r12
	jmp	gtk_window_set_focus
.LFE54:
	.size	message_set_message, .-message_set_message
	.p2align 4,,15
.globl message_set_followers
	.type	message_set_followers, @function
message_set_followers:
.LFB52:
	pushq	%r13
.LCFI3:
	pushq	%r12
.LCFI4:
	pushq	%rbp
.LCFI5:
	movq	%rdi, %rbp
	pushq	%rbx
.LCFI6:
	subq	$40, %rsp
.LCFI7:
	call	gtk_list_store_get_type
	movq	%rax, %rbx
	call	gtk_combo_box_get_type
	movq	%rax, %rsi
	movq	dialog_priv(%rip), %rax
	movq	32(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_combo_box_get_model
	movq	%rbx, %rsi
	movq	%rax, %rdi
	call	g_type_check_instance_cast
	testq	%rbp, %rbp
	movq	%rax, %r12
	je	.L6
	.p2align 4,,10
	.p2align 3
.L5:
	movq	(%rbp), %rbx
	movq	%rsp, %rsi
	movq	%r12, %rdi
	call	gtk_list_store_append
	xorl	%edx, %edx
	xorl	%eax, %eax
	movl	$-1, %r8d
	movq	8(%rbx), %rcx
	movq	%rsp, %rsi
	movq	%r12, %rdi
	call	gtk_list_store_set
	movq	8(%rbp), %rbp
	testq	%rbp, %rbp
	jne	.L5
.L6:
	addq	$40, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.LFE52:
	.size	message_set_followers, .-message_set_followers
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"textview != NULL"
.LC1:
	.string	"new_word != NULL"
	.text
	.p2align 4,,15
.globl message_correct_word
	.type	message_correct_word, @function
message_correct_word:
.LFB51:
	pushq	%rbp
.LCFI8:
	movq	%rsi, %rbp
	pushq	%rbx
.LCFI9:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI10:
	testq	%rdi, %rdi
	je	.L10
	testq	%rsi, %rsi
	je	.L11
	call	gtk_text_view_get_type
	movq	%rbx, %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_text_view_get_buffer
	leaq	112(%rsp), %rdx
	leaq	32(%rsp), %rsi
	movq	%rax, %rdi
	movq	%rax, %rbx
	call	gtk_text_buffer_delete
	leaq	32(%rsp), %rsi
	movq	%rbp, %rdx
	movq	%rbx, %rdi
	movl	$-1, %ecx
	call	gtk_text_buffer_insert
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	ret
	.p2align 4,,10
	.p2align 3
.L10:
	movl	$.LC0, %edx
	movl	$__PRETTY_FUNCTION__.44147, %esi
	xorl	%edi, %edi
	call	g_return_if_fail_warning
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	ret
	.p2align 4,,10
	.p2align 3
.L11:
	movl	$.LC1, %edx
	movl	$__PRETTY_FUNCTION__.44147, %esi
	xorl	%edi, %edi
	call	g_return_if_fail_warning
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	ret
.LFE51:
	.size	message_correct_word, .-message_correct_word
	.p2align 4,,15
	.type	message_spell_free, @function
message_spell_free:
.LFB63:
	pushq	%rbx
.LCFI11:
	movq	%rdi, %rbx
	movq	8(%rdi), %rdi
	testq	%rdi, %rdi
	je	.L15
	call	g_free
.L15:
	movq	%rbx, %rdi
	popq	%rbx
	jmp	g_free
.LFE63:
	.size	message_spell_free, .-message_spell_free
	.p2align 4,,15
	.type	message_text_buffer_undo_cb, @function
message_text_buffer_undo_cb:
.LFB57:
	pushq	%rbx
.LCFI12:
	movq	%rdi, %rbx
	call	gtk_text_buffer_end_user_action
	movq	%rbx, %rdi
	popq	%rbx
	jmp	gtk_text_buffer_begin_user_action
.LFE57:
	.size	message_text_buffer_undo_cb, .-message_text_buffer_undo_cb
	.p2align 4,,15
	.type	message_destroy_cb, @function
message_destroy_cb:
.LFB65:
	subq	$8, %rsp
.LCFI13:
	movq	undo_buffer(%rip), %rdi
	call	gtk_text_buffer_end_user_action
	movq	dialog(%rip), %rdi
	movq	$0, undo_buffer(%rip)
	call	g_object_unref
	movq	$0, dialog(%rip)
	addq	$8, %rsp
	ret
.LFE65:
	.size	message_destroy_cb, .-message_destroy_cb
	.section	.rodata.str1.1
.LC2:
	.string	"Characters Available"
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC3:
	.string	"<span size=\"small\">%s: <span foreground=\"red\">%i</span></span>"
	.align 8
.LC4:
	.string	"<span size=\"small\">%s: %i</span>"
	.align 8
.LC5:
	.string	"/apps/greet-tweet-know/ui/spell"
	.section	.rodata.str1.1
.LC6:
	.string	"misspelled"
	.text
	.p2align 4,,15
	.type	message_text_buffer_changed_cb, @function
message_text_buffer_changed_cb:
.LFB58:
	pushq	%r13
.LCFI14:
	pushq	%r12
.LCFI15:
	movq	%rdi, %r12
	pushq	%rbp
.LCFI16:
	movl	$140, %ebp
	pushq	%rbx
.LCFI17:
	subq	$184, %rsp
.LCFI18:
	movl	$0, 172(%rsp)
	call	gtk_text_buffer_get_char_count
	movl	$.LC2, %esi
	movl	%eax, %ebx
	xorl	%edi, %edi
	movl	$5, %edx
	subl	%eax, %ebp
	call	dcgettext
	cmpl	$140, %ebx
	movq	%rax, %rsi
	jg	.L32
	movl	$.LC4, %edi
	movl	%ebp, %edx
	xorl	%eax, %eax
	call	g_markup_printf_escaped
	movq	%rax, %rbx
	movq	dialog_priv(%rip), %rax
	movl	$1, %esi
	movq	24(%rax), %rdi
	call	gtk_widget_set_sensitive
.L23:
	call	gtk_label_get_type
	movq	%rax, %rsi
	movq	dialog_priv(%rip), %rax
	leaq	80(%rsp), %rbp
	movq	16(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%rbx, %rsi
	movq	%rax, %rdi
	call	gtk_label_set_markup
	movq	%rbx, %rdi
	call	g_free
	call	conf_get
	leaq	172(%rsp), %rdx
	movq	%rax, %rdi
	movl	$.LC5, %esi
	call	conf_get_bool
	movq	%rbp, %rsi
	movq	%r12, %rdi
	call	gtk_text_buffer_get_start_iter
	movl	172(%rsp), %eax
	testl	%eax, %eax
	je	.L33
	call	spell_supported
	testl	%eax, %eax
	jne	.L30
.L31:
	addq	$184, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
	.p2align 4,,10
	.p2align 3
.L34:
	movq	80(%rsp), %rax
	movq	%rsp, %rdi
	movq	%rax, (%rsp)
	movq	88(%rsp), %rax
	movq	%rax, 8(%rsp)
	movq	96(%rsp), %rax
	movq	%rax, 16(%rsp)
	movq	104(%rsp), %rax
	movq	%rax, 24(%rsp)
	movq	112(%rsp), %rax
	movq	%rax, 32(%rsp)
	movq	120(%rsp), %rax
	movq	%rax, 40(%rsp)
	movq	128(%rsp), %rax
	movq	%rax, 48(%rsp)
	movq	136(%rsp), %rax
	movq	%rax, 56(%rsp)
	movq	144(%rsp), %rax
	movq	%rax, 64(%rsp)
	movq	152(%rsp), %rax
	movq	%rax, 72(%rsp)
	call	gtk_text_iter_forward_word_end
	testl	%eax, %eax
	je	.L31
.L27:
	xorl	%ecx, %ecx
	movq	%rsp, %rdx
	movq	%rbp, %rsi
	movq	%r12, %rdi
	call	gtk_text_buffer_get_text
	movq	undo_buffer(%rip), %rdi
	movq	%rax, %rbx
	call	gtk_text_buffer_begin_user_action
	movq	%rbx, %rdi
	call	spell_check
	testl	%eax, %eax
	jne	.L28
	movq	%rsp, %rcx
	movq	%rbp, %rdx
	movl	$.LC6, %esi
	movq	%r12, %rdi
	call	gtk_text_buffer_apply_tag_by_name
.L29:
	movq	%rbx, %rdi
	call	g_free
	movq	(%rsp), %rax
	movq	%rax, 80(%rsp)
	movq	8(%rsp), %rax
	movq	%rax, 88(%rsp)
	movq	16(%rsp), %rax
	movq	%rax, 96(%rsp)
	movq	24(%rsp), %rax
	movq	%rax, 104(%rsp)
	movq	32(%rsp), %rax
	movq	%rax, 112(%rsp)
	movq	40(%rsp), %rax
	movq	%rax, 120(%rsp)
	movq	48(%rsp), %rax
	movq	%rax, 128(%rsp)
	movq	56(%rsp), %rax
	movq	%rax, 136(%rsp)
	movq	64(%rsp), %rax
	movq	%rax, 144(%rsp)
	movq	72(%rsp), %rax
	movq	%rax, 152(%rsp)
.L30:
	movq	%rbp, %rdi
	call	gtk_text_iter_is_start
	testl	%eax, %eax
	jne	.L34
	movq	%rsp, %rdi
	call	gtk_text_iter_forward_word_end
	testl	%eax, %eax
	.p2align 4,,2
	.p2align 3
	je	.L31
	movq	(%rsp), %rax
	movq	%rbp, %rdi
	movq	%rax, 80(%rsp)
	movq	8(%rsp), %rax
	movq	%rax, 88(%rsp)
	movq	16(%rsp), %rax
	movq	%rax, 96(%rsp)
	movq	24(%rsp), %rax
	movq	%rax, 104(%rsp)
	movq	32(%rsp), %rax
	movq	%rax, 112(%rsp)
	movq	40(%rsp), %rax
	movq	%rax, 120(%rsp)
	movq	48(%rsp), %rax
	movq	%rax, 128(%rsp)
	movq	56(%rsp), %rax
	movq	%rax, 136(%rsp)
	movq	64(%rsp), %rax
	movq	%rax, 144(%rsp)
	movq	72(%rsp), %rax
	movq	%rax, 152(%rsp)
	call	gtk_text_iter_backward_word_start
	jmp	.L27
	.p2align 4,,10
	.p2align 3
.L28:
	movq	%rsp, %rcx
	movq	%rbp, %rdx
	movl	$.LC6, %esi
	movq	%r12, %rdi
	call	gtk_text_buffer_remove_tag_by_name
	jmp	.L29
	.p2align 4,,10
	.p2align 3
.L32:
	movl	%ebp, %edx
	movl	$.LC3, %edi
	xorl	%eax, %eax
	call	g_markup_printf_escaped
	movq	%rax, %rbx
	movq	dialog_priv(%rip), %rax
	xorl	%esi, %esi
	movq	24(%rax), %rdi
	call	gtk_widget_set_sensitive
	jmp	.L23
	.p2align 4,,10
	.p2align 3
.L33:
	movq	%rsp, %rsi
	movq	%r12, %rdi
	call	gtk_text_buffer_get_end_iter
	movq	%rsp, %rcx
	movq	%rbp, %rdx
	movq	%r12, %rdi
	movl	$.LC6, %esi
	call	gtk_text_buffer_remove_tag_by_name
	addq	$184, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.LFE58:
	.size	message_text_buffer_changed_cb, .-message_text_buffer_changed_cb
	.section	.rodata.str1.1
.LC7:
	.string	"message_spell"
.LC8:
	.string	"_Check Word Spelling..."
.LC9:
	.string	"activate"
	.text
	.p2align 4,,15
	.type	message_text_populate_popup_cb, @function
message_text_populate_popup_cb:
.LFB60:
	pushq	%r15
.LCFI19:
	pushq	%r14
.LCFI20:
	pushq	%r13
.LCFI21:
	pushq	%r12
.LCFI22:
	pushq	%rbp
.LCFI23:
	pushq	%rbx
.LCFI24:
	subq	$552, %rsp
.LCFI25:
	movq	%rsi, 128(%rsp)
	call	gtk_text_view_get_type
	movq	%rax, %rsi
	movq	dialog_priv(%rip), %rax
	movq	8(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_text_view_get_buffer
	movq	%rax, %rdi
	movq	%rax, %r12
	call	gtk_text_buffer_get_tag_table
	movl	$.LC6, %esi
	movq	%rax, %rdi
	call	gtk_text_tag_table_lookup
	movq	%r12, %rdi
	movq	%rax, %rbp
	call	gtk_text_buffer_get_insert
	leaq	464(%rsp), %rsi
	movq	%rax, %rdx
	movq	%r12, %rdi
	call	gtk_text_buffer_get_iter_at_mark
	movq	464(%rsp), %rbx
	movq	504(%rsp), %rdi
	movq	512(%rsp), %rsi
	movq	472(%rsp), %r11
	movq	480(%rsp), %r10
	movq	488(%rsp), %r9
	movq	496(%rsp), %r8
	movq	520(%rsp), %rcx
	movq	528(%rsp), %rdx
	movq	536(%rsp), %rax
	movq	%rbx, 304(%rsp)
	movq	%rbx, 384(%rsp)
	leaq	384(%rsp), %rbx
	movq	%rdi, 344(%rsp)
	movq	%rsi, 352(%rsp)
	movq	%rdi, 424(%rsp)
	movq	%rsi, 432(%rsp)
	movq	%rbx, %rdi
	movq	%rbp, %rsi
	movq	%r11, 312(%rsp)
	movq	%r10, 320(%rsp)
	movq	%r9, 328(%rsp)
	movq	%r8, 336(%rsp)
	movq	%rcx, 360(%rsp)
	movq	%rdx, 368(%rsp)
	movq	%rax, 376(%rsp)
	movq	%r11, 392(%rsp)
	movq	%r10, 400(%rsp)
	movq	%r9, 408(%rsp)
	movq	%r8, 416(%rsp)
	movq	%rcx, 440(%rsp)
	movq	%rdx, 448(%rsp)
	movq	%rax, 456(%rsp)
	call	gtk_text_iter_backward_to_tag_toggle
	testl	%eax, %eax
	jne	.L38
.L37:
	addq	$552, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	ret
	.p2align 4,,10
	.p2align 3
.L38:
	leaq	304(%rsp), %r13
	movq	%rbp, %rsi
	movq	%r13, %rdi
	call	gtk_text_iter_forward_to_tag_toggle
	testl	%eax, %eax
	je	.L37
	xorl	%ecx, %ecx
	movq	%r13, %rdx
	movq	%rbx, %rsi
	movq	%r12, %rdi
	call	gtk_text_buffer_get_text
	testq	%rax, %rax
	movq	%rax, %rbp
	je	.L37
	cmpb	$0, (%rax)
	je	.L37
	movq	dialog_priv(%rip), %rax
	movq	448(%rsp), %r13
	movl	$176, %edi
	movq	8(%rax), %rax
	movq	%r13, 288(%rsp)
	movq	%rax, 136(%rsp)
	movq	384(%rsp), %rax
	movq	%rax, 120(%rsp)
	movq	%rax, 224(%rsp)
	movq	392(%rsp), %rax
	movq	%rax, 112(%rsp)
	movq	%rax, 232(%rsp)
	movq	400(%rsp), %rax
	movq	%rax, 104(%rsp)
	movq	%rax, 240(%rsp)
	movq	408(%rsp), %rax
	movq	%rax, 96(%rsp)
	movq	%rax, 248(%rsp)
	movq	416(%rsp), %rax
	movq	%rax, 88(%rsp)
	movq	%rax, 256(%rsp)
	movq	424(%rsp), %rax
	movq	%rax, 80(%rsp)
	movq	%rax, 264(%rsp)
	movq	432(%rsp), %rax
	movq	%rax, 72(%rsp)
	movq	%rax, 272(%rsp)
	movq	440(%rsp), %rax
	movq	%rax, 64(%rsp)
	movq	%rax, 280(%rsp)
	movq	456(%rsp), %rax
	movq	%rax, 56(%rsp)
	movq	%rax, 296(%rsp)
	movq	304(%rsp), %rax
	movq	%rax, 48(%rsp)
	movq	%rax, 144(%rsp)
	movq	312(%rsp), %rax
	movq	368(%rsp), %r12
	movq	360(%rsp), %r15
	movq	376(%rsp), %r14
	movq	%rax, 40(%rsp)
	movq	%rax, 152(%rsp)
	movq	320(%rsp), %rax
	movq	%r12, 208(%rsp)
	movq	%r15, 200(%rsp)
	movq	%r14, 216(%rsp)
	movq	%rax, 32(%rsp)
	movq	%rax, 160(%rsp)
	movq	328(%rsp), %rax
	movq	%rax, 24(%rsp)
	movq	%rax, 168(%rsp)
	movq	336(%rsp), %rax
	movq	%rax, 16(%rsp)
	movq	%rax, 176(%rsp)
	movq	344(%rsp), %rax
	movq	%rax, 8(%rsp)
	movq	%rax, 184(%rsp)
	movq	352(%rsp), %rax
	movq	%rax, (%rsp)
	movq	%rax, 192(%rsp)
	call	g_malloc0
	movq	%rax, %rbx
	movq	136(%rsp), %rax
	movq	%rbp, %rdi
	movq	%rax, (%rbx)
	call	g_strdup
	movq	%rax, 8(%rbx)
	movq	56(%rsp), %rax
	movl	$80, %esi
	movl	%r13d, 80(%rbx)
	movl	%r12d, 160(%rbx)
	movq	%r14, 168(%rbx)
	movq	%rax, 88(%rbx)
	movq	64(%rsp), %rax
	movq	%rax, 72(%rbx)
	movq	72(%rsp), %rax
	movq	%rax, 64(%rbx)
	movq	80(%rsp), %rax
	movq	%rax, 56(%rbx)
	movq	88(%rsp), %rax
	movq	%rax, 48(%rbx)
	movq	96(%rsp), %rax
	movq	%rax, 40(%rbx)
	movq	104(%rsp), %rax
	movq	%rax, 32(%rbx)
	movq	112(%rsp), %rax
	movq	%rax, 24(%rbx)
	movq	120(%rsp), %rax
	movq	%r15, 152(%rbx)
	movq	%rax, 16(%rbx)
	movq	(%rsp), %rax
	movq	%rax, 144(%rbx)
	movq	8(%rsp), %rax
	movq	%rax, 136(%rbx)
	movq	16(%rsp), %rax
	movq	%rax, 128(%rbx)
	movq	24(%rsp), %rax
	movq	%rax, 120(%rbx)
	movq	32(%rsp), %rax
	movq	%rax, 112(%rbx)
	movq	40(%rsp), %rax
	movq	%rax, 104(%rbx)
	movq	48(%rsp), %rax
	movq	128(%rsp), %rdi
	movq	%rax, 96(%rbx)
	call	g_type_check_instance_cast
	movl	$message_spell_free, %ecx
	movq	%rbx, %rdx
	movq	%rax, %rdi
	movl	$.LC7, %esi
	call	g_object_set_data_full
	call	gtk_separator_menu_item_new
	movq	%rax, %r12
	call	gtk_menu_shell_get_type
	movq	128(%rsp), %rdi
	movq	%rax, %rsi
	movq	%rax, %r13
	call	g_type_check_instance_cast
	movq	%r12, %rsi
	movq	%rax, %rdi
	call	gtk_menu_shell_prepend
	movq	%r12, %rdi
	call	gtk_widget_show
	movl	$5, %edx
	movl	$.LC8, %esi
	xorl	%edi, %edi
	call	dcgettext
	movq	%rax, %rdi
	call	gtk_menu_item_new_with_mnemonic
	xorl	%r9d, %r9d
	xorl	%r8d, %r8d
	movq	%rbx, %rcx
	movl	$message_text_check_word_spelling_cb, %edx
	movl	$.LC9, %esi
	movq	%rax, %rdi
	movq	%rax, %r12
	call	g_signal_connect_data
	movq	128(%rsp), %rdi
	movq	%r13, %rsi
	call	g_type_check_instance_cast
	movq	%r12, %rsi
	movq	%rax, %rdi
	call	gtk_menu_shell_prepend
	movq	%r12, %rdi
	call	gtk_widget_show
	jmp	.L37
.LFE60:
	.size	message_text_populate_popup_cb, .-message_text_populate_popup_cb
	.p2align 4,,15
	.type	message_text_check_word_spelling_cb, @function
message_text_check_word_spelling_cb:
.LFB61:
	movq	%rsi, %rax
	subq	$168, %rsp
.LCFI26:
	movq	8(%rsi), %rsi
	movq	96(%rax), %rdx
	movq	%rdx, 80(%rsp)
	movq	104(%rax), %rdx
	movq	%rdx, 88(%rsp)
	movq	112(%rax), %rdx
	movq	%rdx, 96(%rsp)
	movq	120(%rax), %rdx
	movq	%rdx, 104(%rsp)
	movq	128(%rax), %rdx
	movq	%rdx, 112(%rsp)
	movq	136(%rax), %rdx
	movq	%rdx, 120(%rsp)
	movq	144(%rax), %rdx
	movq	%rdx, 128(%rsp)
	movq	152(%rax), %rdx
	movq	%rdx, 136(%rsp)
	movq	160(%rax), %rdx
	movq	%rdx, 144(%rsp)
	movq	168(%rax), %rdx
	movq	%rdx, 152(%rsp)
	movq	16(%rax), %rdx
	movq	%rdx, (%rsp)
	movq	24(%rax), %rdx
	movq	%rdx, 8(%rsp)
	movq	32(%rax), %rdx
	movq	%rdx, 16(%rsp)
	movq	40(%rax), %rdx
	movq	%rdx, 24(%rsp)
	movq	48(%rax), %rdx
	movq	%rdx, 32(%rsp)
	movq	56(%rax), %rdx
	movq	%rdx, 40(%rsp)
	movq	64(%rax), %rdx
	movq	%rdx, 48(%rsp)
	movq	72(%rax), %rdx
	movq	%rdx, 56(%rsp)
	movq	80(%rax), %rdx
	movq	%rdx, 64(%rsp)
	movq	88(%rax), %rdx
	movq	%rdx, 72(%rsp)
	movq	(%rax), %rdi
	call	spell_dialog_show
	addq	$168, %rsp
	ret
.LFE61:
	.size	message_text_check_word_spelling_cb, .-message_text_check_word_spelling_cb
	.section	.rodata.str1.1
.LC10:
	.string	"end-user-action"
.LC11:
	.string	"populate-popup"
	.text
	.p2align 4,,15
	.type	message_text_button_release_cb, @function
message_text_button_release_cb:
.LFB59:
	movq	%rbx, -48(%rsp)
.LCFI27:
	movq	%rbp, -40(%rsp)
.LCFI28:
	movq	%rsi, %rbx
	movq	%r15, -8(%rsp)
.LCFI29:
	movq	%r12, -32(%rsp)
.LCFI30:
	movq	%rdi, %r15
	movq	%r13, -24(%rsp)
.LCFI31:
	movq	%r14, -16(%rsp)
.LCFI32:
	subq	$168, %rsp
.LCFI33:
	call	gtk_text_view_get_type
	movq	%rax, %rbp
	movq	dialog_priv(%rip), %rax
	movq	%rbp, %rsi
	movq	8(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_text_view_get_buffer
	cmpl	$3, 52(%rbx)
	movq	%rax, 8(%rsp)
	je	.L42
	movq	%rax, %rdi
	movl	$.LC10, %esi
	xorl	%eax, %eax
	call	g_signal_emit_by_name
.L44:
	movq	120(%rsp), %rbx
	movq	128(%rsp), %rbp
	movq	136(%rsp), %r12
	movq	144(%rsp), %r13
	movq	152(%rsp), %r14
	movq	160(%rsp), %r15
	addq	$168, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L42:
	leaq	104(%rsp), %rbx
	leaq	108(%rsp), %r12
	call	gtk_widget_get_type
	movq	%r15, %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	movq	%rbx, %rdx
	movq	%rax, %rdi
	movq	%r12, %rsi
	call	gtk_widget_get_pointer
	movl	104(%rsp), %r13d
	movl	108(%rsp), %r14d
	movq	%rbp, %rsi
	movq	%r15, %rdi
	call	g_type_check_instance_cast
	movq	%rbx, %r9
	movq	%r12, %r8
	movl	%r13d, %ecx
	movl	%r14d, %edx
	movq	%rax, %rdi
	movl	$1, %esi
	call	gtk_text_view_window_to_buffer_coords
	movl	104(%rsp), %r12d
	movl	108(%rsp), %r13d
	leaq	16(%rsp), %rbx
	movq	%rbp, %rsi
	movq	%r15, %rdi
	call	g_type_check_instance_cast
	movl	%r12d, %ecx
	movq	%rax, %rdi
	movl	%r13d, %edx
	movq	%rbx, %rsi
	call	gtk_text_view_get_iter_at_location
	movq	8(%rsp), %rdi
	movq	%rbx, %rsi
	movq	%rbx, %rdx
	call	gtk_text_buffer_select_range
	movq	dialog(%rip), %rdi
	movl	$.LC11, %esi
	xorl	%eax, %eax
	call	g_signal_emit_by_name
	jmp	.L44
.LFE59:
	.size	message_text_button_release_cb, .-message_text_button_release_cb
	.section	.rodata.str1.1
.LC12:
	.string	"MsgDialog"
	.text
	.p2align 4,,15
.globl message_get_type
	.type	message_get_type, @function
message_get_type:
.LFB45:
	subq	$8, %rsp
.LCFI34:
	movq	g_define_type_id__volatile.44023(%rip), %rax
	testq	%rax, %rax
	je	.L51
.L46:
	movq	g_define_type_id__volatile.44023(%rip), %rax
	addq	$8, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L51:
	movl	$g_define_type_id__volatile.44023, %edi
	call	g_once_init_enter_impl
	testl	%eax, %eax
	je	.L46
	movl	$.LC12, %edi
	call	g_intern_static_string
	movl	$message_init, %r9d
	movq	%rax, %rsi
	movl	$24, %r8d
	movl	$message_class_intern_init, %ecx
	movl	$136, %edx
	movl	$80, %edi
	movl	$0, (%rsp)
	call	g_type_register_static_simple
	movl	$g_define_type_id__volatile.44023, %edi
	movq	%rax, %rsi
	call	g_once_init_leave
	movq	g_define_type_id__volatile.44023(%rip), %rax
	addq	$8, %rsp
	ret
.LFE45:
	.size	message_get_type, .-message_get_type
	.section	.rodata.str1.1
.LC13:
	.string	"Initialising message dialog"
.LC14:
	.string	"SendMessage"
.LC15:
	.string	"char_label"
.LC16:
	.string	"send_message_textview"
.LC17:
	.string	"send_message_dialog"
.LC18:
	.string	"send-message-dialog.ui"
.LC19:
	.string	"send_button"
.LC20:
	.string	"friends_label"
.LC21:
	.string	"friends_combo"
.LC22:
	.string	"destroy"
.LC23:
	.string	"button_release_event"
.LC24:
	.string	"response"
.LC25:
	.string	"changed"
.LC26:
	.string	"modified-changed"
.LC27:
	.string	"underline"
.LC28:
	.string	"text"
	.text
	.p2align 4,,15
.globl send_message_dialog_show
	.type	send_message_dialog_show, @function
send_message_dialog_show:
.LFB50:
	movq	%rbx, -32(%rsp)
.LCFI35:
	movq	%r12, -16(%rsp)
.LCFI36:
	movq	%rdi, %r12
	movq	%rbp, -24(%rsp)
.LCFI37:
	movq	%r13, -8(%rsp)
.LCFI38:
	subq	$104, %rsp
.LCFI39:
	movq	dialog(%rip), %rbx
	testq	%rbx, %rbx
	je	.L53
	cmpq	$0, dialog_priv(%rip)
	je	.L57
.L54:
	call	gtk_text_view_get_type
	movq	%rax, %rsi
	movq	dialog_priv(%rip), %rax
	movq	8(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_text_view_get_buffer
	movq	%rax, %rdi
	movq	%rax, undo_buffer(%rip)
	call	gtk_text_buffer_begin_user_action
	call	gtk_window_get_type
	movq	%rax, %rsi
	movq	dialog_priv(%rip), %rax
	movq	(%rax), %rdi
	call	g_type_check_instance_cast
	movq	72(%rsp), %rbx
	movq	80(%rsp), %rbp
	movq	%rax, %rdi
	movq	88(%rsp), %r12
	movq	96(%rsp), %r13
	addq	$104, %rsp
	jmp	gtk_window_present
	.p2align 4,,10
	.p2align 3
.L57:
	call	message_get_type
	movq	%rbx, %rdi
	movq	%rax, %rsi
	call	g_type_instance_get_private
	movq	%rax, dialog_priv(%rip)
	jmp	.L54
	.p2align 4,,10
	.p2align 3
.L53:
	call	message_get_type
	xorl	%esi, %esi
	movq	%rax, %rdi
	movq	%rax, %r13
	xorl	%eax, %eax
	call	g_object_new
	xorl	%eax, %eax
	movl	$.LC13, %esi
	movl	$.LC14, %edi
	call	debug_impl
	movq	dialog_priv(%rip), %rdx
	movl	$.LC15, %r9d
	movl	$.LC16, %ecx
	movl	$.LC17, %esi
	movl	$.LC18, %edi
	movq	$0, 56(%rsp)
	movq	$.LC19, 40(%rsp)
	movq	$.LC20, 24(%rsp)
	leaq	24(%rdx), %rax
	leaq	8(%rdx), %r8
	movq	$.LC21, 8(%rsp)
	movq	%rax, 48(%rsp)
	leaq	40(%rdx), %rax
	movq	%rax, 32(%rsp)
	leaq	32(%rdx), %rax
	movq	%rax, 16(%rsp)
	leaq	16(%rdx), %rax
	movq	%rax, (%rsp)
	xorl	%eax, %eax
	call	gtkbuilder_get_file
	movq	dialog(%rip), %rsi
	movl	$.LC17, %r9d
	movl	$message_destroy_cb, %r8d
	movq	%r9, %rdx
	movl	$.LC22, %ecx
	movq	%rax, %rbx
	movq	%rax, %rdi
	xorl	%eax, %eax
	movq	$0, 64(%rsp)
	movq	$message_text_populate_popup_cb, 56(%rsp)
	movq	$.LC11, 48(%rsp)
	movq	$.LC16, 40(%rsp)
	movq	$message_text_button_release_cb, 32(%rsp)
	movq	$.LC23, 24(%rsp)
	movq	$.LC16, 16(%rsp)
	movq	$message_response_cb, 8(%rsp)
	movq	$.LC24, (%rsp)
	call	gtkbuilder_connect
	movq	%rbx, %rdi
	call	g_object_unref
	xorl	%edi, %edi
	movl	$5, %edx
	movl	$.LC2, %esi
	call	dcgettext
	movl	$140, %edx
	movq	%rax, %rsi
	movl	$.LC4, %edi
	xorl	%eax, %eax
	call	g_markup_printf_escaped
	movq	%rax, %rbx
	call	gtk_label_get_type
	movq	%rax, %rsi
	movq	dialog_priv(%rip), %rax
	movq	16(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%rbx, %rsi
	movq	%rax, %rdi
	call	gtk_label_set_markup
	movq	%rbx, %rdi
	call	g_free
	call	gtk_text_view_get_type
	movq	%rax, %rbp
	movq	dialog_priv(%rip), %rax
	movq	%rbp, %rsi
	movq	8(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_text_view_get_buffer
	movq	%rax, %rbx
	movq	dialog_priv(%rip), %rax
	movq	%rbp, %rsi
	movq	8(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_text_view_get_buffer
	movq	dialog(%rip), %rcx
	xorl	%r9d, %r9d
	xorl	%r8d, %r8d
	movq	%rbx, %rdi
	movl	$message_text_buffer_changed_cb, %edx
	movl	$.LC25, %esi
	movq	%rax, undo_buffer(%rip)
	call	g_signal_connect_data
	movq	dialog(%rip), %rcx
	xorl	%r9d, %r9d
	xorl	%r8d, %r8d
	movq	%rbx, %rdi
	movl	$message_text_buffer_undo_cb, %edx
	movl	$.LC26, %esi
	call	g_signal_connect_data
	xorl	%r8d, %r8d
	movl	$4, %ecx
	movl	$.LC27, %edx
	movq	%rbx, %rdi
	movl	$.LC6, %esi
	xorl	%eax, %eax
	call	gtk_text_buffer_create_tag
	call	gtk_window_get_type
	movq	%rax, %rsi
	movq	dialog_priv(%rip), %rax
	movq	(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%r12, %rsi
	movq	%rax, %rdi
	call	gtk_window_set_transient_for
	movl	$64, %esi
	movl	$1, %edi
	xorl	%eax, %eax
	call	gtk_list_store_new
	movq	%rax, %rbx
	call	gtk_tree_model_get_type
	movq	%rbx, %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	movq	%rax, %rbx
	call	gtk_combo_box_get_type
	movq	%rax, %rsi
	movq	dialog_priv(%rip), %rax
	movq	32(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%rbx, %rsi
	movq	%rax, %rdi
	call	gtk_combo_box_set_model
	call	gtk_cell_renderer_text_new
	movq	%rax, %r12
	call	gtk_cell_layout_get_type
	movq	%rax, %rbx
	movq	dialog_priv(%rip), %rax
	movq	%rbx, %rsi
	movq	32(%rax), %rdi
	call	g_type_check_instance_cast
	movl	$1, %edx
	movq	%rax, %rdi
	movq	%r12, %rsi
	call	gtk_cell_layout_pack_start
	movq	dialog_priv(%rip), %rax
	movq	%rbx, %rsi
	movq	32(%rax), %rdi
	call	g_type_check_instance_cast
	xorl	%r8d, %r8d
	xorl	%ecx, %ecx
	movq	%rax, %rdi
	movl	$.LC28, %edx
	movq	%r12, %rsi
	xorl	%eax, %eax
	call	gtk_cell_layout_set_attributes
	call	gtk_widget_get_type
	movq	%rax, %rsi
	movq	dialog_priv(%rip), %rax
	movq	(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_widget_show
	cmpq	$0, dialog_priv(%rip)
	je	.L58
.L55:
	movq	dialog_priv(%rip), %rax
	movq	%rbp, %rsi
	movq	8(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_text_view_get_buffer
	movq	72(%rsp), %rbx
	movq	%rax, undo_buffer(%rip)
	movq	%rax, %rdi
	movq	80(%rsp), %rbp
	movq	88(%rsp), %r12
	movq	96(%rsp), %r13
	addq	$104, %rsp
	jmp	gtk_text_buffer_begin_user_action
.L58:
	movq	dialog(%rip), %rdi
	movq	%r13, %rsi
	call	g_type_instance_get_private
	movq	%rax, dialog_priv(%rip)
	jmp	.L55
.LFE50:
	.size	send_message_dialog_show, .-send_message_dialog_show
	.p2align 4,,15
	.type	message_finalize, @function
message_finalize:
.LFB48:
	pushq	%rbx
.LCFI40:
	movq	%rdi, %rbx
	movq	message_parent_class(%rip), %rdi
	movl	$80, %esi
	call	g_type_check_class_cast
	movq	%rbx, %rdi
	movq	48(%rax), %r11
	popq	%rbx
	jmp	*%r11
.LFE48:
	.size	message_finalize, .-message_finalize
	.p2align 4,,15
	.type	message_class_intern_init, @function
message_class_intern_init:
.LFB44:
	pushq	%rbx
.LCFI41:
	movq	%rdi, %rbx
	call	g_type_class_peek_parent
	movq	%rbx, %rdi
	movl	$80, %esi
	movq	%rax, message_parent_class(%rip)
	call	g_type_check_class_cast
	movq	$message_finalize, 48(%rax)
	movq	%rax, %rdi
	movl	$56, %esi
	popq	%rbx
	jmp	g_type_class_add_private
.LFE44:
	.size	message_class_intern_init, .-message_class_intern_init
	.section	.rodata.str1.1
.LC29:
	.string	"Posting message to Twitter"
.LC30:
	.string	"~-._"
.LC31:
	.string	"text != NULL"
.LC32:
	.string	"*text != '\\0'"
	.text
	.p2align 4,,15
	.type	message_response_cb, @function
message_response_cb:
.LFB64:
	movq	%r14, -16(%rsp)
.LCFI42:
	movq	%rbx, -48(%rsp)
.LCFI43:
	movq	%rdi, %r14
	movq	%rbp, -40(%rsp)
.LCFI44:
	movq	%r12, -32(%rsp)
.LCFI45:
	movq	%r13, -24(%rsp)
.LCFI46:
	movq	%r15, -8(%rsp)
.LCFI47:
	subq	$264, %rsp
.LCFI48:
	cmpl	$-5, %esi
	je	.L64
.L82:
	movq	%r14, %rdi
	call	gtk_widget_destroy
.L84:
	movq	216(%rsp), %rbx
	movq	224(%rsp), %rbp
	movq	232(%rsp), %r12
	movq	240(%rsp), %r13
	movq	248(%rsp), %r14
	movq	256(%rsp), %r15
	addq	$264, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L64:
	movl	$.LC29, %esi
	movl	$.LC14, %edi
	xorl	%eax, %eax
	call	debug_impl
	call	gtk_text_view_get_type
	movq	%rax, %rsi
	movq	dialog_priv(%rip), %rax
	leaq	80(%rsp), %r12
	movq	8(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_text_view_get_buffer
	movq	%r12, %rsi
	movq	%rax, %rbx
	movq	%rax, %rdi
	call	gtk_text_buffer_get_start_iter
	movq	%rsp, %rsi
	movq	%rbx, %rdi
	call	gtk_text_buffer_get_end_iter
	movq	%rsp, %rsi
	movq	%r12, %rdi
	call	gtk_text_iter_equal
	testl	%eax, %eax
	jne	.L82
	movl	$1, %ecx
	movq	%rsp, %rdx
	movq	%r12, %rsi
	movq	%rbx, %rdi
	call	gtk_text_buffer_get_text
	testq	%rax, %rax
	movq	%rax, %r15
	je	.L67
	cmpb	$0, (%rax)
	je	.L68
	xorl	%edi, %edi
	movq	%r15, %r12
	call	g_string_new
	movq	%rax, %rbp
	jmp	.L69
	.p2align 4,,10
	.p2align 3
.L86:
	movq	(%rbp), %rax
	movb	%bl, (%rax,%rdx)
	movq	(%rbp), %rax
	movq	%rcx, 8(%rbp)
	movb	$0, 1(%rax,%rdx)
.L73:
	addq	$1, %r12
.L69:
	movzbl	(%r12), %ebx
	testb	%bl, %bl
	je	.L85
	movq	g_ascii_table(%rip), %rdx
	movzbl	%bl, %eax
	testb	$1, (%rdx,%rax,2)
	jne	.L70
	movzbl	%bl, %r13d
	movl	$.LC30, %edi
	movl	%r13d, %esi
	call	strchr
	testq	%rax, %rax
	je	.L71
.L70:
	movq	8(%rbp), %rdx
	leaq	1(%rdx), %rcx
	cmpq	16(%rbp), %rcx
	jb	.L86
	movsbl	%bl,%edx
	movq	$-1, %rsi
	movq	%rbp, %rdi
	call	g_string_insert_c
	jmp	.L73
	.p2align 4,,10
	.p2align 3
.L85:
	movq	%rbp, %rdi
	xorl	%esi, %esi
	call	g_string_free
	movq	%rax, %rbp
.L83:
	movq	%r15, %rdi
	call	g_free
	movq	dialog_priv(%rip), %rbx
	movl	48(%rbx), %edx
	testl	%edx, %edx
	jne	.L80
	movq	%r14, %rdi
	call	gtk_widget_destroy
	movq	%rbp, %rdi
	call	network_post_status
	testq	%rbp, %rbp
	je	.L84
	movq	%rbp, %rdi
	call	g_free
	.p2align 4,,4
	.p2align 3
	jmp	.L84
	.p2align 4,,10
	.p2align 3
.L71:
	movq	8(%rbp), %rdx
	leaq	1(%rdx), %rcx
	cmpq	16(%rbp), %rcx
	jae	.L74
	movq	(%rbp), %rax
	movb	$37, (%rax,%rdx)
	movq	(%rbp), %rax
	movq	%rcx, 8(%rbp)
	movb	$0, 1(%rax,%rdx)
.L75:
	movq	8(%rbp), %rcx
	shrb	$4, %bl
	movzbl	%bl, %eax
	movzbl	hex.44229(%rax), %edx
	leaq	1(%rcx), %rsi
	cmpq	16(%rbp), %rsi
	jae	.L76
	movq	(%rbp), %rax
	movb	%dl, (%rax,%rcx)
	movq	(%rbp), %rax
	movq	%rsi, 8(%rbp)
	movb	$0, 1(%rax,%rcx)
.L77:
	movq	8(%rbp), %rcx
	movq	%r13, %rax
	andl	$15, %eax
	movzbl	hex.44229(%rax), %edx
	leaq	1(%rcx), %rdi
	cmpq	16(%rbp), %rdi
	jae	.L78
	movq	(%rbp), %rax
	movb	%dl, (%rax,%rcx)
	movq	(%rbp), %rax
	movq	%rdi, 8(%rbp)
	movb	$0, 1(%rax,%rcx)
	jmp	.L73
	.p2align 4,,10
	.p2align 3
.L80:
	call	gtk_combo_box_get_type
	movq	32(%rbx), %rdi
	movq	%rax, %rsi
	leaq	160(%rsp), %r12
	call	g_type_check_instance_cast
	movq	%rax, %rbx
	movq	%rax, %rdi
	call	gtk_combo_box_get_model
	movq	%r12, %rsi
	movq	%rbx, %rdi
	movq	%rax, %r13
	call	gtk_combo_box_get_active_iter
	testl	%eax, %eax
	jne	.L87
.L81:
	testq	%rbp, %rbp
	je	.L82
	movq	%rbp, %rdi
	.p2align 4,,5
	.p2align 3
	call	g_free
	.p2align 4,,4
	.p2align 3
	jmp	.L82
	.p2align 4,,10
	.p2align 3
.L68:
	movl	$.LC32, %edx
	movl	$__PRETTY_FUNCTION__.44233, %esi
	xorl	%edi, %edi
	call	g_return_if_fail_warning
	xorl	%ebp, %ebp
	jmp	.L83
	.p2align 4,,10
	.p2align 3
.L74:
	movl	$37, %edx
	movq	$-1, %rsi
	movq	%rbp, %rdi
	call	g_string_insert_c
	jmp	.L75
	.p2align 4,,10
	.p2align 3
.L78:
	movsbl	%dl,%edx
	movq	$-1, %rsi
	movq	%rbp, %rdi
	call	g_string_insert_c
	.p2align 4,,2
	.p2align 3
	jmp	.L73
	.p2align 4,,10
	.p2align 3
.L76:
	movsbl	%dl,%edx
	movq	$-1, %rsi
	movq	%rbp, %rdi
	call	g_string_insert_c
	.p2align 4,,2
	.p2align 3
	jmp	.L77
	.p2align 4,,10
	.p2align 3
.L87:
	leaq	200(%rsp), %rcx
	xorl	%edx, %edx
	xorl	%eax, %eax
	movl	$-1, %r8d
	movq	%r12, %rsi
	movq	%r13, %rdi
	call	gtk_tree_model_get
	movq	200(%rsp), %rdi
	movq	%rbp, %rsi
	call	network_send_message
	movq	200(%rsp), %rdi
	testq	%rdi, %rdi
	je	.L81
	call	g_free
	jmp	.L81
	.p2align 4,,10
	.p2align 3
.L67:
	movl	$.LC31, %edx
	movl	$__PRETTY_FUNCTION__.44233, %esi
	xorl	%edi, %edi
	call	g_return_if_fail_warning
	xorl	%ebp, %ebp
	jmp	.L83
.LFE64:
	.size	message_response_cb, .-message_response_cb
	.p2align 4,,15
	.type	message_init, @function
message_init:
.LFB47:
	subq	$8, %rsp
.LCFI49:
	movq	g_define_type_id__volatile.44023(%rip), %rax
	movq	%rdi, dialog(%rip)
	testq	%rax, %rax
	je	.L95
.L89:
	movq	g_define_type_id__volatile.44023(%rip), %rsi
	call	g_type_instance_get_private
	movq	%rax, dialog_priv(%rip)
	addq	$8, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L95:
	movl	$g_define_type_id__volatile.44023, %edi
	call	g_once_init_enter_impl
	testl	%eax, %eax
	je	.L94
	movl	$.LC12, %edi
	call	g_intern_static_string
	movl	$80, %edi
	movq	%rax, %rsi
	movl	$message_init, %r9d
	movl	$24, %r8d
	movl	$message_class_intern_init, %ecx
	movl	$136, %edx
	movl	$0, (%rsp)
	call	g_type_register_static_simple
	movl	$g_define_type_id__volatile.44023, %edi
	movq	%rax, %rsi
	call	g_once_init_leave
.L94:
	movq	dialog(%rip), %rdi
	jmp	.L89
.LFE47:
	.size	message_init, .-message_init
	.section	.rodata.str1.8
	.align 8
.LC33:
	.string	"Please wait while the list of every one who's following you is loaded."
	.section	.rodata.str1.1
.LC34:
	.string	"Loading followers..."
.LC35:
	.string	""
	.text
	.p2align 4,,15
.globl message_show_friends
	.type	message_show_friends, @function
message_show_friends:
.LFB53:
	pushq	%r14
.LCFI50:
	pushq	%r13
.LCFI51:
	pushq	%r12
.LCFI52:
	pushq	%rbp
.LCFI53:
	pushq	%rbx
.LCFI54:
	subq	$32, %rsp
.LCFI55:
	movq	dialog_priv(%rip), %rax
	testl	%edi, %edi
	movl	%edi, 48(%rax)
	je	.L103
	movq	32(%rax), %rdi
	call	gtk_widget_show
	movq	dialog_priv(%rip), %rax
	movq	40(%rax), %rdi
	call	gtk_widget_show
	movl	$5, %edx
	movl	$.LC33, %esi
	xorl	%edi, %edi
	call	dcgettext
	movq	%rax, %rdi
	call	app_set_statusbar_msg
	movl	$150, %edi
	call	gdk_cursor_new
	movq	%rax, %rbx
	call	gtk_widget_get_type
	movq	%rax, %rsi
	movq	%rax, %r14
	movq	dialog_priv(%rip), %rax
	movq	(%rax), %rdi
	call	g_type_check_instance_cast
	movq	80(%rax), %rdi
	movq	%rbx, %rsi
	call	gdk_window_set_cursor
	movq	dialog_priv(%rip), %rax
	xorl	%esi, %esi
	movq	(%rax), %rdi
	call	gtk_widget_set_sensitive
	xorl	%eax, %eax
	movl	$.LC34, %esi
	movl	$.LC14, %edi
	call	debug_impl
	call	user_get_followers
	testq	%rax, %rax
	movq	%rax, %rbp
	je	.L99
	call	gtk_list_store_get_type
	movq	%rax, %rbx
	call	gtk_combo_box_get_type
	movq	%rax, %rsi
	movq	dialog_priv(%rip), %rax
	movq	32(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_combo_box_get_model
	movq	%rbx, %rsi
	movq	%rax, %rdi
	call	g_type_check_instance_cast
	movq	%rax, %r12
	.p2align 4,,10
	.p2align 3
.L100:
	movq	(%rbp), %rbx
	movq	%rsp, %rsi
	movq	%r12, %rdi
	call	gtk_list_store_append
	xorl	%edx, %edx
	xorl	%eax, %eax
	movl	$-1, %r8d
	movq	8(%rbx), %rcx
	movq	%rsp, %rsi
	movq	%r12, %rdi
	call	gtk_list_store_set
	movq	8(%rbp), %rbp
	testq	%rbp, %rbp
	jne	.L100
.L99:
	movq	dialog_priv(%rip), %rax
	movq	%r14, %rsi
	movq	(%rax), %rdi
	call	g_type_check_instance_cast
	movq	80(%rax), %rdi
	xorl	%esi, %esi
	call	gdk_window_set_cursor
	movq	dialog_priv(%rip), %rax
	movl	$1, %esi
	movq	(%rax), %rdi
	call	gtk_widget_set_sensitive
	movl	$.LC35, %edi
	call	app_set_statusbar_msg
	movq	dialog_priv(%rip), %rax
	movq	%r14, %rsi
	movq	32(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_widget_grab_focus
	addq	$32, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	popq	%r14
	ret
	.p2align 4,,10
	.p2align 3
.L103:
	movq	32(%rax), %rdi
	call	gtk_widget_hide
	movq	dialog_priv(%rip), %rax
	movq	40(%rax), %rdi
	call	gtk_widget_hide
	call	gtk_widget_get_type
	movq	%rax, %rsi
	movq	dialog_priv(%rip), %rax
	movq	8(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_widget_grab_focus
	addq	$32, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	popq	%r14
	ret
.LFE53:
	.size	message_show_friends, .-message_show_friends
	.local	dialog_priv
	.comm	dialog_priv,8,8
	.section	.rodata
	.align 16
	.type	__PRETTY_FUNCTION__.44147, @object
	.size	__PRETTY_FUNCTION__.44147, 21
__PRETTY_FUNCTION__.44147:
	.string	"message_correct_word"
	.local	dialog
	.comm	dialog,8,8
	.local	undo_buffer
	.comm	undo_buffer,8,8
	.align 16
	.type	__PRETTY_FUNCTION__.44233, @object
	.size	__PRETTY_FUNCTION__.44233, 19
__PRETTY_FUNCTION__.44233:
	.string	"url_encode_message"
	.align 16
	.type	hex.44229, @object
	.size	hex.44229, 16
hex.44229:
	.ascii	"0123456789ABCDEF"
	.local	g_define_type_id__volatile.44023
	.comm	g_define_type_id__volatile.44023,8,8
	.local	message_parent_class
	.comm	message_parent_class,8,8
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
	.long	.LFB54
	.long	.LFE54-.LFB54
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI0-.LFB54
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI1-.LCFI0
	.byte	0xe
	.uleb128 0x18
	.byte	0x83
	.uleb128 0x3
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI2-.LCFI1
	.byte	0xe
	.uleb128 0x20
	.align 8
.LEFDE1:
.LSFDE3:
	.long	.LEFDE3-.LASFDE3
.LASFDE3:
	.long	.LASFDE3-.Lframe1
	.long	.LFB52
	.long	.LFE52-.LFB52
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI3-.LFB52
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI4-.LCFI3
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI5-.LCFI4
	.byte	0xe
	.uleb128 0x20
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI6-.LCFI5
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.long	.LCFI7-.LCFI6
	.byte	0xe
	.uleb128 0x50
	.byte	0x83
	.uleb128 0x5
	.align 8
.LEFDE3:
.LSFDE5:
	.long	.LEFDE5-.LASFDE5
.LASFDE5:
	.long	.LASFDE5-.Lframe1
	.long	.LFB51
	.long	.LFE51-.LFB51
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI8-.LFB51
	.byte	0xe
	.uleb128 0x10
	.byte	0x86
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI9-.LCFI8
	.byte	0xe
	.uleb128 0x18
	.byte	0x83
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI10-.LCFI9
	.byte	0xe
	.uleb128 0x20
	.align 8
.LEFDE5:
.LSFDE7:
	.long	.LEFDE7-.LASFDE7
.LASFDE7:
	.long	.LASFDE7-.Lframe1
	.long	.LFB63
	.long	.LFE63-.LFB63
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI11-.LFB63
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
	.long	.LFB57
	.long	.LFE57-.LFB57
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI12-.LFB57
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE9:
.LSFDE11:
	.long	.LEFDE11-.LASFDE11
.LASFDE11:
	.long	.LASFDE11-.Lframe1
	.long	.LFB65
	.long	.LFE65-.LFB65
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI13-.LFB65
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE11:
.LSFDE13:
	.long	.LEFDE13-.LASFDE13
.LASFDE13:
	.long	.LASFDE13-.Lframe1
	.long	.LFB58
	.long	.LFE58-.LFB58
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI14-.LFB58
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI15-.LCFI14
	.byte	0xe
	.uleb128 0x18
	.byte	0x8c
	.uleb128 0x3
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI16-.LCFI15
	.byte	0xe
	.uleb128 0x20
	.byte	0x86
	.uleb128 0x4
	.byte	0x4
	.long	.LCFI17-.LCFI16
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.long	.LCFI18-.LCFI17
	.byte	0xe
	.uleb128 0xe0
	.byte	0x83
	.uleb128 0x5
	.align 8
.LEFDE13:
.LSFDE15:
	.long	.LEFDE15-.LASFDE15
.LASFDE15:
	.long	.LASFDE15-.Lframe1
	.long	.LFB60
	.long	.LFE60-.LFB60
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI19-.LFB60
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI20-.LCFI19
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI21-.LCFI20
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI22-.LCFI21
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.long	.LCFI23-.LCFI22
	.byte	0xe
	.uleb128 0x30
	.byte	0x4
	.long	.LCFI24-.LCFI23
	.byte	0xe
	.uleb128 0x38
	.byte	0x4
	.long	.LCFI25-.LCFI24
	.byte	0xe
	.uleb128 0x260
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
	.align 8
.LEFDE15:
.LSFDE17:
	.long	.LEFDE17-.LASFDE17
.LASFDE17:
	.long	.LASFDE17-.Lframe1
	.long	.LFB61
	.long	.LFE61-.LFB61
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI26-.LFB61
	.byte	0xe
	.uleb128 0xb0
	.align 8
.LEFDE17:
.LSFDE19:
	.long	.LEFDE19-.LASFDE19
.LASFDE19:
	.long	.LASFDE19-.Lframe1
	.long	.LFB59
	.long	.LFE59-.LFB59
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI28-.LFB59
	.byte	0x86
	.uleb128 0x6
	.byte	0x83
	.uleb128 0x7
	.byte	0x4
	.long	.LCFI30-.LCFI28
	.byte	0x8c
	.uleb128 0x5
	.byte	0x8f
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI33-.LCFI30
	.byte	0xe
	.uleb128 0xb0
	.byte	0x8e
	.uleb128 0x3
	.byte	0x8d
	.uleb128 0x4
	.align 8
.LEFDE19:
.LSFDE21:
	.long	.LEFDE21-.LASFDE21
.LASFDE21:
	.long	.LASFDE21-.Lframe1
	.long	.LFB45
	.long	.LFE45-.LFB45
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI34-.LFB45
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE21:
.LSFDE23:
	.long	.LEFDE23-.LASFDE23
.LASFDE23:
	.long	.LASFDE23-.Lframe1
	.long	.LFB50
	.long	.LFE50-.LFB50
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI36-.LFB50
	.byte	0x8c
	.uleb128 0x3
	.byte	0x83
	.uleb128 0x5
	.byte	0x4
	.long	.LCFI39-.LCFI36
	.byte	0xe
	.uleb128 0x70
	.byte	0x8d
	.uleb128 0x2
	.byte	0x86
	.uleb128 0x4
	.align 8
.LEFDE23:
.LSFDE25:
	.long	.LEFDE25-.LASFDE25
.LASFDE25:
	.long	.LASFDE25-.Lframe1
	.long	.LFB48
	.long	.LFE48-.LFB48
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI40-.LFB48
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE25:
.LSFDE27:
	.long	.LEFDE27-.LASFDE27
.LASFDE27:
	.long	.LASFDE27-.Lframe1
	.long	.LFB44
	.long	.LFE44-.LFB44
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI41-.LFB44
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE27:
.LSFDE29:
	.long	.LEFDE29-.LASFDE29
.LASFDE29:
	.long	.LASFDE29-.Lframe1
	.long	.LFB64
	.long	.LFE64-.LFB64
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI43-.LFB64
	.byte	0x83
	.uleb128 0x7
	.byte	0x8e
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI48-.LCFI43
	.byte	0xe
	.uleb128 0x110
	.byte	0x8f
	.uleb128 0x2
	.byte	0x8d
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x6
	.align 8
.LEFDE29:
.LSFDE31:
	.long	.LEFDE31-.LASFDE31
.LASFDE31:
	.long	.LASFDE31-.Lframe1
	.long	.LFB47
	.long	.LFE47-.LFB47
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI49-.LFB47
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE31:
.LSFDE33:
	.long	.LEFDE33-.LASFDE33
.LASFDE33:
	.long	.LASFDE33-.Lframe1
	.long	.LFB53
	.long	.LFE53-.LFB53
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI50-.LFB53
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI51-.LCFI50
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI52-.LCFI51
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI53-.LCFI52
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.long	.LCFI54-.LCFI53
	.byte	0xe
	.uleb128 0x30
	.byte	0x4
	.long	.LCFI55-.LCFI54
	.byte	0xe
	.uleb128 0x50
	.byte	0x83
	.uleb128 0x6
	.byte	0x86
	.uleb128 0x5
	.byte	0x8c
	.uleb128 0x4
	.byte	0x8d
	.uleb128 0x3
	.byte	0x8e
	.uleb128 0x2
	.align 8
.LEFDE33:
	.ident	"GCC: (SUSE Linux) 4.3.2 [gcc-4_3-branch revision 141291]"
	.section	.comment.SUSE.OPTs,"MS",@progbits,1
	.ascii	"OspWg"
	.section	.note.GNU-stack,"",@progbits
