	.file	"users.c"
	.text
	.p2align 4,,15
.globl user_get_friends_and_followers
	.type	user_get_friends_and_followers, @function
user_get_friends_and_followers:
.LFB58:
	subq	$8, %rsp
.LCFI0:
	movq	following_and_followers(%rip), %rax
	testq	%rax, %rax
	je	.L10
	testl	%edi, %edi
	jne	.L2
.L4:
	movl	$1, %edi
	call	network_get_users_glist
	movq	%rax, user_friends(%rip)
.L7:
	xorl	%edi, %edi
	call	network_get_users_glist
	movq	%rax, user_followers(%rip)
.L8:
	call	g_list_alloc
	movq	user_friends(%rip), %rsi
	movq	user_followers(%rip), %rdi
	movq	%rax, following_and_followers(%rip)
	call	g_list_concat
	movl	$user_sort, %esi
	movq	%rax, %rdi
	movq	%rax, following_and_followers(%rip)
	call	g_list_sort
	movq	%rax, following_and_followers(%rip)
.L2:
	addq	$8, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L10:
	testl	%edi, %edi
	je	.L4
	cmpq	$0, user_friends(%rip)
	je	.L12
.L6:
	cmpq	$0, user_followers(%rip)
	jne	.L8
	jmp	.L7
	.p2align 4,,10
	.p2align 3
.L12:
	movl	$1, %edi
	call	network_get_users_glist
	movq	%rax, user_friends(%rip)
	jmp	.L6
.LFE58:
	.size	user_get_friends_and_followers, .-user_get_friends_and_followers
	.p2align 4,,15
.globl user_get_followers
	.type	user_get_followers, @function
user_get_followers:
.LFB57:
	pushq	%rbx
.LCFI1:
	movq	user_followers(%rip), %rbx
	testq	%rbx, %rbx
	je	.L16
.L14:
	movq	%rbx, %rax
	popq	%rbx
	ret
.L16:
	xorl	%edi, %edi
	call	network_get_users_glist
	movq	%rax, %rdi
	movq	%rax, user_followers(%rip)
	call	message_set_followers
	jmp	.L14
.LFE57:
	.size	user_get_followers, .-user_get_followers
	.p2align 4,,15
.globl user_get_friends
	.type	user_get_friends, @function
user_get_friends:
.LFB56:
	pushq	%rbx
.LCFI2:
	movq	user_friends(%rip), %rbx
	testq	%rbx, %rbx
	je	.L20
.L18:
	movq	%rbx, %rax
	popq	%rbx
	ret
.L20:
	movl	$1, %edi
	call	network_get_users_glist
	movq	%rax, %rdi
	movq	%rax, user_friends(%rip)
	call	followers_dialog_load_lists
	jmp	.L18
.LFE56:
	.size	user_get_friends, .-user_get_friends
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"Follower Added"
	.text
	.p2align 4,,15
.globl user_append_follower
	.type	user_append_follower, @function
user_append_follower:
.LFB54:
	subq	$8, %rsp
.LCFI3:
	movq	%rdi, %rsi
	movq	user_followers(%rip), %rdi
	call	g_list_append
	xorl	%edi, %edi
	movl	$5, %edx
	movl	$.LC0, %esi
	movq	%rax, user_followers(%rip)
	call	dcgettext
	addq	$8, %rsp
	movq	%rax, %rdi
	jmp	app_set_statusbar_msg
.LFE54:
	.size	user_append_follower, .-user_append_follower
	.section	.rodata.str1.1
.LC1:
	.string	"Friend Added"
	.text
	.p2align 4,,15
.globl user_append_friend
	.type	user_append_friend, @function
user_append_friend:
.LFB52:
	subq	$8, %rsp
.LCFI4:
	movq	%rdi, %rsi
	movq	user_friends(%rip), %rdi
	call	g_list_append
	xorl	%edi, %edi
	movl	$5, %edx
	movl	$.LC1, %esi
	movq	%rax, user_friends(%rip)
	call	dcgettext
	addq	$8, %rsp
	movq	%rax, %rdi
	jmp	app_set_statusbar_msg
.LFE52:
	.size	user_append_friend, .-user_append_friend
	.p2align 4,,15
.globl user_free
	.type	user_free, @function
user_free:
.LFB50:
	testq	%rdi, %rdi
	pushq	%rbx
