	.file	"preferences.c"
	.text
	.p2align 4,,15
	.type	preferences_notify_bool_cb, @function
preferences_notify_bool_cb:
.LFB52:
	movq	%rbx, -16(%rsp)
.LCFI0:
	movq	%rbp, -8(%rsp)
.LCFI1:
	subq	$40, %rsp
.LCFI2:
	movq	%rsi, %rbx
	movq	%rdx, %rbp
	call	conf_get
	leaq	20(%rsp), %rdx
	movq	%rax, %rdi
	movq	%rbx, %rsi
	call	conf_get_bool
	testl	%eax, %eax
	je	.L3
	movl	20(%rsp), %ebx
	call	gtk_toggle_button_get_type
	movq	%rbp, %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	movl	%ebx, %esi
	movq	%rax, %rdi
	call	gtk_toggle_button_set_active
.L3:
	movq	24(%rsp), %rbx
	movq	32(%rsp), %rbp
	addq	$40, %rsp
	ret
.LFE52:
	.size	preferences_notify_bool_cb, .-preferences_notify_bool_cb
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"key"
	.text
	.p2align 4,,15
	.type	preferences_toggle_button_toggled_cb, @function
preferences_toggle_button_toggled_cb:
.LFB66:
	pushq	%r12
.LCFI3:
	movl	$80, %esi
	pushq	%rbx
.LCFI4:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI5:
	call	g_type_check_instance_cast
	movl	$.LC0, %esi
	movq	%rax, %rdi
	call	g_object_get_data
	movq	%rax, %r12
	call	gtk_toggle_button_get_type
	movq	%rbx, %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_toggle_button_get_active
	movl	%eax, %ebx
	call	conf_get
	addq	$8, %rsp
	movl	%ebx, %edx
	movq	%r12, %rsi
	popq	%rbx
	popq	%r12
	movq	%rax, %rdi
	jmp	conf_set_bool
.LFE66:
	.size	preferences_toggle_button_toggled_cb, .-preferences_toggle_button_toggled_cb
	.p2align 4,,15
	.type	preferences_notify_sensitivity_cb, @function
preferences_notify_sensitivity_cb:
.LFB60:
	movq	%rbp, -8(%rsp)
.LCFI6:
	movq	%rbx, -16(%rsp)
.LCFI7:
	subq	$40, %rsp
.LCFI8:
	movq	%rdx, %rbp
	leaq	20(%rsp), %rdx
	call	conf_get_bool
	testl	%eax, %eax
	je	.L9
	movl	20(%rsp), %ebx
	call	gtk_widget_get_type
	movq	%rbp, %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	movl	%ebx, %esi
	movq	%rax, %rdi
	call	gtk_widget_set_sensitive
.L9:
	movq	24(%rsp), %rbx
	movq	32(%rsp), %rbp
	addq	$40, %rsp
	ret
.LFE60:
	.size	preferences_notify_sensitivity_cb, .-preferences_notify_sensitivity_cb
	.p2align 4,,15
	.type	preferences_widget_sync_string_combo, @function
preferences_widget_sync_string_combo:
.LFB56:
	movq	%rbx, -48(%rsp)
.LCFI9:
	movq	%r13, -24(%rsp)
.LCFI10:
	movq	%rdi, %rbx
	movq	%rbp, -40(%rsp)
.LCFI11:
	movq	%r12, -32(%rsp)
.LCFI12:
	movq	%rsi, %r13
	movq	%r14, -16(%rsp)
.LCFI13:
	movq	%r15, -8(%rsp)
.LCFI14:
	subq	$104, %rsp
.LCFI15:
	call	conf_get
	leaq	40(%rsp), %rdx
	movq	%rax, %rdi
	movq	%rbx, %rsi
	call	conf_get_string
	testl	%eax, %eax
	jne	.L21
.L16:
	movq	56(%rsp), %rbx
	movq	64(%rsp), %rbp
	movq	72(%rsp), %r12
	movq	80(%rsp), %r13
	movq	88(%rsp), %r14
	movq	96(%rsp), %r15
	addq	$104, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L21:
	call	gtk_combo_box_get_type
	movq	%r13, %rdi
	movq	%rax, %rsi
	movq	%rax, %r14
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_combo_box_get_model
	cmpq	$0, 40(%rsp)
	movq	%rax, %r12
	je	.L12
	movq	%rsp, %rsi
	movq	%rax, %rdi
	call	gtk_tree_model_get_iter_first
	testl	%eax, %eax
	je	.L12
	leaq	32(%rsp), %r15
	.p2align 4,,10
	.p2align 3
.L15:
	xorl	%eax, %eax
	movl	$-1, %r8d
	movq	%r15, %rcx
	movl	$1, %edx
	movq	%rsp, %rsi
	movq	%r12, %rdi
	call	gtk_tree_model_get
	movq	32(%rsp), %rbx
	movq	40(%rsp), %rsi
	movq	%rbx, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L20
	movq	%rbx, %rdi
	call	g_free
	movq	%rsp, %rsi
	movq	%r12, %rdi
	call	gtk_tree_model_iter_next
	testl	%eax, %eax
	jne	.L15
.L12:
	movq	%rsp, %rsi
	movq	%r12, %rdi
	call	gtk_tree_model_get_iter_first
	testl	%eax, %eax
	jne	.L20
.L14:
	movq	40(%rsp), %rdi
	call	g_free
	.p2align 4,,3
	.p2align 3
	jmp	.L16
	.p2align 4,,10
	.p2align 3
.L20:
	movq	%r14, %rsi
	movq	%r13, %rdi
	call	g_type_check_instance_cast
	movq	%rsp, %rsi
	movq	%rax, %rdi
	call	gtk_combo_box_set_active_iter
	jmp	.L14
.LFE56:
	.size	preferences_widget_sync_string_combo, .-preferences_widget_sync_string_combo
	.p2align 4,,15
	.type	preferences_notify_string_combo_cb, @function
preferences_notify_string_combo_cb:
.LFB58:
	movq	%rsi, %rdi
	movq	%rdx, %rsi
	jmp	preferences_widget_sync_string_combo
.LFE58:
	.size	preferences_notify_string_combo_cb, .-preferences_notify_string_combo_cb
	.p2align 4,,15
	.type	preferences_string_combo_changed_cb, @function
preferences_string_combo_changed_cb:
.LFB67:
	movq	%rbx, -32(%rsp)
.LCFI16:
	movq	%rbp, -24(%rsp)
.LCFI17:
	movl	$80, %esi
	movq	%r13, -8(%rsp)
.LCFI18:
	movq	%r12, -16(%rsp)
