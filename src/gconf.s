	.file	"gconf.c"
	.text
	.p2align 4,,15
	.type	conf_notify_func, @function
conf_notify_func:
.LFB53:
	movq	%rbx, -24(%rsp)
.LCFI0:
	movq	%r12, -16(%rsp)
.LCFI1:
	movq	%rdx, %rdi
	movq	%r13, -8(%rsp)
.LCFI2:
	subq	$24, %rsp
.LCFI3:
	movq	8(%rcx), %r13
	movq	16(%rcx), %r12
	movq	%rcx, %rbx
	call	gconf_entry_get_key
	movq	(%rbx), %rdi
	movq	%r13, %r11
	movq	(%rsp), %rbx
	movq	%r12, %rdx
	movq	16(%rsp), %r13
	movq	8(%rsp), %r12
	movq	%rax, %rsi
	addq	$24, %rsp
	jmp	*%r11
.LFE53:
	.size	conf_notify_func, .-conf_notify_func
	.p2align 4,,15
.globl conf_shutdown
	.type	conf_shutdown, @function
conf_shutdown:
.LFB43:
	subq	$8, %rsp
.LCFI4:
	movq	global_conf(%rip), %rdi
	testq	%rdi, %rdi
	je	.L5
	call	g_object_unref
	movq	$0, global_conf(%rip)
.L5:
	addq	$8, %rsp
	ret
.LFE43:
	.size	conf_shutdown, .-conf_shutdown
	.p2align 4,,15
	.type	conf_notify_data_free, @function
conf_notify_data_free:
.LFB52:
	pushq	%rbx
.LCFI5:
	movq	%rdi, %rbx
	movq	(%rdi), %rdi
	call	g_object_unref
	movq	%rbx, %rsi
	movl	$24, %edi
	popq	%rbx
	jmp	g_slice_free1
.LFE52:
	.size	conf_notify_data_free, .-conf_notify_data_free
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"Conf"
	.text
	.p2align 4,,15
.globl conf_get_type
	.type	conf_get_type, @function
conf_get_type:
.LFB38:
	subq	$8, %rsp
.LCFI6:
	movq	g_define_type_id__volatile.10652(%rip), %rax
	testq	%rax, %rax
	je	.L15
.L10:
	movq	g_define_type_id__volatile.10652(%rip), %rax
	addq	$8, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L15:
	movl	$g_define_type_id__volatile.10652, %edi
	call	g_once_init_enter_impl
	testl	%eax, %eax
	je	.L10
	movl	$.LC0, %edi
	call	g_intern_static_string
	movl	$conf_init, %r9d
	movq	%rax, %rsi
	movl	$24, %r8d
	movl	$conf_class_intern_init, %ecx
	movl	$136, %edx
	movl	$80, %edi
	movl	$0, (%rsp)
	call	g_type_register_static_simple
	movl	$g_define_type_id__volatile.10652, %edi
	movq	%rax, %rsi
	call	g_once_init_leave
	movq	g_define_type_id__volatile.10652(%rip), %rax
	addq	$8, %rsp
	ret
.LFE38:
	.size	conf_get_type, .-conf_get_type
	.p2align 4,,15
.globl conf_get
	.type	conf_get, @function
conf_get:
.LFB42:
	subq	$8, %rsp
.LCFI7:
	cmpq	$0, global_conf(%rip)
	je	.L19
.L17:
	movq	global_conf(%rip), %rax
	addq	$8, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L19:
	call	conf_get_type
	xorl	%esi, %esi
	movq	%rax, %rdi
	xorl	%eax, %eax
	call	g_object_new
	movq	%rax, global_conf(%rip)
	jmp	.L17
.LFE42:
	.size	conf_get, .-conf_get
	.p2align 4,,15
	.type	conf_class_intern_init, @function
conf_class_intern_init:
.LFB37:
	pushq	%rbx
.LCFI8:
	movq	%rdi, %rbx
	call	g_type_class_peek_parent
	movq	%rbx, %rdi
	movl	$80, %esi
	movq	%rax, conf_parent_class(%rip)
	call	g_type_check_class_cast
	movq	$conf_finalize, 48(%rax)
	movq	%rax, %rdi
	movl	$8, %esi
	popq	%rbx
	jmp	g_type_class_add_private
.LFE37:
	.size	conf_class_intern_init, .-conf_class_intern_init
	.section	.rodata.str1.1
.LC1:
	.string	"/apps/greet-tweet-know"
.LC2:
	.string	"/desktop/gnome/interface"
	.text
	.p2align 4,,15
	.type	conf_finalize, @function
conf_finalize:
.LFB41:
	movq	config_priv(%rip), %rax
	pushq	%rbx
.LCFI9:
	movq	%rdi, %rbx
	xorl	%edx, %edx
	movl	$.LC1, %esi
	movq	(%rax), %rdi
	call	gconf_client_remove_dir
	movq	config_priv(%rip), %rax
	xorl	%edx, %edx
	movl	$.LC2, %esi
	movq	(%rax), %rdi
	call	gconf_client_remove_dir
	movq	config_priv(%rip), %rax
	movq	(%rax), %rdi
	call	g_object_unref
	movq	conf_parent_class(%rip), %rdi
	movl	$80, %esi
	call	g_type_check_class_cast
	movq	%rbx, %rdi
	movq	48(%rax), %r11
	popq	%rbx
	jmp	*%r11
.LFE41:
	.size	conf_finalize, .-conf_finalize
	.p2align 4,,15
	.type	conf_init, @function
conf_init:
.LFB40:
	pushq	%rbx
.LCFI10:
	movq	%rdi, %rbx
	subq	$16, %rsp
.LCFI11:
	movq	g_define_type_id__volatile.10652(%rip), %rax
	testq	%rax, %rax
	je	.L30