.LCFI5:
	movq	%rdi, %rbx
	je	.L34
	movq	8(%rdi), %rdi
	testq	%rdi, %rdi
	je	.L27
	call	g_free
.L27:
	movq	16(%rbx), %rdi
	testq	%rdi, %rdi
	je	.L28
	call	g_free
.L28:
	movq	24(%rbx), %rdi
	testq	%rdi, %rdi
	je	.L29
	call	g_free
.L29:
	movq	32(%rbx), %rdi
	testq	%rdi, %rdi
	je	.L30
	call	g_free
.L30:
	movq	40(%rbx), %rdi
	testq	%rdi, %rdi
	je	.L31
	call	g_free
.L31:
	movq	48(%rbx), %rdi
	testq	%rdi, %rdi
	je	.L32
	call	g_free
.L32:
	movq	56(%rbx), %rdi
	testq	%rdi, %rdi
	je	.L33
	call	g_free
.L33:
	movq	%rbx, %rdi
	popq	%rbx
	jmp	g_free
	.p2align 4,,10
	.p2align 3
.L34:
	popq	%rbx
	.p2align 4,,1
	.p2align 3
	ret
.LFE50:
	.size	user_free, .-user_free
	.p2align 4,,15
.globl user_remove_follower
	.type	user_remove_follower, @function
user_remove_follower:
.LFB55:
	pushq	%rbx
.LCFI6:
	movq	%rdi, %rsi
	movq	%rdi, %rbx
	movq	user_followers(%rip), %rdi
	call	g_list_remove
	movq	%rbx, %rdi
	movq	%rax, user_followers(%rip)
	popq	%rbx
	jmp	user_free
.LFE55:
	.size	user_remove_follower, .-user_remove_follower
	.p2align 4,,15
.globl user_remove_friend
	.type	user_remove_friend, @function
user_remove_friend:
.LFB53:
	pushq	%rbx
.LCFI7:
	movq	%rdi, %rsi
	movq	%rdi, %rbx
	movq	user_friends(%rip), %rdi
	call	g_list_remove
	movq	%rbx, %rdi
	movq	%rax, user_friends(%rip)
	popq	%rbx
	jmp	user_free
.LFE53:
	.size	user_remove_friend, .-user_remove_friend
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC2:
	.string	"Freeing the authenticated user's %s."
	.section	.rodata.str1.1
.LC3:
	.string	"Users"
	.text
	.p2align 4,,15
.globl users_free
	.type	users_free, @function
users_free:
.LFB48:
	pushq	%rbx
.LCFI8:
	movq	%rdi, %rdx
	movq	%rsi, %rbx
	movl	$.LC3, %edi
	movl	$.LC2, %esi
	xorl	%eax, %eax
	call	debug_impl
	movq	%rbx, %rdi
	popq	%rbx
	jmp	g_list_free
.LFE48:
	.size	users_free, .-users_free
	.p2align 4,,15
.globl user_sort
	.type	user_sort, @function
user_sort:
.LFB46:
	movq	8(%rsi), %rsi
	movq	8(%rdi), %rdi
	jmp	strcasecmp
.LFE46:
	.size	user_sort, .-user_sort
	.section	.rodata.str1.1
.LC4:
	.string	"id"
.LC5:
	.string	"name"
.LC6:
	.string	"screen_name"
.LC7:
	.string	"location"
.LC8:
	.string	"description"
.LC9:
	.string	"%s"
.LC10:
	.string	"url"
.LC11:
	.string	"followers_count"
.LC12:
	.string	"friends_count"
.LC13:
	.string	"statuses_count"
.LC14:
	.string	"profile_image_url"
	.text
	.p2align 4,,15
.globl user_new
	.type	user_new, @function
user_new:
.LFB45:
	pushq	%r13
.LCFI9:
	pushq	%r12
.LCFI10:
	pushq	%rbp
.LCFI11:
	pushq	%rbx
.LCFI12:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI13:
	call	xmlBufferCreate
	movl	$96, %edi
	movq	%rax, %r12
	call	g_malloc0
	movq	%rax, %rbp
	movl	getting_followers(%rip), %eax
	testq	%rbx, %rbx
	movl	%eax, 88(%rbp)
	jne	.L56
	jmp	.L44
	.p2align 4,,10
	.p2align 3
.L45:
	movq	48(%rbx), %rbx
	testq	%rbx, %rbx
	je	.L44
