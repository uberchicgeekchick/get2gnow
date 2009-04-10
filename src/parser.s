	.file	"parser.c"
	.text
	.p2align 4,,15
.globl parser_reset_lastid
	.type	parser_reset_lastid, @function
parser_reset_lastid:
.LFB42:
	movl	$0, last_id(%rip)
	ret
.LFE42:
	.size	parser_reset_lastid, .-parser_reset_lastid
	.p2align 4,,15
	.type	display_notification, @function
display_notification:
.LFB38:
	pushq	%rbx
.LCFI0:
	movq	%rdi, %rbx
	call	app_notify
	testq	%rbx, %rbx
	je	.L4
	movq	%rbx, %rdi
	call	g_free
.L4:
	xorl	%eax, %eax
	popq	%rbx
	.p2align 4,,1
	.p2align 3
	ret
.LFE38:
	.size	display_notification, .-display_notification
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"UTF-8"
.LC1:
	.string	"xml"
.LC2:
	.string	"failed to read xml data"
.LC3:
	.string	"Parser"
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC4:
	.string	"failed getting first element of xml data"
	.text
	.p2align 4,,15
.globl parser_parse
	.type	parser_parse, @function
parser_parse:
.LFB37:
	pushq	%rbp
.LCFI1:
	xorl	%r8d, %r8d
	movq	%rdx, %rbp
	movl	$.LC0, %ecx
	movl	$.LC1, %edx
	pushq	%rbx
.LCFI2:
	subq	$8, %rsp
.LCFI3:
	call	xmlReadMemory
	testq	%rax, %rax
	movq	%rax, %rbx
	je	.L12
	movq	%rax, %rdi
	call	xmlDocGetRootElement
	testq	%rax, %rax
	je	.L13
	movq	%rax, (%rbp)
.L9:
	movq	%rbx, %rax
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	ret
.L12:
	movl	$.LC2, %esi
	movl	$.LC3, %edi
	xorl	%eax, %eax
	call	debug_impl
	jmp	.L9
.L13:
	movl	$.LC4, %esi
	movl	$.LC3, %edi
	call	debug_impl
	movq	%rbx, %rdi
	xorl	%ebx, %ebx
	call	xmlFreeDoc
	jmp	.L9
.LFE37:
	.size	parser_parse, .-parser_parse
	.section	.rodata.str1.1
.LC5:
	.string	"statuses"
.LC6:
	.string	"direct-messages"
.LC7:
	.string	"status"
.LC8:
	.string	"direct_message"
.LC9:
	.string	"created_at"
.LC10:
	.string	"id"
.LC11:
	.string	"text"
.LC12:
	.string	"lt;"
.LC13:
	.string	"gt;"
.LC14:
	.string	"&amp;"
.LC15:
	.string	"sender"
.LC16:
	.string	"user"
.LC17:
	.string	"C"
.LC18:
	.string	"%a %b %d %T +0000 %Y"
.LC19:
	.string	"1 second ago"
.LC20:
	.string	"%i seconds ago"
.LC21:
	.string	"1 minute ago"
.LC22:
	.string	"%i minutes ago"
.LC23:
	.string	"1 hour ago"
.LC24:
	.string	"%i hours ago"
.LC25:
	.string	"1 day ago"
.LC26:
	.string	"%i days ago"
.LC27:
	.string	"1 month ago"
.LC28:
	.string	"%i months ago"
.LC29:
	.string	"</b> - "
.LC30:
	.string	" )"
.LC31:
	.string	" ( @"
.LC32:
	.string	"<b>"
.LC33:
	.string	"</small>"
.LC34:
	.string	"<small>"
.LC35:
	.string	"\n"
	.text
	.p2align 4,,15
.globl parser_timeline
	.type	parser_timeline, @function
parser_timeline:
.LFB39:
	pushq	%r15
.LCFI4:
	xorl	%r8d, %r8d
	movl	$.LC0, %ecx
	movl	$.LC1, %edx
	pushq	%r14
.LCFI5:
	pushq	%r13
.LCFI6:
	pushq	%r12
.LCFI7:
	pushq	%rbp
.LCFI8:
	pushq	%rbx
.LCFI9:
	subq	$248, %rsp
.LCFI10:
	movl	last_id(%rip), %eax
	movl	%eax, 84(%rsp)
	call	xmlReadMemory
	testq	%rax, %rax
	movq	%rax, %r15
	je	.L61
	movq	%rax, %rdi
	call	xmlDocGetRootElement
	testq	%rax, %rax
	movq	%rax, %rbx
	je	.L62
	call	tweet_list_get_store
	movq	%rax, %rdi
	movq	%rax, 96(%rsp)
	movq	%rbx, %rbp
	call	gtk_list_store_clear
	movl	$0, 104(%rsp)
	movl	$0, 108(%rsp)
	movl	$0, 112(%rsp)
	jmp	.L51
	.p2align 4,,10
	.p2align 3
