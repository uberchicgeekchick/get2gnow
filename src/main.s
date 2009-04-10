	.file	"main.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"Greet-Tweet-Know"
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC1:
	.string	"%s is already running.  Be sure to check system try for %s's icon.\n"
	.section	.rodata.str1.1
.LC2:
	.string	"/usr/share/locale"
.LC3:
	.string	"greet-tweet-know"
.LC4:
	.string	"UTF-8"
	.text
	.p2align 4,,15
.globl main
	.type	main, @function
main:
.LFB44:
	pushq	%rbx
.LCFI0:
	movl	%edi, %edx
	subq	$16, %rsp
.LCFI1:
	movl	%edi, 12(%rsp)
	movq	%rsi, (%rsp)
	leal	-1(%rdx), %edi
	leaq	-8(%rsi), %rsi
	call	ipc_init_check
	testl	%eax, %eax
	jne	.L7
	movl	$.LC2, %esi
	movl	$.LC3, %edi
	call	bindtextdomain
	movl	$.LC4, %esi
	movl	$.LC3, %edi
	call	bind_textdomain_codeset
	movl	$.LC3, %edi
	call	textdomain
	xorl	%edi, %edi
	movl	$5, %edx
	movl	$.LC0, %esi
	call	dcgettext
	movq	%rax, %rdi
	call	g_set_application_name
	movl	g_threads_got_initialized(%rip), %eax
	testl	%eax, %eax
	je	.L8
.L3:
	leaq	12(%rsp), %rdi
	movq	%rsp, %rsi
	call	gtk_init
	movl	$.LC3, %edi
	call	gtk_window_set_default_icon_name
	call	network_new
	movl	$.LC3, %edi
	call	notify_init
	movl	%eax, %ebx
	call	app_create
	call	gtk_main
	testl	%ebx, %ebx
	je	.L4
	.p2align 4,,6
	.p2align 3
	call	notify_uninit
.L4:
	.p2align 4,,6
	.p2align 3
	call	network_close
	.p2align 4,,5
	.p2align 3
	call	ipc_deinit
	.p2align 4,,5
	.p2align 3
	call	app_get
	movq	%rax, %rdi
	call	g_object_unref
	xorl	%eax, %eax
	addq	$16, %rsp
	popq	%rbx
	ret
	.p2align 4,,10
	.p2align 3
.L8:
	xorl	%edi, %edi
	call	g_thread_init
	.p2align 4,,2
	.p2align 3
	jmp	.L3
.L7:
	movl	$.LC0, %edx
	movl	$.LC1, %edi
	xorl	%eax, %eax
	movq	%rdx, %rsi
	call	g_printf
	call	ipc_deinit
	xorl	%edi, %edi
	call	exit
.LFE44:
	.size	main, .-main
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
	.long	.LFB44
	.long	.LFE44-.LFB44
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI0-.LFB44
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI1-.LCFI0
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE1:
	.ident	"GCC: (SUSE Linux) 4.3.2 [gcc-4_3-branch revision 141291]"
	.section	.comment.SUSE.OPTs,"MS",@progbits,1
	.ascii	"OspWg"
	.section	.note.GNU-stack,"",@progbits