.LCFI19:
	subq	$88, %rsp
.LCFI20:
	movq	%rdi, %rbx
	call	g_type_check_instance_cast
	movl	$.LC0, %esi
	movq	%rax, %rdi
	call	g_object_get_data
	movq	%rax, %r13
	call	gtk_combo_box_get_type
	movq	%rbx, %rdi
	movq	%rax, %rsi
	movq	%rax, %rbp
	call	g_type_check_instance_cast
	movq	%rsp, %rsi
	movq	%rax, %rdi
	call	gtk_combo_box_get_active_iter
	testl	%eax, %eax
	je	.L26
	movq	%rbp, %rsi
	movq	%rbx, %rdi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_combo_box_get_model
	leaq	40(%rsp), %rcx
	movq	%rax, %rdi
	movl	$-1, %r8d
	movl	$1, %edx
	movq	%rsp, %rsi
	xorl	%eax, %eax
	call	gtk_tree_model_get
	movq	40(%rsp), %rbx
	call	conf_get
	movq	%r13, %rsi
	movq	%rax, %rdi
	movq	%rbx, %rdx
	call	conf_set_string
	movq	40(%rsp), %rdi
	call	g_free
.L26:
	movq	56(%rsp), %rbx
	movq	64(%rsp), %rbp
	movq	72(%rsp), %r12
	movq	80(%rsp), %r13
	addq	$88, %rsp
	ret
.LFE67:
	.size	preferences_string_combo_changed_cb, .-preferences_string_combo_changed_cb
	.p2align 4,,15
	.type	preferences_widget_sync_int_combo, @function
preferences_widget_sync_int_combo:
.LFB57:
	movq	%rbx, -40(%rsp)
.LCFI21:
	movq	%rbp, -32(%rsp)
.LCFI22:
	movq	%rdi, %rbx
	movq	%r12, -24(%rsp)
.LCFI23:
	movq	%r13, -16(%rsp)
.LCFI24:
	movq	%rsi, %rbp
	movq	%r14, -8(%rsp)
.LCFI25:
	subq	$88, %rsp
.LCFI26:
	call	conf_get
	leaq	44(%rsp), %rdx
	movq	%rax, %rdi
	movq	%rbx, %rsi
	call	conf_get_int
	testl	%eax, %eax
	jne	.L36
.L32:
	movq	48(%rsp), %rbx
	movq	56(%rsp), %rbp
	movq	64(%rsp), %r12
	movq	72(%rsp), %r13
	movq	80(%rsp), %r14
	addq	$88, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L36:
	call	gtk_combo_box_get_type
	movq	%rbp, %rdi
	movq	%rax, %rsi
	movq	%rax, %r12
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_combo_box_get_model
	movq	%rax, %rbx
	movl	44(%rsp), %eax
	testl	%eax, %eax
	jne	.L37
.L29:
	movq	%rsp, %rsi
	movq	%rbx, %rdi
	call	gtk_tree_model_get_iter_first
	testl	%eax, %eax
	je	.L32
.L35:
	movq	%r12, %rsi
	movq	%rbp, %rdi
	call	g_type_check_instance_cast
	movq	%rsp, %rsi
	movq	%rax, %rdi
	call	gtk_combo_box_set_active_iter
	jmp	.L32
	.p2align 4,,10
	.p2align 3
.L37:
	movq	%rsp, %rsi
	movq	%rbx, %rdi
	call	gtk_tree_model_get_iter_first
	testl	%eax, %eax
	.p2align 4,,2
	.p2align 3
	je	.L29
	leaq	40(%rsp), %r14
	jmp	.L31
	.p2align 4,,10
	.p2align 3
.L30:
	movq	%rsp, %rsi
	movq	%rbx, %rdi
	call	gtk_tree_model_iter_next
	testl	%eax, %eax
	je	.L29
.L31:
	xorl	%eax, %eax
	movl	$-1, %r8d
	movq	%r14, %rcx
	movl	$1, %edx
	movq	%rsp, %rsi
	movq	%rbx, %rdi
	call	gtk_tree_model_get
	movl	40(%rsp), %eax
	cmpl	44(%rsp), %eax
	jne	.L30
	jmp	.L35
.LFE57:
	.size	preferences_widget_sync_int_combo, .-preferences_widget_sync_int_combo
	.p2align 4,,15
	.type	preferences_notify_int_combo_cb, @function
preferences_notify_int_combo_cb:
.LFB59:
	movq	%rsi, %rdi
	movq	%rdx, %rsi
	jmp	preferences_widget_sync_int_combo
.LFE59:
	.size	preferences_notify_int_combo_cb, .-preferences_notify_int_combo_cb
	.p2align 4,,15
	.type	preferences_int_combo_changed_cb, @function
preferences_int_combo_changed_cb:
.LFB68:
	movq	%rbx, -32(%rsp)
.LCFI27:
	movq	%rbp, -24(%rsp)
.LCFI28:
	movl	$80, %esi
	movq	%r13, -8(%rsp)
.LCFI29:
	movq	%r12, -16(%rsp)
.LCFI30:
	subq	$88, %rsp
.LCFI31:
	movq	%rdi, %rbx
	call	g_type_check_instance_cast
	movl	$.LC0, %esi
	movq	%rax, %rdi
	call	g_object_get_data
	movq	%rax, %r13
	call	gtk_combo_box_get_type
	movq	%rbx, %rdi
	movq	%rax, %rsi
	movq	%rax, %rbp
	call	g_type_check_instance_cast
	movq	%rsp, %rsi
	movq	%rax, %rdi
	call	gtk_combo_box_get_active_iter
	testl	%eax, %eax
	je	.L42
	movq	%rbp, %rsi
	movq	%rbx, %rdi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_combo_box_get_model
	leaq	44(%rsp), %rcx
	movq	%rax, %rdi
	movl	$1, %edx
	movq	%rsp, %rsi
	movl	$-1, %r8d
	xorl	%eax, %eax
	call	gtk_tree_model_get
	movl	44(%rsp), %ebx
	call	conf_get
	movq	%r13, %rsi
	movq	%rax, %rdi
	movl	%ebx, %edx
	call	conf_set_int
.L42:
	movq	56(%rsp), %rbx
	movq	64(%rsp), %rbp
	movq	72(%rsp), %r12
	movq	80(%rsp), %r13
	addq	$88, %rsp
	ret
.LFE68:
	.size	preferences_int_combo_changed_cb, .-preferences_int_combo_changed_cb
	.p2align 4,,15
	.type	preferences_languages_load_foreach, @function
preferences_languages_load_foreach:
.LFB50:
	movq	%rbx, -48(%rsp)
