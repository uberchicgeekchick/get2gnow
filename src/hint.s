	.file	"hint.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"<b>%s</b>"
.LC1:
	.string	"%s"
.LC2:
	.string	"Do not show this again"
.LC3:
	.string	"conf_path"
.LC4:
	.string	"user_data"
.LC5:
	.string	"func"
.LC6:
	.string	"response"
.LC7:
	.string	"conf_path != NULL"
.LC8:
	.string	"message1 != NULL"
	.text
	.p2align 4,,15
.globl hint_dialog_show
	.type	hint_dialog_show, @function
hint_dialog_show:
.LFB38:
	pushq	%r15
.LCFI0:
	movq	%r9, %r15
	pushq	%r14
.LCFI1:
	pushq	%r13
.LCFI2:
	movq	%rdx, %r13
	pushq	%r12
.LCFI3:
	movq	%rcx, %r12
	pushq	%rbp
.LCFI4:
	movq	%rdi, %rbp
	pushq	%rbx
.LCFI5:
	movq	%rsi, %rbx
	subq	$24, %rsp
.LCFI6:
	testq	%rdi, %rdi
	movq	%r8, (%rsp)
	movl	$1, 20(%rsp)
	je	.L2
	testq	%rsi, %rsi
	je	.L3
	call	conf_get
	leaq	20(%rsp), %rdx
	movq	%rax, %rdi
	movq	%rbp, %rsi
	call	conf_get_bool
	testl	%eax, %eax
	je	.L4
	movl	20(%rsp), %edx
	xorl	%eax, %eax
	testl	%edx, %edx
	je	.L5
.L4:
	movq	%rbx, %r9
	movl	$.LC0, %r8d
	movl	$2, %ecx
	xorl	%edx, %edx
	movq	%r12, %rdi
	movl	$2, %esi
	xorl	%eax, %eax
	call	gtk_message_dialog_new_with_markup
	movq	%rax, %r12
	call	gtk_message_dialog_get_type
	movq	%r12, %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	movq	%r13, %rdx
	movq	%rax, %rdi
	movl	$.LC1, %esi
	xorl	%eax, %eax
	call	gtk_message_dialog_format_secondary_text
	movl	$5, %edx
	movl	$.LC2, %esi
	xorl	%edi, %edi
	call	dcgettext
	movq	%rax, %rdi
	call	gtk_check_button_new_with_label
	movq	%rax, %r14
	call	gtk_toggle_button_get_type
	movq	%r14, %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	movl	$1, %esi
	movq	%rax, %rdi
	call	gtk_toggle_button_set_active
	xorl	%edi, %edi
	movl	$6, %esi
	call	gtk_vbox_new
	movq	%rax, %rbx
	call	gtk_container_get_type
	movq	%rbx, %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	movl	$6, %esi
	movq	%rax, %rdi
	call	gtk_container_set_border_width
	call	gtk_box_get_type
	movq	%rbx, %rdi
	movq	%rax, %rsi
	movq	%rax, %r13
	call	g_type_check_instance_cast
	xorl	%r8d, %r8d
	xorl	%ecx, %ecx
	xorl	%edx, %edx
	movq	%rax, %rdi
	movq	%r14, %rsi
	call	gtk_box_pack_start
	call	gtk_dialog_get_type
	movq	%r12, %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	movq	240(%rax), %rdi
	movq	%r13, %rsi
	call	g_type_check_instance_cast
	xorl	%r8d, %r8d
	xorl	%ecx, %ecx
	xorl	%edx, %edx
	movq	%rbx, %rsi
	movq	%rax, %rdi
	call	gtk_box_pack_start
	movq	%rbp, %rdi
	call	g_strdup
	movl	$80, %esi
	movq	%rax, %rbx
	movq	%r12, %rdi
	call	g_type_check_instance_cast
	movl	$g_free, %ecx
	movq	%rax, %rdi
	movq	%rbx, %rdx
	movl	$.LC3, %esi
	call	g_object_set_data_full
	movl	$80, %esi
	movq	%r12, %rdi
	call	g_type_check_instance_cast
	movq	%r15, %rdx
	movq	%rax, %rdi
	movl	$.LC4, %esi
	call	g_object_set_data
	movl	$80, %esi
	movq	%r12, %rdi
	call	g_type_check_instance_cast
	movq	(%rsp), %rdx
	movq	%rax, %rdi
	movl	$.LC5, %esi
	call	g_object_set_data
	xorl	%r9d, %r9d
	xorl	%r8d, %r8d
	movq	%r14, %rcx
	movl	$hint_dialog_response_cb, %edx
	movl	$.LC6, %esi
	movq	%r12, %rdi
	call	g_signal_connect_data
	movq	%r12, %rdi
	call	gtk_widget_show_all
	movl	$1, %eax
.L5:
	addq	$24, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	ret
	.p2align 4,,10
	.p2align 3
.L2:
	movl	$.LC7, %edx
	movl	$__PRETTY_FUNCTION__.39154, %esi
	xorl	%edi, %edi
	call	g_return_if_fail_warning
	addq	$24, %rsp
	xorl	%eax, %eax
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	ret
	.p2align 4,,10
	.p2align 3