.L19:
	movq	48(%rbp), %rbp
	testq	%rbp, %rbp
	je	.L63
.L51:
	cmpl	$1, 8(%rbp)
	jne	.L19
	movq	16(%rbp), %rdi
	movl	$.LC5, %esi
	call	g_str_equal
	testl	%eax, %eax
	je	.L64
.L20:
	movq	24(%rbp), %rbp
	movq	48(%rbp), %rbp
	testq	%rbp, %rbp
	jne	.L51
	.p2align 4,,10
	.p2align 3
.L63:
	movl	104(%rsp), %eax
	testl	%eax, %eax
	jle	.L52
	movl	104(%rsp), %eax
	movl	%eax, last_id(%rip)
.L52:
	movq	%r15, %rdi
	call	xmlFreeDoc
	call	xmlCleanupParser
	movl	$1, %eax
.L18:
	addq	$248, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	ret
	.p2align 4,,10
	.p2align 3
.L64:
	movq	16(%rbp), %rdi
	movl	$.LC6, %esi
	call	g_str_equal
	testl	%eax, %eax
	jne	.L20
	movq	16(%rbp), %rdi
	movl	$.LC7, %esi
	call	g_str_equal
	testl	%eax, %eax
	jne	.L22
	movq	16(%rbp), %rdi
	movl	$.LC8, %esi
	call	g_str_equal
	testl	%eax, %eax
	je	.L19
.L22:
	movq	24(%rbp), %rbx
	call	xmlBufferCreate
	movl	$32, %edi
	movq	%rax, 120(%rsp)
	call	g_malloc0
	testq	%rbx, %rbx
	movq	%rax, %r14
	jne	.L54
	jmp	.L23
	.p2align 4,,10
	.p2align 3
.L24:
	movq	48(%rbx), %rbx
	testq	%rbx, %rbx
	.p2align 4,,3
	.p2align 3
	je	.L23
.L54:
	cmpl	$1, 8(%rbx)
	.p2align 4,,5
	.p2align 3
	jne	.L24
	movq	120(%rsp), %rdi
	movq	%rbx, %rsi
	call	xmlNodeBufGetContent
	testl	%eax, %eax
	jne	.L24
	movq	16(%rbx), %rdi
	movl	$.LC9, %esi
	call	g_str_equal
	testl	%eax, %eax
	jne	.L65
	movq	16(%rbx), %rdi
	movl	$.LC10, %esi
	call	g_str_equal
	testl	%eax, %eax
	je	.L27
	movq	120(%rsp), %rdi
	call	xmlBufferContent
	movq	%rax, %rdi
	call	g_strdup
	movq	%rax, 24(%r14)
.L26:
	movq	120(%rsp), %rdi
	call	xmlBufferEmpty
	jmp	.L24
	.p2align 4,,10
	.p2align 3
.L23:
	movq	120(%rsp), %rdi
	leaq	128(%rsp), %rbx
	call	xmlBufferFree
	movq	24(%r14), %rdi
	xorl	%esi, %esi
	movl	$10, %edx
	call	strtol
	movl	104(%rsp), %ecx
	movl	104(%rsp), %edx
	movl	%eax, 116(%rsp)
	testl	%ecx, %ecx
	cmove	%eax, %edx
	movq	16(%r14), %rax
	xorl	%edi, %edi
	movl	%edx, 104(%rsp)
	movq	%rax, 88(%rsp)
	call	time
	movq	%rax, 232(%rsp)
	call	tzset
	leaq	232(%rsp), %rdi
	call	gmtime
	movq	%rax, %rdi
	movl	$-1, 32(%rax)
	call	mktime
	movl	$.LC17, %esi
	movl	$2, %edi
	movq	%rax, %r13
	call	setlocale
	movq	88(%rsp), %rdi
	movq	%rbx, %rdx
	movl	$.LC18, %esi
	movq	%rax, %r12
	call	strptime
	movq	%rbx, %rdi
	movl	$-1, 160(%rsp)
	call	mktime
	movq	%r12, %rsi
	movq	%rax, %rbx
	movl	$2, %edi
	call	setlocale
	movslq	%ebx,%rsi
	movslq	%r13d,%rdi
	call	difftime
	cvttsd2si	%xmm0, %ebx
	movl	$5, %edx
	movl	$.LC19, %esi
	cmpl	$1, %ebx
	jle	.L58
	cmpl	$59, %ebx
	movl	$5, %edx
	movl	$.LC20, %esi
	jle	.L59
	cmpl	$119, %ebx
	jle	.L66
	movl	%ebx, %eax
	movl	$-2004318071, %r13d
	movl	$.LC22, %esi
	imull	%r13d
	leal	(%rdx,%rbx), %r12d
	sarl	$31, %ebx
	movl	$5, %edx
	sarl	$5, %r12d
	subl	%ebx, %r12d
	cmpl	$59, %r12d
	jle	.L60
	cmpl	$119, %r12d
	movl	$5, %edx
	movl	$.LC23, %esi
	jle	.L58
	movl	%r12d, %eax
	movl	%r12d, %esi
	imull	%r13d
	sarl	$31, %esi
	leal	(%rdx,%r12), %ebx
	sarl	$5, %ebx
	subl	%esi, %ebx
	cmpl	$23, %ebx
	jle	.L67
	cmpl	$47, %ebx
	movl	$5, %edx
	movl	$.LC25, %esi
	jle	.L58
	movl	$715827883, %eax
	movl	%ebx, %esi
	imull	%ebx
	sarl	$31, %esi
	movl	%edx, %r12d
	sarl	$2, %r12d
	subl	%esi, %r12d
	cmpl	$29, %r12d
	jle	.L68
	cmpl	$59, %r12d
	movl	$5, %edx
	movl	$.LC27, %esi
	jle	.L58
	movl	$5, %edx
	movl	$.LC28, %esi
	xorl	%edi, %edi
	call	dcgettext
	movq	%rax, %rdi
	movl	%r12d, %eax
	imull	%r13d
	xorl	%eax, %eax
	leal	(%rdx,%r12), %esi
	movl	%r12d, %edx
	sarl	$31, %edx
	sarl	$4, %esi
	subl	%edx, %esi
	call	g_strdup_printf
	movq	%rax, %r13
	jmp	.L37