.L25:
	movq	g_define_type_id__volatile.10652(%rip), %rsi
	movq	%rbx, %rdi
	call	g_type_instance_get_private
	movq	%rax, %rbx
	movq	%rax, config_priv(%rip)
	call	gconf_client_get_default
	movq	%rax, (%rbx)
	movq	config_priv(%rip), %rax
	xorl	%ecx, %ecx
	movl	$1, %edx
	movl	$.LC1, %esi
	movq	(%rax), %rdi
	call	gconf_client_add_dir
	movq	config_priv(%rip), %rax
	xorl	%ecx, %ecx
	xorl	%edx, %edx
	movl	$.LC2, %esi
	movq	(%rax), %rdi
	addq	$16, %rsp
	popq	%rbx
	jmp	gconf_client_add_dir
	.p2align 4,,10
	.p2align 3
.L30:
	movl	$g_define_type_id__volatile.10652, %edi
	call	g_once_init_enter_impl
	testl	%eax, %eax
	je	.L25
	movl	$.LC0, %edi
	call	g_intern_static_string
	movl	$conf_init, %r9d
	movq	%rax, %rsi
	movl	$24, %r8d
	movl	$conf_class_intern_init, %ecx
	movl	$136, %edx
	movl	$80, %edi
	movl	$0, (%rsp)
	call	g_type_register_static_simple
	movl	$g_define_type_id__volatile.10652, %edi
	movq	%rax, %rsi
	call	g_once_init_leave
	jmp	.L25
.LFE40:
	.size	conf_init, .-conf_init
	.section	.rodata.str1.1
.LC3:
	.string	"IS_CONF(conf)"
	.text
	.p2align 4,,15
.globl conf_notify_remove
	.type	conf_notify_remove, @function
conf_notify_remove:
.LFB55:
	pushq	%rbp
.LCFI12:
	movl	%esi, %ebp
	pushq	%rbx
.LCFI13:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI14:
	movq	g_define_type_id__volatile.10652(%rip), %rax
	testq	%rax, %rax
	je	.L41
.L32:
	testq	%rbx, %rbx
	movq	g_define_type_id__volatile.10652(%rip), %rsi
	je	.L34
	movq	(%rbx), %rax
	testq	%rax, %rax
	je	.L34
	cmpq	%rsi, (%rax)
	je	.L35
.L34:
	movq	%rbx, %rdi
	call	g_type_check_instance_is_a
	testl	%eax, %eax
	.p2align 4,,2
	.p2align 3
	jne	.L35
	movl	$.LC3, %edx
	movl	$__PRETTY_FUNCTION__.10954, %esi
	xorl	%edi, %edi
	call	g_return_if_fail_warning
	addq	$8, %rsp
	xorl	%eax, %eax
	popq	%rbx
	popq	%rbp
	ret
	.p2align 4,,10
	.p2align 3
.L35:
	movq	config_priv(%rip), %rax
	movl	%ebp, %esi
	movq	(%rax), %rdi
	call	gconf_client_notify_remove
	addq	$8, %rsp
	movl	$1, %eax
	popq	%rbx
	popq	%rbp
	ret
	.p2align 4,,10
	.p2align 3
.L41:
	movl	$g_define_type_id__volatile.10652, %edi
	call	g_once_init_enter_impl
	testl	%eax, %eax
	je	.L32
	movl	$.LC0, %edi
	call	g_intern_static_string
	movl	$conf_init, %r9d
	movq	%rax, %rsi
	movl	$24, %r8d
	movl	$conf_class_intern_init, %ecx
	movl	$136, %edx
	movl	$80, %edi
	movl	$0, (%rsp)
	call	g_type_register_static_simple
	movl	$g_define_type_id__volatile.10652, %edi
	movq	%rax, %rsi
	call	g_once_init_leave
	jmp	.L32
.LFE55:
	.size	conf_notify_remove, .-conf_notify_remove
	.p2align 4,,15
.globl conf_notify_add
	.type	conf_notify_add, @function
conf_notify_add:
.LFB54:
	movq	%rbp, -32(%rsp)
.LCFI15:
	movq	%r12, -24(%rsp)
.LCFI16:
	movq	%rdi, %rbp
	movq	%r13, -16(%rsp)
.LCFI17:
	movq	%r14, -8(%rsp)
.LCFI18:
	movq	%rdx, %r12
	movq	%rbx, -40(%rsp)
.LCFI19:
	subq	$56, %rsp
.LCFI20:
	movq	g_define_type_id__volatile.10652(%rip), %rax
	movq	%rsi, %r14
	movq	%rcx, %r13
	testq	%rax, %rax
	je	.L52
.L43:
	testq	%rbp, %rbp
	movq	g_define_type_id__volatile.10652(%rip), %rsi
	je	.L45
	movq	(%rbp), %rax
	testq	%rax, %rax
	je	.L45
	cmpq	%rsi, (%rax)
	je	.L46
.L45:
	movq	%rbp, %rdi
	call	g_type_check_instance_is_a
	testl	%eax, %eax
	.p2align 4,,2
	.p2align 3
	jne	.L46
	movl	$.LC3, %edx
	movl	$__PRETTY_FUNCTION__.10929, %esi
	xorl	%edi, %edi
	call	g_return_if_fail_warning
	xorl	%eax, %eax
	movq	16(%rsp), %rbx
	movq	24(%rsp), %rbp
	movq	32(%rsp), %r12
	movq	40(%rsp), %r13
	movq	48(%rsp), %r14
	addq	$56, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L46:
	movl	$24, %edi
	call	g_slice_alloc
	movq	%rbp, %rdi
	movq	%rax, %rbx
	movq	%r12, 8(%rax)
	movq	%r13, 16(%rax)
	call	g_object_ref
	movq	%rax, (%rbx)
	movq	config_priv(%rip), %rax
	movq	%rbx, %rcx
	movq	%r14, %rsi
	movq	16(%rsp), %rbx
	movq	24(%rsp), %rbp
	movq	32(%rsp), %r12
	movq	40(%rsp), %r13
	xorl	%r9d, %r9d
	movq	(%rax), %rdi
	movq	48(%rsp), %r14
	movl	$conf_notify_data_free, %r8d
	movl	$conf_notify_func, %edx
	addq	$56, %rsp
	jmp	gconf_client_notify_add
	.p2align 4,,10
	.p2align 3
