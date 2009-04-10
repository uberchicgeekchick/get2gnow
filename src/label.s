	.file	"label.c"
	.text
	.p2align 4,,15
	.type	label_finalize, @function
label_finalize:
.LFB48:
	rep
	ret
.LFE48:
	.size	label_finalize, .-label_finalize
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	" \t\n"
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC1:
	.string	"<a href=\"http://twitter.com/%s\">%s</a>%c%s"
	.align 8
.LC2:
	.string	"<a href=\"http://twitter.com/%s\">%s</a>"
	.section	.rodata.str1.1
.LC3:
	.string	"<a href=\"%s\">%s</a>"
.LC4:
	.string	" "
	.text
	.p2align 4,,15
.globl label_set_text
	.type	label_set_text, @function
label_set_text:
.LFB51:
	pushq	%r15
.LCFI0:
	movq	%rsi, %rax
	pushq	%r14
.LCFI1:
	pushq	%r13
.LCFI2:
	pushq	%r12
.LCFI3:
	pushq	%rbp
.LCFI4:
	pushq	%rbx
.LCFI5:
	subq	$40, %rsp
.LCFI6:
	testq	%rsi, %rsi
	movq	%rdi, (%rsp)
	je	.L22
	xorl	%ebx, %ebx
	cmpb	$0, (%rsi)
	jne	.L28
.L6:
	call	sexy_url_label_get_type
	movq	(%rsp), %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	movq	%rbx, %rsi
	movq	%rax, %rdi
	call	sexy_url_label_set_markup
	addq	$40, %rsp
	movq	%rbx, %rdi
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	jmp	g_free
	.p2align 4,,10
	.p2align 3
.L22:
	addq	$40, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	ret
.L28:
	xorl	%edx, %edx
	movl	$.LC0, %esi
	movq	%rax, %rdi
	call	g_strsplit_set
	movq	(%rax), %r15
	movq	%rax, 8(%rsp)
	testq	%r15, %r15
	je	.L7
	addq	$8, %rax
	movq	%rax, 24(%rsp)
	movq	8(%rsp), %rax
	movq	%rax, 32(%rsp)
	.p2align 4,,10
	.p2align 3
.L21:
	movq	%r15, %rdi
	movl	$prefix.43759+8, %r14d
	call	strlen
	movl	%eax, 20(%rsp)
	.p2align 4,,10
	.p2align 3
.L13:
	movl	(%r14), %r12d
	cmpl	%r12d, 20(%rsp)
	jle	.L8
	xorl	%ebp, %ebp
	testl	%r12d, %r12d
	jle	.L10
	movq	-8(%r14), %r13
	xorl	%ebx, %ebx
	jmp	.L11
	.p2align 4,,10
	.p2align 3
.L29:
	leal	1(%rbx), %ebp
	addq	$1, %rbx
	cmpl	%ebx, %r12d
	jle	.L10
.L11:
	movsbl	(%r15,%rbx),%edi
	movl	%ebx, %ebp
	call	g_ascii_tolower
	cmpb	(%r13,%rbx), %al
	je	.L29
.L10:
	cmpl	%ebp, %r12d
	je	.L12
.L8:
	addq	$16, %r14
	cmpq	$prefix.43759+104, %r14
	jne	.L13
.L14:
	movq	24(%rsp), %rcx
	addq	$8, 24(%rsp)
	movq	(%rcx), %r15
	movq	%rcx, 32(%rsp)
	testq	%r15, %r15
	jne	.L21
.L7:
	movq	8(%rsp), %rsi
	movl	$.LC4, %edi
	call	g_strjoinv
	movq	8(%rsp), %rdi
	movq	%rax, %rbx
	call	g_strfreev
	jmp	.L6
.L12:
	cmpb	$64, (%r15)
	jne	.L15
	movzbl	1(%r15), %edx
	testb	%dl, %dl
	.p2align 4,,5
	.p2align 3
	je	.L16
	xorl	%r8d, %r8d
	.p2align 4,,10
	.p2align 3
.L19:
	movq	g_ascii_table(%rip), %rcx
	movzbl	%dl, %eax
	testb	$1, (%rcx,%rax,2)
	jne	.L17
	cmpb	$95, %dl
	jne	.L18
.L17:
	addq	$1, %r8
	movzbl	1(%r15,%r8), %edx
	testb	%dl, %dl
	jne	.L19
.L16:
	leaq	1(%r15), %rsi
	movq	%r15, %rdx
	movl	$.LC2, %edi
	xorl	%eax, %eax
	call	g_strdup_printf
	movq	%rax, %rbx
.L20:
	movq	32(%rsp), %rcx
	movq	(%rcx), %rdi
	call	g_free
	movq	32(%rsp), %rax
	movq	%rbx, (%rax)
	jmp	.L14