.L66:
	movl	$5, %edx
	movl	$.LC21, %esi
.L58:
	xorl	%edi, %edi
	call	dcgettext
	movq	%rax, %rdi
	call	g_strdup
	movq	%rax, %r13
.L37:
	movq	(%r14), %rax
	movl	$.LC31, %edx
	movl	$.LC29, %r9d
	movl	$.LC30, %r8d
	movl	$.LC32, %edi
	movq	8(%rax), %rcx
	movq	16(%rax), %rsi
	movq	8(%r14), %rax
	movq	$0, 40(%rsp)
	movq	$.LC33, 32(%rsp)
	movq	$.LC34, 16(%rsp)
	movq	$.LC35, 8(%rsp)
	movq	%r13, (%rsp)
	movq	%rax, 24(%rsp)
	xorl	%eax, %eax
	call	g_strconcat
	movl	116(%rsp), %edx
	cmpl	%edx, last_id(%rip)
	movq	%rax, %r12
	jge	.L46
	movl	84(%rsp), %edx
	testl	%edx, %edx
	jle	.L46
	cmpl	$1, 108(%rsp)
	jne	.L69
.L47:
	movq	%r12, %rdi
	call	g_strdup
	movl	112(%rsp), %edi
	movq	%rax, %rdx
	movl	$display_notification, %esi
	call	g_timeout_add_seconds
	addl	$5, 112(%rsp)
	movl	$1, 108(%rsp)
.L46:
	leaq	192(%rsp), %rbx
	movq	96(%rsp), %rdi
	movq	%rbx, %rsi
	call	gtk_list_store_append
	movq	24(%r14), %rdi
	xorl	%edx, %edx
	xorl	%esi, %esi
	call	strtoul
	movq	(%r14), %rdx
	movq	96(%rsp), %rdi
	movl	$2, %r8d
	movq	%r12, %rcx
	movq	%rbx, %rsi
	movq	16(%rdx), %r9
	movl	$-1, 64(%rsp)
	movq	%rax, 56(%rsp)
	movl	$6, 48(%rsp)
	movq	8(%rdx), %rax
	movl	$1, %edx
	movl	$5, 32(%rsp)
	movl	$4, 16(%rsp)
	movq	%r13, 8(%rsp)
	movl	$3, (%rsp)
	movq	%rax, 40(%rsp)
	movq	8(%r14), %rax
	movq	%rax, 24(%rsp)
	xorl	%eax, %eax
	call	gtk_list_store_set
	movq	%r12, %rdi
	call	g_free
	movq	(%r14), %rax
	movq	48(%rax), %rdi
	movq	192(%rsp), %rax
	movq	%rax, (%rsp)
	movq	200(%rsp), %rax
	movq	%rax, 8(%rsp)
	movq	208(%rsp), %rax
	movq	%rax, 16(%rsp)
	movq	216(%rsp), %rax
	movq	%rax, 24(%rsp)
	call	network_get_image
	movq	(%r14), %rdi
	call	user_free
	movq	8(%r14), %rdi
	testq	%rdi, %rdi
	je	.L48
	call	g_free
.L48:
	movq	16(%r14), %rdi
	testq	%rdi, %rdi
	je	.L49
	call	g_free