.L52:
	movl	$g_define_type_id__volatile.10652, %edi
	call	g_once_init_enter_impl
	testl	%eax, %eax
	je	.L43
	movl	$.LC0, %edi
	call	g_intern_static_string
	movl	$conf_init, %r9d
	movq	%rax, %rsi
	movl	$24, %r8d
	movl	$conf_class_intern_init, %ecx
	movl	$136, %edx
	movl	$80, %edi
	movl	$0, (%rsp)
	call	g_type_register_static_simple
	movl	$g_define_type_id__volatile.10652, %edi
	movq	%rax, %rsi
	call	g_once_init_leave
	jmp	.L43
.LFE54:
	.size	conf_notify_add, .-conf_notify_add
	.p2align 4,,15
.globl conf_get_string_list
	.type	conf_get_string_list, @function
conf_get_string_list:
.LFB51:
	movq	%rbx, -24(%rsp)
.LCFI21:
	movq	%rbp, -16(%rsp)
.LCFI22:
	movq	%rdi, %rbx
	movq	%r12, -8(%rsp)
.LCFI23:
	subq	$56, %rsp
.LCFI24:
	movq	g_define_type_id__volatile.10652(%rip), %rax
	movq	%rsi, %r12
	movq	%rdx, %rbp
	movq	$0, 24(%rsp)
	movq	$0, (%rdx)
	testq	%rax, %rax
	je	.L64
.L54:
	testq	%rbx, %rbx
	movq	g_define_type_id__volatile.10652(%rip), %rsi
	je	.L56
	movq	(%rbx), %rax
	testq	%rax, %rax
	je	.L56
	cmpq	%rsi, (%rax)
	je	.L57
.L56:
	movq	%rbx, %rdi
	call	g_type_check_instance_is_a
	testl	%eax, %eax
	.p2align 4,,2
	.p2align 3
	jne	.L57
	movl	$.LC3, %edx
	movl	$__PRETTY_FUNCTION__.10888, %esi
	xorl	%edi, %edi
	call	g_return_if_fail_warning
	xorl	%eax, %eax
.L60:
	movq	32(%rsp), %rbx
	movq	40(%rsp), %rbp
	movq	48(%rsp), %r12
	addq	$56, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L57:
	movq	config_priv(%rip), %rax
	leaq	24(%rsp), %rcx
	movl	$1, %edx
	movq	%r12, %rsi
	movq	(%rax), %rdi
	call	gconf_client_get_list
	movq	24(%rsp), %rdi
	movq	%rax, (%rbp)
	movl	$1, %eax
	testq	%rdi, %rdi
	je	.L60
	call	g_error_free
	xorl	%eax, %eax
	jmp	.L60
	.p2align 4,,10
	.p2align 3
.L64:
	movl	$g_define_type_id__volatile.10652, %edi
	call	g_once_init_enter_impl
	testl	%eax, %eax
	je	.L54
	movl	$.LC0, %edi
	call	g_intern_static_string
	movl	$conf_init, %r9d
	movq	%rax, %rsi
	movl	$24, %r8d
	movl	$conf_class_intern_init, %ecx
	movl	$136, %edx
	movl	$80, %edi
	movl	$0, (%rsp)
	call	g_type_register_static_simple
	movl	$g_define_type_id__volatile.10652, %edi
	movq	%rax, %rsi
	call	g_once_init_leave
	jmp	.L54
.LFE51:
	.size	conf_get_string_list, .-conf_get_string_list
	.p2align 4,,15
.globl conf_set_string_list
	.type	conf_set_string_list, @function
conf_set_string_list:
.LFB50:
	movq	%rbx, -24(%rsp)
.LCFI25:
	movq	%rbp, -16(%rsp)
.LCFI26:
	movq	%rdi, %rbx
	movq	%r12, -8(%rsp)
.LCFI27:
	subq	$40, %rsp
.LCFI28:
	movq	g_define_type_id__volatile.10652(%rip), %rax
	movq	%rsi, %r12
	movq	%rdx, %rbp
	testq	%rax, %rax
	je	.L75
.L66:
	testq	%rbx, %rbx
	movq	g_define_type_id__volatile.10652(%rip), %rsi
	je	.L68
	movq	(%rbx), %rax
	testq	%rax, %rax
	je	.L68
	cmpq	%rsi, (%rax)
	je	.L69
.L68:
	movq	%rbx, %rdi
	call	g_type_check_instance_is_a
	testl	%eax, %eax
	.p2align 4,,2
	.p2align 3
	jne	.L69
	movl	$.LC3, %edx
	movl	$__PRETTY_FUNCTION__.10865, %esi
	xorl	%edi, %edi
	call	g_return_if_fail_warning
	xorl	%eax, %eax
	movq	16(%rsp), %rbx
	movq	24(%rsp), %rbp
	movq	32(%rsp), %r12
	addq	$40, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L69:
	movq	config_priv(%rip), %rax
	movq	%rbp, %rcx
	movq	%r12, %rsi
	movq	16(%rsp), %rbx
	movq	24(%rsp), %rbp
	xorl	%r8d, %r8d
	movq	32(%rsp), %r12
	movl	$1, %edx
	movq	(%rax), %rdi
	addq	$40, %rsp
	jmp	gconf_client_set_list
	.p2align 4,,10
	.p2align 3