.L56:
	cmpl	$1, 8(%rbx)
	.p2align 4,,5
	.p2align 3
	jne	.L45
	movq	%rbx, %rsi
	movq	%r12, %rdi
	call	xmlNodeBufGetContent
	testl	%eax, %eax
	jne	.L45
	movq	%r12, %rdi
	call	xmlBufferContent
	movq	16(%rbx), %rdi
	movl	$.LC4, %esi
	movq	%rax, %r13
	call	g_str_equal
	testl	%eax, %eax
	je	.L46
	xorl	%edx, %edx
	xorl	%esi, %esi
	movq	%r13, %rdi
	call	strtoul
	movq	%rax, (%rbp)
.L47:
	movq	%r12, %rdi
	call	xmlBufferEmpty
	movq	48(%rbx), %rbx
	testq	%rbx, %rbx
	jne	.L56
	.p2align 4,,10
	.p2align 3
.L44:
	movq	%r12, %rdi
	call	xmlBufferFree
	addq	$8, %rsp
	movq	%rbp, %rax
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
	.p2align 4,,10
	.p2align 3
.L46:
	movq	16(%rbx), %rdi
	movl	$.LC5, %esi
	call	g_str_equal
	testl	%eax, %eax
	jne	.L59
	movq	16(%rbx), %rdi
	movl	$.LC6, %esi
	call	g_str_equal
	testl	%eax, %eax
	je	.L49
	movq	%r13, %rdi
	call	g_strdup
	movq	%rax, 8(%rbp)
	.p2align 4,,2
	.p2align 3
	jmp	.L47
	.p2align 4,,10
	.p2align 3
.L59:
	movq	%r13, %rdi
	call	g_strdup
	movq	%rax, 16(%rbp)
	.p2align 4,,2
	.p2align 3
	jmp	.L47
.L49:
	movq	16(%rbx), %rdi
	movl	$.LC7, %esi
	call	g_str_equal
	testl	%eax, %eax
	jne	.L60
	movq	16(%rbx), %rdi
	movl	$.LC8, %esi
	call	g_str_equal
	testl	%eax, %eax
	je	.L51
	movq	%r13, %rsi
	movl	$.LC9, %edi
	xorl	%eax, %eax
	call	g_markup_printf_escaped
	movq	%rax, 32(%rbp)
	jmp	.L47
.L60:
	movq	%r13, %rdi
	call	g_strdup
	movq	%rax, 24(%rbp)
	.p2align 4,,2
	.p2align 3
	jmp	.L47
.L51:
	movq	16(%rbx), %rdi
	movl	$.LC10, %esi
	call	g_str_equal
	testl	%eax, %eax
	je	.L52
	movq	%r13, %rdi
	call	g_strdup
	movq	%rax, 40(%rbp)
	.p2align 4,,2
	.p2align 3
	jmp	.L47
.L52:
	movq	16(%rbx), %rdi
	movl	$.LC11, %esi
	call	g_str_equal
	testl	%eax, %eax
	je	.L53
	xorl	%edx, %edx
	xorl	%esi, %esi
	movq	%r13, %rdi
	call	strtoul
	movq	%rax, 80(%rbp)
	jmp	.L47
.L53:
	movq	16(%rbx), %rdi
	movl	$.LC12, %esi
	call	g_str_equal
	testl	%eax, %eax
	je	.L54
	xorl	%edx, %edx
	xorl	%esi, %esi
	movq	%r13, %rdi
	call	strtoul
	movq	%rax, 72(%rbp)
	jmp	.L47
.L54:
	movq	16(%rbx), %rdi
	movl	$.LC13, %esi
	call	g_str_equal
	testl	%eax, %eax
	je	.L55
	xorl	%edx, %edx
	xorl	%esi, %esi
	movq	%r13, %rdi
	call	strtoul
	movq	%rax, 64(%rbp)
	jmp	.L47
.L55:
	movq	16(%rbx), %rdi
	movl	$.LC14, %esi
	call	g_str_equal
	testl	%eax, %eax
	je	.L47
	movq	%r13, %rdi
	call	g_strdup
	movq	%rax, %rdi
	movq	%rax, 48(%rbp)
	call	images_get_filename
	movq	%rax, 56(%rbp)
	jmp	.L47
.LFE45:
	.size	user_new, .-user_new
	.section	.rodata.str1.1
.LC15:
	.string	"user"
.LC16:
	.string	"users"
	.text
	.p2align 4,,15
.globl users_new
	.type	users_new, @function
