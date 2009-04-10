	.file	"ipc.c"
	.text
	.p2align 4,,15
.globl ipc_deinit
	.type	ipc_deinit, @function
ipc_deinit:
.LFB53:
	subq	$8, %rsp
.LCFI0:
	movq	input(%rip), %rax
	testq	%rax, %rax
	je	.L6
	movl	24(%rax), %edi
	testl	%edi, %edi
	jne	.L8
.L3:
	movq	16(%rax), %rdi
	testq	%rdi, %rdi
	je	.L4
	xorl	%edx, %edx
	movl	$1, %esi
	call	g_io_channel_shutdown
	movq	input(%rip), %rax
	movq	16(%rax), %rdi
	call	g_io_channel_unref
	movq	input(%rip), %rax
	movq	$0, 16(%rax)
.L4:
	cmpq	$0, 8(%rax)
	je	.L5
	movq	8(%rax), %rdi
	movl	$-1, (%rax)
	call	unlink
	movq	input(%rip), %rax
	movq	8(%rax), %rdi
	call	g_free
	movq	input(%rip), %rax
	movq	$0, 8(%rax)
.L5:
	movq	32(%rax), %rdi
	movl	$1, %esi
	call	g_byte_array_free
	movq	input(%rip), %rdi
	call	g_free
	movq	$0, input(%rip)
.L6:
	addq	$8, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L8:
	call	g_source_remove
	movq	input(%rip), %rax
	movl	$0, 24(%rax)
	jmp	.L3
.LFE53:
	.size	ipc_deinit, .-ipc_deinit
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"greet-tweet-know"
.LC1:
	.string	"%s-%s-"
.LC2:
	.string	"%s/"
.LC3:
	.string	"open"
.LC4:
	.string	""
.LC5:
	.string	"%s/%s-%s-%d"
.LC6:
	.string	"mkfifo"
.LC7:
	.string	"input == NULL"
.LC8:
	.string	"dir != NULL"
	.text
	.p2align 4,,15
.globl ipc_init_check
	.type	ipc_init_check, @function
ipc_init_check:
.LFB49:
	pushq	%r15
.LCFI1:
	pushq	%r14
.LCFI2:
	pushq	%r13
.LCFI3:
	pushq	%r12
.LCFI4:
	pushq	%rbp
.LCFI5:
	pushq	%rbx
.LCFI6:
	subq	$40, %rsp
.LCFI7:
	cmpq	$0, input(%rip)
	movl	%edi, 12(%rsp)
	movq	%rsi, (%rsp)
	je	.L32
	movl	$.LC7, %edx
	movl	$__PRETTY_FUNCTION__.46744, %esi
	xorl	%edi, %edi
	call	g_return_if_fail_warning
	xorl	%eax, %eax
.L21:
	addq	$40, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	ret
	.p2align 4,,10
	.p2align 3
.L32:
	call	g_get_tmp_dir
	xorl	%edx, %edx
	xorl	%esi, %esi
	movq	%rax, %rdi
	movq	%rax, 16(%rsp)
	call	g_dir_open
	testq	%rax, %rax
	movq	%rax, %r15
	je	.L11
	call	g_get_user_name
	movl	$.LC0, %edx
	movq	%rax, %rsi
	movl	$.LC1, %edi
	xorl	%eax, %eax
	call	g_strdup_printf
	movq	%rax, %rdi
	movq	%rax, 24(%rsp)
	call	strlen
	movl	%eax, %ebp
	call	g_get_current_dir
	movl	$.LC2, %edi
	movq	%rax, %rsi
	movq	%rax, %rbx
	xorl	%eax, %eax
	mov	%ebp, %ebp
	call	g_strdup_printf
	movq	%rbx, %rdi
	movq	%rax, 32(%rsp)
	call	g_free
	.p2align 4,,10
	.p2align 3
.L31:
	movq	%r15, %rdi
	call	g_dir_read_name
	testq	%rax, %rax
	movq	%rax, %r12
	je	.L33
	movq	24(%rsp), %rsi
	movq	%rbp, %rdx
	movq	%r12, %rdi
	call	strncmp
	testl	%eax, %eax
	jne	.L31
	call	__errno_location
	leaq	(%r12,%rbp), %rdi
	xorl	%esi, %esi
	movl	$10, %edx
	movl	$0, (%rax)
	movq	%rax, %rbx
	call	strtol
	movq	16(%rsp), %rdi
	xorl	%edx, %edx
	movq	%r12, %rsi
	movq	%rax, %r13
	xorl	%eax, %eax
	call	g_build_filename
	movl	(%rbx), %edx
	movq	%rax, %r12
	testl	%edx, %edx
	jne	.L13
	testl	%r13d, %r13d
	jle	.L13
	xorl	%esi, %esi
	movl	%r13d, %edi
	call	kill
	testl	%eax, %eax
	je	.L34