.L75:
	movl	$g_define_type_id__volatile.10652, %edi
	call	g_once_init_enter_impl
	testl	%eax, %eax
	je	.L66
	movl	$.LC0, %edi
	call	g_intern_static_string
	movl	$conf_init, %r9d
	movq	%rax, %rsi
	movl	$24, %r8d
	movl	$conf_class_intern_init, %ecx
	movl	$136, %edx
	movl	$80, %edi
	movl	$0, (%rsp)
	call	g_type_register_static_simple
	movl	$g_define_type_id__volatile.10652, %edi
	movq	%rax, %rsi
	call	g_once_init_leave
	jmp	.L66
.LFE50:
	.size	conf_set_string_list, .-conf_set_string_list
	.section	.rodata.str1.1
.LC4:
	.string	"None"
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC5:
	.string	"Getting string:'%s'(='%s'), error:'%s'"
	.section	.rodata.str1.1
.LC6:
	.string	"Config"
	.text
	.p2align 4,,15
.globl conf_get_string
	.type	conf_get_string, @function
conf_get_string:
.LFB49:
	pushq	%r12
.LCFI29:
	movq	%rsi, %r12
	pushq	%rbp
.LCFI30:
	movq	%rdx, %rbp
	pushq	%rbx
.LCFI31:
	movq	%rdi, %rbx
	subq	$32, %rsp
.LCFI32:
	movq	g_define_type_id__volatile.10652(%rip), %rax
	movq	$0, (%rdx)
	movq	$0, 24(%rsp)
	testq	%rax, %rax
	je	.L89
.L77:
	testq	%rbx, %rbx
	movq	g_define_type_id__volatile.10652(%rip), %rsi
	je	.L79
	movq	(%rbx), %rax
	testq	%rax, %rax
	je	.L79
	cmpq	%rsi, (%rax)
	je	.L80
.L79:
	movq	%rbx, %rdi
	call	g_type_check_instance_is_a
	testl	%eax, %eax
	.p2align 4,,2
	.p2align 3
	jne	.L80
	movl	$.LC3, %edx
	movl	$__PRETTY_FUNCTION__.10840, %esi
	xorl	%edi, %edi
	call	g_return_if_fail_warning
	xorl	%eax, %eax
.L85:
	addq	$32, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
	.p2align 4,,10
	.p2align 3
.L80:
	movq	config_priv(%rip), %rax
	leaq	24(%rsp), %rdx
	movq	%r12, %rsi
	movq	(%rax), %rdi
	call	gconf_client_get_string
	movq	24(%rsp), %rdx
	movq	%rax, %rcx
	movq	%rax, (%rbp)
	movl	$.LC4, %eax
	testq	%rdx, %rdx
	je	.L83
	movq	8(%rdx), %rax
.L83:
	movq	%rax, %r8
	movl	$.LC6, %edi
	xorl	%eax, %eax
	movq	%r12, %rdx
	movl	$.LC5, %esi
	call	debug_impl
	movq	24(%rsp), %rdi
	movl	$1, %eax
	testq	%rdi, %rdi
	je	.L85
	call	g_error_free
	addq	$32, %rsp
	xorl	%eax, %eax
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
	.p2align 4,,10
	.p2align 3
.L89:
	movl	$g_define_type_id__volatile.10652, %edi
	call	g_once_init_enter_impl
	testl	%eax, %eax
	je	.L77
	movl	$.LC0, %edi
	call	g_intern_static_string
	movl	$conf_init, %r9d
	movq	%rax, %rsi
	movl	$24, %r8d
	movl	$conf_class_intern_init, %ecx
	movl	$136, %edx
	movl	$80, %edi
	movl	$0, (%rsp)
	call	g_type_register_static_simple
	movl	$g_define_type_id__volatile.10652, %edi
	movq	%rax, %rsi
	call	g_once_init_leave
	jmp	.L77
.LFE49:
	.size	conf_get_string, .-conf_get_string
	.section	.rodata.str1.1
.LC7:
	.string	"Setting string:'%s' to '%s'"
	.text
	.p2align 4,,15
.globl conf_set_string
	.type	conf_set_string, @function
conf_set_string:
.LFB48:
	movq	%rbx, -24(%rsp)
.LCFI33:
	movq	%rbp, -16(%rsp)
.LCFI34:
	movq	%rdi, %rbx
	movq	%r12, -8(%rsp)
.LCFI35:
	subq	$40, %rsp
.LCFI36:
	movq	g_define_type_id__volatile.10652(%rip), %rax
	movq	%rsi, %r12
	movq	%rdx, %rbp
	testq	%rax, %rax
	je	.L100
.L91:
	testq	%rbx, %rbx
	movq	g_define_type_id__volatile.10652(%rip), %rsi
	je	.L93
	movq	(%rbx), %rax
	testq	%rax, %rax
	je	.L93
	cmpq	%rsi, (%rax)
	je	.L94
.L93:
	movq	%rbx, %rdi
	call	g_type_check_instance_is_a
	testl	%eax, %eax
	.p2align 4,,2
	.p2align 3
	jne	.L94
	movl	$.LC3, %edx
	movl	$__PRETTY_FUNCTION__.10817, %esi
	xorl	%edi, %edi
	call	g_return_if_fail_warning
	xorl	%eax, %eax
	movq	16(%rsp), %rbx
	movq	24(%rsp), %rbp
	movq	32(%rsp), %r12
	addq	$40, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L94:
	movq	%rbp, %rcx
	movq	%r12, %rdx
	movl	$.LC7, %esi
	movl	$.LC6, %edi
	xorl	%eax, %eax
	call	debug_impl
	movq	config_priv(%rip), %rax
	movq	%rbp, %rdx
	movq	%r12, %rsi
	movq	16(%rsp), %rbx
	movq	24(%rsp), %rbp
	xorl	%ecx, %ecx
	movq	32(%rsp), %r12
	movq	(%rax), %rdi
	addq	$40, %rsp
	jmp	gconf_client_set_string
	.p2align 4,,10
	.p2align 3