.L15:
	movq	%r15, %rdx
	movq	%r15, %rsi
	movl	$.LC3, %edi
	xorl	%eax, %eax
	call	g_strdup_printf
	movq	%rax, %rbx
	jmp	.L20
.L18:
	leaq	1(%r15,%r8), %rax
	movl	$.LC1, %edi
	movsbl	(%rax),%ecx
	movb	$0, (%rax)
	movq	32(%rsp), %rax
	movq	(%rax), %rdx
	xorl	%eax, %eax
	leaq	1(%rdx), %rsi
	leaq	2(%rdx,%r8), %r8
	call	g_strdup_printf
	movq	%rax, %rbx
	jmp	.L20
.LFE51:
	.size	label_set_text, .-label_set_text
	.section	.rodata.str1.1
.LC5:
	.string	"Label"
	.text
	.p2align 4,,15
.globl label_get_type
	.type	label_get_type, @function
label_get_type:
.LFB45:
	pushq	%rbx
.LCFI7:
	subq	$16, %rsp
.LCFI8:
	movq	g_define_type_id__volatile.43704(%rip), %rax
	testq	%rax, %rax
	je	.L36
.L31:
	movq	g_define_type_id__volatile.43704(%rip), %rax
	addq	$16, %rsp
	popq	%rbx
	ret
	.p2align 4,,10
	.p2align 3
.L36:
	movl	$g_define_type_id__volatile.43704, %edi
	call	g_once_init_enter_impl
	testl	%eax, %eax
	je	.L31
	movl	$.LC5, %edi
	call	g_intern_static_string
	movq	%rax, %rbx
	call	sexy_url_label_get_type
	movq	%rbx, %rsi
	movq	%rax, %rdi
	movl	$label_init, %r9d
	movl	$216, %r8d
	movl	$label_class_intern_init, %ecx
	movl	$816, %edx
	movl	$0, (%rsp)
	call	g_type_register_static_simple
	movl	$g_define_type_id__volatile.43704, %edi
	movq	%rax, %rsi
	call	g_once_init_leave
	movq	g_define_type_id__volatile.43704(%rip), %rax
	addq	$16, %rsp
	popq	%rbx
	ret
.LFE45:
	.size	label_get_type, .-label_get_type
	.p2align 4,,15
	.type	label_class_intern_init, @function
label_class_intern_init:
.LFB44:
	pushq	%rbx
.LCFI9:
	movq	%rdi, %rbx
	call	g_type_class_peek_parent
	movq	%rbx, %rdi
	movl	$80, %esi
	movq	%rax, label_parent_class(%rip)
	call	g_type_check_class_cast
	movq	$label_finalize, 48(%rax)
	popq	%rbx
	ret
.LFE44:
	.size	label_class_intern_init, .-label_class_intern_init
	.section	.rodata.str1.1
.LC6:
	.string	"ypad"
.LC7:
	.string	"xpad"
.LC8:
	.string	"yalign"
.LC9:
	.string	"xalign"
.LC11:
	.string	"url-activated"
	.text
	.p2align 4,,15
	.type	label_init, @function
label_init:
.LFB47:
	pushq	%rbx
.LCFI10:
	movq	%rdi, %rbx
	subq	$16, %rsp
.LCFI11:
	call	gtk_label_get_type
	movq	%rbx, %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	movl	$1, %esi
	movq	%rax, %rdi
	call	gtk_label_set_line_wrap
	xorpd	%xmm1, %xmm1
	movq	%rbx, %rdi
	movl	$.LC6, %r9d
	movl	$6, %r8d
	movl	$.LC7, %ecx
	movl	$.LC8, %edx
	movl	$.LC9, %esi
	movl	$2, %eax
	movapd	%xmm1, %xmm0
	movq	$0, 8(%rsp)
	movl	$4, (%rsp)
	call	g_object_set
	movq	%rbx, %rdi
	addq	$16, %rsp
	xorl	%r9d, %r9d
	popq	%rbx
	xorl	%r8d, %r8d
	xorl	%ecx, %ecx
	movl	$label_url_activated_cb, %edx
	movl	$.LC11, %esi
	jmp	g_signal_connect_data
.LFE47:
	.size	label_init, .-label_init
	.section	.rodata.str1.1
.LC12:
	.string	"Couldn't show URL: '%s'"
	.text
	.p2align 4,,15
	.type	label_url_activated_cb, @function
label_url_activated_cb:
.LFB49:
	pushq	%rbx
.LCFI12:
	movq	%rsi, %rbx
	xorl	%edx, %edx
	xorl	%esi, %esi
	movq	%rbx, %rdi
	call	g_app_info_launch_default_for_uri
	testl	%eax, %eax
	je	.L44
	popq	%rbx
	ret
	.p2align 4,,10
	.p2align 3