.L3:
	movl	$.LC8, %edx
	movl	$__PRETTY_FUNCTION__.39154, %esi
	xorl	%edi, %edi
	call	g_return_if_fail_warning
	addq	$24, %rsp
	xorl	%eax, %eax
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	ret
.LFE38:
	.size	hint_dialog_show, .-hint_dialog_show
	.p2align 4,,15
.globl hint_show
	.type	hint_show, @function
hint_show:
.LFB39:
	jmp	hint_dialog_show
.LFE39:
	.size	hint_show, .-hint_show
	.p2align 4,,15
	.type	hint_dialog_response_cb, @function
hint_dialog_response_cb:
.LFB37:
	movq	%rbx, -40(%rsp)
.LCFI7:
	movq	%rbp, -32(%rsp)
.LCFI8:
	movl	$80, %esi
	movq	%r12, -24(%rsp)
.LCFI9:
	movq	%r13, -16(%rsp)
.LCFI10:
	movq	%rdx, %rbx
	movq	%r14, -8(%rsp)
.LCFI11:
	subq	$40, %rsp
.LCFI12:
	movq	%rdi, %r12
	call	g_type_check_instance_cast
	movl	$.LC3, %esi
	movq	%rax, %rdi
	call	g_object_get_data
	movl	$80, %esi
	movq	%r12, %rdi
	movq	%rax, %r14
	call	g_type_check_instance_cast
	movl	$.LC5, %esi
	movq	%rax, %rdi
	call	g_object_get_data
	movl	$80, %esi
	movq	%r12, %rdi
	movq	%rax, %rbp
	call	g_type_check_instance_cast
	movl	$.LC4, %esi
	movq	%rax, %rdi
	call	g_object_get_data
	movq	%rax, %r13
	call	gtk_toggle_button_get_type
	movq	%rbx, %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_toggle_button_get_active
	movl	%eax, %ebx
	call	conf_get
	xorl	%edx, %edx
	testl	%ebx, %ebx
	movq	%rax, %rdi
	sete	%dl
	movq	%r14, %rsi
	call	conf_set_bool
	movq	%r12, %rdi
	call	gtk_widget_destroy
	testq	%rbp, %rbp
	je	.L12
	movq	%r13, %rsi
	movq	%r14, %rdi
	movq	%rbp, %r11
	movq	(%rsp), %rbx
	movq	8(%rsp), %rbp
	movq	16(%rsp), %r12
	movq	24(%rsp), %r13
	movq	32(%rsp), %r14
	addq	$40, %rsp
	jmp	*%r11
	.p2align 4,,10
	.p2align 3
.L12:
	movq	(%rsp), %rbx
	movq	8(%rsp), %rbp
	movq	16(%rsp), %r12
	movq	24(%rsp), %r13
	movq	32(%rsp), %r14
	addq	$40, %rsp
	ret
.LFE37:
	.size	hint_dialog_response_cb, .-hint_dialog_response_cb
	.section	.rodata
	.align 16
	.type	__PRETTY_FUNCTION__.39154, @object
	.size	__PRETTY_FUNCTION__.39154, 17
__PRETTY_FUNCTION__.39154:
	.string	"hint_dialog_show"
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
	.long	.LFB38
	.long	.LFE38-.LFB38
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI0-.LFB38
	.byte	0xe
	.uleb128 0x10
	.byte	0x8f
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI1-.LCFI0
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI2-.LCFI1
	.byte	0xe
	.uleb128 0x20
	.byte	0x8d
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI3-.LCFI2
	.byte	0xe
	.uleb128 0x28
	.byte	0x8c
	.uleb128 0x5
	.byte	0x4
	.long	.LCFI4-.LCFI3
	.byte	0xe
	.uleb128 0x30
	.byte	0x86
	.uleb128 0x6
	.byte	0x4
	.long	.LCFI5-.LCFI4
	.byte	0xe
	.uleb128 0x38
	.byte	0x83
	.uleb128 0x7
	.byte	0x4
	.long	.LCFI6-.LCFI5
	.byte	0xe
	.uleb128 0x50
	.align 8
.LEFDE1:
.LSFDE3:
	.long	.LEFDE3-.LASFDE3
.LASFDE3:
	.long	.LASFDE3-.Lframe1
	.long	.LFB39
	.long	.LFE39-.LFB39
	.uleb128 0x0
	.align 8
.LEFDE3:
.LSFDE5:
	.long	.LEFDE5-.LASFDE5
.LASFDE5:
	.long	.LASFDE5-.Lframe1
	.long	.LFB37
	.long	.LFE37-.LFB37
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI10-.LFB37
	.byte	0x8d
	.uleb128 0x3
	.byte	0x8c
	.uleb128 0x4
	.byte	0x86
	.uleb128 0x5
	.byte	0x83
	.uleb128 0x6
	.byte	0x4
	.long	.LCFI12-.LCFI10
	.byte	0xe
	.uleb128 0x30
	.byte	0x8e
	.uleb128 0x2
	.align 8
.LEFDE5:
	.ident	"GCC: (SUSE Linux) 4.3.2 [gcc-4_3-branch revision 141291]"
	.section	.comment.SUSE.OPTs,"MS",@progbits,1
	.ascii	"OspWg"
	.section	.note.GNU-stack,"",@progbits