.L13:
	movq	%r12, %rdi
	call	unlink
.L15:
	movq	%r12, %rdi
	call	g_free
	jmp	.L31
	.p2align 4,,10
	.p2align 3
.L33:
	movq	%r15, %rdi
	call	g_dir_close
	cmpq	$0, input(%rip)
	je	.L35
	movl	$.LC7, %edx
	movl	$__PRETTY_FUNCTION__.46892, %esi
	xorl	%edi, %edi
	call	g_return_if_fail_warning
.L25:
	movq	24(%rsp), %rdi
	call	g_free
	addq	$40, %rsp
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
.L34:
	movl	$2049, %esi
	movq	%r12, %rdi
	call	open
	cmpl	$-1, %eax
	movl	%eax, %r13d
	jne	.L14
	movl	$.LC3, %edi
	call	perror
	movq	%r12, %rdi
	call	unlink
	jmp	.L15
.L11:
	movl	$.LC8, %edx
	movl	$__PRETTY_FUNCTION__.46744, %esi
	xorl	%edi, %edi
	call	g_return_if_fail_warning
	addq	$40, %rsp
	xorl	%eax, %eax
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	ret
.L35:
	movl	$48, %edi
	call	g_malloc0
	movq	%rax, %rbx
	movl	$-1, (%rax)
	movq	$0, 8(%rax)
	movq	$0, 16(%rax)
	movl	$0, 24(%rax)
	movl	$0, 40(%rax)
	movq	%rax, input(%rip)
	call	g_byte_array_new
	movq	input(%rip), %r13
	movq	%rax, 32(%rbx)
	call	getpid
	movl	%eax, %ebx
	call	g_get_user_name
	movq	%rax, %r12
	call	g_get_tmp_dir
	movl	%ebx, %r8d
	movq	%rax, %rsi
	movl	$.LC0, %ecx
	movq	%r12, %rdx
	movl	$.LC5, %edi
	xorl	%eax, %eax
	call	g_strdup_printf
	movq	%rax, 8(%r13)
	movq	input(%rip), %rax
	movq	8(%rax), %rdi
	call	unlink
	movq	input(%rip), %rax
	movl	$384, %esi
	movq	8(%rax), %rdi
	call	mkfifo
	testl	%eax, %eax
	jne	.L36
	movq	input(%rip), %rbx
	xorl	%eax, %eax
	movl	$2050, %esi
	movq	8(%rbx), %rdi
	call	open
	movl	%eax, (%rbx)
	addl	$1, %eax
	je	.L37
	movq	input(%rip), %rbx
	movl	(%rbx), %edi
	call	g_io_channel_unix_new
	movq	%rax, 16(%rbx)
	movq	input(%rip), %rax
	xorl	%edx, %edx
	xorl	%esi, %esi
	movq	16(%rax), %rdi
	call	g_io_channel_set_encoding
	movq	input(%rip), %rbx
	movl	$ipc_read, %edx
	movl	$27, %esi
	movq	16(%rbx), %rdi
	movq	%rbx, %rcx
	call	g_io_add_watch
	movl	%eax, 24(%rbx)
	movq	input(%rip), %rax
	movl	$1, 40(%rax)
	jmp	.L25
.L36:
	movl	$.LC6, %edi
	call	perror
	call	ipc_deinit
	jmp	.L25
.L37:
	movl	$.LC3, %edi
	call	perror
	call	ipc_deinit
	jmp	.L25
.L14:
	movl	$1, %edx
	movl	$.LC4, %esi
	movl	%eax, %edi
	call	write
	movl	12(%rsp), %eax
	testl	%eax, %eax
	jle	.L16
	movq	(%rsp), %rbp
	xorl	%r14d, %r14d
	jmp	.L20
.L38:
	movq	(%rbp), %rsi
	xorl	%edi, %edi
	call	gnome_vfs_uri_make_full_from_relative
	movq	%rax, %rbx
