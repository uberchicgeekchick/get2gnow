	.file	"ui-utils.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"ghelp:greet-tweet-know"
.LC1:
	.string	"Unable to open help file"
.LC2:
	.string	"%s"
.LC3:
	.string	"response"
	.text
	.p2align 4,,15
.globl help_show
	.type	help_show, @function
help_show:
.LFB40:
	pushq	%r13
.LCFI0:
	pushq	%r12
.LCFI1:
	movq	%rdi, %r12
	pushq	%rbx
.LCFI2:
	subq	$16, %rsp
.LCFI3:
	movq	$0, 8(%rsp)
	call	gtk_widget_get_type
	movq	%r12, %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_widget_get_screen
	movq	%rax, %rbx
	call	gtk_get_current_event_time
	leaq	8(%rsp), %rcx
	movl	%eax, %edx
	movl	$.LC0, %esi
	movq	%rbx, %rdi
	call	gtk_show_uri
	cmpq	$0, 8(%rsp)
	je	.L3
	movl	$5, %edx
	xorl	%edi, %edi
	movl	$.LC1, %esi
	call	dcgettext
	movq	%rax, %rbx
	call	gtk_window_get_type
	movq	%r12, %rdi
	movq	%rax, %rsi
	movq	%rax, %r13
	call	g_type_check_instance_cast
	movq	%rbx, %r8
	movl	$2, %ecx
	movq	%rax, %rdi
	movl	$3, %edx
	movl	$3, %esi
	xorl	%eax, %eax
	call	gtk_message_dialog_new
	movq	%rax, %rbx
	movq	8(%rsp), %rax
	movq	8(%rax), %r12
	call	gtk_message_dialog_get_type
	movq	%rbx, %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	movq	%r12, %rdx
	movq	%rax, %rdi
	movl	$.LC2, %esi
	xorl	%eax, %eax
	call	gtk_message_dialog_format_secondary_text
	xorl	%r9d, %r9d
	xorl	%r8d, %r8d
	xorl	%ecx, %ecx
	movl	$gtk_widget_destroy, %edx
	movl	$.LC3, %esi
	movq	%rbx, %rdi
	call	g_signal_connect_data
	movq	%r13, %rsi
	movq	%rbx, %rdi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_window_present
	movq	8(%rsp), %rdi
	testq	%rdi, %rdi
	je	.L3
	call	g_error_free
.L3:
	addq	$16, %rsp
	popq	%rbx
	popq	%r12
	popq	%r13
	ret
.LFE40:
	.size	help_show, .-help_show
	.section	.rodata.str1.1
.LC4:
	.string	"visible"
.LC5:
	.string	"window != NULL"
	.text
	.p2align 4,,15
.globl window_get_is_visible
	.type	window_get_is_visible, @function
window_get_is_visible:
.LFB38:
	pushq	%rbx
.LCFI4:
	movq	%rdi, %rbx
	subq	$16, %rsp
.LCFI5:
	testq	%rdi, %rdi
	je	.L6
	leaq	12(%rsp), %rdx
	xorl	%eax, %eax
	xorl	%ecx, %ecx
	movl	$.LC4, %esi
	call	g_object_get
	movl	12(%rsp), %eax
	testl	%eax, %eax
	jne	.L10
.L7:
	xorl	%eax, %eax
	addq	$16, %rsp
	popq	%rbx
	ret
	.p2align 4,,10
	.p2align 3
.L10:
	call	gtk_widget_get_type
	movq	%rbx, %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	movq	80(%rax), %rdi
	testq	%rdi, %rdi
	je	.L7
	call	gdk_window_get_state
	shrl	%eax
	addq	$16, %rsp
	notl	%eax
	andl	$1, %eax
	popq	%rbx
	ret
	.p2align 4,,10
	.p2align 3
.L6:
	movl	$.LC5, %edx
	movl	$__PRETTY_FUNCTION__.39069, %esi
	xorl	%edi, %edi
	call	g_return_if_fail_warning
	xorl	%eax, %eax
	addq	$16, %rsp
	popq	%rbx
	ret