.L100:
	movl	$g_define_type_id__volatile.10652, %edi
	call	g_once_init_enter_impl
	testl	%eax, %eax
	je	.L91
	movl	$.LC0, %edi
	call	g_intern_static_string
	movl	$conf_init, %r9d
	movq	%rax, %rsi
	movl	$24, %r8d
	movl	$conf_class_intern_init, %ecx
	movl	$136, %edx
	movl	$80, %edi
	movl	$0, (%rsp)
	call	g_type_register_static_simple
	movl	$g_define_type_id__volatile.10652, %edi
	movq	%rax, %rsi
	call	g_once_init_leave
	jmp	.L91
.LFE48:
	.size	conf_set_string, .-conf_set_string
	.section	.rodata.str1.1
.LC8:
	.string	"false"
.LC9:
	.string	"true"
	.section	.rodata.str1.8
	.align 8
.LC10:
	.string	"Getting bool:'%s'(=%d ---> %s), error:'%s'"
	.text
	.p2align 4,,15
.globl conf_get_bool
	.type	conf_get_bool, @function
conf_get_bool:
.LFB47:
	pushq	%r12
.LCFI37:
	movq	%rsi, %r12
	pushq	%rbp
.LCFI38:
	movq	%rdx, %rbp
	pushq	%rbx
.LCFI39:
	movq	%rdi, %rbx
	subq	$32, %rsp
.LCFI40:
	movq	g_define_type_id__volatile.10652(%rip), %rax
	movl	$0, (%rdx)
	movq	$0, 24(%rsp)
	testq	%rax, %rax
	je	.L116
.L102:
	testq	%rbx, %rbx
	movq	g_define_type_id__volatile.10652(%rip), %rsi
	je	.L104
	movq	(%rbx), %rax
	testq	%rax, %rax
	je	.L104
	cmpq	%rsi, (%rax)
	je	.L105
.L104:
	movq	%rbx, %rdi
	call	g_type_check_instance_is_a
	testl	%eax, %eax
	.p2align 4,,2
	.p2align 3
	jne	.L105
	movl	$.LC3, %edx
	movl	$__PRETTY_FUNCTION__.10787, %esi
	xorl	%edi, %edi
	call	g_return_if_fail_warning
	xorl	%eax, %eax
.L112:
	addq	$32, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
	.p2align 4,,10
	.p2align 3
.L105:
	movq	config_priv(%rip), %rax
	leaq	24(%rsp), %rdx
	movq	%r12, %rsi
	movq	(%rax), %rdi
	call	gconf_client_get_bool
	movl	%eax, %edx
	movl	%eax, (%rbp)
	movq	24(%rsp), %rax
	movl	$.LC4, %ecx
	testq	%rax, %rax
	je	.L108
	movq	8(%rax), %rcx
.L108:
	testl	%edx, %edx
	movl	$.LC8, %eax
	movl	$.LC9, %r8d
	cmove	%rax, %r8
	movq	%rcx, %r9
	xorl	%eax, %eax
	movl	%edx, %ecx
	movl	$.LC6, %edi
	movq	%r12, %rdx
	movl	$.LC10, %esi
	call	debug_impl
	movq	24(%rsp), %rdi
	movl	$1, %eax
	testq	%rdi, %rdi
	je	.L112
	call	g_error_free
	addq	$32, %rsp
	xorl	%eax, %eax
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
	.p2align 4,,10
	.p2align 3
.L116:
	movl	$g_define_type_id__volatile.10652, %edi
	call	g_once_init_enter_impl
	testl	%eax, %eax
	je	.L102
	movl	$.LC0, %edi
	call	g_intern_static_string
	movl	$conf_init, %r9d
	movq	%rax, %rsi
	movl	$24, %r8d
	movl	$conf_class_intern_init, %ecx
	movl	$136, %edx
	movl	$80, %edi
	movl	$0, (%rsp)
	call	g_type_register_static_simple
	movl	$g_define_type_id__volatile.10652, %edi
	movq	%rax, %rsi
	call	g_once_init_leave
	jmp	.L102
.LFE47:
	.size	conf_get_bool, .-conf_get_bool
	.section	.rodata.str1.8
	.align 8
.LC11:
	.string	"Setting bool:'%s' to %d ---> %s"
	.text
	.p2align 4,,15
.globl conf_set_bool
	.type	conf_set_bool, @function
conf_set_bool:
.LFB46:
	movq	%rbx, -24(%rsp)
.LCFI41:
	movq	%rbp, -16(%rsp)
.LCFI42:
	movq	%rdi, %rbx
	movq	%r12, -8(%rsp)
.LCFI43:
	subq	$40, %rsp
.LCFI44:
	movq	g_define_type_id__volatile.10652(%rip), %rax
	movq	%rsi, %r12
	movl	%edx, %ebp
	testq	%rax, %rax
	je	.L129
.L118:
	testq	%rbx, %rbx
	movq	g_define_type_id__volatile.10652(%rip), %rsi
	je	.L120
	movq	(%rbx), %rax
	testq	%rax, %rax
	je	.L120
	cmpq	%rsi, (%rax)
	je	.L121
