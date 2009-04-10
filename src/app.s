	.file	"app.c"
	.text
	.p2align 4,,15
.globl app_get_window
	.type	app_get_window, @function
app_get_window:
.LFB102:
	movq	app_priv(%rip), %rax
	movq	(%rax), %rax
	ret
.LFE102:
	.size	app_get_window, .-app_get_window
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"<b>%s ( @%s )</b> - %s"
	.text
	.p2align 4,,15
.globl app_expand_message
	.type	app_expand_message, @function
app_expand_message:
.LFB108:
	movq	%rbx, -32(%rsp)
.LCFI0:
	movq	%rbp, -24(%rsp)
.LCFI1:
	xorl	%eax, %eax
	movq	%r12, -16(%rsp)
.LCFI2:
	movq	%r13, -8(%rsp)
.LCFI3:
	subq	$40, %rsp
.LCFI4:
	movq	%rcx, %r13
	movq	%rdx, %rcx
	movq	%rdi, %rdx
	movl	$.LC0, %edi
	movq	%r8, %rbp
	call	g_strdup_printf
	movq	%rax, %r12
	call	label_get_type
	movq	%rax, %rbx
	movq	app_priv(%rip), %rax
	movq	%rbx, %rsi
	movq	184(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%r13, %rsi
	movq	%rax, %rdi
	call	label_set_text
	movq	app_priv(%rip), %rax
	movq	%rbx, %rsi
	movq	176(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%r12, %rsi
	movq	%rax, %rdi
	call	label_set_text
	movq	%r12, %rdi
	call	g_free
	testq	%rbp, %rbp
	je	.L4
	movq	%rbp, %rdi
	call	images_expand_pixbuf
	movq	%rax, %rbx
	call	gtk_image_get_type
	movq	%rax, %rsi
	movq	app_priv(%rip), %rax
	movq	168(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%rbx, %rsi
	movq	%rax, %rdi
	call	gtk_image_set_from_pixbuf
	testq	%rbx, %rbx
	je	.L4
	movq	%rbx, %rdi
	call	g_object_unref
.L4:
	movq	app_priv(%rip), %rax
	movq	8(%rsp), %rbx
	movq	16(%rsp), %rbp
	movq	24(%rsp), %r12
	movq	32(%rsp), %r13
	movq	160(%rax), %rdi
	addq	$40, %rsp
	jmp	gtk_widget_show
.LFE108:
	.size	app_expand_message, .-app_expand_message
	.p2align 4,,15
.globl app_set_image
	.type	app_set_image, @function
app_set_image:
.LFB107:
	pushq	%rbx
.LCFI5:
	call	images_get_pixbuf_from_filename
	testq	%rax, %rax
	movq	%rax, %rbx
	je	.L9
	call	tweet_list_get_store
	leaq	16(%rsp), %rsi
	movq	%rax, %rdi
	movl	$-1, %r8d
	movq	%rbx, %rcx
	xorl	%edx, %edx
	xorl	%eax, %eax
	call	gtk_list_store_set
	movq	%rbx, %rdi
	call	g_object_unref
.L9:
	popq	%rbx
	ret
.LFE107:
	.size	app_set_image, .-app_set_image
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC1:
	.string	"/apps/greet-tweet-know/ui/notify"
	.section	.rodata.str1.1
.LC2:
	.string	"greet-tweet-know"
.LC3:
	.string	"Greet-Tweet-Know"
	.section	.rodata.str1.8
	.align 8
.LC4:
	.string	"Error displaying notification: %s"
	.section	.rodata.str1.1
.LC5:
	.string	"AppSetup"
	.text
	.p2align 4,,15
.globl app_notify
	.type	app_notify, @function
app_notify:
.LFB106:
	pushq	%rbx
.LCFI6:
	movq	%rdi, %rbx
	subq	$16, %rsp
.LCFI7:
	call	conf_get
	leaq	12(%rsp), %rdx
	movq	%rax, %rdi
	movl	$.LC1, %esi
	call	conf_get_bool
	movl	12(%rsp), %eax
	testl	%eax, %eax
	jne	.L13
.L12:
	addq	$16, %rsp
	popq	%rbx
	ret
	.p2align 4,,10
	.p2align 3
.L13:
	xorl	%ecx, %ecx
	movl	$.LC2, %edx
	movq	%rbx, %rsi
	movl	$.LC3, %edi
	movq	$0, (%rsp)
	call	notify_notification_new
	movl	$8000, %esi
	movq	%rax, %rbx
	movq	%rax, %rdi
	call	notify_notification_set_timeout
	movq	%rsp, %rsi
	movq	%rbx, %rdi
	call	notify_notification_show
	movl	$80, %esi
	movq	%rbx, %rdi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	g_object_unref
	movq	(%rsp), %rax
	testq	%rax, %rax
	je	.L12
	movq	8(%rax), %rdx
	movl	$.LC4, %esi
	movl	$.LC5, %edi
	xorl	%eax, %eax
	call	debug_impl
	movq	(%rsp), %rdi
	call	g_error_free
	addq	$16, %rsp
	popq	%rbx
	ret
.LFE106:
	.size	app_notify, .-app_notify
	.section	.rodata.str1.8
	.align 8
.LC6:
	.string	"/apps/greet-tweet-know/ui/sound"
	.section	.rodata.str1.1
.LC7:
	.string	"New tweet received"
.LC8:
	.string	"message-new-instant"
.LC9:
	.string	"event.id"
.LC10:
	.string	"application.name"
.LC11:
	.string	"event.description"
	.text
	.p2align 4,,15
.globl app_notify_sound
	.type	app_notify_sound, @function
app_notify_sound:
.LFB105:
	movq	%rbx, -16(%rsp)
.LCFI8:
	movq	%r12, -8(%rsp)
.LCFI9:
	subq	$56, %rsp
.LCFI10:
	call	conf_get
	leaq	36(%rsp), %rdx
	movq	%rax, %rdi
	movl	$.LC6, %esi
	call	conf_get_bool
	movl	36(%rsp), %edx
	testl	%edx, %edx
	je	.L16
	movl	$5, %edx
	movl	$.LC7, %esi
	xorl	%edi, %edi
	call	dcgettext
	movq	%rax, %rbx
	call	g_get_application_name
	movq	%rax, %r12
	call	ca_gtk_context_get
	movl	$.LC8, %r9d
	movq	%rax, %rdi
	movl	$.LC9, %r8d
	movq	%r12, %rcx
	movl	$.LC10, %edx
	xorl	%esi, %esi
	xorl	%eax, %eax
	movq	$0, 16(%rsp)
	movq	%rbx, 8(%rsp)
	movq	$.LC11, (%rsp)
	call	ca_context_play
.L16:
	movq	40(%rsp), %rbx
	movq	48(%rsp), %r12
	addq	$56, %rsp
	ret
.LFE105:
	.size	app_notify_sound, .-app_notify_sound
	.p2align 4,,15
.globl app_set_statusbar_msg
	.type	app_set_statusbar_msg, @function
app_set_statusbar_msg:
.LFB103:
	movq	%r12, -8(%rsp)
.LCFI11:
	movq	%rbx, -24(%rsp)
.LCFI12:
	movq	%rdi, %r12
	movq	%rbp, -16(%rsp)
.LCFI13:
	subq	$24, %rsp
.LCFI14:
	movq	app_priv(%rip), %rax
	movq	16(%rax), %rbx
	testq	%rbx, %rbx
	je	.L21
	call	gtk_statusbar_get_type
	movq	%rax, %rbp
	movq	(%rbx), %rax
	testq	%rax, %rax
	je	.L19
	cmpq	%rbp, (%rax)
	je	.L20
.L19:
	movq	%rbp, %rsi
	movq	%rbx, %rdi
	call	g_type_check_instance_is_a
	testl	%eax, %eax
	jne	.L20
.L21:
	movq	(%rsp), %rbx
	movq	8(%rsp), %rbp
	movq	16(%rsp), %r12
	addq	$24, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L20:
	movq	%rbp, %rsi
	movq	%rbx, %rdi
	call	g_type_check_instance_cast
	movl	$1, %esi
	movq	%rax, %rdi
	call	gtk_statusbar_pop
	movq	app_priv(%rip), %rax
	movq	%rbp, %rsi
	movq	16(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%r12, %rdx
	movq	(%rsp), %rbx
	movq	8(%rsp), %rbp
	movq	16(%rsp), %r12
	movq	%rax, %rdi
	movl	$1, %esi
	addq	$24, %rsp
	jmp	gtk_statusbar_push
.LFE103:
	.size	app_set_statusbar_msg, .-app_set_statusbar_msg
	.section	.rodata.str1.1
.LC12:
	.string	"sensitive"
	.text
	.p2align 4,,15
.globl app_state_on_connection
	.type	app_state_on_connection, @function
app_state_on_connection:
.LFB101:
	pushq	%rbp
.LCFI15:
	movl	%edi, %ebp
	pushq	%rbx
.LCFI16:
	subq	$8, %rsp
.LCFI17:
	movq	app_priv(%rip), %rax
	movq	24(%rax), %rbx
	testq	%rbx, %rbx
	je	.L23
	.p2align 4,,10
	.p2align 3
.L28:
	movq	(%rbx), %rdi
	xorl	%ecx, %ecx
	xorl	%eax, %eax
	movl	%ebp, %edx
	movl	$.LC12, %esi
	call	g_object_set
	movq	8(%rbx), %rbx
	testq	%rbx, %rbx
	jne	.L28
	movq	app_priv(%rip), %rax
.L23:
	movq	32(%rax), %rbx
	testq	%rbx, %rbx
	je	.L25
	testl	%ebp, %ebp
	sete	%al
	movzbl	%al, %ebp
	.p2align 4,,10
	.p2align 3
.L26:
	movq	(%rbx), %rdi
	xorl	%ecx, %ecx
	xorl	%eax, %eax
	movl	%ebp, %edx
	movl	$.LC12, %esi
	call	g_object_set
	movq	8(%rbx), %rbx
	testq	%rbx, %rbx
	jne	.L26
.L25:
	addq	$8, %rsp
	xorl	%edi, %edi
	popq	%rbx
	popq	%rbp
	jmp	g_list_free
.LFE101:
	.size	app_state_on_connection, .-app_state_on_connection
	.section	.rodata.str1.1
.LC13:
	.string	"app != NULL"
.LC14:
	.string	"app.c"
	.text
	.p2align 4,,15
.globl app_get
	.type	app_get, @function
app_get:
.LFB92:
	subq	$8, %rsp
.LCFI18:
	movq	app(%rip), %rax
	testq	%rax, %rax
	je	.L32
	addq	$8, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L32:
	movl	$.LC13, %r8d
	movl	$__PRETTY_FUNCTION__.45069, %ecx
	movl	$687, %edx
	movl	$.LC14, %esi
	xorl	%edi, %edi
	call	g_assertion_message_expr
.LFE92:
	.size	app_get, .-app_get
	.section	.rodata.str1.1
.LC15:
	.string	"Default timeline in not set"
.LC16:
	.string	"combined"
	.section	.rodata.str1.8
	.align 8
.LC17:
	.string	"https://twitter.com/statuses/public_timeline.xml"
	.align 8
.LC18:
	.string	"https://twitter.com/statuses/user_timeline.xml"
	.text
	.p2align 4,,15
	.type	app_set_default_timeline, @function
app_set_default_timeline:
.LFB97:
	testq	%rsi, %rsi
	movq	%rsi, %r8
	je	.L35
	cmpb	$0, (%rsi)
	jne	.L36
.L35:
	movl	$.LC15, %edx
	movl	$16, %esi
	xorl	%edi, %edi
	xorl	%eax, %eax
	jmp	g_log
	.p2align 4,,10
	.p2align 3
.L36:
	movl	$.LC16, %edi
	movl	$9, %ecx
	repz cmpsb
	je	.L41
	movl	$.LC17, %edi
	movl	$49, %ecx
	movq	%r8, %rsi
	repz cmpsb
	je	.L42
	movl	$.LC18, %edi
	movl	$47, %ecx
	movq	%r8, %rsi
	repz cmpsb
	je	.L43
	movq	app_priv(%rip), %rax
	movl	$1, %esi
	movq	72(%rax), %rdi
	jmp	gtk_radio_action_set_current_value
	.p2align 4,,10
	.p2align 3
.L41:
	movq	app_priv(%rip), %rax
	movl	$1, %esi
	movq	56(%rax), %rdi
	jmp	gtk_radio_action_set_current_value
	.p2align 4,,10
	.p2align 3
.L42:
	movq	app_priv(%rip), %rax
	movl	$1, %esi
	movq	64(%rax), %rdi
	jmp	gtk_radio_action_set_current_value
	.p2align 4,,10
	.p2align 3
.L43:
	movq	app_priv(%rip), %rax
	movl	$1, %esi
	movq	80(%rax), %rdi
	jmp	gtk_radio_action_set_current_value
.LFE97:
	.size	app_set_default_timeline, .-app_set_default_timeline
	.p2align 4,,15
	.type	configure_event_timeout_cb, @function
configure_event_timeout_cb:
.LFB93:
	pushq	%r12
.LCFI19:
	movq	%rdi, %r12
	pushq	%rbx
.LCFI20:
	subq	$24, %rsp
.LCFI21:
	call	gtk_window_get_type
	movq	%r12, %rdi
	movq	%rax, %rsi
	movq	%rax, %rbx
	call	g_type_check_instance_cast
	leaq	8(%rsp), %rdx
	leaq	12(%rsp), %rsi
	movq	%rax, %rdi
	call	gtk_window_get_size
	movq	%rbx, %rsi
	movq	%r12, %rdi
	call	g_type_check_instance_cast
	leaq	16(%rsp), %rdx
	leaq	20(%rsp), %rsi
	movq	%rax, %rdi
	call	gtk_window_get_position
	movl	8(%rsp), %ecx
	movl	12(%rsp), %edx
	movl	16(%rsp), %esi
	movl	20(%rsp), %edi
	call	geometry_save_for_main_window
	movq	app_priv(%rip), %rax
	movl	$0, 152(%rax)
	addq	$24, %rsp
	xorl	%eax, %eax
	popq	%rbx
	popq	%r12
	ret
.LFE93:
	.size	configure_event_timeout_cb, .-configure_event_timeout_cb
	.section	.rodata.str1.8
	.align 8
.LC19:
	.string	"/apps/greet-tweet-know/ui/main_window_hidden"
	.text
	.p2align 4,,15
	.type	app_toggle_visibility, @function
app_toggle_visibility:
.LFB64:
	pushq	%rbp
.LCFI22:
	pushq	%rbx
.LCFI23:
	subq	$24, %rsp
.LCFI24:
	call	gtk_window_get_type
	movq	%rax, %rsi
	movq	%rax, %rbx
	movq	app_priv(%rip), %rax
	movq	(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	window_get_is_visible
	testl	%eax, %eax
	movl	%eax, %ebp
	je	.L47
	movq	app_priv(%rip), %rax
	movq	112(%rax), %rdi
	call	gtk_status_icon_is_embedded
	testl	%eax, %eax
	jne	.L50
.L47:
	movq	app_priv(%rip), %rax
	movq	(%rax), %rdi
	call	geometry_load_for_main_window
	movq	app_priv(%rip), %rax
	movq	%rbx, %rsi
	movq	(%rax), %rdi
	call	g_type_check_instance_cast
	movl	$1, %esi
	movq	%rax, %rdi
	call	window_present
.L48:
	call	conf_get
	movl	%ebp, %edx
	movq	%rax, %rdi
	movl	$.LC19, %esi
	call	conf_set_bool
	addq	$24, %rsp
	popq	%rbx
	popq	%rbp
	ret
	.p2align 4,,10
	.p2align 3
.L50:
	movq	app_priv(%rip), %rax
	movq	%rbx, %rsi
	movq	(%rax), %rdi
	call	g_type_check_instance_cast
	leaq	8(%rsp), %rdx
	leaq	12(%rsp), %rsi
	movq	%rax, %rdi
	call	gtk_window_get_size
	movq	app_priv(%rip), %rax
	movq	%rbx, %rsi
	movq	(%rax), %rdi
	call	g_type_check_instance_cast
	leaq	16(%rsp), %rdx
	leaq	20(%rsp), %rsi
	movq	%rax, %rdi
	call	gtk_window_get_position
	movq	app_priv(%rip), %rax
	movq	(%rax), %rdi
	call	gtk_widget_hide
	movl	20(%rsp), %edi
	movl	8(%rsp), %ecx
	movl	12(%rsp), %edx
	movl	16(%rsp), %esi
	call	geometry_save_for_main_window
	movq	app_priv(%rip), %rax
	movl	152(%rax), %edi
	testl	%edi, %edi
	je	.L48
	call	g_source_remove
	movq	app_priv(%rip), %rax
	movl	$0, 152(%rax)
	jmp	.L48
.LFE64:
	.size	app_toggle_visibility, .-app_toggle_visibility
	.p2align 4,,15
	.type	app_show_hide_cb, @function
app_show_hide_cb:
.LFB86:
	jmp	app_toggle_visibility
.LFE86:
	.size	app_show_hide_cb, .-app_show_hide_cb
	.p2align 4,,15
	.type	app_status_icon_activate_cb, @function
app_status_icon_activate_cb:
.LFB87:
	jmp	app_toggle_visibility
.LFE87:
	.size	app_status_icon_activate_cb, .-app_status_icon_activate_cb
	.p2align 4,,15
.globl app_set_visibility
	.type	app_set_visibility, @function
app_set_visibility:
.LFB65:
	movq	%rbx, -16(%rsp)
.LCFI25:
	movq	%rbp, -8(%rsp)
.LCFI26:
	subq	$24, %rsp
.LCFI27:
	movq	app_priv(%rip), %rax
	movl	%edi, %ebx
	movq	(%rax), %rbp
	call	conf_get
	xorl	%edx, %edx
	testl	%ebx, %ebx
	movq	%rax, %rdi
	sete	%dl
	movl	$.LC19, %esi
	call	conf_set_bool
	testl	%ebx, %ebx
	je	.L58
	call	gtk_window_get_type
	movq	%rbp, %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	movq	8(%rsp), %rbx
	movq	16(%rsp), %rbp
	movq	%rax, %rdi
	movl	$1, %esi
	addq	$24, %rsp
	jmp	window_present
	.p2align 4,,10
	.p2align 3
.L58:
	movq	%rbp, %rdi
	movq	8(%rsp), %rbx
	movq	16(%rsp), %rbp
	addq	$24, %rsp
	jmp	gtk_widget_hide
.LFE65:
	.size	app_set_visibility, .-app_set_visibility
	.p2align 4,,15
	.type	app_status_icon_popup_menu_cb, @function
app_status_icon_popup_menu_cb:
.LFB88:
	movq	%rbx, -32(%rsp)
.LCFI28:
	movq	%r12, -24(%rsp)
.LCFI29:
	movq	%rcx, %r12
	movq	%r13, -16(%rsp)
.LCFI30:
	movq	%r14, -8(%rsp)
.LCFI31:
	subq	$40, %rsp
.LCFI32:
	movl	%edx, %r13d
	movl	%esi, %r14d
	call	gtk_window_get_type
	movq	%rax, %rsi
	movq	app_priv(%rip), %rax
	movq	(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	window_get_is_visible
	movl	%eax, %ebx
	movq	app_priv(%rip), %rax
	movl	$app_show_hide_cb, %r9d
	xorl	%r8d, %r8d
	xorl	%ecx, %ecx
	xorl	%edx, %edx
	movl	$24, %esi
	movq	128(%rax), %rdi
	movq	%r12, (%rsp)
	call	g_signal_handlers_block_matched
	movq	app_priv(%rip), %rax
	movl	%ebx, %esi
	movq	128(%rax), %rdi
	call	gtk_toggle_action_set_active
	movq	app_priv(%rip), %rax
	movl	$app_show_hide_cb, %r9d
	xorl	%r8d, %r8d
	xorl	%ecx, %ecx
	xorl	%edx, %edx
	movl	$24, %esi
	movq	128(%rax), %rdi
	movq	%r12, (%rsp)
	call	g_signal_handlers_unblock_matched
	movq	app_priv(%rip), %rbx
	call	gtk_menu_get_type
	movq	%rax, %rsi
	movq	112(%rbx), %r12
	movq	120(%rbx), %rdi
	call	g_type_check_instance_cast
	movl	%r14d, %r9d
	movq	%r12, %r8
	movq	%rax, %rdi
	movl	$gtk_status_icon_position_menu, %ecx
	xorl	%edx, %edx
	xorl	%esi, %esi
	movl	%r13d, (%rsp)
	call	gtk_menu_popup
	movq	8(%rsp), %rbx
	movq	16(%rsp), %r12
	movq	24(%rsp), %r13
	movq	32(%rsp), %r14
	addq	$40, %rsp
	ret
.LFE88:
	.size	app_status_icon_popup_menu_cb, .-app_status_icon_popup_menu_cb
	.section	.rodata.str1.8
	.align 8
.LC20:
	.string	"https://twitter.com/statuses/replies.xml"
	.text
	.p2align 4,,15
	.type	app_view_direct_replies_cb, @function
app_view_direct_replies_cb:
.LFB75:
	movq	app_priv(%rip), %rax
	cmpq	%rsi, 96(%rax)
	je	.L64
	rep
	ret
	.p2align 4,,10
	.p2align 3
.L64:
	movl	$.LC20, %edi
	jmp	network_get_timeline
.LFE75:
	.size	app_view_direct_replies_cb, .-app_view_direct_replies_cb
	.section	.rodata.str1.8
	.align 8
.LC21:
	.string	"https://twitter.com/direct_messages.xml"
	.text
	.p2align 4,,15
	.type	app_view_direct_messages_cb, @function
app_view_direct_messages_cb:
.LFB74:
	movq	app_priv(%rip), %rax
	cmpq	%rsi, 88(%rax)
	je	.L68
	rep
	ret
	.p2align 4,,10
	.p2align 3
.L68:
	movl	$.LC21, %edi
	jmp	network_get_timeline
.LFE74:
	.size	app_view_direct_messages_cb, .-app_view_direct_messages_cb
	.section	.rodata.str1.8
	.align 8
.LC22:
	.string	"https://twitter.com/statuses/friends_timeline.xml"
	.text
	.p2align 4,,15
	.type	app_friends_timeline_cb, @function
app_friends_timeline_cb:
.LFB72:
	movq	app_priv(%rip), %rax
	cmpq	%rsi, 72(%rax)
	je	.L72
	rep
	ret
	.p2align 4,,10
	.p2align 3
.L72:
	movl	$.LC22, %edi
	jmp	network_get_timeline
.LFE72:
	.size	app_friends_timeline_cb, .-app_friends_timeline_cb
	.p2align 4,,15
	.type	app_public_timeline_cb, @function
app_public_timeline_cb:
.LFB71:
	movq	app_priv(%rip), %rax
	cmpq	%rsi, 64(%rax)
	je	.L76
	rep
	ret
	.p2align 4,,10
	.p2align 3
.L76:
	movl	$.LC17, %edi
	jmp	network_get_timeline
.LFE71:
	.size	app_public_timeline_cb, .-app_public_timeline_cb
	.p2align 4,,15
	.type	app_about_cb, @function
app_about_cb:
.LFB81:
	subq	$8, %rsp
.LCFI33:
	call	gtk_window_get_type
	movq	%rax, %rsi
	movq	app_priv(%rip), %rax
	movq	(%rax), %rdi
	call	g_type_check_instance_cast
	addq	$8, %rsp
	movq	%rax, %rdi
	jmp	about_dialog_new
.LFE81:
	.size	app_about_cb, .-app_about_cb
	.p2align 4,,15
	.type	app_friends_timelines_cb, @function
app_friends_timelines_cb:
.LFB76:
	subq	$8, %rsp
.LCFI34:
	call	gtk_window_get_type
	movq	%rax, %rsi
	movq	app_priv(%rip), %rax
	movq	(%rax), %rdi
	call	g_type_check_instance_cast
	addq	$8, %rsp
	movq	%rax, %rdi
	jmp	followers_dialog_show
.LFE76:
	.size	app_friends_timelines_cb, .-app_friends_timelines_cb
	.p2align 4,,15
	.type	app_help_contents_cb, @function
app_help_contents_cb:
.LFB82:
	subq	$8, %rsp
.LCFI35:
	call	gtk_window_get_type
	movq	%rax, %rsi
	movq	app_priv(%rip), %rax
	movq	(%rax), %rdi
	call	g_type_check_instance_cast
	addq	$8, %rsp
	movq	%rax, %rdi
	jmp	help_show
.LFE82:
	.size	app_help_contents_cb, .-app_help_contents_cb
	.p2align 4,,15
	.type	app_mine_timeline_cb, @function
app_mine_timeline_cb:
.LFB73:
	movq	app_priv(%rip), %rax
	cmpq	%rsi, 80(%rax)
	je	.L86
	rep
	ret
	.p2align 4,,10
	.p2align 3
.L86:
	xorl	%edi, %edi
	jmp	network_get_user
.LFE73:
	.size	app_mine_timeline_cb, .-app_mine_timeline_cb
	.p2align 4,,15
	.type	app_combined_timeline_cb, @function
app_combined_timeline_cb:
.LFB70:
	movq	app_priv(%rip), %rax
	cmpq	%rsi, 56(%rax)
	je	.L90
	rep
	ret
	.p2align 4,,10
	.p2align 3
.L90:
	jmp	network_get_combined_timeline
.LFE70:
	.size	app_combined_timeline_cb, .-app_combined_timeline_cb
	.p2align 4,,15
	.type	app_preferences_cb, @function
app_preferences_cb:
.LFB80:
	subq	$8, %rsp
.LCFI36:
	call	gtk_window_get_type
	movq	%rax, %rsi
	movq	app_priv(%rip), %rax
	movq	(%rax), %rdi
	call	g_type_check_instance_cast
	addq	$8, %rsp
	movq	%rax, %rdi
	jmp	preferences_dialog_show
.LFE80:
	.size	app_preferences_cb, .-app_preferences_cb
	.p2align 4,,15
	.type	friends_block, @function
friends_block:
.LFB85:
	subq	$8, %rsp
.LCFI37:
	call	gtk_window_get_type
	movq	%rax, %rsi
	movq	app_priv(%rip), %rax
	movq	(%rax), %rdi
	call	g_type_check_instance_cast
	addq	$8, %rsp
	movq	%rax, %rdi
	jmp	popup_friend_block
.LFE85:
	.size	friends_block, .-friends_block
	.p2align 4,,15
	.type	friends_unfollow, @function
friends_unfollow:
.LFB84:
	subq	$8, %rsp
.LCFI38:
	call	gtk_window_get_type
	movq	%rax, %rsi
	movq	app_priv(%rip), %rax
	movq	(%rax), %rdi
	call	g_type_check_instance_cast
	addq	$8, %rsp
	movq	%rax, %rdi
	jmp	popup_friend_unfollow
.LFE84:
	.size	friends_unfollow, .-friends_unfollow
	.p2align 4,,15
	.type	friends_follow, @function
friends_follow:
.LFB83:
	subq	$8, %rsp
.LCFI39:
	call	gtk_window_get_type
	movq	%rax, %rsi
	movq	app_priv(%rip), %rax
	movq	(%rax), %rdi
	call	g_type_check_instance_cast
	addq	$8, %rsp
	movq	%rax, %rdi
	jmp	popup_friend_follow
.LFE83:
	.size	friends_follow, .-friends_follow
	.p2align 4,,15
	.type	app_friends_manager_cb, @function
app_friends_manager_cb:
.LFB79:
	subq	$8, %rsp
.LCFI40:
	call	gtk_window_get_type
	movq	%rax, %rsi
	movq	app_priv(%rip), %rax
	movq	(%rax), %rdi
	call	g_type_check_instance_cast
	addq	$8, %rsp
	movq	%rax, %rdi
	jmp	friends_manager_show
.LFE79:
	.size	app_friends_manager_cb, .-app_friends_manager_cb
	.p2align 4,,15
	.type	app_accounts_cb, @function
app_accounts_cb:
.LFB78:
	subq	$8, %rsp
.LCFI41:
	call	gtk_window_get_type
	movq	%rax, %rsi
	movq	app_priv(%rip), %rax
	movq	(%rax), %rdi
	call	g_type_check_instance_cast
	addq	$8, %rsp
	movq	%rax, %rdi
	jmp	accounts_dialog_show
.LFE78:
	.size	app_accounts_cb, .-app_accounts_cb
	.p2align 4,,15
	.type	app_quit_cb, @function
app_quit_cb:
.LFB68:
	movq	app_priv(%rip), %rax
	movq	(%rax), %rdi
	jmp	gtk_widget_destroy
.LFE68:
	.size	app_quit_cb, .-app_quit_cb
	.p2align 4,,15
	.type	app_refresh_cb, @function
app_refresh_cb:
.LFB69:
	subq	$8, %rsp
.LCFI42:
	xorl	%edi, %edi
	call	show_tweets_submenu_entries
	addq	$8, %rsp
	jmp	network_refresh
.LFE69:
	.size	app_refresh_cb, .-app_refresh_cb
	.p2align 4,,15
	.type	app_window_configure_event_cb, @function
app_window_configure_event_cb:
.LFB94:
	movq	%rbx, -16(%rsp)
.LCFI43:
	movq	%rbp, -8(%rsp)
.LCFI44:
	subq	$24, %rsp
.LCFI45:
	movq	app_priv(%rip), %rbx
	movq	%rdi, %rbp
	movl	152(%rbx), %edi
	testl	%edi, %edi
	je	.L108
	call	g_source_remove
	movq	app_priv(%rip), %rbx
.L108:
	movq	%rbp, %rdx
	movl	$configure_event_timeout_cb, %esi
	movl	$500, %edi
	call	g_timeout_add
	movl	%eax, 152(%rbx)
	xorl	%eax, %eax
	movq	8(%rsp), %rbx
	movq	16(%rsp), %rbp
	addq	$24, %rsp
	ret
.LFE94:
	.size	app_window_configure_event_cb, .-app_window_configure_event_cb
	.p2align 4,,15
	.type	main_window_destroy_cb, @function
main_window_destroy_cb:
.LFB61:
	subq	$8, %rsp
.LCFI46:
	call	unset_selected_tweet
	addq	$8, %rsp
	jmp	gtk_main_quit
.LFE61:
	.size	main_window_destroy_cb, .-main_window_destroy_cb
	.section	.rodata.str1.1
.LC23:
	.string	"App"
	.text
	.p2align 4,,15
.globl app_get_type
	.type	app_get_type, @function
app_get_type:
.LFB53:
	subq	$8, %rsp
.LCFI47:
	movq	g_define_type_id__volatile.44596(%rip), %rax
	testq	%rax, %rax
	je	.L118
.L113:
	movq	g_define_type_id__volatile.44596(%rip), %rax
	addq	$8, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L118:
	movl	$g_define_type_id__volatile.44596, %edi
	call	g_once_init_enter_impl
	testl	%eax, %eax
	je	.L113
	movl	$.LC23, %edi
	call	g_intern_static_string
	movl	$app_init, %r9d
	movq	%rax, %rsi
	movl	$24, %r8d
	movl	$app_class_intern_init, %ecx
	movl	$136, %edx
	movl	$80, %edi
	movl	$0, (%rsp)
	call	g_type_register_static_simple
	movl	$g_define_type_id__volatile.44596, %edi
	movq	%rax, %rsi
	call	g_once_init_leave
	movq	g_define_type_id__volatile.44596(%rip), %rax
	addq	$8, %rsp
	ret
.LFE53:
	.size	app_get_type, .-app_get_type
	.p2align 4,,15
	.type	app_class_intern_init, @function
app_class_intern_init:
.LFB52:
	pushq	%rbx
.LCFI48:
	movq	%rdi, %rbx
	call	g_type_class_peek_parent
	movq	%rbx, %rdi
	movl	$80, %esi
	movq	%rax, app_parent_class(%rip)
	call	g_type_check_class_cast
	movq	$app_finalize, 48(%rax)
	movq	%rax, %rdi
	movl	$192, %esi
	popq	%rbx
	jmp	g_type_class_add_private
.LFE52:
	.size	app_class_intern_init, .-app_class_intern_init
	.section	.rodata.str1.8
	.align 8
.LC24:
	.string	"/apps/greet-tweet-know/auth/user_id"
	.align 8
.LC25:
	.string	"/apps/greet-tweet-know/tweets/home_timeline"
	.text
	.p2align 4,,15
	.type	app_login, @function
app_login:
.LFB96:
	pushq	%rbx
.LCFI49:
	subq	$16, %rsp
.LCFI50:
	call	conf_get
	movq	%rax, %rbx
	movq	app_priv(%rip), %rax
	movq	136(%rax), %rdi
	call	g_free
	movq	app_priv(%rip), %rdx
	movq	%rbx, %rdi
	movl	$.LC24, %esi
	movq	$0, 136(%rdx)
	addq	$136, %rdx
	call	conf_get_string
	movq	app_priv(%rip), %rax
	movq	144(%rax), %rdi
	call	g_free
	movq	app_priv(%rip), %rbx
	movq	136(%rbx), %rdi
	movq	$0, 144(%rbx)
	testq	%rdi, %rdi
	je	.L133
	cmpb	$0, (%rdi)
	jne	.L123
.L133:
	movq	$0, 144(%rbx)
.L124:
	movq	136(%rbx), %rdi
	testq	%rdi, %rdi
	je	.L125
	cmpb	$0, (%rdi)
	je	.L125
	movq	144(%rbx), %rsi
	testq	%rsi, %rsi
	je	.L125
	cmpb	$0, (%rsi)
	jne	.L126
.L125:
	call	gtk_window_get_type
	movq	(%rbx), %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	accounts_dialog_show
	addq	$16, %rsp
	popq	%rbx
	ret
	.p2align 4,,10
	.p2align 3
.L123:
	leaq	144(%rbx), %rsi
	call	keyring_get_password
	testl	%eax, %eax
	jne	.L134
	movq	app_priv(%rip), %rbx
	jmp	.L133
	.p2align 4,,10
	.p2align 3
.L126:
	call	network_login
	movq	$0, 8(%rsp)
	call	conf_get
	leaq	8(%rsp), %rdx
	movq	%rax, %rdi
	movl	$.LC25, %esi
	call	conf_get_string
	movq	8(%rsp), %rdi
	testq	%rdi, %rdi
	je	.L128
	cmpb	$0, (%rdi)
	jne	.L129
.L128:
	movl	$.LC22, %edi
	call	g_strdup
	movq	app(%rip), %rdi
	movl	$.LC22, %esi
	movq	%rax, 8(%rsp)
	call	app_set_default_timeline
	movq	8(%rsp), %rdi
.L129:
	call	network_get_timeline
	movq	8(%rsp), %rdi
	call	g_free
	addq	$16, %rsp
	popq	%rbx
	ret
	.p2align 4,,10
	.p2align 3
.L134:
	movq	app_priv(%rip), %rbx
	jmp	.L124
.LFE96:
	.size	app_login, .-app_login
	.p2align 4,,15
	.type	app_connect_cb, @function
app_connect_cb:
.LFB66:
	movq	%rsi, %rdi
	jmp	app_login
.LFE66:
	.size	app_connect_cb, .-app_connect_cb
	.p2align 4,,15
	.type	app_disconnect_cb, @function
app_disconnect_cb:
.LFB67:
	pushq	%rbx
.LCFI51:
	call	tweet_list_get_store
	movq	%rax, %rdi
	call	gtk_list_store_clear
	call	network_logout
	movq	app_priv(%rip), %rax
	movq	24(%rax), %rbx
	testq	%rbx, %rbx
	je	.L138
	.p2align 4,,10
	.p2align 3
.L144:
	movq	(%rbx), %rdi
	xorl	%ecx, %ecx
	xorl	%edx, %edx
	xorl	%eax, %eax
	movl	$.LC12, %esi
	call	g_object_set
	movq	8(%rbx), %rbx
	testq	%rbx, %rbx
	jne	.L144
	movq	app_priv(%rip), %rax
.L138:
	movq	32(%rax), %rbx
	testq	%rbx, %rbx
	je	.L140
	.p2align 4,,10
	.p2align 3
.L143:
	movq	(%rbx), %rdi
	xorl	%ecx, %ecx
	xorl	%eax, %eax
	movl	$1, %edx
	movl	$.LC12, %esi
	call	g_object_set
	movq	8(%rbx), %rbx
	testq	%rbx, %rbx
	jne	.L143
.L140:
	popq	%rbx
	xorl	%edi, %edi
	jmp	g_list_free
.LFE67:
	.size	app_disconnect_cb, .-app_disconnect_cb
	.section	.rodata.str1.1
.LC26:
	.string	"Beginning...."
.LC27:
	.string	"Initialising interface"
.LC28:
	.string	"main_statusbar"
.LC29:
	.string	"main_scrolledwindow"
.LC30:
	.string	"main_window"
.LC31:
	.string	"main-window.ui"
.LC32:
	.string	"expand_image"
.LC33:
	.string	"expand_vbox"
.LC34:
	.string	"expand_box"
.LC35:
	.string	"friends_timelines"
.LC36:
	.string	"view_direct_replies"
.LC37:
	.string	"view_direct_messages"
.LC38:
	.string	"view_my_timeline"
.LC39:
	.string	"view_friends_timeline"
.LC40:
	.string	"view_public_timeline"
.LC41:
	.string	"view_combined_timeline"
.LC42:
	.string	"destroy"
.LC43:
	.string	"activate"
.LC44:
	.string	"help_about"
.LC45:
	.string	"help_contents"
.LC46:
	.string	"changed"
.LC47:
	.string	"preferences"
.LC48:
	.string	"friends_block"
.LC49:
	.string	"friends_unfollow"
.LC50:
	.string	"friends_follow"
.LC51:
	.string	"friends_manager"
.LC52:
	.string	"accounts"
.LC53:
	.string	"tweets_new_dm"
.LC54:
	.string	"tweets_retweet"
.LC55:
	.string	"tweets_reply"
.LC56:
	.string	"tweets_new_tweet"
.LC57:
	.string	"quit"
.LC58:
	.string	"twitter_refresh"
.LC59:
	.string	"twitter_disconnect"
.LC60:
	.string	"twitter_connect"
.LC61:
	.string	"configure_event"
.LC62:
	.string	"delete_event"
.LC63:
	.string	"tweets1"
.LC64:
	.string	"view1"
.LC65:
	.string	"friends"
	.section	.rodata.str1.8
	.align 8
.LC66:
	.string	"Configuring notification area widget..."
	.section	.rodata.str1.1
.LC67:
	.string	"_Show "
.LC68:
	.string	"tray_show_app"
.LC69:
	.string	"toggled"
.LC70:
	.string	"_New Tweet"
.LC71:
	.string	"gtk-new"
.LC72:
	.string	"tray_new_message"
.LC73:
	.string	"New _DM"
.LC74:
	.string	"gtk-leave-fullscreen"
.LC75:
	.string	"tray_new_dm"
.LC76:
	.string	"_About"
.LC77:
	.string	"gtk-about"
.LC78:
	.string	"tray_about"
.LC79:
	.string	"_Quit"
.LC80:
	.string	"gtk-quit"
.LC81:
	.string	"tray_quit"
.LC82:
	.string	"popup_menu"
.LC83:
	.string	"greet-tweet-know/avatars"
.LC84:
	.string	".gnome2"
.LC85:
	.string	"Making directory: %s"
	.section	.rodata.str1.8
	.align 8
.LC86:
	.string	"/apps/greet-tweet-know/auth/auto_login"
	.text
	.p2align 4,,15
.globl app_create
	.type	app_create, @function
app_create:
.LFB91:
	pushq	%r15
.LCFI52:
	pushq	%r14
.LCFI53:
	pushq	%r13
.LCFI54:
	pushq	%r12
.LCFI55:
	pushq	%rbp
.LCFI56:
	pushq	%rbx
.LCFI57:
	subq	$696, %rsp
.LCFI58:
	movq	g_define_type_id__volatile.44596(%rip), %rax
	testq	%rax, %rax
	je	.L165
.L148:
	movq	g_define_type_id__volatile.44596(%rip), %rdi
	xorl	%esi, %esi
	xorl	%eax, %eax
	call	g_object_new
	xorl	%eax, %eax
	movl	$.LC26, %esi
	movl	$.LC5, %edi
	call	debug_impl
	xorl	%eax, %eax
	movl	$.LC27, %esi
	movl	$.LC5, %edi
	call	debug_impl
	movq	app_priv(%rip), %rdx
	leaq	672(%rsp), %r8
	movl	$.LC28, %r9d
	movl	$.LC29, %ecx
	movl	$.LC30, %esi
	movl	$.LC31, %edi
	movq	$0, 168(%rsp)
	movq	$.LC32, 152(%rsp)
	leaq	168(%rdx), %rax
	movq	$.LC33, 136(%rsp)
	movq	$.LC34, 120(%rsp)
	movq	$.LC35, 104(%rsp)
	movq	$.LC36, 88(%rsp)
	movq	%rax, 160(%rsp)
	leaq	664(%rsp), %rax
	movq	$.LC37, 72(%rsp)
	movq	$.LC38, 56(%rsp)
	movq	$.LC39, 40(%rsp)
	movq	%rax, 144(%rsp)
	leaq	160(%rdx), %rax
	movq	$.LC40, 24(%rsp)
	movq	$.LC41, 8(%rsp)
	movq	%rax, 128(%rsp)
	leaq	104(%rdx), %rax
	movq	%rax, 112(%rsp)
	leaq	96(%rdx), %rax
	movq	%rax, 96(%rsp)
	leaq	88(%rdx), %rax
	movq	%rax, 80(%rsp)
	leaq	80(%rdx), %rax
	movq	%rax, 64(%rsp)
	leaq	72(%rdx), %rax
	movq	%rax, 48(%rsp)
	leaq	64(%rdx), %rax
	movq	%rax, 32(%rsp)
	leaq	56(%rdx), %rax
	movq	%rax, 16(%rsp)
	leaq	16(%rdx), %rax
	movq	%rax, (%rsp)
	xorl	%eax, %eax
	call	gtkbuilder_get_file
	movq	%rax, %r13
	movq	$.LC40, 608(%rsp)
	movq	$.LC41, 616(%rsp)
	movq	$.LC39, 624(%rsp)
	movq	$.LC38, 632(%rsp)
	movq	$.LC37, 640(%rsp)
	movq	$.LC36, 648(%rsp)
	call	gtk_radio_action_get_type
	movq	%r13, %rdi
	movq	%rax, %r14
	movl	$.LC40, %esi
	call	gtk_builder_get_object
	movq	%r14, %rsi
	movq	%rax, %rdi
	call	g_type_check_instance_cast
	movq	%rax, %r12
	movq	app_priv(%rip), %rax
	movq	%r12, %rdi
	movq	48(%rax), %rsi
	call	gtk_radio_action_set_group
	movq	app_priv(%rip), %rbx
	movq	%r12, %rdi
	call	gtk_radio_action_get_group
	movq	616(%rsp), %rsi
	movq	%r13, %rdi
	movq	%rax, 48(%rbx)
	call	gtk_builder_get_object
	movq	%r14, %rsi
	movq	%rax, %rdi
	call	g_type_check_instance_cast
	movq	%rax, %r12
	movq	app_priv(%rip), %rax
	movq	%r12, %rdi
	movq	48(%rax), %rsi
	call	gtk_radio_action_set_group
	movq	app_priv(%rip), %rbx
	movq	%r12, %rdi
	call	gtk_radio_action_get_group
	movq	624(%rsp), %rsi
	movq	%r13, %rdi
	movq	%rax, 48(%rbx)
	call	gtk_builder_get_object
	movq	%r14, %rsi
	movq	%rax, %rdi
	call	g_type_check_instance_cast
	movq	%rax, %r12
	movq	app_priv(%rip), %rax
	movq	%r12, %rdi
	movq	48(%rax), %rsi
	call	gtk_radio_action_set_group
	movq	app_priv(%rip), %rbx
	movq	%r12, %rdi
	call	gtk_radio_action_get_group
	movq	632(%rsp), %rsi
	movq	%r13, %rdi
	movq	%rax, 48(%rbx)
	call	gtk_builder_get_object
	movq	%r14, %rsi
	movq	%rax, %rdi
	call	g_type_check_instance_cast
	movq	%rax, %r12
	movq	app_priv(%rip), %rax
	movq	%r12, %rdi
	movq	48(%rax), %rsi
	call	gtk_radio_action_set_group
	movq	app_priv(%rip), %rbx
	movq	%r12, %rdi
	call	gtk_radio_action_get_group
	movq	640(%rsp), %rsi
	movq	%r13, %rdi
	movq	%rax, 48(%rbx)
	call	gtk_builder_get_object
	movq	%r14, %rsi
	movq	%rax, %rdi
	call	g_type_check_instance_cast
	movq	%rax, %r12
	movq	app_priv(%rip), %rax
	movq	%r12, %rdi
	movq	48(%rax), %rsi
	call	gtk_radio_action_set_group
	movq	app_priv(%rip), %rbx
	movq	%r12, %rdi
	call	gtk_radio_action_get_group
	movq	%r13, %rdi
	movl	$.LC36, %esi
	movq	%rax, 48(%rbx)
	call	gtk_builder_get_object
	movq	%r14, %rsi
	movq	%rax, %rdi
	call	g_type_check_instance_cast
	movq	%rax, %r12
	movq	app_priv(%rip), %rax
	movq	%r12, %rdi
	movq	48(%rax), %rsi
	call	gtk_radio_action_set_group
	movq	app_priv(%rip), %rbx
	movq	%r12, %rdi
	call	gtk_radio_action_get_group
	movq	%rax, 48(%rbx)
	call	conf_get
	leaq	656(%rsp), %rdx
	movl	$.LC25, %esi
	movq	%rax, %rdi
	movq	%rax, %rbp
	call	conf_get_string
	movq	656(%rsp), %rsi
	movq	app(%rip), %rdi
	call	app_set_default_timeline
	movq	656(%rsp), %rdi
	call	g_free
	movq	$0, 592(%rsp)
	movq	$app_about_cb, 584(%rsp)
	movl	$.LC30, %r9d
	movq	$.LC43, 576(%rsp)
	movq	$.LC44, 568(%rsp)
	movq	%r9, %rdx
	movq	$app_friends_timelines_cb, 560(%rsp)
	movq	$.LC43, 552(%rsp)
	movl	$main_window_destroy_cb, %r8d
	movq	$.LC35, 544(%rsp)
	movq	$app_help_contents_cb, 536(%rsp)
	movl	$.LC42, %ecx
	movq	$.LC43, 528(%rsp)
	movq	$.LC45, 520(%rsp)
	movq	%r13, %rdi
	movq	$app_view_direct_replies_cb, 512(%rsp)
	movq	$.LC46, 504(%rsp)
	xorl	%eax, %eax
	movq	$.LC36, 496(%rsp)
	movq	$app_view_direct_messages_cb, 488(%rsp)
	movq	$.LC46, 480(%rsp)
	movq	$.LC37, 472(%rsp)
	movq	$app_mine_timeline_cb, 464(%rsp)
	movq	$.LC46, 456(%rsp)
	movq	$.LC38, 448(%rsp)
	movq	$app_friends_timeline_cb, 440(%rsp)
	movq	$.LC46, 432(%rsp)
	movq	$.LC39, 424(%rsp)
	movq	$app_public_timeline_cb, 416(%rsp)
	movq	$.LC46, 408(%rsp)
	movq	$.LC40, 400(%rsp)
	movq	$app_combined_timeline_cb, 392(%rsp)
	movq	$.LC46, 384(%rsp)
	movq	$.LC41, 376(%rsp)
	movq	$app_preferences_cb, 368(%rsp)
	movq	$.LC43, 360(%rsp)
	movq	$.LC47, 352(%rsp)
	movq	$friends_block, 344(%rsp)
	movq	$.LC43, 336(%rsp)
	movq	$.LC48, 328(%rsp)
	movq	$friends_unfollow, 320(%rsp)
	movq	$.LC43, 312(%rsp)
	movq	$.LC49, 304(%rsp)
	movq	$friends_follow, 296(%rsp)
	movq	$.LC43, 288(%rsp)
	movq	$.LC50, 280(%rsp)
	movq	$app_friends_manager_cb, 272(%rsp)
	movq	$.LC43, 264(%rsp)
	movq	$.LC51, 256(%rsp)
	movq	$app_accounts_cb, 248(%rsp)
	movq	$.LC43, 240(%rsp)
	movq	$.LC52, 232(%rsp)
	movq	$tweets_new_dm, 224(%rsp)
	movq	$.LC43, 216(%rsp)
	movq	$.LC53, 208(%rsp)
	movq	$tweets_retweet, 200(%rsp)
	movq	$.LC43, 192(%rsp)
	movq	$.LC54, 184(%rsp)
	movq	$tweets_reply, 176(%rsp)
	movq	$.LC43, 168(%rsp)
	movq	$.LC55, 160(%rsp)
	movq	$tweets_new_tweet, 152(%rsp)
	movq	$.LC43, 144(%rsp)
	movq	$.LC56, 136(%rsp)
	movq	$app_quit_cb, 128(%rsp)
	movq	$.LC43, 120(%rsp)
	movq	$.LC57, 112(%rsp)
	movq	$app_refresh_cb, 104(%rsp)
	movq	$.LC43, 96(%rsp)
	movq	$.LC58, 88(%rsp)
	movq	$app_disconnect_cb, 80(%rsp)
	movq	$.LC43, 72(%rsp)
	movq	$.LC59, 64(%rsp)
	movq	$app_connect_cb, 56(%rsp)
	movq	app(%rip), %rsi
	movq	$.LC43, 48(%rsp)
	movq	$.LC60, 40(%rsp)
	movq	$app_window_configure_event_cb, 32(%rsp)
	movq	$.LC61, 24(%rsp)
	movq	$.LC30, 16(%rsp)
	movq	$main_window_delete_event_cb, 8(%rsp)
	movq	$.LC62, (%rsp)
	call	gtkbuilder_connect
	movq	%r13, %rdi
	movl	$.LC55, %esi
	call	gtk_builder_get_object
	xorl	%edi, %edi
	movq	%rax, %rsi
	call	g_list_prepend
	movq	%r13, %rdi
	movq	%rax, %rbx
	movl	$.LC54, %esi
	call	gtk_builder_get_object
	movq	%rbx, %rdi
	movq	%rax, %rsi
	call	g_list_prepend
	movq	app_priv(%rip), %rdx
	movq	%r13, %rdi
	movl	$.LC59, %esi
	movq	%rax, 40(%rdx)
	call	gtk_builder_get_object
	xorl	%edi, %edi
	movq	%rax, %rsi
	call	g_list_prepend
	movq	%r13, %rdi
	movq	%rax, %rbx
	movl	$.LC63, %esi
	call	gtk_builder_get_object
	movq	%rbx, %rdi
	movq	%rax, %rsi
	call	g_list_prepend
	movq	%r13, %rdi
	movq	%rax, %rbx
	movl	$.LC64, %esi
	call	gtk_builder_get_object
	movq	%rbx, %rdi
	movq	%rax, %rsi
	call	g_list_prepend
	movq	%r13, %rdi
	movq	%rax, %rbx
	movl	$.LC65, %esi
	call	gtk_builder_get_object
	movq	%rbx, %rdi
	movq	%rax, %rsi
	call	g_list_prepend
	movq	app_priv(%rip), %rdx
	movq	%r13, %rdi
	movl	$.LC60, %esi
	movq	%rax, 24(%rdx)
	call	gtk_builder_get_object
	xorl	%edi, %edi
	movq	%rax, %rsi
	call	g_list_prepend
	movq	app_priv(%rip), %rdx
	movq	%r13, %rdi
	movq	%rax, 32(%rdx)
	call	g_object_unref
	call	gtk_widget_get_type
	movq	%rax, %r13
	movq	%rax, %rsi
	movq	app_priv(%rip), %rax
	movq	(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_widget_hide
	xorl	%eax, %eax
	movl	$.LC66, %esi
	movl	$.LC5, %edi
	call	debug_impl
	movq	app_priv(%rip), %rbx
	xorl	%edi, %edi
	movl	$5, %edx
	movl	$.LC67, %esi
	call	dcgettext
	xorl	%ecx, %ecx
	movq	%rax, %rsi
	xorl	%edx, %edx
	movl	$.LC68, %edi
	call	gtk_toggle_action_new
	movq	%rax, 128(%rbx)
	movq	app_priv(%rip), %rax
	movl	$80, %esi
	movq	app(%rip), %rbx
	movq	128(%rax), %rdi
	call	g_type_check_instance_cast
	xorl	%r9d, %r9d
	xorl	%r8d, %r8d
	movq	%rbx, %rcx
	movq	%rax, %rdi
	movl	$app_show_hide_cb, %edx
	movl	$.LC69, %esi
	call	g_signal_connect_data
	xorl	%edi, %edi
	movl	$5, %edx
	movl	$.LC70, %esi
	call	dcgettext
	xorl	%edx, %edx
	movl	$.LC71, %ecx
	movq	%rax, %rsi
	movl	$.LC72, %edi
	call	gtk_action_new
	movq	app(%rip), %rbx
	movl	$80, %esi
	movq	%rax, %rdi
	movq	%rax, %r15
	call	g_type_check_instance_cast
	xorl	%r9d, %r9d
	xorl	%r8d, %r8d
	movq	%rbx, %rcx
	movq	%rax, %rdi
	movl	$tweets_new_tweet, %edx
	movl	$.LC43, %esi
	call	g_signal_connect_data
	xorl	%edi, %edi
	movl	$5, %edx
	movl	$.LC73, %esi
	call	dcgettext
	xorl	%edx, %edx
	movl	$.LC74, %ecx
	movq	%rax, %rsi
	movl	$.LC75, %edi
	call	gtk_action_new
	movq	app(%rip), %rbx
	movl	$80, %esi
	movq	%rax, %rdi
	movq	%rax, %r12
	call	g_type_check_instance_cast
	xorl	%r9d, %r9d
	xorl	%r8d, %r8d
	movq	%rbx, %rcx
	movq	%rax, %rdi
	movl	$tweets_new_dm, %edx
	movl	$.LC43, %esi
	call	g_signal_connect_data
	xorl	%edi, %edi
	movl	$5, %edx
	movl	$.LC76, %esi
	call	dcgettext
	xorl	%edx, %edx
	movl	$.LC77, %ecx
	movq	%rax, %rsi
	movl	$.LC78, %edi
	call	gtk_action_new
	movq	app(%rip), %rbx
	movq	%r12, %rdi
	movl	$80, %esi
	call	g_type_check_instance_cast
	xorl	%r9d, %r9d
	xorl	%r8d, %r8d
	movq	%rax, %rdi
	movq	%rbx, %rcx
	movl	$app_about_cb, %edx
	movl	$.LC43, %esi
	call	g_signal_connect_data
	xorl	%edi, %edi
	movl	$5, %edx
	movl	$.LC79, %esi
	call	dcgettext
	movl	$.LC80, %ecx
	xorl	%edx, %edx
	movq	%rax, %rsi
	movl	$.LC81, %edi
	call	gtk_action_new
	movq	app(%rip), %rbx
	movl	$80, %esi
	movq	%rax, %rdi
	movq	%rax, %r14
	call	g_type_check_instance_cast
	xorl	%r9d, %r9d
	xorl	%r8d, %r8d
	movq	%rbx, %rcx
	movl	$app_quit_cb, %edx
	movq	%rax, %rdi
	movl	$.LC43, %esi
	call	g_signal_connect_data
	movq	app_priv(%rip), %rbx
	call	gtk_menu_new
	movq	%rax, 120(%rbx)
	call	gtk_action_get_type
	movq	%rax, %rsi
	movq	app_priv(%rip), %rax
	movq	128(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_action_create_menu_item
	movq	%rax, %rbx
	call	gtk_menu_shell_get_type
	movq	%rax, %r12
	movq	app_priv(%rip), %rax
	movq	%r12, %rsi
	movq	120(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%rbx, %rsi
	movq	%rax, %rdi
	call	gtk_menu_shell_append
	call	gtk_separator_menu_item_new
	movq	%rax, %rdi
	movq	%rax, %rbx
	call	gtk_widget_show
	movq	app_priv(%rip), %rax
	movq	%r12, %rsi
	movq	120(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%rbx, %rsi
	movq	%rax, %rdi
	call	gtk_menu_shell_append
	movq	%r15, %rdi
	call	gtk_action_create_menu_item
	movq	%rax, %rbx
	movq	app_priv(%rip), %rax
	movq	%r12, %rsi
	movq	120(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%rbx, %rsi
	movq	%rax, %rdi
	call	gtk_menu_shell_append
	movq	%r14, %rdi
	call	gtk_action_create_menu_item
	movq	%rax, %rbx
	movq	app_priv(%rip), %rax
	movq	%r12, %rsi
	movq	120(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%rbx, %rsi
	movq	%rax, %rdi
	call	gtk_menu_shell_append
	movq	app_priv(%rip), %rbx
	movl	$.LC2, %edi
	call	gtk_status_icon_new_from_icon_name
	movq	app(%rip), %rcx
	xorl	%r9d, %r9d
	xorl	%r8d, %r8d
	movq	%rax, 112(%rbx)
	movq	app_priv(%rip), %rax
	movl	$app_status_icon_activate_cb, %edx
	movl	$.LC43, %esi
	movq	112(%rax), %rdi
	call	g_signal_connect_data
	movq	app_priv(%rip), %rax
	movq	app(%rip), %rcx
	xorl	%r9d, %r9d
	xorl	%r8d, %r8d
	movl	$app_status_icon_popup_menu_cb, %edx
	movl	$.LC82, %esi
	movq	112(%rax), %rdi
	call	g_signal_connect_data
	movq	app_priv(%rip), %rax
	movl	$1, %esi
	movq	112(%rax), %rdi
	call	gtk_status_icon_set_visible
	movq	app_priv(%rip), %rax
	movq	(%rax), %rdi
	call	geometry_load_for_main_window
	movq	app_priv(%rip), %rbx
	call	tweet_list_new
	movq	%r13, %rsi
	movq	%rax, 8(%rbx)
	movq	app_priv(%rip), %rax
	movq	8(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_widget_show
	movq	app_priv(%rip), %rax
	movq	%r13, %rsi
	movq	8(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%rax, %rbx
	call	gtk_container_get_type
	movq	672(%rsp), %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	movq	%rbx, %rsi
	movq	%rax, %rdi
	call	gtk_container_add
	movq	app_priv(%rip), %rbx
	call	label_new
	movq	%r13, %rsi
	movq	%rax, 176(%rbx)
	movq	app_priv(%rip), %rax
	movq	176(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_widget_show
	movq	app_priv(%rip), %rax
	movq	%r13, %rsi
	movq	176(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%rax, %rbx
	call	gtk_box_get_type
	movq	664(%rsp), %rdi
	movq	%rax, %rsi
	movq	%rax, %r12
	call	g_type_check_instance_cast
	xorl	%r8d, %r8d
	movl	$1, %ecx
	movl	$1, %edx
	movq	%rbx, %rsi
	movq	%rax, %rdi
	call	gtk_box_pack_start
	movq	app_priv(%rip), %rbx
	call	label_new
	movq	%r13, %rsi
	movq	%rax, 184(%rbx)
	movq	app_priv(%rip), %rax
	movq	184(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_widget_show
	movq	app_priv(%rip), %rax
	movq	%r13, %rsi
	movq	184(%rax), %rdi
	call	g_type_check_instance_cast
	movq	664(%rsp), %rdi
	movq	%rax, %rbx
	movq	%r12, %rsi
	call	g_type_check_instance_cast
	xorl	%r8d, %r8d
	movq	%rax, %rdi
	movl	$1, %ecx
	movl	$1, %edx
	movq	%rbx, %rsi
	call	gtk_box_pack_end
	movq	app_priv(%rip), %rax
	movq	%r13, %rsi
	movq	160(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_widget_hide
	movq	app_priv(%rip), %rax
	movq	24(%rax), %rbx
	testq	%rbx, %rbx
	je	.L150
	.p2align 4,,10
	.p2align 3
.L161:
	movq	(%rbx), %rdi
	xorl	%ecx, %ecx
	xorl	%edx, %edx
	xorl	%eax, %eax
	movl	$.LC12, %esi
	call	g_object_set
	movq	8(%rbx), %rbx
	testq	%rbx, %rbx
	jne	.L161
	movq	app_priv(%rip), %rax
.L150:
	movq	32(%rax), %rbx
	testq	%rbx, %rbx
	je	.L152
	.p2align 4,,10
	.p2align 3
.L160:
	movq	(%rbx), %rdi
	xorl	%ecx, %ecx
	xorl	%eax, %eax
	movl	$1, %edx
	movl	$.LC12, %esi
	call	g_object_set
	movq	8(%rbx), %rbx
	testq	%rbx, %rbx
	jne	.L160
.L152:
	xorl	%edi, %edi
	call	g_list_free
	call	g_get_home_dir
	xorl	%ecx, %ecx
	movq	%rax, %rdi
	movl	$.LC83, %edx
	movl	$.LC84, %esi
	xorl	%eax, %eax
	call	g_build_filename
	movl	$20, %esi
	movq	%rax, %rdi
	movq	%rax, %rbx
	call	g_file_test
	testl	%eax, %eax
	je	.L166
.L154:
	movq	%rbx, %rdi
	call	g_free
	leaq	680(%rsp), %rdx
	movl	$.LC19, %esi
	movq	%rbp, %rdi
	call	conf_get_bool
	movl	680(%rsp), %esi
	testl	%esi, %esi
	jne	.L155
	movq	app_priv(%rip), %rax
	movq	(%rax), %rdi
	call	gtk_widget_show
.L156:
	leaq	684(%rsp), %rdx
	movl	$.LC86, %esi
	movq	%rbp, %rdi
	call	conf_get_bool
	movl	684(%rsp), %ecx
	testl	%ecx, %ecx
	je	.L157
	movq	app(%rip), %rdi
	call	app_login
.L157:
	call	unset_selected_tweet
	xorl	%edi, %edi
	call	show_tweets_submenu_entries
	addq	$696, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	ret
	.p2align 4,,10
	.p2align 3
.L155:
	movq	app_priv(%rip), %rax
	movq	(%rax), %rdi
	call	gtk_widget_hide
	jmp	.L156
	.p2align 4,,10
	.p2align 3
.L166:
	movq	%rbx, %rdx
	movl	$.LC85, %esi
	movl	$.LC5, %edi
	call	debug_impl
	movl	$448, %esi
	movq	%rbx, %rdi
	call	g_mkdir_with_parents
	jmp	.L154
	.p2align 4,,10
	.p2align 3
.L165:
	movl	$g_define_type_id__volatile.44596, %edi
	call	g_once_init_enter_impl
	testl	%eax, %eax
	je	.L148
	movl	$.LC23, %edi
	call	g_intern_static_string
	movl	$app_init, %r9d
	movq	%rax, %rsi
	movl	$24, %r8d
	movl	$app_class_intern_init, %ecx
	movl	$136, %edx
	movl	$80, %edi
	movl	$0, (%rsp)
	call	g_type_register_static_simple
	movl	$g_define_type_id__volatile.44596, %edi
	movq	%rax, %rsi
	call	g_once_init_leave
	jmp	.L148
.LFE91:
	.size	app_create, .-app_create
	.p2align 4,,15
	.type	app_init, @function
app_init:
.LFB55:
	subq	$8, %rsp
.LCFI59:
	movq	g_define_type_id__volatile.44596(%rip), %rax
	movq	%rdi, app(%rip)
	testq	%rax, %rax
	je	.L174
.L168:
	movq	g_define_type_id__volatile.44596(%rip), %rsi
	call	g_type_instance_get_private
	movq	%rax, app_priv(%rip)
	movq	$0, 24(%rax)
	movq	$0, 32(%rax)
	movq	$0, 40(%rax)
	movq	$0, 48(%rax)
	addq	$8, %rsp
	jmp	unset_selected_tweet
	.p2align 4,,10
	.p2align 3
.L174:
	movl	$g_define_type_id__volatile.44596, %edi
	call	g_once_init_enter_impl
	testl	%eax, %eax
	je	.L173
	movl	$.LC23, %edi
	call	g_intern_static_string
	movl	$80, %edi
	movq	%rax, %rsi
	movl	$app_init, %r9d
	movl	$24, %r8d
	movl	$app_class_intern_init, %ecx
	movl	$136, %edx
	movl	$0, (%rsp)
	call	g_type_register_static_simple
	movl	$g_define_type_id__volatile.44596, %edi
	movq	%rax, %rsi
	call	g_once_init_leave
.L173:
	movq	app(%rip), %rdi
	jmp	.L168
.LFE55:
	.size	app_init, .-app_init
	.p2align 4,,15
	.type	app_finalize, @function
app_finalize:
.LFB56:
	pushq	%rbp
.LCFI60:
	movq	%rdi, %rbp
	pushq	%rbx
.LCFI61:
	subq	$8, %rsp
.LCFI62:
	movq	g_define_type_id__volatile.44596(%rip), %rax
	testq	%rax, %rax
	je	.L186
.L176:
	movq	g_define_type_id__volatile.44596(%rip), %rsi
	movq	%rbp, %rdi
	call	g_type_check_instance_cast
	movq	%rax, %rbx
	movq	g_define_type_id__volatile.44596(%rip), %rax
	testq	%rax, %rax
	je	.L187
.L178:
	movq	g_define_type_id__volatile.44596(%rip), %rsi
	movq	%rbx, %rdi
	call	g_type_instance_get_private
	movl	152(%rax), %edi
	movq	%rax, app_priv(%rip)
	testl	%edi, %edi
	je	.L180
	call	g_source_remove
	movq	app_priv(%rip), %rax
.L180:
	movq	24(%rax), %rdi
	call	g_list_free
	movq	app_priv(%rip), %rax
	movq	32(%rax), %rdi
	call	g_list_free
	movq	app_priv(%rip), %rax
	movq	48(%rax), %rdi
	call	g_slist_free
	call	conf_shutdown
	movq	app_parent_class(%rip), %rdi
	movl	$80, %esi
	call	g_type_check_class_cast
	movq	48(%rax), %r11
	addq	$8, %rsp
	movq	%rbp, %rdi
	popq	%rbx
	popq	%rbp
	jmp	*%r11
	.p2align 4,,10
	.p2align 3
.L187:
	movl	$g_define_type_id__volatile.44596, %edi
	call	g_once_init_enter_impl
	testl	%eax, %eax
	je	.L178
	movl	$.LC23, %edi
	call	g_intern_static_string
	movl	$app_init, %r9d
	movq	%rax, %rsi
	movl	$24, %r8d
	movl	$app_class_intern_init, %ecx
	movl	$136, %edx
	movl	$80, %edi
	movl	$0, (%rsp)
	call	g_type_register_static_simple
	movl	$g_define_type_id__volatile.44596, %edi
	movq	%rax, %rsi
	call	g_once_init_leave
	jmp	.L178
	.p2align 4,,10
	.p2align 3
.L186:
	movl	$g_define_type_id__volatile.44596, %edi
	call	g_once_init_enter_impl
	testl	%eax, %eax
	je	.L176
	movl	$.LC23, %edi
	call	g_intern_static_string
	movl	$app_init, %r9d
	movq	%rax, %rsi
	movl	$24, %r8d
	movl	$app_class_intern_init, %ecx
	movl	$136, %edx
	movl	$80, %edi
	movl	$0, (%rsp)
	call	g_type_register_static_simple
	movl	$g_define_type_id__volatile.44596, %edi
	movq	%rax, %rsi
	call	g_once_init_leave
	jmp	.L176
.LFE56:
	.size	app_finalize, .-app_finalize
	.section	.rodata.str1.8
	.align 8
.LC87:
	.string	"Click on the notification area icon to show Greet-Tweet-Know."
	.align 8
.LC88:
	.string	"Greet-Tweet-Know is still running, it is just hidden."
	.align 8
.LC89:
	.string	"/apps/greet-tweet-know/hints/close_main_window"
	.align 8
.LC90:
	.string	"Since no system or notification tray has been found, this action would normally quit Greet-Tweet-Know.\n\nThis is just a reminder, from now on, Greet-Tweet-Know will quit when performing this action unless you uncheck the option below."
	.section	.rodata.str1.1
.LC91:
	.string	"You were about to quit!"
	.text
	.p2align 4,,15
	.type	main_window_delete_event_cb, @function
main_window_delete_event_cb:
.LFB62:
	pushq	%r12
.LCFI63:
	pushq	%rbx
.LCFI64:
	subq	$8, %rsp
.LCFI65:
	movq	app_priv(%rip), %rax
	movq	112(%rax), %rdi
	call	gtk_status_icon_is_embedded
	testl	%eax, %eax
	jne	.L192
	call	gtk_window_get_type
	movq	%rax, %rsi
	movq	app_priv(%rip), %rax
	movq	(%rax), %rdi
	call	g_type_check_instance_cast
	xorl	%edi, %edi
	movl	$5, %edx
	movl	$.LC90, %esi
	movq	%rax, %r12
	call	dcgettext
	xorl	%edi, %edi
	movq	%rax, %rbx
	movl	$5, %edx
	movl	$.LC91, %esi
	call	dcgettext
	xorl	%r9d, %r9d
	xorl	%r8d, %r8d
	movq	%r12, %rcx
	movq	%rbx, %rdx
	movq	%rax, %rsi
	movl	$.LC89, %edi
	call	hint_dialog_show
	testl	%eax, %eax
	setne	%al
	addq	$8, %rsp
	popq	%rbx
	movzbl	%al, %eax
	popq	%r12
	ret
	.p2align 4,,10
	.p2align 3
.L192:
	call	gtk_window_get_type
	movq	%rax, %rsi
	movq	app_priv(%rip), %rax
	movq	(%rax), %rdi
	call	g_type_check_instance_cast
	movl	$5, %edx
	movl	$.LC87, %esi
	xorl	%edi, %edi
	movq	%rax, %r12
	call	dcgettext
	movl	$5, %edx
	movq	%rax, %rbx
	movl	$.LC88, %esi
	xorl	%edi, %edi
	call	dcgettext
	movq	%r12, %rcx
	xorl	%r9d, %r9d
	xorl	%r8d, %r8d
	movq	%rbx, %rdx
	movq	%rax, %rsi
	movl	$.LC89, %edi
	call	hint_show
	movq	app_priv(%rip), %rax
	movq	(%rax), %rbx
	call	conf_get
	movl	$1, %edx
	movq	%rax, %rdi
	movl	$.LC19, %esi
	call	conf_set_bool
	movq	%rbx, %rdi
	call	gtk_widget_hide
	addq	$8, %rsp
	movl	$1, %eax
	popq	%rbx
	popq	%r12
	ret
.LFE62:
	.size	main_window_delete_event_cb, .-main_window_delete_event_cb
.globl app_priv
	.bss
	.align 8
	.type	app_priv, @object
	.size	app_priv, 8
app_priv:
	.zero	8
	.local	app
	.comm	app,8,8
	.section	.rodata
	.type	__PRETTY_FUNCTION__.45069, @object
	.size	__PRETTY_FUNCTION__.45069, 8
__PRETTY_FUNCTION__.45069:
	.string	"app_get"
	.local	g_define_type_id__volatile.44596
	.comm	g_define_type_id__volatile.44596,8,8
	.local	app_parent_class
	.comm	app_parent_class,8,8
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
	.long	.LFB102
	.long	.LFE102-.LFB102
	.uleb128 0x0
	.align 8
.LEFDE1:
.LSFDE3:
	.long	.LEFDE3-.LASFDE3
.LASFDE3:
	.long	.LASFDE3-.Lframe1
	.long	.LFB108
	.long	.LFE108-.LFB108
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI4-.LFB108
	.byte	0xe
	.uleb128 0x30
	.byte	0x8d
	.uleb128 0x2
	.byte	0x8c
	.uleb128 0x3
	.byte	0x86
	.uleb128 0x4
	.byte	0x83
	.uleb128 0x5
	.align 8
.LEFDE3:
.LSFDE5:
	.long	.LEFDE5-.LASFDE5
.LASFDE5:
	.long	.LASFDE5-.Lframe1
	.long	.LFB107
	.long	.LFE107-.LFB107
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI5-.LFB107
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
	.long	.LFB106
	.long	.LFE106-.LFB106
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI6-.LFB106
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI7-.LCFI6
	.byte	0xe
	.uleb128 0x20
	.align 8
.LEFDE7:
.LSFDE9:
	.long	.LEFDE9-.LASFDE9
.LASFDE9:
	.long	.LASFDE9-.Lframe1
	.long	.LFB105
	.long	.LFE105-.LFB105
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI10-.LFB105
	.byte	0xe
	.uleb128 0x40
	.byte	0x8c
	.uleb128 0x2
	.byte	0x83
	.uleb128 0x3
	.align 8
.LEFDE9:
.LSFDE11:
	.long	.LEFDE11-.LASFDE11
.LASFDE11:
	.long	.LASFDE11-.Lframe1
	.long	.LFB103
	.long	.LFE103-.LFB103
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI12-.LFB103
	.byte	0x83
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI14-.LCFI12
	.byte	0xe
	.uleb128 0x20
	.byte	0x86
	.uleb128 0x3
	.align 8
.LEFDE11:
.LSFDE13:
	.long	.LEFDE13-.LASFDE13
.LASFDE13:
	.long	.LASFDE13-.Lframe1
	.long	.LFB101
	.long	.LFE101-.LFB101
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI15-.LFB101
	.byte	0xe
	.uleb128 0x10
	.byte	0x86
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI16-.LCFI15
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI17-.LCFI16
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x3
	.align 8
.LEFDE13:
.LSFDE15:
	.long	.LEFDE15-.LASFDE15
.LASFDE15:
	.long	.LASFDE15-.Lframe1
	.long	.LFB92
	.long	.LFE92-.LFB92
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI18-.LFB92
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE15:
.LSFDE17:
	.long	.LEFDE17-.LASFDE17
.LASFDE17:
	.long	.LASFDE17-.Lframe1
	.long	.LFB97
	.long	.LFE97-.LFB97
	.uleb128 0x0
	.align 8
.LEFDE17:
.LSFDE19:
	.long	.LEFDE19-.LASFDE19
.LASFDE19:
	.long	.LASFDE19-.Lframe1
	.long	.LFB93
	.long	.LFE93-.LFB93
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI19-.LFB93
	.byte	0xe
	.uleb128 0x10
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI20-.LCFI19
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI21-.LCFI20
	.byte	0xe
	.uleb128 0x30
	.byte	0x83
	.uleb128 0x3
	.align 8
.LEFDE19:
.LSFDE21:
	.long	.LEFDE21-.LASFDE21
.LASFDE21:
	.long	.LASFDE21-.Lframe1
	.long	.LFB64
	.long	.LFE64-.LFB64
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI22-.LFB64
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI23-.LCFI22
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI24-.LCFI23
	.byte	0xe
	.uleb128 0x30
	.byte	0x83
	.uleb128 0x3
	.byte	0x86
	.uleb128 0x2
	.align 8
.LEFDE21:
.LSFDE23:
	.long	.LEFDE23-.LASFDE23
.LASFDE23:
	.long	.LASFDE23-.Lframe1
	.long	.LFB86
	.long	.LFE86-.LFB86
	.uleb128 0x0
	.align 8
.LEFDE23:
.LSFDE25:
	.long	.LEFDE25-.LASFDE25
.LASFDE25:
	.long	.LASFDE25-.Lframe1
	.long	.LFB87
	.long	.LFE87-.LFB87
	.uleb128 0x0
	.align 8
.LEFDE25:
.LSFDE27:
	.long	.LEFDE27-.LASFDE27
.LASFDE27:
	.long	.LASFDE27-.Lframe1
	.long	.LFB65
	.long	.LFE65-.LFB65
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI27-.LFB65
	.byte	0xe
	.uleb128 0x20
	.byte	0x86
	.uleb128 0x2
	.byte	0x83
	.uleb128 0x3
	.align 8
.LEFDE27:
.LSFDE29:
	.long	.LEFDE29-.LASFDE29
.LASFDE29:
	.long	.LASFDE29-.Lframe1
	.long	.LFB88
	.long	.LFE88-.LFB88
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI29-.LFB88
	.byte	0x8c
	.uleb128 0x4
	.byte	0x83
	.uleb128 0x5
	.byte	0x4
	.long	.LCFI32-.LCFI29
	.byte	0xe
	.uleb128 0x30
	.byte	0x8e
	.uleb128 0x2
	.byte	0x8d
	.uleb128 0x3
	.align 8
.LEFDE29:
.LSFDE31:
	.long	.LEFDE31-.LASFDE31
.LASFDE31:
	.long	.LASFDE31-.Lframe1
	.long	.LFB75
	.long	.LFE75-.LFB75
	.uleb128 0x0
	.align 8
.LEFDE31:
.LSFDE33:
	.long	.LEFDE33-.LASFDE33
.LASFDE33:
	.long	.LASFDE33-.Lframe1
	.long	.LFB74
	.long	.LFE74-.LFB74
	.uleb128 0x0
	.align 8
.LEFDE33:
.LSFDE35:
	.long	.LEFDE35-.LASFDE35
.LASFDE35:
	.long	.LASFDE35-.Lframe1
	.long	.LFB72
	.long	.LFE72-.LFB72
	.uleb128 0x0
	.align 8
.LEFDE35:
.LSFDE37:
	.long	.LEFDE37-.LASFDE37
.LASFDE37:
	.long	.LASFDE37-.Lframe1
	.long	.LFB71
	.long	.LFE71-.LFB71
	.uleb128 0x0
	.align 8
.LEFDE37:
.LSFDE39:
	.long	.LEFDE39-.LASFDE39
.LASFDE39:
	.long	.LASFDE39-.Lframe1
	.long	.LFB81
	.long	.LFE81-.LFB81
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI33-.LFB81
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE39:
.LSFDE41:
	.long	.LEFDE41-.LASFDE41
.LASFDE41:
	.long	.LASFDE41-.Lframe1
	.long	.LFB76
	.long	.LFE76-.LFB76
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI34-.LFB76
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE41:
.LSFDE43:
	.long	.LEFDE43-.LASFDE43
.LASFDE43:
	.long	.LASFDE43-.Lframe1
	.long	.LFB82
	.long	.LFE82-.LFB82
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI35-.LFB82
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE43:
.LSFDE45:
	.long	.LEFDE45-.LASFDE45
.LASFDE45:
	.long	.LASFDE45-.Lframe1
	.long	.LFB73
	.long	.LFE73-.LFB73
	.uleb128 0x0
	.align 8
.LEFDE45:
.LSFDE47:
	.long	.LEFDE47-.LASFDE47
.LASFDE47:
	.long	.LASFDE47-.Lframe1
	.long	.LFB70
	.long	.LFE70-.LFB70
	.uleb128 0x0
	.align 8
.LEFDE47:
.LSFDE49:
	.long	.LEFDE49-.LASFDE49
.LASFDE49:
	.long	.LASFDE49-.Lframe1
	.long	.LFB80
	.long	.LFE80-.LFB80
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI36-.LFB80
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE49:
.LSFDE51:
	.long	.LEFDE51-.LASFDE51
.LASFDE51:
	.long	.LASFDE51-.Lframe1
	.long	.LFB85
	.long	.LFE85-.LFB85
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI37-.LFB85
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE51:
.LSFDE53:
	.long	.LEFDE53-.LASFDE53
.LASFDE53:
	.long	.LASFDE53-.Lframe1
	.long	.LFB84
	.long	.LFE84-.LFB84
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI38-.LFB84
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE53:
.LSFDE55:
	.long	.LEFDE55-.LASFDE55
.LASFDE55:
	.long	.LASFDE55-.Lframe1
	.long	.LFB83
	.long	.LFE83-.LFB83
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI39-.LFB83
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE55:
.LSFDE57:
	.long	.LEFDE57-.LASFDE57
.LASFDE57:
	.long	.LASFDE57-.Lframe1
	.long	.LFB79
	.long	.LFE79-.LFB79
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI40-.LFB79
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE57:
.LSFDE59:
	.long	.LEFDE59-.LASFDE59
.LASFDE59:
	.long	.LASFDE59-.Lframe1
	.long	.LFB78
	.long	.LFE78-.LFB78
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI41-.LFB78
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE59:
.LSFDE61:
	.long	.LEFDE61-.LASFDE61
.LASFDE61:
	.long	.LASFDE61-.Lframe1
	.long	.LFB68
	.long	.LFE68-.LFB68
	.uleb128 0x0
	.align 8
.LEFDE61:
.LSFDE63:
	.long	.LEFDE63-.LASFDE63
.LASFDE63:
	.long	.LASFDE63-.Lframe1
	.long	.LFB69
	.long	.LFE69-.LFB69
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI42-.LFB69
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE63:
.LSFDE65:
	.long	.LEFDE65-.LASFDE65
.LASFDE65:
	.long	.LASFDE65-.Lframe1
	.long	.LFB94
	.long	.LFE94-.LFB94
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI45-.LFB94
	.byte	0xe
	.uleb128 0x20
	.byte	0x86
	.uleb128 0x2
	.byte	0x83
	.uleb128 0x3
	.align 8
.LEFDE65:
.LSFDE67:
	.long	.LEFDE67-.LASFDE67
.LASFDE67:
	.long	.LASFDE67-.Lframe1
	.long	.LFB61
	.long	.LFE61-.LFB61
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI46-.LFB61
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE67:
.LSFDE69:
	.long	.LEFDE69-.LASFDE69
.LASFDE69:
	.long	.LASFDE69-.Lframe1
	.long	.LFB53
	.long	.LFE53-.LFB53
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI47-.LFB53
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE69:
.LSFDE71:
	.long	.LEFDE71-.LASFDE71
.LASFDE71:
	.long	.LASFDE71-.Lframe1
	.long	.LFB52
	.long	.LFE52-.LFB52
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI48-.LFB52
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE71:
.LSFDE73:
	.long	.LEFDE73-.LASFDE73
.LASFDE73:
	.long	.LASFDE73-.Lframe1
	.long	.LFB96
	.long	.LFE96-.LFB96
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI49-.LFB96
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI50-.LCFI49
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE73:
.LSFDE75:
	.long	.LEFDE75-.LASFDE75
.LASFDE75:
	.long	.LASFDE75-.Lframe1
	.long	.LFB66
	.long	.LFE66-.LFB66
	.uleb128 0x0
	.align 8
.LEFDE75:
.LSFDE77:
	.long	.LEFDE77-.LASFDE77
.LASFDE77:
	.long	.LASFDE77-.Lframe1
	.long	.LFB67
	.long	.LFE67-.LFB67
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI51-.LFB67
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE77:
.LSFDE79:
	.long	.LEFDE79-.LASFDE79
.LASFDE79:
	.long	.LASFDE79-.Lframe1
	.long	.LFB91
	.long	.LFE91-.LFB91
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI52-.LFB91
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI53-.LCFI52
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI54-.LCFI53
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI55-.LCFI54
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.long	.LCFI56-.LCFI55
	.byte	0xe
	.uleb128 0x30
	.byte	0x4
	.long	.LCFI57-.LCFI56
	.byte	0xe
	.uleb128 0x38
	.byte	0x4
	.long	.LCFI58-.LCFI57
	.byte	0xe
	.uleb128 0x2f0
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
.LEFDE79:
.LSFDE81:
	.long	.LEFDE81-.LASFDE81
.LASFDE81:
	.long	.LASFDE81-.Lframe1
	.long	.LFB55
	.long	.LFE55-.LFB55
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI59-.LFB55
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE81:
.LSFDE83:
	.long	.LEFDE83-.LASFDE83
.LASFDE83:
	.long	.LASFDE83-.Lframe1
	.long	.LFB56
	.long	.LFE56-.LFB56
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI60-.LFB56
	.byte	0xe
	.uleb128 0x10
	.byte	0x86
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI61-.LCFI60
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI62-.LCFI61
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x3
	.align 8
.LEFDE83:
.LSFDE85:
	.long	.LEFDE85-.LASFDE85
.LASFDE85:
	.long	.LASFDE85-.Lframe1
	.long	.LFB62
	.long	.LFE62-.LFB62
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI63-.LFB62
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI64-.LCFI63
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI65-.LCFI64
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x3
	.byte	0x8c
	.uleb128 0x2
	.align 8
.LEFDE85:
	.ident	"GCC: (SUSE Linux) 4.3.2 [gcc-4_3-branch revision 141291]"
	.section	.comment.SUSE.OPTs,"MS",@progbits,1
	.ascii	"OspWg"
	.section	.note.GNU-stack,"",@progbits