.LCFI32:
	movq	%r14, -16(%rsp)
.LCFI33:
	movq	%rcx, %rbx
	movq	%r15, -8(%rsp)
.LCFI34:
	movq	%rbp, -40(%rsp)
.LCFI35:
	movq	%rdi, %r14
	movq	%r12, -32(%rsp)
.LCFI36:
	movq	%r13, -24(%rsp)
.LCFI37:
	subq	$72, %rsp
.LCFI38:
	testq	%rcx, %rcx
	movq	%rdx, %r15
	movl	$1, %eax
	je	.L45
	leaq	16(%rsp), %rcx
	xorl	%eax, %eax
	movl	$-1, %r8d
	movl	$1, %edx
	movq	%r15, %rsi
	call	gtk_tree_model_get
	movq	16(%rsp), %r12
	xorl	%eax, %eax
	testq	%r12, %r12
	je	.L45
	movq	(%rbx), %rdi
	xorl	%ebp, %ebp
	testq	%rdi, %rdi
	je	.L48
	movl	$1, %r13d
	.p2align 4,,10
	.p2align 3
.L52:
	movq	%r12, %rsi
	call	strcmp
	movq	8(%rbx), %rdi
	testl	%eax, %eax
	cmove	%r13d, %ebp
	addq	$8, %rbx
	testq	%rdi, %rdi
	jne	.L52
.L48:
	call	gtk_list_store_get_type
	movq	%r14, %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	movl	$-1, %r8d
	movq	%rax, %rdi
	movl	%ebp, %ecx
	xorl	%eax, %eax
	xorl	%edx, %edx
	movq	%r15, %rsi
	call	gtk_list_store_set
	xorl	%eax, %eax
.L45:
	movq	24(%rsp), %rbx
	movq	32(%rsp), %rbp
	movq	40(%rsp), %r12
	movq	48(%rsp), %r13
	movq	56(%rsp), %r14
	movq	64(%rsp), %r15
	addq	$72, %rsp
	ret
.LFE50:
	.size	preferences_languages_load_foreach, .-preferences_languages_load_foreach
	.section	.rodata.str1.1
.LC1:
	.string	"en"
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC2:
	.string	"/apps/greet-tweet-know/ui/spell_checker_languages"
	.text
	.p2align 4,,15
	.type	preferences_languages_cell_toggled_cb, @function
preferences_languages_cell_toggled_cb:
.LFB51:
	movq	%rbx, -48(%rsp)
.LCFI39:
	movq	%rbp, -40(%rsp)
.LCFI40:
	movq	%rdx, %rbp
	movq	%r12, -32(%rsp)
.LCFI41:
	movq	%r13, -24(%rsp)
.LCFI42:
	movq	%rsi, %rbx
	movq	%r14, -16(%rsp)
.LCFI43:
	movq	%r15, -8(%rsp)
.LCFI44:
	subq	$104, %rsp
.LCFI45:
	call	gtk_tree_view_get_type
	movq	16(%rbp), %rdi
	movq	%rax, %rsi
	movq	%rax, %r15
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_tree_view_get_model
	movq	%rax, %r12
	call	gtk_list_store_get_type
	movq	%r12, %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	movq	%rbx, %rdi
	movq	%rax, %r14
	call	gtk_tree_path_new_from_string
	movq	%rsp, %rsi
	movq	%rax, %rdx
	movq	%r12, %rdi
	movq	%rax, %r13
	call	gtk_tree_model_get_iter
	leaq	44(%rsp), %rcx
	xorl	%edx, %edx
	xorl	%eax, %eax
	movl	$-1, %r8d
	movq	%rsp, %rsi
	movq	%r12, %rdi
	call	gtk_tree_model_get
	movl	44(%rsp), %ecx
	xorl	%edx, %edx
	movl	$-1, %r8d
	xorl	%eax, %eax
	movq	%rsp, %rsi
	movq	%r14, %rdi
	xorl	$1, %ecx
	movl	%ecx, 44(%rsp)
	call	gtk_list_store_set
	movq	%r13, %rdi
	call	gtk_tree_path_free
	movq	16(%rbp), %rdi
	movq	%r15, %rsi
	movq	$0, 32(%rsp)
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_tree_view_get_model
	leaq	32(%rsp), %rdx
	movq	%rax, %rdi
	movl	$preferences_languages_save_foreach, %esi
	call	gtk_tree_model_foreach
	cmpq	$0, 32(%rsp)
	je	.L57
.L55:
	movq	32(%rsp), %rbx
	call	conf_get
	movl	$.LC2, %esi
	movq	%rax, %rdi
	movq	%rbx, %rdx
	call	conf_set_string
	movq	32(%rsp), %rdi
	call	g_free
	movq	56(%rsp), %rbx
	movq	64(%rsp), %rbp
	movq	72(%rsp), %r12
	movq	80(%rsp), %r13
	movq	88(%rsp), %r14
	movq	96(%rsp), %r15
	addq	$104, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L57:
	movl	$.LC1, %edi
	call	g_strdup
	movq	%rax, 32(%rsp)
	jmp	.L55
.LFE51:
	.size	preferences_languages_cell_toggled_cb, .-preferences_languages_cell_toggled_cb
	.section	.rodata.str1.1
.LC3:
	.string	"%s,%s"
	.text
	.p2align 4,,15
	.type	preferences_languages_save_foreach, @function
preferences_languages_save_foreach:
.LFB48:
	movq	%rbx, -24(%rsp)
.LCFI46:
	movq	%rbp, -16(%rsp)
.LCFI47:
	movq	%rdx, %rbx
	movq	%r12, -8(%rsp)
.LCFI48:
	subq	$40, %rsp
.LCFI49:
	testq	%rcx, %rcx
	movq	%rdi, %r12
	movq	%rcx, %rbp
	movl	$1, %eax
	je	.L60
	leaq	12(%rsp), %rcx
	xorl	%edx, %edx
	xorl	%eax, %eax
	movl	$-1, %r8d
	movq	%rbx, %rsi
	call	gtk_tree_model_get
	movl	12(%rsp), %edx
	testl	%edx, %edx
	jne	.L65
.L61:
	xorl	%eax, %eax
.L60:
	movq	16(%rsp), %rbx
	movq	24(%rsp), %rbp
	movq	32(%rsp), %r12
	addq	$40, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L65:
	movl	$1, %edx
	xorl	%eax, %eax
	movq	%rsp, %rcx
	movl	$-1, %r8d
	movq	%rbx, %rsi
	movq	%r12, %rdi
	call	gtk_tree_model_get
	movq	(%rsp), %rdx
	testq	%rdx, %rdx
	je	.L61
	movq	(%rbp), %rbx
	testq	%rbx, %rbx
	je	.L66
	movl	$.LC3, %edi
	movq	%rbx, %rsi
	xorl	%eax, %eax
	call	g_strdup_printf
	movq	%rbx, %rdi
	movq	%rax, (%rbp)
	call	g_free