.L120:
	movq	%rbx, %rdi
	call	g_type_check_instance_is_a
	testl	%eax, %eax
	.p2align 4,,2
	.p2align 3
	jne	.L121
	movl	$.LC3, %edx
	movl	$__PRETTY_FUNCTION__.10763, %esi
	xorl	%edi, %edi
	call	g_return_if_fail_warning
	xorl	%eax, %eax
	movq	16(%rsp), %rbx
	movq	24(%rsp), %rbp
	movq	32(%rsp), %r12
	addq	$40, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L121:
	testl	%ebp, %ebp
	movl	$.LC8, %eax
	movl	$.LC9, %r8d
	cmove	%rax, %r8
	movl	%ebp, %ecx
	movq	%r12, %rdx
	movl	$.LC11, %esi
	movl	$.LC6, %edi
	xorl	%eax, %eax
	call	debug_impl
	movq	config_priv(%rip), %rax
	movl	%ebp, %edx
	movq	%r12, %rsi
	movq	16(%rsp), %rbx
	movq	24(%rsp), %rbp
	xorl	%ecx, %ecx
	movq	32(%rsp), %r12
	movq	(%rax), %rdi
	addq	$40, %rsp
	jmp	gconf_client_set_bool
	.p2align 4,,10
	.p2align 3
.L129:
	movl	$g_define_type_id__volatile.10652, %edi
	call	g_once_init_enter_impl
	testl	%eax, %eax
	je	.L118
	movl	$.LC0, %edi
	call	g_intern_static_string
	movl	$conf_init, %r9d
	movq	%rax, %rsi
	movl	$24, %r8d
	movl	$conf_class_intern_init, %ecx
	movl	$136, %edx
	movl	$80, %edi
	movl	$0, (%rsp)
	call	g_type_register_static_simple
	movl	$g_define_type_id__volatile.10652, %edi
	movq	%rax, %rsi
	call	g_once_init_leave
	jmp	.L118
.LFE46:
	.size	conf_set_bool, .-conf_set_bool
	.section	.rodata.str1.8
	.align 8
.LC12:
	.string	"Getting int:'%s'(=%d), error:'%s'"
	.text
	.p2align 4,,15
.globl conf_get_int
	.type	conf_get_int, @function
conf_get_int:
.LFB45:
	pushq	%r12
.LCFI45:
	movq	%rsi, %r12
	pushq	%rbp
.LCFI46:
	movq	%rdx, %rbp
	pushq	%rbx
.LCFI47:
	movq	%rdi, %rbx
	subq	$32, %rsp
.LCFI48:
	movq	g_define_type_id__volatile.10652(%rip), %rax
	movl	$0, (%rdx)
	movq	$0, 24(%rsp)
	testq	%rax, %rax
	je	.L143
.L131:
	testq	%rbx, %rbx
	movq	g_define_type_id__volatile.10652(%rip), %rsi
	je	.L133
	movq	(%rbx), %rax
	testq	%rax, %rax
	je	.L133
	cmpq	%rsi, (%rax)
	je	.L134
.L133:
	movq	%rbx, %rdi
	call	g_type_check_instance_is_a
	testl	%eax, %eax
	.p2align 4,,2
	.p2align 3
	jne	.L134
	movl	$.LC3, %edx
	movl	$__PRETTY_FUNCTION__.10734, %esi
	xorl	%edi, %edi
	call	g_return_if_fail_warning
	xorl	%eax, %eax
.L139:
	addq	$32, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
	.p2align 4,,10
	.p2align 3
.L134:
	movq	config_priv(%rip), %rax
	leaq	24(%rsp), %rdx
	movq	%r12, %rsi
	movq	(%rax), %rdi
	call	gconf_client_get_int
	movq	24(%rsp), %rdx
	movl	%eax, (%rbp)
	movl	$.LC4, %ecx
	testq	%rdx, %rdx
	je	.L137
	movq	8(%rdx), %rcx
.L137:
	movq	%rcx, %r8
	movl	$.LC6, %edi
	movl	%eax, %ecx
	movq	%r12, %rdx
	xorl	%eax, %eax
	movl	$.LC12, %esi
	call	debug_impl
	movq	24(%rsp), %rdi
	movl	$1, %eax
	testq	%rdi, %rdi
	je	.L139
	call	g_error_free
	addq	$32, %rsp
	xorl	%eax, %eax
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
	.p2align 4,,10
	.p2align 3
.L143:
	movl	$g_define_type_id__volatile.10652, %edi
	call	g_once_init_enter_impl
	testl	%eax, %eax
	je	.L131
	movl	$.LC0, %edi
	call	g_intern_static_string
	movl	$conf_init, %r9d
	movq	%rax, %rsi
	movl	$24, %r8d
	movl	$conf_class_intern_init, %ecx
	movl	$136, %edx
	movl	$80, %edi
	movl	$0, (%rsp)
	call	g_type_register_static_simple
	movl	$g_define_type_id__volatile.10652, %edi
	movq	%rax, %rsi
	call	g_once_init_leave
	jmp	.L131
.LFE45:
	.size	conf_get_int, .-conf_get_int
	.section	.rodata.str1.1
.LC13:
	.string	"Setting int:'%s' to %d"
	.text
	.p2align 4,,15
.globl conf_set_int
	.type	conf_set_int, @function
conf_set_int:
.LFB44:
	movq	%rbx, -24(%rsp)
.LCFI49:
	movq	%rbp, -16(%rsp)
.LCFI50:
	movq	%rdi, %rbx
	movq	%r12, -8(%rsp)
.LCFI51:
	subq	$40, %rsp
.LCFI52:
	movq	g_define_type_id__volatile.10652(%rip), %rax
	movq	%rsi, %r12
	movl	%edx, %ebp
	testq	%rax, %rax
	je	.L154
