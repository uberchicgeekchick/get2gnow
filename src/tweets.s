	.file	"tweets.c"
	.text
	.p2align 4,,15
.globl tweets_new_dm
	.type	tweets_new_dm, @function
tweets_new_dm:
.LFB41:
	subq	$8, %rsp
.LCFI0:
	call	gtk_window_get_type
	movq	%rax, %rsi
	movq	app_priv(%rip), %rax
	movq	(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	send_message_dialog_show
	movl	$1, %edi
	addq	$8, %rsp
	jmp	message_show_friends
.LFE41:
	.size	tweets_new_dm, .-tweets_new_dm
	.p2align 4,,15
.globl tweets_new_tweet
	.type	tweets_new_tweet, @function
tweets_new_tweet:
.LFB37:
	subq	$8, %rsp
.LCFI1:
	cmpq	$0, in_reply_to_status_id(%rip)
	je	.L4
	movq	$0, in_reply_to_status_id(%rip)
.L4:
	call	gtk_window_get_type
	movq	%rax, %rsi
	movq	app_priv(%rip), %rax
	movq	(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	send_message_dialog_show
	xorl	%edi, %edi
	addq	$8, %rsp
	jmp	message_show_friends
.LFE37:
	.size	tweets_new_tweet, .-tweets_new_tweet
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"RT @%s %s"
	.text
	.p2align 4,,15
.globl tweets_retweet
	.type	tweets_retweet, @function
tweets_retweet:
.LFB39:
	movq	selected_tweet(%rip), %rax
	pushq	%rbx
.LCFI2:
	testq	%rax, %rax
	je	.L9
	movq	16(%rax), %rdx
	movq	8(%rax), %rsi
	movl	$.LC0, %edi
	xorl	%eax, %eax
	call	g_strdup_printf
	movq	%rax, %rbx
	movq	selected_tweet(%rip), %rax
	movq	(%rax), %rax
	movq	%rax, in_reply_to_status_id(%rip)
	call	gtk_window_get_type
	movq	%rax, %rsi
	movq	app_priv(%rip), %rax
	movq	(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	send_message_dialog_show
	xorl	%edi, %edi
	call	message_show_friends
	testq	%rbx, %rbx
	je	.L9
	movq	%rbx, %rdi
	call	message_set_message
	movq	%rbx, %rdi
	popq	%rbx
	jmp	g_free
	.p2align 4,,10
	.p2align 3
.L9:
	popq	%rbx
	ret
.LFE39:
	.size	tweets_retweet, .-tweets_retweet
	.p2align 4,,15
.globl unset_selected_tweet
	.type	unset_selected_tweet, @function
unset_selected_tweet:
.LFB33:
	subq	$8, %rsp
.LCFI3:
	movq	selected_tweet(%rip), %rdi
	testq	%rdi, %rdi
	je	.L14
	movq	8(%rdi), %rax
	testq	%rax, %rax
	je	.L12
	movq	%rax, %rdi
	call	g_free
	movq	selected_tweet(%rip), %rdi
.L12:
	movq	16(%rdi), %rax
	testq	%rax, %rax
	je	.L13
	movq	%rax, %rdi
	call	g_free
	movq	selected_tweet(%rip), %rdi
.L13:
	testq	%rdi, %rdi
	je	.L14
	addq	$8, %rsp
	jmp	g_free
	.p2align 4,,10
	.p2align 3
.L14:
	addq	$8, %rsp
	ret
.LFE33:
	.size	unset_selected_tweet, .-unset_selected_tweet
	.section	.rodata.str1.1
.LC1:
	.string	"sensitive"
	.text
	.p2align 4,,15
.globl show_tweets_submenu_entries
	.type	show_tweets_submenu_entries, @function
show_tweets_submenu_entries:
.LFB34:
	pushq	%rbp
.LCFI4:
	movl	%edi, %ebp
	pushq	%rbx
.LCFI5:
	subq	$8, %rsp
.LCFI6:
	movq	app_priv(%rip), %rax
	movq	40(%rax), %rbx
	testq	%rbx, %rbx
	je	.L18
	.p2align 4,,10
	.p2align 3
.L19:
	movq	(%rbx), %rdi
	xorl	%ecx, %ecx
	xorl	%eax, %eax
	movl	%ebp, %edx
	movl	$.LC1, %esi
	call	g_object_set
	movq	8(%rbx), %rbx
	testq	%rbx, %rbx
	jne	.L19
.L18:
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	ret
.LFE34:
	.size	show_tweets_submenu_entries, .-show_tweets_submenu_entries
	.p2align 4,,15
.globl set_selected_tweet
	.type	set_selected_tweet, @function
set_selected_tweet:
.LFB32:
	movq	%rbp, -24(%rsp)
.LCFI7:
	movq	%r12, -16(%rsp)
.LCFI8:
	movq	%rdi, %rbp
	movq	%r13, -8(%rsp)
.LCFI9:
	movq	%rbx, -32(%rsp)
.LCFI10:
	subq	$40, %rsp
.LCFI11:
	movq	selected_tweet(%rip), %rdi
	movq	%rsi, %r12
	movq	%rdx, %r13
	testq	%rdi, %rdi
	je	.L22
	call	g_free
.L22:
	movl	$24, %edi
	call	g_malloc0
	movq	%r12, %rdi
	movq	%rax, %rbx
	movq	%rbp, (%rax)
	movq	%rax, selected_tweet(%rip)
	call	g_strdup
	movq	%rax, 8(%rbx)
	movq	selected_tweet(%rip), %rbx
	movq	%r13, %rdi
	call	g_strdup
	movq	%rax, 16(%rbx)
	movq	8(%rsp), %rbx
	movq	16(%rsp), %rbp
	movq	24(%rsp), %r12
	movq	32(%rsp), %r13
	addq	$40, %rsp
	ret
.LFE32:
	.size	set_selected_tweet, .-set_selected_tweet
	.section	.rodata.str1.1
.LC2:
	.string	"@%s "
	.text
	.p2align 4,,15
.globl tweets_reply
	.type	tweets_reply, @function
tweets_reply:
.LFB38:
	movq	selected_tweet(%rip), %rax
	pushq	%rbx
.LCFI12:
	testq	%rax, %rax
	je	.L26
	movq	8(%rax), %rsi
	movl	$.LC2, %edi
	xorl	%eax, %eax
	call	g_strdup_printf
	movq	%rax, %rbx
	movq	selected_tweet(%rip), %rax
	movq	(%rax), %rax
	movq	%rax, in_reply_to_status_id(%rip)
	call	gtk_window_get_type
	movq	%rax, %rsi
	movq	app_priv(%rip), %rax
	movq	(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	send_message_dialog_show
	xorl	%edi, %edi
	call	message_show_friends
	testq	%rbx, %rbx
	je	.L26
	movq	%rbx, %rdi
	call	message_set_message
	movq	%rbx, %rdi
	popq	%rbx
	jmp	g_free
	.p2align 4,,10
	.p2align 3
.L26:
	popq	%rbx
	ret
.LFE38:
	.size	tweets_reply, .-tweets_reply
	.p2align 4,,15
.globl tweets_key_pressed
	.type	tweets_key_pressed, @function
tweets_key_pressed:
.LFB36:
	pushq	%rbp
.LCFI13:
	movq	%rdx, %rbp
	pushq	%rbx
.LCFI14:
	subq	$8, %rsp
.LCFI15:
	movl	28(%rsi), %edx
	cmpl	$65293, %edx
	jne	.L47
	movl	24(%rsi), %eax
	cmpl	$4, %eax
	je	.L42
	cmpl	$8, %eax
	je	.L43
	subl	$1, %eax
	.p2align 4,,3
	.p2align 3
	je	.L48
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	jmp	tweets_reply
	.p2align 4,,10
	.p2align 3
.L47:
	movl	i.43401(%rip), %eax
	subl	$1, %eax
	cmpl	$18, %eax
	jbe	.L29
	movl	$1, i.43401(%rip)
.L29:
	cmpl	$65362, %edx
	je	.L32
	jbe	.L49
	cmpl	$65366, %edx
	.p2align 4,,2
	.p2align 3
	je	.L34
	cmpl	$65367, %edx
	.p2align 4,,7
	.p2align 3
	je	.L35
	cmpl	$65364, %edx
	.p2align 4,,5
	.p2align 3
	jne	.L45
	movq	path.43402(%rip), %rdi
	testq	%rdi, %rdi
	je	.L37
	call	gtk_tree_path_down
	.p2align 4,,10
	.p2align 3
.L38:
	cmpq	$0, path.43402(%rip)
	je	.L37
.L39:
	movq	path.43402(%rip), %rbx
	call	gtk_tree_view_get_type
	movq	%rbp, %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	addq	$8, %rsp
	movq	%rbx, %rsi
	movq	%rax, %rdi
	popq	%rbx
	popq	%rbp
	xorl	%ecx, %ecx
	xorl	%edx, %edx
	jmp	gtk_tree_view_set_cursor
	.p2align 4,,10
	.p2align 3
.L49:
	cmpl	$65289, %edx
	je	.L31
	cmpl	$65360, %edx
	je	.L31
.L45:
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	.p2align 4,,3
	.p2align 3
	ret
	.p2align 4,,10
	.p2align 3
.L42:
	cmpq	$0, in_reply_to_status_id(%rip)
	je	.L44
	movq	$0, in_reply_to_status_id(%rip)
.L44:
	call	gtk_window_get_type
	movq	%rax, %rsi
	movq	app_priv(%rip), %rax
	movq	(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	send_message_dialog_show
	addq	$8, %rsp
	xorl	%edi, %edi
	popq	%rbx
	popq	%rbp
	jmp	message_show_friends
	.p2align 4,,10
	.p2align 3
.L48:
	call	gtk_window_get_type
	movq	%rax, %rsi
	movq	app_priv(%rip), %rax
	movq	(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	send_message_dialog_show
	addq	$8, %rsp
	movl	$1, %edi
	popq	%rbx
	popq	%rbp
	jmp	message_show_friends
	.p2align 4,,10
	.p2align 3
.L43:
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	jmp	tweets_retweet
	.p2align 4,,10
	.p2align 3
.L32:
	movq	path.43402(%rip), %rdi
	testq	%rdi, %rdi
	je	.L37
	call	gtk_tree_path_up
	jmp	.L38
	.p2align 4,,10
	.p2align 3
.L31:
	movq	path.43402(%rip), %rdi
	testq	%rdi, %rdi
	je	.L37
	call	gtk_tree_path_free
	jmp	.L38
	.p2align 4,,10
	.p2align 3
.L35:
	movl	$20, i.43401(%rip)
	.p2align 4,,3
	.p2align 3
	jmp	.L38
	.p2align 4,,10
	.p2align 3
.L34:
	addl	$5, i.43401(%rip)
	jmp	.L38
	.p2align 4,,10
	.p2align 3
.L37:
	call	gtk_tree_path_new_first
	movq	%rax, path.43402(%rip)
	jmp	.L39
.LFE36:
	.size	tweets_key_pressed, .-tweets_key_pressed
.globl in_reply_to_status_id
	.bss
	.align 8
	.type	in_reply_to_status_id, @object
	.size	in_reply_to_status_id, 8
in_reply_to_status_id:
	.zero	8
	.local	selected_tweet
	.comm	selected_tweet,8,8
	.local	path.43402
	.comm	path.43402,8,8
	.local	i.43401
	.comm	i.43401,4,4
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
	.long	.LFB41
	.long	.LFE41-.LFB41
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI0-.LFB41
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE1:
.LSFDE3:
	.long	.LEFDE3-.LASFDE3
.LASFDE3:
	.long	.LASFDE3-.Lframe1
	.long	.LFB37
	.long	.LFE37-.LFB37
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI1-.LFB37
	.byte	0xe
	.uleb128 0x10
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
	.long	.LCFI2-.LFB39
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
	.long	.LFB33
	.long	.LFE33-.LFB33
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI3-.LFB33
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE7:
.LSFDE9:
	.long	.LEFDE9-.LASFDE9
.LASFDE9:
	.long	.LASFDE9-.Lframe1
	.long	.LFB34
	.long	.LFE34-.LFB34
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI4-.LFB34
	.byte	0xe
	.uleb128 0x10
	.byte	0x86
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI5-.LCFI4
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI6-.LCFI5
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x3
	.align 8
.LEFDE9:
.LSFDE11:
	.long	.LEFDE11-.LASFDE11
.LASFDE11:
	.long	.LASFDE11-.Lframe1
	.long	.LFB32
	.long	.LFE32-.LFB32
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI8-.LFB32
	.byte	0x8c
	.uleb128 0x3
	.byte	0x86
	.uleb128 0x4
	.byte	0x4
	.long	.LCFI11-.LCFI8
	.byte	0xe
	.uleb128 0x30
	.byte	0x83
	.uleb128 0x5
	.byte	0x8d
	.uleb128 0x2
	.align 8
.LEFDE11:
.LSFDE13:
	.long	.LEFDE13-.LASFDE13
.LASFDE13:
	.long	.LASFDE13-.Lframe1
	.long	.LFB38
	.long	.LFE38-.LFB38
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI12-.LFB38
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
	.long	.LFB36
	.long	.LFE36-.LFB36
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI13-.LFB36
	.byte	0xe
	.uleb128 0x10
	.byte	0x86
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI14-.LCFI13
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI15-.LCFI14
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x3
	.align 8
.LEFDE15:
	.ident	"GCC: (SUSE Linux) 4.3.2 [gcc-4_3-branch revision 141291]"
	.section	.comment.SUSE.OPTs,"MS",@progbits,1
	.ascii	"OspWg"
	.section	.note.GNU-stack,"",@progbits