.L63:
	movq	(%rsp), %rdi
	call	g_free
	xorl	%eax, %eax
	jmp	.L60
	.p2align 4,,10
	.p2align 3
.L66:
	movq	%rdx, %rdi
	call	g_strdup
	movq	%rax, (%rbp)
	.p2align 4,,2
	.p2align 3
	jmp	.L63
.LFE48:
	.size	preferences_languages_save_foreach, .-preferences_languages_save_foreach
	.p2align 4,,15
	.type	preferences_response_cb, @function
preferences_response_cb:
.LFB69:
	jmp	gtk_widget_destroy
.LFE69:
	.size	preferences_response_cb, .-preferences_response_cb
	.p2align 4,,15
	.type	preferences_destroy_cb, @function
preferences_destroy_cb:
.LFB70:
	pushq	%r12
.LCFI50:
	xorl	%edi, %edi
	movq	%rsi, %r12
	pushq	%rbp
.LCFI51:
	pushq	%rbx
.LCFI52:
	movq	80(%rsi), %rbp
	testq	%rbp, %rbp
	je	.L71
	.p2align 4,,10
	.p2align 3
.L73:
	movq	(%rbp), %rbx
	call	conf_get
	movq	%rax, %rdi
	movl	%ebx, %esi
	call	conf_notify_remove
	movq	8(%rbp), %rbp
	testq	%rbp, %rbp
	jne	.L73
	movq	80(%r12), %rdi
.L71:
	call	g_list_free
	popq	%rbx
	popq	%rbp
	movq	%r12, %rdi
	popq	%r12
	jmp	g_free
.LFE70:
	.size	preferences_destroy_cb, .-preferences_destroy_cb
	.section	.rodata.str1.1
.LC4:
	.string	"toggled"
	.text
	.p2align 4,,15
	.type	preferences_hookup_toggle_button, @function
preferences_hookup_toggle_button:
.LFB62:
	movq	%rbp, -24(%rsp)
.LCFI53:
	movq	%r12, -16(%rsp)
.LCFI54:
	movq	%rdx, %rbp
	movq	%r13, -8(%rsp)
.LCFI55:
	movq	%rbx, -32(%rsp)
.LCFI56:
	subq	$56, %rsp
.LCFI57:
	movq	%rdi, %r13
	movq	%rsi, %r12
	call	conf_get
	leaq	20(%rsp), %rdx
	movq	%rax, %rdi
	movq	%r12, %rsi
	call	conf_get_bool
	testl	%eax, %eax
	je	.L76
	movl	20(%rsp), %ebx
	call	gtk_toggle_button_get_type
	movq	%rbp, %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	movl	%ebx, %esi
	movq	%rax, %rdi
	call	gtk_toggle_button_set_active
.L76:
	movq	%r12, %rdi
	call	g_strdup
	movl	$80, %esi
	movq	%rax, %rbx
	movq	%rbp, %rdi
	call	g_type_check_instance_cast
	movl	$g_free, %ecx
	movq	%rax, %rdi
	movq	%rbx, %rdx
	movl	$.LC0, %esi
	call	g_object_set_data_full
	xorl	%r9d, %r9d
	xorl	%r8d, %r8d
	xorl	%ecx, %ecx
	movl	$preferences_toggle_button_toggled_cb, %edx
	movl	$.LC4, %esi
	movq	%rbp, %rdi
	call	g_signal_connect_data
	call	conf_get
	movq	%rbp, %rcx
	movq	%rax, %rdi
	movl	$preferences_notify_bool_cb, %edx
	movq	%r12, %rsi
	call	conf_notify_add
	testl	%eax, %eax
	je	.L78
	movq	80(%r13), %rdi
	mov	%eax, %esi
	call	g_list_prepend
	movq	%rax, 80(%r13)
.L78:
	movq	24(%rsp), %rbx
	movq	32(%rsp), %rbp
	movq	40(%rsp), %r12
	movq	48(%rsp), %r13
	addq	$56, %rsp
	ret
.LFE62:
	.size	preferences_hookup_toggle_button, .-preferences_hookup_toggle_button
	.section	.rodata.str1.1
.LC5:
	.string	"combobox_timeline"
.LC6:
	.string	"preferences_notebook"
.LC7:
	.string	"preferences_dialog"
.LC8:
	.string	"preferences.ui"
.LC9:
	.string	"spell_treeview"
.LC10:
	.string	"sound_checkbutton"
.LC11:
	.string	"spell_checkbutton"
.LC12:
	.string	"notify_checkbutton"
.LC13:
	.string	"autoconnect_checkbutton"
.LC14:
	.string	"expand_checkbutton"
.LC15:
	.string	"combobox_reload"
.LC16:
	.string	"destroy"
.LC17:
	.string	"response"
.LC18:
	.string	"text"
	.section	.rodata.str1.8
	.align 8
.LC19:
	.string	"https://twitter.com/statuses/public_timeline.xml"
	.align 8
.LC20:
	.string	"/apps/greet-tweet-know/ui/expand_messages"
	.align 8
.LC21:
	.string	"/apps/greet-tweet-know/auth/auto_login"
	.align 8
.LC22:
	.string	"/apps/greet-tweet-know/ui/notify"
	.align 8
.LC23:
	.string	"/apps/greet-tweet-know/ui/sound"
	.align 8
.LC24:
	.string	"/apps/greet-tweet-know/ui/spell"
	.align 8
.LC25:
	.string	"/apps/greet-tweet-know/tweets/home_timeline"
	.section	.rodata.str1.1
.LC26:
	.string	"changed"
	.section	.rodata.str1.8
	.align 8
.LC27:
	.string	"/apps/greet-tweet-know/tweets/reload_timeline"
	.section	.rodata.str1.1
.LC28:
	.string	"active"
.LC29:
	.string	"Language"
.LC30:
	.string	"column"
.LC31:
	.string	","
	.text
	.p2align 4,,15
.globl preferences_dialog_show
	.type	preferences_dialog_show, @function
preferences_dialog_show:
.LFB71:
	pushq	%r15
.LCFI58:
	pushq	%r14
.LCFI59:
	pushq	%r13
.LCFI60:
	pushq	%r12
.LCFI61:
	pushq	%rbp
.LCFI62:
	movq	%rdi, %rbp
	pushq	%rbx
.LCFI63:
	subq	$200, %rsp