.L145:
	testq	%rbx, %rbx
	movq	g_define_type_id__volatile.10652(%rip), %rsi
	je	.L147
	movq	(%rbx), %rax
	testq	%rax, %rax
	je	.L147
	cmpq	%rsi, (%rax)
	je	.L148
.L147:
	movq	%rbx, %rdi
	call	g_type_check_instance_is_a
	testl	%eax, %eax
	.p2align 4,,2
	.p2align 3
	jne	.L148
	movl	$.LC3, %edx
	movl	$__PRETTY_FUNCTION__.10711, %esi
	xorl	%edi, %edi
	call	g_return_if_fail_warning
	xorl	%eax, %eax
	movq	16(%rsp), %rbx
	movq	24(%rsp), %rbp
	movq	32(%rsp), %r12
	addq	$40, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L148:
	movl	%ebp, %ecx
	movq	%r12, %rdx
	movl	$.LC13, %esi
	movl	$.LC6, %edi
	xorl	%eax, %eax
	call	debug_impl
	movq	config_priv(%rip), %rax
	movl	%ebp, %edx
	movq	%r12, %rsi
	movq	16(%rsp), %rbx
	movq	24(%rsp), %rbp
	xorl	%ecx, %ecx
	movq	32(%rsp), %r12
	movq	(%rax), %rdi
	addq	$40, %rsp
	jmp	gconf_client_set_int
	.p2align 4,,10
	.p2align 3
.L154:
	movl	$g_define_type_id__volatile.10652, %edi
	call	g_once_init_enter_impl
	testl	%eax, %eax
	je	.L145
	movl	$.LC0, %edi
	call	g_intern_static_string
	movl	$conf_init, %r9d
	movq	%rax, %rsi
	movl	$24, %r8d
	movl	$conf_class_intern_init, %ecx
	movl	$136, %edx
	movl	$80, %edi
	movl	$0, (%rsp)
	call	g_type_register_static_simple
	movl	$g_define_type_id__volatile.10652, %edi
	movq	%rax, %rsi
	call	g_once_init_leave
	jmp	.L145
.LFE44:
	.size	conf_set_int, .-conf_set_int
	.local	config_priv
	.comm	config_priv,8,8
	.section	.rodata
	.align 16
	.type	__PRETTY_FUNCTION__.10954, @object
	.size	__PRETTY_FUNCTION__.10954, 19
__PRETTY_FUNCTION__.10954:
	.string	"conf_notify_remove"
	.align 16
	.type	__PRETTY_FUNCTION__.10929, @object
	.size	__PRETTY_FUNCTION__.10929, 16
__PRETTY_FUNCTION__.10929:
	.string	"conf_notify_add"
	.align 16
	.type	__PRETTY_FUNCTION__.10888, @object
	.size	__PRETTY_FUNCTION__.10888, 21
__PRETTY_FUNCTION__.10888:
	.string	"conf_get_string_list"
	.align 16
	.type	__PRETTY_FUNCTION__.10865, @object
	.size	__PRETTY_FUNCTION__.10865, 21
__PRETTY_FUNCTION__.10865:
	.string	"conf_set_string_list"
	.align 16
	.type	__PRETTY_FUNCTION__.10840, @object
	.size	__PRETTY_FUNCTION__.10840, 16
__PRETTY_FUNCTION__.10840:
	.string	"conf_get_string"
	.align 16
	.type	__PRETTY_FUNCTION__.10817, @object
	.size	__PRETTY_FUNCTION__.10817, 16
__PRETTY_FUNCTION__.10817:
	.string	"conf_set_string"
	.type	__PRETTY_FUNCTION__.10787, @object
	.size	__PRETTY_FUNCTION__.10787, 14
__PRETTY_FUNCTION__.10787:
	.string	"conf_get_bool"
	.type	__PRETTY_FUNCTION__.10763, @object
	.size	__PRETTY_FUNCTION__.10763, 14
__PRETTY_FUNCTION__.10763:
	.string	"conf_set_bool"
	.type	__PRETTY_FUNCTION__.10734, @object
	.size	__PRETTY_FUNCTION__.10734, 13
__PRETTY_FUNCTION__.10734:
	.string	"conf_get_int"
	.type	__PRETTY_FUNCTION__.10711, @object
	.size	__PRETTY_FUNCTION__.10711, 13
__PRETTY_FUNCTION__.10711:
	.string	"conf_set_int"
	.local	global_conf
	.comm	global_conf,8,8
	.local	g_define_type_id__volatile.10652
	.comm	g_define_type_id__volatile.10652,8,8
	.local	conf_parent_class
	.comm	conf_parent_class,8,8
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
	.long	.LFB53
	.long	.LFE53-.LFB53
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI3-.LFB53
	.byte	0xe
	.uleb128 0x20
	.byte	0x8d
	.uleb128 0x2
	.byte	0x8c
	.uleb128 0x3
	.byte	0x83
	.uleb128 0x4
	.align 8
.LEFDE1:
.LSFDE3:
	.long	.LEFDE3-.LASFDE3
.LASFDE3:
	.long	.LASFDE3-.Lframe1
	.long	.LFB43
	.long	.LFE43-.LFB43
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI4-.LFB43
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE3:
.LSFDE5:
	.long	.LEFDE5-.LASFDE5
.LASFDE5:
	.long	.LASFDE5-.Lframe1
	.long	.LFB52
	.long	.LFE52-.LFB52
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI5-.LFB52
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
	.long	.LFB38
	.long	.LFE38-.LFB38
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI6-.LFB38
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE7:
.LSFDE9:
	.long	.LEFDE9-.LASFDE9
.LASFDE9:
	.long	.LASFDE9-.Lframe1
	.long	.LFB42
	.long	.LFE42-.LFB42
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI7-.LFB42
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE9:
.LSFDE11:
	.long	.LEFDE11-.LASFDE11