users_new:
.LFB47:
	pushq	%r12
.LCFI14:
	pushq	%rbp
.LCFI15:
	pushq	%rbx
.LCFI16:
	subq	$16, %rsp
.LCFI17:
	leaq	8(%rsp), %rdx
	movq	$0, 8(%rsp)
	call	parser_parse
	testq	%rax, %rax
	movq	%rax, %r12
	je	.L72
	movq	8(%rsp), %rbx
	xorl	%ebp, %ebp
	testq	%rbx, %rbx
	jne	.L70
	jmp	.L65
	.p2align 4,,10
	.p2align 3
.L66:
	movq	48(%rbx), %rbx
	testq	%rbx, %rbx
	je	.L65
.L70:
	cmpl	$1, 8(%rbx)
	.p2align 4,,5
	.p2align 3
	jne	.L66
	movq	16(%rbx), %rdi
	movl	$.LC15, %esi
	call	g_str_equal
	testl	%eax, %eax
	je	.L67
	movq	24(%rbx), %rdi
	call	user_new
	movq	%rbp, %rdi
	movq	%rax, %rsi
	call	g_list_append
	movq	48(%rbx), %rbx
	movq	%rax, %rbp
	testq	%rbx, %rbx
	jne	.L70
	.p2align 4,,10
	.p2align 3
.L65:
	movq	%r12, %rdi
	call	xmlFreeDoc
	call	xmlCleanupParser
.L63:
	addq	$16, %rsp
	movq	%rbp, %rax
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
	.p2align 4,,10
	.p2align 3
.L67:
	movq	16(%rbx), %rdi
	movl	$.LC16, %esi
	call	g_str_equal
	testl	%eax, %eax
	je	.L66
	movq	24(%rbx), %rbx
	jmp	.L66
.L72:
	.p2align 4,,7
	.p2align 3
	call	xmlCleanupParser
	xorl	%ebp, %ebp
	.p2align 4,,4
	.p2align 3
	jmp	.L63
.LFE47:
	.size	users_new, .-users_new
	.p2align 4,,15
.globl user_parse_new
	.type	user_parse_new, @function
user_parse_new:
.LFB44:
	pushq	%rbp
.LCFI18:
	pushq	%rbx
.LCFI19:
	subq	$24, %rsp
.LCFI20:
	leaq	16(%rsp), %rdx
	movq	$0, 16(%rsp)
	call	parser_parse
	testq	%rax, %rax
	movq	%rax, %rbp
	je	.L79
	movq	16(%rsp), %rax
	movl	$.LC15, %esi
	xorl	%ebx, %ebx
	movq	16(%rax), %rdi
	call	g_str_equal
	testl	%eax, %eax
	jne	.L80
.L77:
	movq	%rbp, %rdi
	call	xmlFreeDoc
	call	xmlCleanupParser
.L75:
	movq	%rbx, %rax
	addq	$24, %rsp
	popq	%rbx
	popq	%rbp
	ret
	.p2align 4,,10
	.p2align 3
.L80:
	movq	16(%rsp), %rax
	movq	24(%rax), %rdi
	call	user_new
	movq	%rax, %rbx
	jmp	.L77
.L79:
	call	xmlCleanupParser
	xorl	%ebx, %ebx
	.p2align 4,,4
	.p2align 3
	jmp	.L75
.LFE44:
	.size	user_parse_new, .-user_parse_new
	.section	.rodata.str1.8
	.align 8
.LC17:
	.string	"friends, ie who the user is following, and the authenticated user's followers"
	.align 8
.LC18:
	.string	"friends, ie who they're following"
	.section	.rodata.str1.1
.LC19:
	.string	"followers"
	.text
	.p2align 4,,15
.globl user_free_lists
	.type	user_free_lists, @function
user_free_lists:
.LFB51:
	pushq	%rbx
.LCFI21:
	movq	following_and_followers(%rip), %rbx
	testq	%rbx, %rbx
	je	.L82
	movl	$.LC3, %edi
	movl	$.LC17, %edx
	movl	$.LC2, %esi
	xorl	%eax, %eax
	call	debug_impl
	movq	%rbx, %rdi
	call	g_list_free
.L82:
	movq	user_friends(%rip), %rbx
	testq	%rbx, %rbx
	je	.L83
	movl	$.LC3, %edi
	movl	$.LC18, %edx
	movl	$.LC2, %esi
	xorl	%eax, %eax
	call	debug_impl
	movq	%rbx, %rdi
	call	g_list_free