.L18:
	testq	%rbx, %rbx
	je	.L19
	movq	%rbx, %rdi
	call	strlen
	leaq	1(%rax), %rdx
	movl	%r13d, %edi
	movq	%rbx, %rsi
	call	write
	movq	%rbx, %rdi
	call	g_free
.L19:
	addl	$1, %r14d
	addq	$8, %rbp
	cmpl	%r14d, 12(%rsp)
	jle	.L16
.L20:
	movq	(%rbp), %rdi
	call	g_path_is_absolute
	testl	%eax, %eax
	jne	.L38
	movq	(%rbp), %rsi
	movq	32(%rsp), %rdi
	call	gnome_vfs_uri_make_full_from_relative
	movq	%rax, %rbx
	jmp	.L18
.L16:
	movl	%r13d, %edi
	call	close
	movq	%r12, %rdi
	call	g_free
	movq	%r15, %rdi
	call	g_dir_close
	movq	24(%rsp), %rdi
	call	g_free
	movl	$1, %eax
	jmp	.L21
.LFE49:
	.size	ipc_init_check, .-ipc_init_check
	.section	.rodata.str1.1
.LC9:
	.string	"poll"
.LC10:
	.string	"read"
.LC11:
	.string	"ipc.c"
.LC12:
	.string	"ipc.c:254"
.LC13:
	.string	"%s: error"
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC14:
	.string	"input->buffer->len > 0 && input->buffer->data[input->buffer->len-1] == 0"
	.text
	.p2align 4,,15
	.type	ipc_read, @function
ipc_read:
.LFB51:
	pushq	%r15
.LCFI8:
	pushq	%r14
.LCFI9:
	pushq	%r13
.LCFI10:
	pushq	%r12
.LCFI11:
	movq	%rdx, %r12
	pushq	%rbp
.LCFI12:
	xorl	%ebp, %ebp
	pushq	%rbx
.LCFI13:
	subq	$24, %rsp
.LCFI14:
	andl	$24, %esi
	jne	.L64
.L41:
	leaq	15(%rsp), %r15
	movl	$1, %ebx
	xorl	%r14d, %r14d
.L61:
	testl	%ebp, %ebp
	sete	%al
	testl	%ebx, %ebx
	jne	.L65
.L59:
	testl	%ebp, %ebp
	jne	.L66
	testl	%r14d, %r14d
	movl	$1, %eax
	jne	.L67
.L54:
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
.L65:
	testb	%al, %al
	je	.L59
	movl	(%r12), %eax
	xorl	%edx, %edx
	movl	$1, %esi
	movq	%rsp, %rdi
	movw	$3, 4(%rsp)
	movw	$0, 6(%rsp)
	xorl	%ebx, %ebx
	movl	%eax, (%rsp)
	call	poll
	testl	%eax, %eax
	je	.L59
	cmpl	$1, %eax
	je	.L46
	addl	$1, %eax
	.p2align 4,,5
	.p2align 3
	je	.L63
	xorl	%r8d, %r8d
	movl	$__PRETTY_FUNCTION__.46847, %ecx
	movl	$248, %edx
	movl	$.LC11, %esi
	xorl	%edi, %edi
	call	g_assertion_message
	.p2align 4,,10
	.p2align 3
.L46:
	testb	$8, 6(%rsp)
	je	.L47
.L63:
	call	__errno_location
	movl	(%rax), %edx
	movl	$.LC9, %edi
	movl	$1, %ebx
	cmpl	$4, %edx
	setne	%al
	cmpl	$11, %edx
	setne	%dl
	andl	%edx, %eax
	movzbl	%al, %ebp
	call	perror
	jmp	.L61
	.p2align 4,,10
	.p2align 3
.L47:
	movl	(%r12), %edi
	movl	$1, %edx
	movq	%r15, %rsi
	call	read
	cmpl	$1, %eax
	je	.L68
	addl	$1, %eax
	movl	%ebx, %ebp
	jne	.L61
	movl	$.LC10, %edi
	call	perror
	call	__errno_location
	movl	(%rax), %eax
	cmpl	$4, %eax
	je	.L49
	cmpl	$11, %eax
	movl	$1, %ebp
	movl	$1, %ebx
	jne	.L61
	.p2align 4,,10
	.p2align 3
.L49:
	xorl	%ebp, %ebp
	movl	$1, %ebx
	jmp	.L61
	.p2align 4,,10
	.p2align 3