.LASFDE11:
	.long	.LASFDE11-.Lframe1
	.long	.LFB37
	.long	.LFE37-.LFB37
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI8-.LFB37
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
	.uleb128 0x2
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
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE13:
.LSFDE15:
	.long	.LEFDE15-.LASFDE15
.LASFDE15:
	.long	.LASFDE15-.Lframe1
	.long	.LFB40
	.long	.LFE40-.LFB40
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI10-.LFB40
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI11-.LCFI10
	.byte	0xe
	.uleb128 0x20
	.align 8
.LEFDE15:
.LSFDE17:
	.long	.LEFDE17-.LASFDE17
.LASFDE17:
	.long	.LASFDE17-.Lframe1
	.long	.LFB55
	.long	.LFE55-.LFB55
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI12-.LFB55
	.byte	0xe
	.uleb128 0x10
	.byte	0x86
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI13-.LCFI12
	.byte	0xe
	.uleb128 0x18
	.byte	0x83
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI14-.LCFI13
	.byte	0xe
	.uleb128 0x20
	.align 8
.LEFDE17:
.LSFDE19:
	.long	.LEFDE19-.LASFDE19
.LASFDE19:
	.long	.LASFDE19-.Lframe1
	.long	.LFB54
	.long	.LFE54-.LFB54
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI16-.LFB54
	.byte	0x8c
	.uleb128 0x4
	.byte	0x86
	.uleb128 0x5
	.byte	0x4
	.long	.LCFI20-.LCFI16
	.byte	0xe
	.uleb128 0x40
	.byte	0x83
	.uleb128 0x6
	.byte	0x8e
	.uleb128 0x2
	.byte	0x8d
	.uleb128 0x3
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
	.long	.LCFI22-.LFB51
	.byte	0x86
	.uleb128 0x3
	.byte	0x83
	.uleb128 0x4
	.byte	0x4
	.long	.LCFI24-.LCFI22
	.byte	0xe
	.uleb128 0x40
	.byte	0x8c
	.uleb128 0x2
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
	.long	.LCFI26-.LFB50
	.byte	0x86
	.uleb128 0x3
	.byte	0x83
	.uleb128 0x4
	.byte	0x4
	.long	.LCFI28-.LCFI26
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
	.long	.LFB49
	.long	.LFE49-.LFB49
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI29-.LFB49
	.byte	0xe
	.uleb128 0x10
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI30-.LCFI29
	.byte	0xe
	.uleb128 0x18
	.byte	0x86
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI31-.LCFI30
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x4
	.byte	0x4
	.long	.LCFI32-.LCFI31
	.byte	0xe
	.uleb128 0x40
	.align 8
.LEFDE25:
.LSFDE27:
	.long	.LEFDE27-.LASFDE27
.LASFDE27:
	.long	.LASFDE27-.Lframe1
	.long	.LFB48
	.long	.LFE48-.LFB48
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI34-.LFB48
	.byte	0x86
	.uleb128 0x3
	.byte	0x83
	.uleb128 0x4
	.byte	0x4
	.long	.LCFI36-.LCFI34
	.byte	0xe
	.uleb128 0x30
	.byte	0x8c
	.uleb128 0x2
	.align 8
.LEFDE27:
.LSFDE29:
	.long	.LEFDE29-.LASFDE29
.LASFDE29:
	.long	.LASFDE29-.Lframe1
	.long	.LFB47
	.long	.LFE47-.LFB47
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI37-.LFB47
	.byte	0xe
	.uleb128 0x10
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI38-.LCFI37
	.byte	0xe
	.uleb128 0x18
	.byte	0x86
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI39-.LCFI38
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x4
	.byte	0x4
	.long	.LCFI40-.LCFI39
	.byte	0xe
	.uleb128 0x40
	.align 8
.LEFDE29:
.LSFDE31:
	.long	.LEFDE31-.LASFDE31
.LASFDE31:
	.long	.LASFDE31-.Lframe1
	.long	.LFB46
	.long	.LFE46-.LFB46
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI42-.LFB46
	.byte	0x86
	.uleb128 0x3
	.byte	0x83
	.uleb128 0x4
	.byte	0x4
	.long	.LCFI44-.LCFI42
	.byte	0xe
	.uleb128 0x30
	.byte	0x8c
	.uleb128 0x2
	.align 8
.LEFDE31:
.LSFDE33:
	.long	.LEFDE33-.LASFDE33
.LASFDE33:
	.long	.LASFDE33-.Lframe1
	.long	.LFB45
	.long	.LFE45-.LFB45
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI45-.LFB45
	.byte	0xe
	.uleb128 0x10
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI46-.LCFI45
	.byte	0xe
	.uleb128 0x18
	.byte	0x86
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI47-.LCFI46
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x4
	.byte	0x4
	.long	.LCFI48-.LCFI47
	.byte	0xe
	.uleb128 0x40
	.align 8
.LEFDE33:
.LSFDE35:
	.long	.LEFDE35-.LASFDE35
.LASFDE35:
	.long	.LASFDE35-.Lframe1
	.long	.LFB44
	.long	.LFE44-.LFB44
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI50-.LFB44
	.byte	0x86
	.uleb128 0x3
	.byte	0x83
	.uleb128 0x4
	.byte	0x4
	.long	.LCFI52-.LCFI50
	.byte	0xe
	.uleb128 0x30
	.byte	0x8c
	.uleb128 0x2
	.align 8
.LEFDE35:
	.ident	"GCC: (SUSE Linux) 4.3.2 [gcc-4_3-branch revision 141291]"
	.section	.comment.SUSE.OPTs,"MS",@progbits,1
	.ascii	"OspWg"
	.section	.note.GNU-stack,"",@progbits