.LCFI64:
	movq	prefs.44379(%rip), %rbx
	testq	%rbx, %rbx
	je	.L80
	call	gtk_window_get_type
	movq	(%rbx), %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_window_present
.L95:
	addq	$200, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	ret
	.p2align 4,,10
	.p2align 3
.L80:
	movl	$88, %edi
	leaq	144(%rsp), %r15
	call	g_malloc0
	leaq	16(%rax), %rdx
	leaq	8(%rax), %r8
	movq	%rax, prefs.44379(%rip)
	movl	$.LC5, %r9d
	movl	$.LC6, %ecx
	movl	$.LC7, %esi
	movq	%rdx, 112(%rsp)
	leaq	64(%rax), %rdx
	movl	$.LC8, %edi
	movq	$0, 120(%rsp)
	movq	$.LC9, 104(%rsp)
	movq	%rdx, 96(%rsp)
	leaq	72(%rax), %rdx
	movq	$.LC10, 88(%rsp)
	movq	$.LC11, 72(%rsp)
	movq	$.LC12, 56(%rsp)
	movq	%rdx, 80(%rsp)
	leaq	56(%rax), %rdx
	movq	$.LC13, 40(%rsp)
	movq	$.LC14, 24(%rsp)
	movq	$.LC15, 8(%rsp)
	movq	%rdx, 64(%rsp)
	leaq	48(%rax), %rdx
	movq	%rdx, 48(%rsp)
	leaq	40(%rax), %rdx
	movq	%rdx, 32(%rsp)
	leaq	32(%rax), %rdx
	movq	%rdx, 16(%rsp)
	leaq	24(%rax), %rdx
	movq	%rdx, (%rsp)
	movq	%rax, %rdx
	xorl	%eax, %eax
	call	gtkbuilder_get_file
	movq	prefs.44379(%rip), %rsi
	movl	$.LC7, %r9d
	movl	$preferences_destroy_cb, %r8d
	movq	%r9, %rdx
	movl	$.LC16, %ecx
	movq	%rax, %rbx
	movq	%rax, %rdi
	xorl	%eax, %eax
	movq	$0, 16(%rsp)
	movq	$preferences_response_cb, 8(%rsp)
	movq	$.LC17, (%rsp)
	call	gtkbuilder_connect
	movq	%rbx, %rdi
	call	g_object_unref
	movq	prefs.44379(%rip), %rax
	movl	$80, %esi
	movq	(%rax), %rdi
	call	g_type_check_instance_cast
	movl	$prefs.44379, %esi
	movq	%rax, %rdi
	call	g_object_add_weak_pointer
	call	gtk_window_get_type
	movq	%rax, %rsi
	movq	prefs.44379(%rip), %rax
	movq	(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%rbp, %rsi
	movq	%rax, %rdi
	call	gtk_window_set_transient_for
	movq	prefs.44379(%rip), %r12
	call	gtk_cell_renderer_text_new
	movq	%rax, %rbx
	call	gtk_cell_layout_get_type
	movq	24(%r12), %rdi
	movq	%rax, %rsi
	movq	%rax, %rbp
	call	g_type_check_instance_cast
	movl	$1, %edx
	movq	%rax, %rdi
	movq	%rbx, %rsi
	call	gtk_cell_layout_pack_start
	movq	24(%r12), %rdi
	movq	%rbp, %rsi
	call	g_type_check_instance_cast
	xorl	%r8d, %r8d
	xorl	%ecx, %ecx
	movq	%rax, %rdi
	movq	%rbx, %rsi
	movl	$.LC18, %edx
	xorl	%eax, %eax
	call	gtk_cell_layout_set_attributes
	movl	$64, %edx
	movl	$64, %esi
	movl	$2, %edi
	xorl	%eax, %eax
	call	gtk_list_store_new
	movq	%rax, %r13
	call	gtk_tree_model_get_type
	movq	%r13, %rdi
	movq	%rax, %rsi
	movq	%rax, 128(%rsp)
	call	g_type_check_instance_cast
	movq	%rax, %rbx
	call	gtk_combo_box_get_type
	movq	24(%r12), %rdi
	movq	%rax, %rsi
	movq	%rax, %r14
	movl	$timelines.44086+8, %r12d
	call	g_type_check_instance_cast
	movq	%rbx, %rsi
	movq	%rax, %rdi
	movl	$.LC19, %ebx
	call	gtk_combo_box_set_model
.L83:
	movq	%r15, %rsi
	movq	%r13, %rdi
	call	gtk_list_store_append
	movq	(%r12), %rsi
	xorl	%edi, %edi
	movl	$5, %edx
	call	dcgettext
	movq	%rbx, %r9
	movq	%rax, %rcx
	xorl	%edx, %edx
	xorl	%eax, %eax
	movl	$1, %r8d
	movq	%r15, %rsi
	movq	%r13, %rdi
	movl	$-1, (%rsp)
	call	gtk_list_store_set
	movq	8(%r12), %rbx
	addq	$16, %r12
	testq	%rbx, %rbx
	jne	.L83
	movq	%r13, %rdi
	call	g_object_unref
	movq	prefs.44379(%rip), %r12
	call	gtk_cell_renderer_text_new
	movq	%rbp, %rsi
	movq	%rax, %rbx
	movq	32(%r12), %rdi
	call	g_type_check_instance_cast
	movl	$1, %edx
	movq	%rax, %rdi
	movq	%rbx, %rsi
	call	gtk_cell_layout_pack_start
	movq	32(%r12), %rdi
	movq	%rbp, %rsi
	call	g_type_check_instance_cast
	xorl	%r8d, %r8d
	xorl	%ecx, %ecx
	movq	%rax, %rdi
	movq	%rbx, %rsi
	xorl	%eax, %eax
	movl	$.LC18, %edx
	call	gtk_cell_layout_set_attributes
	movl	$24, %edx
	movl	$64, %esi
	movl	$2, %edi
	xorl	%eax, %eax
	call	gtk_list_store_new
	movq	128(%rsp), %rsi
	movq	%rax, %rdi
	movq	%rax, %r13
	call	g_type_check_instance_cast
	movq	32(%r12), %rdi
	movq	%rax, %rbx
	movq	%r14, %rsi
	call	g_type_check_instance_cast
	movq	%rbx, %rsi
	movq	%rax, %rdi
	call	gtk_combo_box_set_model
	cmpq	$0, reload_list+8(%rip)
	je	.L84
	movl	$reload_list, %r12d
	.p2align 4,,10
	.p2align 3
.L85:
	movq	%r15, %rsi
	movq	%r13, %rdi
	call	gtk_list_store_append
	movl	(%r12), %ebx
	movq	8(%r12), %rsi
	xorl	%edi, %edi
	movl	$5, %edx
	addq	$16, %r12
	call	dcgettext
	xorl	%edx, %edx
	movq	%rax, %rcx
	movl	%ebx, %r9d
	xorl	%eax, %eax
	movl	$1, %r8d
	movq	%r15, %rsi
	movq	%r13, %rdi
	movl	$-1, (%rsp)
	call	gtk_list_store_set
	cmpq	$0, 8(%r12)
	jne	.L85
.L84:
	movq	%r13, %rdi
	call	g_object_unref
	movq	prefs.44379(%rip), %r13
	movl	$.LC20, %esi
	movq	40(%r13), %rdx
	movq	%r13, %rdi
	call	preferences_hookup_toggle_button
	movq	48(%r13), %rdx
	movl	$.LC21, %esi
	movq	%r13, %rdi
	call	preferences_hookup_toggle_button
	movq	56(%r13), %rdx
	movl	$.LC22, %esi
	movq	%r13, %rdi
	call	preferences_hookup_toggle_button
	movq	64(%r13), %rdx
	movl	$.LC23, %esi
	movq	%r13, %rdi
	call	preferences_hookup_toggle_button
	movq	72(%r13), %rdx
	movl	$.LC24, %esi
	movq	%r13, %rdi
	call	preferences_hookup_toggle_button
	movq	16(%r13), %rbx
	call	conf_get
	leaq	188(%rsp), %rdx
	movq	%rax, %rdi
	movl	$.LC2, %esi
	call	conf_get_bool
	testl	%eax, %eax
	jne	.L99
.L86:
	call	conf_get
	movq	%rbx, %rcx
	movq	%rax, %rdi
	movl	$preferences_notify_sensitivity_cb, %edx
	movl	$.LC2, %esi
	call	conf_notify_add
	testl	%eax, %eax
	je	.L87
	movq	80(%r13), %rdi
	mov	%eax, %esi
	call	g_list_prepend
	movq	%rax, 80(%r13)
.L87:
	movq	24(%r13), %rbx
	movl	$.LC25, %edi
	movq	%rbx, %rsi
	call	preferences_widget_sync_string_combo
	movl	$.LC25, %edi
	call	g_strdup
	movl	$80, %esi
	movq	%rax, %r12
	movq	%rbx, %rdi
	call	g_type_check_instance_cast
	movl	$g_free, %ecx
	movq	%rax, %rdi
	movq	%r12, %rdx
	movl	$.LC0, %esi
	call	g_object_set_data_full
	xorl	%r9d, %r9d
	xorl	%r8d, %r8d
	xorl	%ecx, %ecx
	movl	$preferences_string_combo_changed_cb, %edx
	movl	$.LC26, %esi
	movq	%rbx, %rdi
	call	g_signal_connect_data
	call	conf_get
	movq	%rbx, %rcx
	movq	%rax, %rdi
	movl	$preferences_notify_string_combo_cb, %edx
	movl	$.LC25, %esi
	call	conf_notify_add
	testl	%eax, %eax
	je	.L88
	movq	80(%r13), %rdi
	mov	%eax, %esi
	call	g_list_prepend
	movq	%rax, 80(%r13)
.L88:
	movq	32(%r13), %rbx
	movl	$.LC27, %edi
	movq	%rbx, %rsi
	call	preferences_widget_sync_int_combo
	movl	$.LC27, %edi
	call	g_strdup
	movl	$80, %esi
	movq	%rax, %r12
	movq	%rbx, %rdi
	call	g_type_check_instance_cast
	movl	$g_free, %ecx
	movq	%rax, %rdi
	movq	%r12, %rdx
	movl	$.LC0, %esi
	call	g_object_set_data_full
	xorl	%r9d, %r9d
	xorl	%r8d, %r8d
	xorl	%ecx, %ecx
	movl	$preferences_int_combo_changed_cb, %edx
	movl	$.LC26, %esi
	movq	%rbx, %rdi
	call	g_signal_connect_data
	call	conf_get
	movq	%rbx, %rcx
	movq	%rax, %rdi
	movl	$preferences_notify_int_combo_cb, %edx
	movl	$.LC27, %esi
	call	conf_notify_add
	testl	%eax, %eax
	je	.L89
	movq	80(%r13), %rdi
	mov	%eax, %esi
	call	g_list_prepend
	movq	%rax, 80(%r13)
.L89:
	movq	prefs.44379(%rip), %r12
	call	gtk_tree_view_get_type
	movq	%rax, %rsi
	movq	%rax, 136(%rsp)
	movq	16(%r12), %rdi
	call	g_type_check_instance_cast
	movl	$64, %ecx
	movl	$64, %edx
	movq	%rax, %r13
	movl	$20, %esi
	movl	$3, %edi
	xorl	%eax, %eax
	call	gtk_list_store_new
	movq	128(%rsp), %rsi
	movq	%rax, %rdi
	movq	%rax, %r14
	call	g_type_check_instance_cast
	movq	%r13, %rdi
	movq	%rax, %rsi
	call	gtk_tree_view_set_model
	movq	%r13, %rdi
	call	gtk_tree_view_get_selection
	movl	$1, %esi
	movq	%rax, %rdi
	call	gtk_tree_selection_set_mode
	movq	128(%rsp), %rsi
	movq	%r14, %rdi
	call	g_type_check_instance_cast
	call	gtk_cell_renderer_toggle_new
	xorl	%r9d, %r9d
	movq	%rax, %rbx
	xorl	%r8d, %r8d
	movq	%r12, %rcx
	movl	$preferences_languages_cell_toggled_cb, %edx
	movl	$.LC4, %esi
	movq	%rax, %rdi
	call	g_signal_connect_data
	xorl	%r8d, %r8d
	xorl	%ecx, %ecx
	movl	$.LC28, %edx
	movq	%rbx, %rsi
	xorl	%edi, %edi
	xorl	%eax, %eax
	call	gtk_tree_view_column_new_with_attributes
	movq	%r13, %rdi
	movq	%rax, %rsi
	call	gtk_tree_view_append_column
	call	gtk_cell_renderer_text_new
	xorl	%edi, %edi
	movq	%rax, %r12
	movl	$5, %edx
	movl	$.LC29, %esi
	call	dcgettext
	movq	%r12, %rcx
	movl	$2, %r9d
	movl	$.LC18, %r8d
	movq	%rax, %rdx
	movl	$-1, %esi
	movq	%r13, %rdi
	xorl	%eax, %eax
	movq	$0, (%rsp)
	call	gtk_tree_view_insert_column_with_attributes
	movq	%r12, %rdi
	movl	$80, %esi
	movl	%eax, %ebx
	call	g_type_check_instance_cast
	movl	$2, %edx
	movq	%rax, %rdi
	movl	$.LC30, %esi
	call	g_object_set_data
	leal	-1(%rbx), %esi
	movq	%r13, %rdi
	call	gtk_tree_view_get_column
	movl	$2, %esi
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
	movq	%r14, %rdi
	call	g_object_unref
	movq	prefs.44379(%rip), %rax
	movq	136(%rsp), %rsi
	movq	16(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%rax, %rbx
	call	gtk_list_store_get_type
	movq	%rbx, %rdi
	movq	%rax, %r12
	call	gtk_tree_view_get_model
	movq	%r12, %rsi
	movq	%rax, %rdi
	call	g_type_check_instance_cast
	movq	%rax, %r14
	call	spell_get_language_codes
	testq	%rax, %rax
	movq	%rax, %rbp
	je	.L90
	movq	%rax, %rbx
	.p2align 4,,10
	.p2align 3
.L92:
	movq	(%rbx), %r13
	movq	%r13, %rdi
	call	spell_get_language_name
	testq	%rax, %rax
	movq	%rax, %r12
	je	.L91
	movq	%r15, %rsi
	movq	%r14, %rdi
	call	gtk_list_store_append
	movq	%r12, %r9
	movl	$2, %r8d
	movq	%r13, %rcx
	movl	$1, %edx
	movq	%r15, %rsi
	movq	%r14, %rdi
	xorl	%eax, %eax
	movl	$-1, (%rsp)
	call	gtk_list_store_set
.L91:
	movq	8(%rbx), %rbx
	testq	%rbx, %rbx
	jne	.L92
.L90:
	movq	%rbp, %rdi
	call	spell_free_language_codes
	movq	prefs.44379(%rip), %rbp
	call	conf_get
	leaq	176(%rsp), %rdx
	movq	%rax, %rdi
	movl	$.LC2, %esi
	call	conf_get_string
	testl	%eax, %eax
	je	.L93
	movq	176(%rsp), %rdi
	testq	%rdi, %rdi
	je	.L93
	movl	$-1, %edx
	movl	$.LC31, %esi
	call	g_strsplit
	movq	176(%rsp), %rdi
	movq	%rax, %rbx
	call	g_free
	movq	136(%rsp), %rsi
	movq	16(%rbp), %rdi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_tree_view_get_model
	movq	%rbx, %rdx
	movq	%rax, %rdi
	movl	$preferences_languages_load_foreach, %esi
	call	gtk_tree_model_foreach
	movq	%rbx, %rdi
	call	g_strfreev
.L93:
	call	spell_supported
	testl	%eax, %eax
	jne	.L100
.L94:
	movq	prefs.44379(%rip), %rax
	movq	(%rax), %rdi
	call	gtk_widget_show
	jmp	.L95
	.p2align 4,,10
	.p2align 3
.L99:
	movl	188(%rsp), %esi
	movq	%rbx, %rdi
	call	gtk_widget_set_sensitive
	jmp	.L86
	.p2align 4,,10
	.p2align 3
.L100:
	call	gtk_notebook_get_type
	movq	%rax, %rsi
	movq	prefs.44379(%rip), %rax
	movq	8(%rax), %rdi
	call	g_type_check_instance_cast
	movl	$1, %esi
	movq	%rax, %rdi
	call	gtk_notebook_get_nth_page
	movq	%rax, %rdi
	call	gtk_widget_show
	jmp	.L94
.LFE71:
	.size	preferences_dialog_show, .-preferences_dialog_show
.globl reload_list
	.section	.rodata.str1.1
.LC32:
	.string	"3 minutes"
.LC33:
	.string	"5 minutes"
.LC34:
	.string	"15 minutes"
.LC35:
	.string	"30 minutes"
.LC36:
	.string	"hour"
	.data
	.align 32
	.type	reload_list, @object
	.size	reload_list, 96
reload_list:
	.long	3
	.zero	4
	.quad	.LC32
	.long	5
	.zero	4
	.quad	.LC33
	.long	15
	.zero	4
	.quad	.LC34
	.long	30
	.zero	4
	.quad	.LC35
	.long	60
	.zero	4
	.quad	.LC36
	.long	0
	.zero	4
	.quad	0
	.local	prefs.44379
	.comm	prefs.44379,8,8
	.section	.rodata.str1.1
.LC37:
	.string	"Public"
	.section	.rodata.str1.8
	.align 8
.LC38:
	.string	"https://twitter.com/statuses/friends_timeline.xml"
	.section	.rodata.str1.1
.LC39:
	.string	"Friends"
	.section	.rodata.str1.8
	.align 8
.LC40:
	.string	"https://twitter.com/statuses/user_timeline.xml"
	.section	.rodata.str1.1
.LC41:
	.string	"Mine"
	.section	.rodata
	.align 32
	.type	timelines.44086, @object
	.size	timelines.44086, 56
timelines.44086:
	.quad	.LC19
	.quad	.LC37
	.quad	.LC38
	.quad	.LC39
	.quad	.LC40
	.quad	.LC41
	.quad	0
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
	.long	.LFB52
	.long	.LFE52-.LFB52
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI2-.LFB52
	.byte	0xe
	.uleb128 0x30
	.byte	0x86
	.uleb128 0x2
	.byte	0x83
	.uleb128 0x3
	.align 8
.LEFDE1:
.LSFDE3:
	.long	.LEFDE3-.LASFDE3
.LASFDE3:
	.long	.LASFDE3-.Lframe1
	.long	.LFB66
	.long	.LFE66-.LFB66
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI3-.LFB66
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI4-.LCFI3
	.byte	0xe
	.uleb128 0x18
	.byte	0x83
	.uleb128 0x3
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI5-.LCFI4
	.byte	0xe
	.uleb128 0x20
	.align 8
.LEFDE3:
.LSFDE5:
	.long	.LEFDE5-.LASFDE5
.LASFDE5:
	.long	.LASFDE5-.Lframe1
	.long	.LFB60
	.long	.LFE60-.LFB60
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI8-.LFB60
	.byte	0xe
	.uleb128 0x30
	.byte	0x83
	.uleb128 0x3
	.byte	0x86
	.uleb128 0x2
	.align 8
.LEFDE5:
.LSFDE7:
	.long	.LEFDE7-.LASFDE7
.LASFDE7:
	.long	.LASFDE7-.Lframe1
	.long	.LFB56
	.long	.LFE56-.LFB56
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI10-.LFB56
	.byte	0x8d
	.uleb128 0x4
	.byte	0x83
	.uleb128 0x7
	.byte	0x4
	.long	.LCFI15-.LCFI10
	.byte	0xe
	.uleb128 0x70
	.byte	0x8f
	.uleb128 0x2
	.byte	0x8e
	.uleb128 0x3
	.byte	0x8c
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x6
	.align 8
.LEFDE7:
.LSFDE9:
	.long	.LEFDE9-.LASFDE9
.LASFDE9:
	.long	.LASFDE9-.Lframe1
	.long	.LFB58
	.long	.LFE58-.LFB58
	.uleb128 0x0
	.align 8
.LEFDE9:
.LSFDE11:
	.long	.LEFDE11-.LASFDE11
.LASFDE11:
	.long	.LASFDE11-.Lframe1
	.long	.LFB67
	.long	.LFE67-.LFB67
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI20-.LFB67
	.byte	0xe
	.uleb128 0x60
	.byte	0x8c
	.uleb128 0x3
	.byte	0x8d
	.uleb128 0x2
	.byte	0x86
	.uleb128 0x4
	.byte	0x83
	.uleb128 0x5
	.align 8
.LEFDE11:
.LSFDE13:
	.long	.LEFDE13-.LASFDE13
.LASFDE13:
	.long	.LASFDE13-.Lframe1
	.long	.LFB57
	.long	.LFE57-.LFB57
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI22-.LFB57
	.byte	0x86
	.uleb128 0x5
	.byte	0x83
	.uleb128 0x6
	.byte	0x4
	.long	.LCFI26-.LCFI22
	.byte	0xe
	.uleb128 0x60
	.byte	0x8e
	.uleb128 0x2
	.byte	0x8d
	.uleb128 0x3
	.byte	0x8c
	.uleb128 0x4
	.align 8
.LEFDE13:
.LSFDE15:
	.long	.LEFDE15-.LASFDE15
.LASFDE15:
	.long	.LASFDE15-.Lframe1
	.long	.LFB59
	.long	.LFE59-.LFB59
	.uleb128 0x0
	.align 8
.LEFDE15:
.LSFDE17:
	.long	.LEFDE17-.LASFDE17
.LASFDE17:
	.long	.LASFDE17-.Lframe1
	.long	.LFB68
	.long	.LFE68-.LFB68
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI31-.LFB68
	.byte	0xe
	.uleb128 0x60
	.byte	0x8c
	.uleb128 0x3
	.byte	0x8d
	.uleb128 0x2
	.byte	0x86
	.uleb128 0x4
	.byte	0x83
	.uleb128 0x5
	.align 8
.LEFDE17:
.LSFDE19:
	.long	.LEFDE19-.LASFDE19
.LASFDE19:
	.long	.LASFDE19-.Lframe1
	.long	.LFB50
	.long	.LFE50-.LFB50
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI33-.LFB50
	.byte	0x8e
	.uleb128 0x3
	.byte	0x83
	.uleb128 0x7
	.byte	0x4
	.long	.LCFI38-.LCFI33
	.byte	0xe
	.uleb128 0x50
	.byte	0x8d
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x6
	.byte	0x8f
	.uleb128 0x2
	.align 8
.LEFDE19:
.LSFDE21:
	.long	.LEFDE21-.LASFDE21
.LASFDE21:
	.long	.LASFDE21-.Lframe1
	.long	.LFB51
	.long	.LFE51-.LFB51
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI40-.LFB51
	.byte	0x86
	.uleb128 0x6
	.byte	0x83
	.uleb128 0x7
	.byte	0x4
	.long	.LCFI45-.LCFI40
	.byte	0xe
	.uleb128 0x70
	.byte	0x8f
	.uleb128 0x2
	.byte	0x8e
	.uleb128 0x3
	.byte	0x8d
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x5
	.align 8
.LEFDE21:
.LSFDE23:
	.long	.LEFDE23-.LASFDE23
.LASFDE23:
	.long	.LASFDE23-.Lframe1
	.long	.LFB48
	.long	.LFE48-.LFB48
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI47-.LFB48
	.byte	0x86
	.uleb128 0x3
	.byte	0x83
	.uleb128 0x4
	.byte	0x4
	.long	.LCFI49-.LCFI47
	.byte	0xe
	.uleb128 0x30
	.byte	0x8c
	.uleb128 0x2
	.align 8
.LEFDE23:
.LSFDE25:
	.long	.LEFDE25-.LASFDE25
.LASFDE25:
	.long	.LASFDE25-.Lframe1
	.long	.LFB69
	.long	.LFE69-.LFB69
	.uleb128 0x0
	.align 8
.LEFDE25:
.LSFDE27:
	.long	.LEFDE27-.LASFDE27
.LASFDE27:
	.long	.LASFDE27-.Lframe1
	.long	.LFB70
	.long	.LFE70-.LFB70
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI50-.LFB70
	.byte	0xe
	.uleb128 0x10
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI51-.LCFI50
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI52-.LCFI51
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x4
	.byte	0x86
	.uleb128 0x3
	.align 8
.LEFDE27:
.LSFDE29:
	.long	.LEFDE29-.LASFDE29
.LASFDE29:
	.long	.LASFDE29-.Lframe1
	.long	.LFB62
	.long	.LFE62-.LFB62
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI54-.LFB62
	.byte	0x8c
	.uleb128 0x3
	.byte	0x86
	.uleb128 0x4
	.byte	0x4
	.long	.LCFI57-.LCFI54
	.byte	0xe
	.uleb128 0x40
	.byte	0x83
	.uleb128 0x5
	.byte	0x8d
	.uleb128 0x2
	.align 8
.LEFDE29:
.LSFDE31:
	.long	.LEFDE31-.LASFDE31
.LASFDE31:
	.long	.LASFDE31-.Lframe1
	.long	.LFB71
	.long	.LFE71-.LFB71
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI58-.LFB71
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI59-.LCFI58
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI60-.LCFI59
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI61-.LCFI60
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.long	.LCFI62-.LCFI61
	.byte	0xe
	.uleb128 0x30
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
	.long	.LCFI63-.LCFI62
	.byte	0xe
	.uleb128 0x38
	.byte	0x4
	.long	.LCFI64-.LCFI63
	.byte	0xe
	.uleb128 0x100
	.byte	0x83
	.uleb128 0x7
	.align 8
.LEFDE31:
	.ident	"GCC: (SUSE Linux) 4.3.2 [gcc-4_3-branch revision 141291]"
	.section	.comment.SUSE.OPTs,"MS",@progbits,1
	.ascii	"OspWg"
	.section	.note.GNU-stack,"",@progbits