.L44:
	movq	%rbx, %rcx
	movl	$.LC12, %edx
	movl	$16, %esi
	popq	%rbx
	xorl	%edi, %edi
	jmp	g_log
.LFE49:
	.size	label_url_activated_cb, .-label_url_activated_cb
	.p2align 4,,15
.globl label_new
	.type	label_new, @function
label_new:
.LFB50:
	pushq	%rbx
.LCFI13:
	subq	$16, %rsp
.LCFI14:
	movq	g_define_type_id__volatile.43704(%rip), %rax
	testq	%rax, %rax
	je	.L51
.L46:
	movq	g_define_type_id__volatile.43704(%rip), %rdi
	addq	$16, %rsp
	xorl	%esi, %esi
	popq	%rbx
	xorl	%eax, %eax
	jmp	g_object_new
	.p2align 4,,10
	.p2align 3
.L51:
	movl	$g_define_type_id__volatile.43704, %edi
	call	g_once_init_enter_impl
	testl	%eax, %eax
	je	.L46
	movl	$.LC5, %edi
	call	g_intern_static_string
	movq	%rax, %rbx
	call	sexy_url_label_get_type
	movq	%rbx, %rsi
	movq	%rax, %rdi
	movl	$label_init, %r9d
	movl	$216, %r8d
	movl	$label_class_intern_init, %ecx
	movl	$816, %edx
	movl	$0, (%rsp)
	call	g_type_register_static_simple
	movl	$g_define_type_id__volatile.43704, %edi
	movq	%rax, %rsi
	call	g_once_init_leave
	jmp	.L46
.LFE50:
	.size	label_new, .-label_new
	.section	.rodata.str1.1
.LC13:
	.string	"ftp."
.LC14:
	.string	"www."
.LC15:
	.string	"ftp://"
.LC16:
	.string	"http://"
.LC17:
	.string	"https://"
.LC18:
	.string	"@"
	.section	.rodata
	.align 32
	.type	prefix.43759, @object
	.size	prefix.43759, 96
prefix.43759:
	.quad	.LC13
	.long	4
	.zero	4
	.quad	.LC14
	.long	4
	.zero	4
	.quad	.LC15
	.long	6
	.zero	4
	.quad	.LC16
	.long	7
	.zero	4
	.quad	.LC17
	.long	8
	.zero	4
	.quad	.LC18
	.long	1
	.zero	4
	.local	g_define_type_id__volatile.43704
	.comm	g_define_type_id__volatile.43704,8,8
	.local	label_parent_class
	.comm	label_parent_class,8,8
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
	.long	.LFB48
	.long	.LFE48-.LFB48
	.uleb128 0x0
	.align 8
.LEFDE1:
.LSFDE3:
	.long	.LEFDE3-.LASFDE3
.LASFDE3:
	.long	.LASFDE3-.Lframe1
	.long	.LFB51
	.long	.LFE51-.LFB51
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI0-.LFB51
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI1-.LCFI0
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI2-.LCFI1
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI3-.LCFI2
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.long	.LCFI4-.LCFI3
	.byte	0xe
	.uleb128 0x30
	.byte	0x4
	.long	.LCFI5-.LCFI4
	.byte	0xe
	.uleb128 0x38
	.byte	0x4
	.long	.LCFI6-.LCFI5
	.byte	0xe
	.uleb128 0x60
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
.LEFDE3:
.LSFDE5:
	.long	.LEFDE5-.LASFDE5
.LASFDE5:
	.long	.LASFDE5-.Lframe1
	.long	.LFB45
	.long	.LFE45-.LFB45
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI7-.LFB45
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI8-.LCFI7
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE5:
.LSFDE7:
	.long	.LEFDE7-.LASFDE7
.LASFDE7:
	.long	.LASFDE7-.Lframe1
	.long	.LFB44
	.long	.LFE44-.LFB44
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI9-.LFB44
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
	.long	.LFB47
	.long	.LFE47-.LFB47
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI10-.LFB47
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI11-.LCFI10
	.byte	0xe
	.uleb128 0x20
	.align 8
.LEFDE9:
.LSFDE11:
	.long	.LEFDE11-.LASFDE11
.LASFDE11:
	.long	.LASFDE11-.Lframe1
	.long	.LFB49
	.long	.LFE49-.LFB49
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI12-.LFB49
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
	.long	.LFB50
	.long	.LFE50-.LFB50
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI13-.LFB50
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI14-.LCFI13
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE13:
	.ident	"GCC: (SUSE Linux) 4.3.2 [gcc-4_3-branch revision 141291]"
	.section	.comment.SUSE.OPTs,"MS",@progbits,1
	.ascii	"OspWg"
	.section	.note.GNU-stack,"",@progbits