.LFE38:
	.size	window_get_is_visible, .-window_get_is_visible
	.p2align 4,,15
.globl window_present
	.type	window_present, @function
window_present:
.LFB39:
	movq	%rbx, -24(%rsp)
.LCFI6:
	movq	%r12, -8(%rsp)
.LCFI7:
	movq	%rdi, %rbx
	movq	%rbp, -16(%rsp)
.LCFI8:
	subq	$40, %rsp
.LCFI9:
	testq	%rdi, %rdi
	movl	%esi, %r12d
	je	.L12
	leaq	12(%rsp), %rdx
	xorl	%ecx, %ecx
	movl	$.LC4, %esi
	xorl	%eax, %eax
	call	g_object_get
	call	gtk_widget_get_type
	movq	%rbx, %rdi
	movq	%rax, %rsi
	movq	%rax, %rbp
	call	g_type_check_instance_cast
	movq	80(%rax), %rdi
	movl	$1, %eax
	testq	%rdi, %rdi
	je	.L14
	call	gdk_window_get_state
	shrl	%eax
	andl	$1, %eax
.L14:
	movl	12(%rsp), %edx
	testl	%edx, %edx
	je	.L15
	testb	%al, %al
	je	.L15
	movq	%rbx, %rdi
	movq	%rbp, %rsi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_widget_hide
.L15:
	call	gtk_get_current_event_time
	testl	%r12d, %r12d
	jne	.L19
.L16:
	movq	%rbx, %rdi
	call	gtk_window_present
.L18:
	movq	16(%rsp), %rbx
	movq	24(%rsp), %rbp
	movq	32(%rsp), %r12
	addq	$40, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L19:
	testl	%eax, %eax
	je	.L16
	movl	%eax, %esi
	movq	%rbx, %rdi
	call	gtk_window_present_with_time
	.p2align 4,,2
	.p2align 3
	jmp	.L18
	.p2align 4,,10
	.p2align 3
.L12:
	movl	$.LC5, %edx
	movl	$__PRETTY_FUNCTION__.39087, %esi
	xorl	%edi, %edi
	call	g_return_if_fail_warning
	jmp	.L18
.LFE39:
	.size	window_present, .-window_present
	.section	.rodata
	.type	__PRETTY_FUNCTION__.39087, @object
	.size	__PRETTY_FUNCTION__.39087, 15
__PRETTY_FUNCTION__.39087:
	.string	"window_present"
	.align 16
	.type	__PRETTY_FUNCTION__.39069, @object
	.size	__PRETTY_FUNCTION__.39069, 22
__PRETTY_FUNCTION__.39069:
	.string	"window_get_is_visible"
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
	.long	.LFB40
	.long	.LFE40-.LFB40
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI0-.LFB40
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI1-.LCFI0
	.byte	0xe
	.uleb128 0x18
	.byte	0x8c
	.uleb128 0x3
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI2-.LCFI1
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI3-.LCFI2
	.byte	0xe
	.uleb128 0x30
	.byte	0x83
	.uleb128 0x4
	.align 8
.LEFDE1:
.LSFDE3:
	.long	.LEFDE3-.LASFDE3
.LASFDE3:
	.long	.LASFDE3-.Lframe1
	.long	.LFB38
	.long	.LFE38-.LFB38
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI4-.LFB38
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
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
	.long	.LFB39
	.long	.LFE39-.LFB39
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI7-.LFB39
	.byte	0x8c
	.uleb128 0x2
	.byte	0x83
	.uleb128 0x4
	.byte	0x4
	.long	.LCFI9-.LCFI7
	.byte	0xe
	.uleb128 0x30
	.byte	0x86
	.uleb128 0x3
	.align 8
.LEFDE5:
	.ident	"GCC: (SUSE Linux) 4.3.2 [gcc-4_3-branch revision 141291]"
	.section	.comment.SUSE.OPTs,"MS",@progbits,1
	.ascii	"OspWg"
	.section	.note.GNU-stack,"",@progbits