.L49:
	movq	24(%r14), %rdi
	testq	%rdi, %rdi
	je	.L50
	call	g_free
.L50:
	movq	%r14, %rdi
	call	g_free
	movq	%r13, %rdi
	call	g_free
	jmp	.L19
.L65:
	movq	120(%rsp), %rdi
	call	xmlBufferContent
	movq	%rax, %rdi
	call	g_strdup
	movq	%rax, 16(%r14)
	jmp	.L26
.L69:
	call	app_notify_sound
	.p2align 4,,6
	.p2align 3
	jmp	.L47
.L67:
	movl	$5, %edx
	movl	$.LC24, %esi
.L59:
	xorl	%edi, %edi
	call	dcgettext
	movl	%ebx, %esi
	movq	%rax, %rdi
	xorl	%eax, %eax
	call	g_strdup_printf
	movq	%rax, %r13
	jmp	.L37
.L68:
	movl	$5, %edx
	movl	$.LC26, %esi
.L60:
	xorl	%edi, %edi
	call	dcgettext
	movl	%r12d, %esi
	movq	%rax, %rdi
	xorl	%eax, %eax
	call	g_strdup_printf
	movq	%rax, %r13
	jmp	.L37
.L27:
	movq	16(%rbx), %rdi
	movl	$.LC11, %esi
	call	g_str_equal
	testl	%eax, %eax
	je	.L28
	movq	120(%rsp), %rdi
	call	xmlBufferContent
	movq	$-1, %rsi
	movq	%rax, %rdi
	call	g_markup_escape_text
	movq	%rax, %r12
	movq	%rax, 8(%r14)
.L57:
	movq	%r12, %rdi
	movl	$.LC14, %esi
	call	strstr
	testq	%rax, %rax
	movq	%rax, %r12
	je	.L26
	leaq	5(%r12), %r13
	movl	$.LC12, %edi
	movl	$3, %ecx
	movq	%r13, %rsi
	repz cmpsb
	je	.L30
	movq	%r13, %rsi
	movl	$.LC13, %edi
	movl	$3, %ecx
	repz cmpsb
	je	.L30
	movq	%r13, %r12
	jmp	.L57
.L30:
	movq	%r13, %rdi
	call	strlen
	leaq	1(%r12), %rdi
	leaq	1(%rax), %rdx
	movq	%r13, %rsi
	call	memmove
	jmp	.L57
.L61:
	movl	$.LC2, %esi
	movl	$.LC3, %edi
	xorl	%eax, %eax
	call	debug_impl
.L16:
	call	xmlCleanupParser
	xorl	%eax, %eax
	jmp	.L18
.L28:
	movq	16(%rbx), %rdi
	movl	$.LC15, %esi
	call	g_str_equal
	testl	%eax, %eax
	jne	.L33
	movq	16(%rbx), %rdi
	movl	$.LC16, %esi
	call	g_str_equal
	testl	%eax, %eax
	je	.L26
.L33:
	movq	24(%rbx), %rdi
	call	user_new
	movq	%rax, (%r14)
	.p2align 4,,2
	.p2align 3
	jmp	.L26
.L62:
	movl	$.LC4, %esi
	movl	$.LC3, %edi
	xorl	%eax, %eax
	call	debug_impl
	movq	%r15, %rdi
	call	xmlFreeDoc
	jmp	.L16
.LFE39:
	.size	parser_timeline, .-parser_timeline
	.local	last_id
	.comm	last_id,4,4
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
	.long	.LFB38
	.long	.LFE38-.LFB38
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI0-.LFB38
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
	.uleb128 0x2
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
	.long	.LCFI1-.LFB37
	.byte	0xe
	.uleb128 0x10
	.byte	0x86
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI2-.LCFI1
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI3-.LCFI2
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x3
	.align 8
.LEFDE5:
.LSFDE7:
	.long	.LEFDE7-.LASFDE7
.LASFDE7:
	.long	.LASFDE7-.Lframe1
	.long	.LFB39
	.long	.LFE39-.LFB39
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI4-.LFB39
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI5-.LCFI4
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI6-.LCFI5
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI7-.LCFI6
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.long	.LCFI8-.LCFI7
	.byte	0xe
	.uleb128 0x30
	.byte	0x4
	.long	.LCFI9-.LCFI8
	.byte	0xe
	.uleb128 0x38
	.byte	0x4
	.long	.LCFI10-.LCFI9
	.byte	0xe
	.uleb128 0x130
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
.LEFDE7:
	.ident	"GCC: (SUSE Linux) 4.3.2 [gcc-4_3-branch revision 141291]"
	.section	.comment.SUSE.OPTs,"MS",@progbits,1
	.ascii	"OspWg"
	.section	.note.GNU-stack,"",@progbits