.L83:
	movq	user_followers(%rip), %rbx
	testq	%rbx, %rbx
	je	.L85
	movl	$.LC3, %edi
	movl	$.LC19, %edx
	movl	$.LC2, %esi
	xorl	%eax, %eax
	call	debug_impl
	movq	%rbx, %rdi
	popq	%rbx
	jmp	g_list_free
	.p2align 4,,10
	.p2align 3
.L85:
	popq	%rbx
	ret
.LFE51:
	.size	user_free_lists, .-user_free_lists
	.local	following_and_followers
	.comm	following_and_followers,8,8
	.local	user_friends
	.comm	user_friends,8,8
	.local	user_followers
	.comm	user_followers,8,8
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
	.long	.LFB58
	.long	.LFE58-.LFB58
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI0-.LFB58
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE1:
.LSFDE3:
	.long	.LEFDE3-.LASFDE3
.LASFDE3:
	.long	.LASFDE3-.Lframe1
	.long	.LFB57
	.long	.LFE57-.LFB57
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI1-.LFB57
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
	.long	.LFB56
	.long	.LFE56-.LFB56
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI2-.LFB56
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
	.long	.LFB54
	.long	.LFE54-.LFB54
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI3-.LFB54
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE7:
.LSFDE9:
	.long	.LEFDE9-.LASFDE9
.LASFDE9:
	.long	.LASFDE9-.Lframe1
	.long	.LFB52
	.long	.LFE52-.LFB52
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI4-.LFB52
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE9:
.LSFDE11:
	.long	.LEFDE11-.LASFDE11
.LASFDE11:
	.long	.LASFDE11-.Lframe1
	.long	.LFB50
	.long	.LFE50-.LFB50
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI5-.LFB50
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
	.long	.LFB55
	.long	.LFE55-.LFB55
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI6-.LFB55
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
	.long	.LFB53
	.long	.LFE53-.LFB53
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI7-.LFB53
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE15:
.LSFDE17:
	.long	.LEFDE17-.LASFDE17
.LASFDE17:
	.long	.LASFDE17-.Lframe1
	.long	.LFB48
	.long	.LFE48-.LFB48
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI8-.LFB48
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE17:
.LSFDE19:
	.long	.LEFDE19-.LASFDE19
.LASFDE19:
	.long	.LASFDE19-.Lframe1
	.long	.LFB46
	.long	.LFE46-.LFB46
	.uleb128 0x0
	.align 8
.LEFDE19:
.LSFDE21:
	.long	.LEFDE21-.LASFDE21
.LASFDE21:
	.long	.LASFDE21-.Lframe1
	.long	.LFB45
	.long	.LFE45-.LFB45
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI9-.LFB45
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI10-.LCFI9
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI11-.LCFI10
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI12-.LCFI11
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI13-.LCFI12
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE21:
.LSFDE23:
	.long	.LEFDE23-.LASFDE23
.LASFDE23:
	.long	.LASFDE23-.Lframe1
	.long	.LFB47
	.long	.LFE47-.LFB47
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI14-.LFB47
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI15-.LCFI14
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI16-.LCFI15
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI17-.LCFI16
	.byte	0xe
	.uleb128 0x30
	.byte	0x83
	.uleb128 0x4
	.byte	0x86
	.uleb128 0x3
	.byte	0x8c
	.uleb128 0x2
	.align 8
.LEFDE23:
.LSFDE25:
	.long	.LEFDE25-.LASFDE25
.LASFDE25:
	.long	.LASFDE25-.Lframe1
	.long	.LFB44
	.long	.LFE44-.LFB44
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI18-.LFB44
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI19-.LCFI18
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI20-.LCFI19
	.byte	0xe
	.uleb128 0x30
	.byte	0x83
	.uleb128 0x3
	.byte	0x86
	.uleb128 0x2
	.align 8
.LEFDE25:
.LSFDE27:
	.long	.LEFDE27-.LASFDE27
.LASFDE27:
	.long	.LASFDE27-.Lframe1
	.long	.LFB51
	.long	.LFE51-.LFB51
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI21-.LFB51
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE27:
	.ident	"GCC: (SUSE Linux) 4.3.2 [gcc-4_3-branch revision 141291]"
	.section	.comment.SUSE.OPTs,"MS",@progbits,1
	.ascii	"OspWg"
	.section	.note.GNU-stack,"",@progbits