.L64:
	call	__errno_location
	movl	(%rax), %edx
	cmpl	$4, %edx
	setne	%al
	cmpl	$11, %edx
	setne	%dl
	andl	%edx, %eax
	movzbl	%al, %ebp
	jmp	.L41
	.p2align 4,,10
	.p2align 3
.L68:
	movq	32(%r12), %rdi
	movl	$1, %edx
	movq	%r15, %rsi
	call	g_byte_array_append
	cmpb	$0, 15(%rsp)
	jne	.L49
	movl	%ebx, %ebp
	movl	$1, %r14d
	jmp	.L61
.L66:
	xorl	%eax, %eax
	movl	$.LC12, %ecx
	movl	$.LC13, %edx
	movl	$8, %esi
	xorl	%edi, %edi
	call	g_log
	call	ipc_deinit
	xorl	%eax, %eax
	jmp	.L54
.L67:
	movq	32(%r12), %rax
	movl	8(%rax), %edx
	testl	%edx, %edx
	je	.L56
	movq	(%rax), %rax
	subl	$1, %edx
	cmpb	$0, (%rax,%rdx)
	jne	.L56
	call	gtk_window_get_type
	movq	%rax, %rsi
	movq	app_priv(%rip), %rax
	movq	(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_window_present
	movq	32(%r12), %rdi
	cmpl	$4096, 8(%rdi)
	ja	.L69
	xorl	%esi, %esi
	call	g_byte_array_set_size
	movl	$1, %eax
	jmp	.L54
.L56:
	movl	$.LC14, %r8d
	movl	$__PRETTY_FUNCTION__.46810, %ecx
	movl	$177, %edx
	movl	$.LC11, %esi
	xorl	%edi, %edi
	call	g_assertion_message_expr
.L69:
	movl	$1, %esi
	call	g_byte_array_free
	call	g_byte_array_new
	movq	%rax, 32(%r12)
	movl	$1, %eax
	jmp	.L54
.LFE51:
	.size	ipc_read, .-ipc_read
	.local	input
	.comm	input,8,8
	.section	.rodata
	.type	__PRETTY_FUNCTION__.46744, @object
	.size	__PRETTY_FUNCTION__.46744, 15
__PRETTY_FUNCTION__.46744:
	.string	"ipc_init_check"
	.type	__PRETTY_FUNCTION__.46892, @object
	.size	__PRETTY_FUNCTION__.46892, 9
__PRETTY_FUNCTION__.46892:
	.string	"ipc_main"
	.type	__PRETTY_FUNCTION__.46847, @object
	.size	__PRETTY_FUNCTION__.46847, 9
__PRETTY_FUNCTION__.46847:
	.string	"ipc_read"
	.type	__PRETTY_FUNCTION__.46810, @object
	.size	__PRETTY_FUNCTION__.46810, 11
__PRETTY_FUNCTION__.46810:
	.string	"ipc_commit"
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
	.long	.LCFI0-.LFB53
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE1:
.LSFDE3:
	.long	.LEFDE3-.LASFDE3
.LASFDE3:
	.long	.LASFDE3-.Lframe1
	.long	.LFB49
	.long	.LFE49-.LFB49
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI1-.LFB49
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
	.byte	0x4
	.long	.LCFI5-.LCFI4
	.byte	0xe
	.uleb128 0x30
	.byte	0x4
	.long	.LCFI6-.LCFI5
	.byte	0xe
	.uleb128 0x38
	.byte	0x4
	.long	.LCFI7-.LCFI6
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
	.long	.LFB51
	.long	.LFE51-.LFB51
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI8-.LFB51
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI9-.LCFI8
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI10-.LCFI9
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI11-.LCFI10
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
	.long	.LCFI12-.LCFI11
	.byte	0xe
	.uleb128 0x30
	.byte	0x86
	.uleb128 0x6
	.byte	0x4
	.long	.LCFI13-.LCFI12
	.byte	0xe
	.uleb128 0x38
	.byte	0x4
	.long	.LCFI14-.LCFI13
	.byte	0xe
	.uleb128 0x50
	.byte	0x83
	.uleb128 0x7
	.align 8
.LEFDE5:
	.ident	"GCC: (SUSE Linux) 4.3.2 [gcc-4_3-branch revision 141291]"
	.section	.comment.SUSE.OPTs,"MS",@progbits,1
	.ascii	"OspWg"
	.section	.note.GNU-stack,"",@progbits
