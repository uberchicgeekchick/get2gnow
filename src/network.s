	.file	"network.c"
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC0:
	.string	"https://twitter.com/friendships/destroy/%s.xml"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC1:
	.string	"Post: %s"
.LC2:
	.string	"Network"
.LC3:
	.string	"POST"
.LC4:
	.string	"Greet-Tweet-Know"
.LC5:
	.string	"X-Twitter-Client"
.LC6:
	.string	"0.0.eCa666"
.LC7:
	.string	"X-Twitter-Client-Version"
	.text
	.p2align 4,,15
.globl network_unfollow_user
	.type	network_unfollow_user, @function
network_unfollow_user:
.LFB63:
	movq	%rbx, -16(%rsp)
.LCFI0:
	movq	%r12, -8(%rsp)
.LCFI1:
	subq	$24, %rsp
.LCFI2:
	testq	%rdi, %rdi
	je	.L3
	cmpb	$0, (%rdi)
	jne	.L5
.L3:
	movq	8(%rsp), %rbx
	movq	16(%rsp), %r12
	addq	$24, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L5:
	movq	%rdi, %rsi
	xorl	%eax, %eax
	movl	$.LC0, %edi
	call	g_strdup_printf
	movl	$.LC1, %esi
	movq	%rax, %r12
	movq	%rax, %rdx
	movl	$.LC2, %edi
	xorl	%eax, %eax
	call	debug_impl
	movq	%r12, %rsi
	movl	$.LC3, %edi
	call	soup_message_new
	movq	56(%rax), %rdi
	movq	%rax, %rbx
	movl	$.LC4, %edx
	movl	$.LC5, %esi
	call	soup_message_headers_append
	movq	56(%rbx), %rdi
	movl	$.LC6, %edx
	movl	$.LC7, %esi
	call	soup_message_headers_append
	movq	soup_connection(%rip), %rdi
	movq	%rbx, %rsi
	xorl	%ecx, %ecx
	movl	$network_cb_on_del, %edx
	call	soup_session_queue_message
	movq	%r12, %rdi
	movq	8(%rsp), %rbx
	movq	16(%rsp), %r12
	addq	$24, %rsp
	jmp	g_free
.LFE63:
	.size	network_unfollow_user, .-network_unfollow_user
	.section	.rodata.str1.1
.LC8:
	.string	"Loading timeline..."
.LC9:
	.string	"Get: %s"
.LC10:
	.string	"GET"
	.text
	.p2align 4,,15
.globl network_refresh
	.type	network_refresh, @function
network_refresh:
.LFB52:
	cmpq	$0, current_timeline(%rip)
	pushq	%rbx
.LCFI3:
	je	.L8
	movl	processing(%rip), %eax
	testl	%eax, %eax
	je	.L9
.L8:
	popq	%rbx
	ret
	.p2align 4,,10
	.p2align 3
.L9:
	movl	$5, %edx
	movl	$.LC8, %esi
	xorl	%edi, %edi
	call	dcgettext
	movq	%rax, %rdi
	call	app_set_statusbar_msg
	movq	current_timeline(%rip), %rbx
	movl	$.LC9, %esi
	movl	$.LC2, %edi
	xorl	%eax, %eax
	movl	$1, processing(%rip)
	movq	%rbx, %rdx
	call	debug_impl
	movq	%rbx, %rsi
	movl	$.LC10, %edi
	call	soup_message_new
	popq	%rbx
	movq	soup_connection(%rip), %rdi
	movq	%rax, %rsi
	xorl	%ecx, %ecx
	movl	$network_cb_on_timeline, %edx
	jmp	soup_session_queue_message
.LFE52:
	.size	network_refresh, .-network_refresh
	.section	.rodata.str1.1
.LC11:
	.string	"X-RateLimit-Remaining"
	.section	.rodata.str1.8
	.align 8
.LC12:
	.string	"You have %d request left before the API's RateLimit is reached."
	.section	.rodata.str1.1
.LC18:
	.string	"Access denied"
.LC19:
	.string	"HTTP communication error"
.LC20:
	.string	"Internal server error"
.LC21:
	.string	"Stopped"
.LC22:
	.string	"%s: %s."
	.text
	.p2align 4,,15
	.type	network_check_http, @function
network_check_http:
.LFB67:
	pushq	%rbp
.LCFI4:
	movq	%rdi, %rbp
	pushq	%rbx
.LCFI5:
	subq	$40, %rsp
.LCFI6:
	movq	request_timer(%rip), %rdi
	call	g_timer_start
	movq	72(%rbp), %rdi
	movl	$.LC11, %esi
	movq	$1, 32(%rsp)
	call	soup_message_headers_get
	testq	%rax, %rax
	je	.L40
	xorl	%esi, %esi
	movl	$10, %edx
	movq	%rax, %rdi
	call	strtol
	testl	%eax, %eax
	movl	%eax, %ebx
	jne	.L38
	movsd	.LC13(%rip), %xmm1
	leaq	32(%rsp), %rbx
	movsd	%xmm1, 16(%rsp)
	.p2align 4,,10
	.p2align 3
.L30:
	movq	request_timer(%rip), %rdi
	movq	%rbx, %rsi
	call	g_timer_elapsed
	movsd	16(%rsp), %xmm1
	ucomisd	%xmm0, %xmm1
	ja	.L30
.L14:
	movq	request_timer(%rip), %rdi
	call	g_timer_stop
	movl	32(%rbp), %edx
	cmpl	$401, %edx
	je	.L41
	leal	-400(%rdx), %eax
	cmpl	$99, %eax
	jbe	.L42
	leal	-500(%rdx), %eax
	cmpl	$99, %eax
	jbe	.L43
	leal	-200(%rdx), %eax
	movl	$1, %edx
	cmpl	$99, %eax
	ja	.L44
	addq	$40, %rsp
	movl	%edx, %eax
	popq	%rbx
	popq	%rbp
	ret
	.p2align 4,,10
	.p2align 3
.L38:
	movl	%eax, %edx
	movl	$.LC12, %esi
	xorl	%eax, %eax
	movl	$.LC2, %edi
	call	debug_impl
	cmpl	$10, %ebx
	jle	.L45
	cmpl	$20, %ebx
	jg	.L15
	leaq	32(%rsp), %rbx
	.p2align 4,,10
	.p2align 3
.L16:
	movq	request_timer(%rip), %rdi
	movq	%rbx, %rsi
	call	g_timer_elapsed
	movsd	.LC14(%rip), %xmm1
	ucomisd	%xmm0, %xmm1
	ja	.L16
	jmp	.L14
	.p2align 4,,10
	.p2align 3
.L40:
	movsd	.LC17(%rip), %xmm0
	leaq	32(%rsp), %rbx
	movsd	%xmm0, 8(%rsp)
.L31:
	movq	request_timer(%rip), %rdi
	movq	%rbx, %rsi
	call	g_timer_elapsed
	movsd	8(%rsp), %xmm1
	ucomisd	%xmm0, %xmm1
	ja	.L31
	jmp	.L14
	.p2align 4,,10
	.p2align 3
.L42:
	movl	$.LC19, %esi
	movl	$5, %edx
	xorl	%edi, %edi
	call	dcgettext
	movq	%rax, %rsi
.L21:
	movq	40(%rbp), %rdx
	movl	$.LC22, %edi
	xorl	%eax, %eax
	call	g_strdup_printf
	movq	%rax, %rbx
	movq	%rax, %rdi
	call	app_set_statusbar_msg
	movq	%rbx, %rdi
	call	g_free
	addq	$40, %rsp
	xorl	%edx, %edx
	popq	%rbx
	movl	%edx, %eax
	popq	%rbp
	ret
	.p2align 4,,10
	.p2align 3
.L43:
	movl	$.LC20, %esi
	movl	$5, %edx
	xorl	%edi, %edi
	call	dcgettext
	movq	%rax, %rsi
	jmp	.L21
	.p2align 4,,10
	.p2align 3
.L41:
	movl	$.LC18, %esi
	movw	$5, %dx
	xorl	%edi, %edi
	call	dcgettext
	movq	%rax, %rsi
	jmp	.L21
	.p2align 4,,10
	.p2align 3
.L44:
	movl	$.LC21, %esi
	movl	$5, %edx
	xorl	%edi, %edi
	call	dcgettext
	movq	%rax, %rsi
	jmp	.L21
.L45:
	movsd	.LC13(%rip), %xmm0
	leaq	32(%rsp), %rbx
	movsd	%xmm0, 16(%rsp)
	jmp	.L30
.L15:
	cmpl	$40, %ebx
	jg	.L17
	leaq	32(%rsp), %rbx
	.p2align 4,,10
	.p2align 3
.L18:
	movq	request_timer(%rip), %rdi
	movq	%rbx, %rsi
	call	g_timer_elapsed
	movsd	.LC15(%rip), %xmm1
	ucomisd	%xmm0, %xmm1
	ja	.L18
	jmp	.L14
.L17:
	cmpl	$60, %ebx
	.p2align 4,,2
	.p2align 3
	jg	.L40
	leaq	32(%rsp), %rbx
	.p2align 4,,10
	.p2align 3
.L19:
	movq	request_timer(%rip), %rdi
	movq	%rbx, %rsi
	call	g_timer_elapsed
	movsd	.LC16(%rip), %xmm1
	ucomisd	%xmm0, %xmm1
	ja	.L19
	jmp	.L14
.LFE67:
	.size	network_check_http, .-network_check_http
	.section	.rodata.str1.1
.LC23:
	.string	"Delete user response: %i"
.LC24:
	.string	"Friend Removed"
.LC25:
	.string	"Failed to remove user"
	.text
	.p2align 4,,15
	.type	network_cb_on_del, @function
network_cb_on_del:
.LFB75:
	pushq	%rbp
.LCFI7:
	movq	%rdx, %rbp
	xorl	%eax, %eax
	movl	$.LC2, %edi
	pushq	%rbx
.LCFI8:
	movq	%rsi, %rbx
	subq	$8, %rsp
.LCFI9:
	movl	32(%rsi), %edx
	movl	$.LC23, %esi
	call	debug_impl
	movq	%rbx, %rdi
	call	network_check_http
	testl	%eax, %eax
	je	.L47
	xorl	%edi, %edi
	movl	$5, %edx
	movl	$.LC24, %esi
	call	dcgettext
	movq	%rax, %rdi
	call	app_set_statusbar_msg
	testq	%rbp, %rbp
	je	.L50
.L51:
	addq	$8, %rsp
	movq	%rbp, %rdi
	popq	%rbx
	popq	%rbp
	jmp	user_remove_friend
	.p2align 4,,10
	.p2align 3
.L47:
	xorl	%edi, %edi
	movl	$5, %edx
	movl	$.LC25, %esi
	call	dcgettext
	movq	%rax, %rdi
	call	app_set_statusbar_msg
	testq	%rbp, %rbp
	jne	.L51
.L50:
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	ret
.LFE75:
	.size	network_cb_on_del, .-network_cb_on_del
	.section	.rodata.str1.1
.LC26:
	.string	"Message Sent"
.LC27:
	.string	"Message response: %i"
	.text
	.p2align 4,,15
	.type	network_cb_on_message, @function
network_cb_on_message:
.LFB71:
	pushq	%rbx
.LCFI10:
	movq	%rsi, %rdi
	movq	%rsi, %rbx
	call	network_check_http
	testl	%eax, %eax
	je	.L53
	xorl	%edi, %edi
	movl	$5, %edx
	movl	$.LC26, %esi
	call	dcgettext
	movq	%rax, %rdi
	call	app_set_statusbar_msg
.L53:
	movl	32(%rbx), %edx
	movl	$.LC27, %esi
	movl	$.LC2, %edi
	popq	%rbx
	xorl	%eax, %eax
	jmp	debug_impl
.LFE71:
	.size	network_cb_on_message, .-network_cb_on_message
	.section	.rodata.str1.1
.LC28:
	.string	"Status Sent"
.LC29:
	.string	"Tweet response: %i"
	.text
	.p2align 4,,15
	.type	network_cb_on_post, @function
network_cb_on_post:
.LFB70:
	pushq	%rbx
.LCFI11:
	movq	%rsi, %rdi
	movq	%rsi, %rbx
	call	network_check_http
	testl	%eax, %eax
	je	.L56
	xorl	%edi, %edi
	movl	$5, %edx
	movl	$.LC28, %esi
	call	dcgettext
	movq	%rax, %rdi
	call	app_set_statusbar_msg
.L56:
	movl	32(%rbx), %edx
	movl	$.LC29, %esi
	movl	$.LC2, %edi
	popq	%rbx
	xorl	%eax, %eax
	movq	$0, in_reply_to_status_id(%rip)
	jmp	debug_impl
.LFE70:
	.size	network_cb_on_post, .-network_cb_on_post
	.section	.rodata.str1.1
.LC30:
	.string	"Add user response: %i"
.LC31:
	.string	"Parsing new user"
.LC32:
	.string	"Failed to add user"
	.text
	.p2align 4,,15
	.type	network_cb_on_add, @function
network_cb_on_add:
.LFB74:
	pushq	%rbx
.LCFI12:
	movl	32(%rsi), %edx
	xorl	%eax, %eax
	movq	%rsi, %rbx
	movl	$.LC2, %edi
	movl	$.LC30, %esi
	call	debug_impl
	movq	%rbx, %rdi
	call	network_check_http
	testl	%eax, %eax
	jne	.L62
	popq	%rbx
	.p2align 4,,3
	.p2align 3
	ret
	.p2align 4,,10
	.p2align 3
.L62:
	xorl	%eax, %eax
	movl	$.LC31, %esi
	movl	$.LC2, %edi
	call	debug_impl
	movq	64(%rbx), %rax
	movq	8(%rax), %rsi
	movq	(%rax), %rdi
	call	user_parse_new
	testq	%rax, %rax
	je	.L60
	popq	%rbx
	movq	%rax, %rdi
	jmp	user_append_friend
	.p2align 4,,10
	.p2align 3
.L60:
	xorl	%edi, %edi
	movl	$5, %edx
	movl	$.LC32, %esi
	call	dcgettext
	popq	%rbx
	movq	%rax, %rdi
	jmp	app_set_statusbar_msg
.LFE74:
	.size	network_cb_on_add, .-network_cb_on_add
	.section	.rodata.str1.1
.LC33:
	.string	"Image response: %i"
	.text
	.p2align 4,,15
	.type	network_cb_on_image, @function
network_cb_on_image:
.LFB73:
	pushq	%rbp
.LCFI13:
	movq	%rdx, %rbp
	xorl	%eax, %eax
	movl	$.LC2, %edi
	pushq	%rbx
.LCFI14:
	movq	%rsi, %rbx
	subq	$40, %rsp
.LCFI15:
	movl	32(%rsi), %edx
	movl	$.LC33, %esi
	call	debug_impl
	movq	%rbx, %rdi
	call	network_check_http
	testl	%eax, %eax
	jne	.L67
.L64:
	movq	(%rbp), %rdi
	testq	%rdi, %rdi
	je	.L65
	.p2align 4,,5
	.p2align 3
	call	g_free
.L65:
	movq	$0, (%rbp)
	addq	$40, %rsp
	movq	%rbp, %rdi
	popq	%rbx
	popq	%rbp
	jmp	g_free
	.p2align 4,,10
	.p2align 3
.L67:
	movq	64(%rbx), %rax
	movq	(%rbp), %rdi
	xorl	%ecx, %ecx
	movq	8(%rax), %rdx
	movq	(%rax), %rsi
	call	g_file_set_contents
	testl	%eax, %eax
	je	.L64
	movq	8(%rbp), %rax
	movq	%rax, (%rsp)
	movq	16(%rbp), %rax
	movq	%rax, 8(%rsp)
	movq	24(%rbp), %rax
	movq	%rax, 16(%rsp)
	movq	32(%rbp), %rax
	movq	%rax, 24(%rsp)
	movq	(%rbp), %rdi
	call	app_set_image
	jmp	.L64
.LFE73:
	.size	network_cb_on_image, .-network_cb_on_image
	.section	.rodata.str1.8
	.align 8
.LC34:
	.string	"https://twitter.com/users/show/%s.xml"
	.text
	.p2align 4,,15
.globl network_fetch_profile
	.type	network_fetch_profile, @function
network_fetch_profile:
.LFB54:
	movq	%rbx, -16(%rsp)
.LCFI16:
	movq	%r12, -8(%rsp)
.LCFI17:
	subq	$24, %rsp
.LCFI18:
	testq	%rdi, %rdi
	jne	.L72
.L69:
	xorl	%eax, %eax
	movq	8(%rsp), %rbx
	movq	16(%rsp), %r12
	addq	$24, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L72:
	cmpb	$0, (%rdi)
	je	.L69
	movq	%rdi, %rsi
	xorl	%eax, %eax
	movl	$.LC34, %edi
	call	g_strdup_printf
	movl	$.LC9, %esi
	movq	%rax, %rdx
	movq	%rax, %r12
	movl	$.LC2, %edi
	xorl	%eax, %eax
	call	debug_impl
	movq	%r12, %rsi
	movl	$.LC10, %edi
	call	soup_message_new
	movq	soup_connection(%rip), %rdi
	movq	%rax, %rsi
	movq	%rax, %rbx
	call	soup_session_send_message
	movq	%rbx, %rdi
	call	network_check_http
	testl	%eax, %eax
	movl	$0, %eax
	movq	%r12, %rdi
	cmove	%rax, %rbx
	call	g_free
	movq	64(%rbx), %rax
	movq	16(%rsp), %r12
	movq	8(%rsp), %rbx
	movq	8(%rax), %rsi
	movq	(%rax), %rdi
	addq	$24, %rsp
	jmp	user_parse_new
.LFE54:
	.size	network_fetch_profile, .-network_fetch_profile
	.section	.rodata.str1.1
.LC35:
	.string	"Timeline response: %i"
.LC36:
	.string	"Stopping timeout id: %i"
	.section	.rodata.str1.8
	.align 8
.LC37:
	.string	"/apps/greet-tweet-know/tweets/reload_timeline"
	.section	.rodata.str1.1
.LC38:
	.string	"Starting timeout id: %i"
.LC39:
	.string	"Parsing timeline"
.LC40:
	.string	"Timeline Loaded"
.LC41:
	.string	"Timeline Parser Error."
	.text
	.p2align 4,,15
	.type	network_cb_on_timeline, @function
network_cb_on_timeline:
.LFB72:
	pushq	%rbp
.LCFI19:
	movq	%rdx, %rbp
	xorl	%eax, %eax
	movl	$.LC2, %edi
	pushq	%rbx
.LCFI20:
	movq	%rsi, %rbx
	subq	$24, %rsp
.LCFI21:
	movl	32(%rsi), %edx
	movl	$.LC35, %esi
	call	debug_impl
	movl	timeout_id(%rip), %edx
	movl	$0, processing(%rip)
	testl	%edx, %edx
	jne	.L86
.L76:
	call	conf_get
	leaq	20(%rsp), %rdx
	movq	%rax, %rdi
	movl	$.LC37, %esi
	call	conf_get_int
	cmpl	$2, 20(%rsp)
	jg	.L77
	movl	$3, 20(%rsp)
.L77:
	movl	20(%rsp), %edx
	movl	$network_timeout, %esi
	imull	$60000, %edx, %edi
	xorl	%edx, %edx
	call	g_timeout_add
	movl	$.LC38, %esi
	movl	%eax, %edx
	movl	%eax, timeout_id(%rip)
	movl	$.LC2, %edi
	xorl	%eax, %eax
	call	debug_impl
	movq	%rbx, %rdi
	call	network_check_http
	testl	%eax, %eax
	jne	.L78
.L85:
	testq	%rbp, %rbp
	.p2align 4,,2
	.p2align 3
	je	.L83
.L82:
	movq	%rbp, %rdi
	.p2align 4,,5
	.p2align 3
	call	g_free
.L83:
	addq	$24, %rsp
	popq	%rbx
	popq	%rbp
	ret
	.p2align 4,,10
	.p2align 3
.L86:
	movl	$.LC36, %esi
	movl	$.LC2, %edi
	xorl	%eax, %eax
	call	debug_impl
	movl	timeout_id(%rip), %edi
	call	g_source_remove
	jmp	.L76
	.p2align 4,,10
	.p2align 3
.L78:
	xorl	%eax, %eax
	movl	$.LC39, %esi
	movl	$.LC2, %edi
	call	debug_impl
	movq	64(%rbx), %rax
	movq	8(%rax), %rsi
	movq	(%rax), %rdi
	call	parser_timeline
	testl	%eax, %eax
	je	.L80
	xorl	%edi, %edi
	movl	$5, %edx
	movl	$.LC40, %esi
	call	dcgettext
	movq	%rax, %rdi
	call	app_set_statusbar_msg
	testq	%rbp, %rbp
	je	.L83
	movq	current_timeline(%rip), %rdi
	testq	%rdi, %rdi
	je	.L81
	call	g_free
.L81:
	movq	%rbp, %rdi
	movq	$0, current_timeline(%rip)
	call	g_strdup
	movq	%rax, current_timeline(%rip)
	jmp	.L82
	.p2align 4,,10
	.p2align 3
.L80:
	xorl	%edi, %edi
	movl	$5, %edx
	movl	$.LC41, %esi
	call	dcgettext
	movq	%rax, %rdi
	call	app_set_statusbar_msg
	jmp	.L85
.LFE72:
	.size	network_cb_on_timeline, .-network_cb_on_timeline
	.section	.rodata.str1.1
.LC42:
	.string	"Login response: %i"
	.text
	.p2align 4,,15
	.type	network_cb_on_login, @function
network_cb_on_login:
.LFB69:
	pushq	%rbx
.LCFI22:
	movl	32(%rsi), %edx
	xorl	%eax, %eax
	movq	%rsi, %rbx
	movl	$.LC2, %edi
	movl	$.LC42, %esi
	call	debug_impl
	movq	%rbx, %rdi
	call	network_check_http
	testl	%eax, %eax
	jne	.L90
	popq	%rbx
	xorl	%edi, %edi
	jmp	app_state_on_connection
	.p2align 4,,10
	.p2align 3
.L90:
	popq	%rbx
	movl	$1, %edi
	jmp	app_state_on_connection
.LFE69:
	.size	network_cb_on_login, .-network_cb_on_login
	.p2align 4,,15
	.type	network_cb_on_auth, @function
network_cb_on_auth:
.LFB68:
	movq	global_username(%rip), %rsi
	movq	%rdx, %rax
	testq	%rsi, %rsi
	je	.L93
	cmpb	$0, (%rsi)
	je	.L93
	movq	global_password(%rip), %rdx
	testq	%rdx, %rdx
	je	.L93
	cmpb	$0, (%rdx)
	jne	.L94
.L93:
	rep
	ret
	.p2align 4,,10
	.p2align 3
.L94:
	movq	%rax, %rdi
	jmp	soup_auth_authenticate
.LFE68:
	.size	network_cb_on_auth, .-network_cb_on_auth
	.section	.rodata.str1.1
.LC43:
	.string	"Cancelled all connections"
	.text
	.p2align 4,,15
.globl network_stop
	.type	network_stop, @function
network_stop:
.LFB47:
	subq	$8, %rsp
.LCFI23:
	movl	$.LC2, %edi
	movl	$.LC43, %esi
	xorl	%eax, %eax
	call	debug_impl
	movq	soup_connection(%rip), %rdi
	addq	$8, %rsp
	jmp	soup_session_abort
.LFE47:
	.size	network_stop, .-network_stop
	.section	.rodata.str1.1
.LC44:
	.string	"Libsoup closed"
	.text
	.p2align 4,,15
.globl network_close
	.type	network_close, @function
network_close:
.LFB46:
	subq	$8, %rsp
.LCFI24:
	movl	$.LC43, %esi
	xorl	%eax, %eax
	movl	$.LC2, %edi
	call	debug_impl
	movq	soup_connection(%rip), %rdi
	call	soup_session_abort
	movq	request_timer(%rip), %rdi
	call	g_timer_destroy
	movq	soup_connection(%rip), %rdi
	movq	$0, request_timer(%rip)
	call	g_object_unref
	movl	$.LC44, %esi
	movl	$.LC2, %edi
	xorl	%eax, %eax
	addq	$8, %rsp
	jmp	debug_impl
.LFE46:
	.size	network_close, .-network_close
	.section	.rodata.str1.8
	.align 8
.LC45:
	.string	"https://twitter.com/statuses/followers.xml"
	.section	.rodata.str1.1
.LC46:
	.string	"%s?page=%d"
.LC47:
	.string	"Users response: %i"
.LC48:
	.string	"Parsing user list"
	.section	.rodata.str1.8
	.align 8
.LC49:
	.string	"https://twitter.com/statuses/friends.xml"
	.section	.rodata.str1.1
.LC50:
	.string	"Users parser error."
	.text
	.p2align 4,,15
.globl network_get_users_glist
	.type	network_get_users_glist, @function
network_get_users_glist:
.LFB56:
	pushq	%r13
.LCFI25:
	xorl	%eax, %eax
	pushq	%r12
.LCFI26:
	pushq	%rbp
.LCFI27:
	pushq	%rbx
.LCFI28:
	subq	$8, %rsp
.LCFI29:
	testl	%edi, %edi
	movq	$0, all_users(%rip)
	sete	%al
	xorl	%r12d, %r12d
	xorl	%r13d, %r13d
	testl	%edi, %edi
	movl	%eax, getting_followers(%rip)
	jne	.L117
	.p2align 4,,10
	.p2align 3
.L106:
	addl	$1, %r12d
	movl	$.LC45, %esi
	movl	$.LC46, %edi
	movl	%r12d, %edx
	xorl	%eax, %eax
	call	g_strdup_printf
	movl	$.LC9, %esi
	movq	%rax, %rdx
	movq	%rax, %rbp
	movl	$.LC2, %edi
	xorl	%eax, %eax
	call	debug_impl
	movq	%rbp, %rsi
	movl	$.LC10, %edi
	call	soup_message_new
	movq	soup_connection(%rip), %rdi
	movq	%rax, %rbx
	movq	%rax, %rsi
	call	soup_session_send_message
	movq	%rbx, %rdi
	call	network_check_http
	testl	%eax, %eax
	movl	$.LC47, %esi
	movl	$.LC2, %edi
	cmove	%r13, %rbx
	xorl	%eax, %eax
	movl	32(%rbx), %edx
	call	debug_impl
	movq	%rbx, %rdi
	call	network_check_http
	testl	%eax, %eax
	jne	.L121
.L102:
	xorl	%ebx, %ebx
.L104:
	testq	%rbp, %rbp
	je	.L105
	movq	%rbp, %rdi
	call	g_free
.L105:
	testl	%ebx, %ebx
	jne	.L106
.L107:
	movq	all_users(%rip), %rdi
	testq	%rdi, %rdi
	je	.L122
	movl	$user_sort, %esi
	call	g_list_sort
	movq	%rax, all_users(%rip)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
	.p2align 4,,10
	.p2align 3
.L109:
	xorl	%ebx, %ebx
.L111:
	testq	%rbp, %rbp
	je	.L112
	movq	%rbp, %rdi
	call	g_free
.L112:
	testl	%ebx, %ebx
	je	.L107
.L117:
	addl	$1, %r12d
	movl	$.LC49, %esi
	movl	$.LC46, %edi
	movl	%r12d, %edx
	xorl	%eax, %eax
	call	g_strdup_printf
	movl	$.LC9, %esi
	movq	%rax, %rdx
	movq	%rax, %rbp
	movl	$.LC2, %edi
	xorl	%eax, %eax
	call	debug_impl
	movq	%rbp, %rsi
	movl	$.LC10, %edi
	call	soup_message_new
	movq	soup_connection(%rip), %rdi
	movq	%rax, %rbx
	movq	%rax, %rsi
	call	soup_session_send_message
	movq	%rbx, %rdi
	call	network_check_http
	testl	%eax, %eax
	movl	$.LC47, %esi
	movl	$.LC2, %edi
	cmove	%r13, %rbx
	xorl	%eax, %eax
	movl	32(%rbx), %edx
	call	debug_impl
	movq	%rbx, %rdi
	call	network_check_http
	testl	%eax, %eax
	je	.L109
	xorl	%eax, %eax
	movl	$.LC48, %esi
	movl	$.LC2, %edi
	call	debug_impl
	movq	64(%rbx), %rax
	movq	8(%rax), %rsi
	movq	(%rax), %rdi
	call	users_new
	testq	%rax, %rax
	je	.L109
	movq	all_users(%rip), %rdi
	testq	%rdi, %rdi
	je	.L120
	movq	%rax, %rsi
	call	g_list_concat
.L120:
	movq	%rax, all_users(%rip)
	movl	$1, %ebx
	jmp	.L111
	.p2align 4,,10
	.p2align 3
.L121:
	xorl	%eax, %eax
	movl	$.LC48, %esi
	movl	$.LC2, %edi
	call	debug_impl
	movq	64(%rbx), %rax
	movq	8(%rax), %rsi
	movq	(%rax), %rdi
	call	users_new
	testq	%rax, %rax
	je	.L102
	movq	all_users(%rip), %rdi
	testq	%rdi, %rdi
	je	.L103
	movq	%rax, %rsi
	movl	$1, %ebx
	call	g_list_concat
	movq	%rax, all_users(%rip)
	jmp	.L104
	.p2align 4,,10
	.p2align 3
.L103:
	movq	%rax, all_users(%rip)
	movl	$1, %ebx
	jmp	.L104
	.p2align 4,,10
	.p2align 3
.L122:
	movl	$5, %edx
	movl	$.LC50, %esi
	call	dcgettext
	movq	%rax, %rdi
	call	app_set_statusbar_msg
	movq	all_users(%rip), %rax
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.LFE56:
	.size	network_get_users_glist, .-network_get_users_glist
	.section	.rodata.str1.8
	.align 8
.LC51:
	.string	"https://twitter.com/statuses/friends_timeline.xml"
	.align 8
.LC52:
	.string	"https://twitter.com/statuses/replies.xml"
	.align 8
.LC53:
	.string	"https://twitter.com/direct_messages.xml"
	.section	.rodata.str1.1
.LC54:
	.string	"combined"
	.text
	.p2align 4,,15
.globl network_get_combined_timeline
	.type	network_get_combined_timeline, @function
network_get_combined_timeline:
.LFB57:
	pushq	%rbp
.LCFI30:
	pushq	%rbx
.LCFI31:
	movl	$.LC51, %ebx
	subq	$40, %rsp
.LCFI32:
	leaq	8(%rsp), %rbp
	movq	$.LC51, (%rsp)
	movq	$.LC52, 8(%rsp)
	movq	$.LC53, 16(%rsp)
	movq	$0, 24(%rsp)
.L125:
	movq	%rbx, %rdx
	xorl	%eax, %eax
	movl	$.LC9, %esi
	movl	$.LC2, %edi
	call	debug_impl
	movq	%rbx, %rsi
	movl	$.LC10, %edi
	call	soup_message_new
	movq	soup_connection(%rip), %rdi
	movq	%rax, %rsi
	movl	$.LC54, %ecx
	movl	$network_cb_on_timeline, %edx
	call	soup_session_queue_message
	movq	(%rbp), %rbx
	addq	$8, %rbp
	testq	%rbx, %rbx
	jne	.L125
	movq	current_timeline(%rip), %rdi
	testq	%rdi, %rdi
	je	.L126
	call	g_free
.L126:
	movl	$.LC54, %edi
	call	g_strdup
	movq	%rax, current_timeline(%rip)
	addq	$40, %rsp
	popq	%rbx
	popq	%rbp
	ret
.LFE57:
	.size	network_get_combined_timeline, .-network_get_combined_timeline
	.section	.rodata.str1.1
.LC55:
	.string	"Begin login.. "
.LC56:
	.string	"Connecting..."
.LC57:
	.string	"authenticate"
	.section	.rodata.str1.8
	.align 8
.LC58:
	.string	"https://twitter.com/account/verify_credentials.xml"
	.text
	.p2align 4,,15
.globl network_login
	.type	network_login, @function
network_login:
.LFB48:
	pushq	%rbp
.LCFI33:
	movq	%rsi, %rbp
	pushq	%rbx
.LCFI34:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI35:
	movq	request_timer(%rip), %rdi
	testq	%rdi, %rdi
	je	.L130
	call	g_timer_destroy
.L130:
	call	g_timer_new
	movl	$.LC2, %edi
	movq	%rax, request_timer(%rip)
	movl	$.LC55, %esi
	xorl	%eax, %eax
	call	debug_impl
	movq	global_username(%rip), %rdi
	testq	%rdi, %rdi
	je	.L131
	call	g_free
.L131:
	movq	%rbx, %rdi
	movq	$0, global_username(%rip)
	call	g_strdup
	movq	global_password(%rip), %rdi
	movq	%rax, global_username(%rip)
	testq	%rdi, %rdi
	je	.L132
	call	g_free
.L132:
	movq	%rbp, %rdi
	movq	$0, global_password(%rip)
	call	g_strdup
	movl	$5, %edx
	movl	$.LC56, %esi
	xorl	%edi, %edi
	movq	%rax, global_password(%rip)
	call	dcgettext
	movq	%rax, %rdi
	call	app_set_statusbar_msg
	movq	soup_connection(%rip), %rdi
	xorl	%ecx, %ecx
	xorl	%r9d, %r9d
	xorl	%r8d, %r8d
	movl	$network_cb_on_auth, %edx
	movl	$.LC57, %esi
	call	g_signal_connect_data
	movl	$.LC58, %edx
	movl	$.LC9, %esi
	movl	$.LC2, %edi
	xorl	%eax, %eax
	call	debug_impl
	movl	$.LC58, %esi
	movl	$.LC10, %edi
	call	soup_message_new
	movq	soup_connection(%rip), %rdi
	addq	$8, %rsp
	movq	%rax, %rsi
	popq	%rbx
	popq	%rbp
	xorl	%ecx, %ecx
	movl	$network_cb_on_login, %edx
	jmp	soup_session_queue_message
.LFE48:
	.size	network_login, .-network_login
	.section	.rodata.str1.1
.LC59:
	.string	""
.LC60:
	.string	"re-"
.LC61:
	.string	"Libsoup %sstarted"
.LC62:
	.string	"max-conns"
	.section	.rodata.str1.8
	.align 8
.LC63:
	.string	"/system/http_proxy/use_http_proxy"
	.section	.rodata.str1.1
.LC64:
	.string	"/system/http_proxy/host"
.LC65:
	.string	"/system/http_proxy/port"
	.section	.rodata.str1.8
	.align 8
.LC66:
	.string	"/system/http_proxy/use_authentication"
	.section	.rodata.str1.1
.LC67:
	.string	"http://%s:%d"
	.section	.rodata.str1.8
	.align 8
.LC68:
	.string	"/system/http_proxy/authentication_user"
	.align 8
.LC69:
	.string	"/system/http_proxy/authentication_password"
	.section	.rodata.str1.1
.LC70:
	.string	"http://%s:%s@%s:%d"
.LC71:
	.string	"Proxy uri: %s"
.LC72:
	.string	"proxy-uri"
	.text
	.p2align 4,,15
.globl network_new
	.type	network_new, @function
network_new:
.LFB45:
	pushq	%rbp
.LCFI36:
	movl	$.LC60, %eax
	movl	$.LC59, %edx
	movl	$.LC61, %esi
	movl	$.LC2, %edi
	pushq	%rbx
.LCFI37:
	subq	$40, %rsp
.LCFI38:
	cmpq	$0, soup_connection(%rip)
	movl	$0, 36(%rsp)
	cmovne	%rax, %rdx
	xorl	%eax, %eax
	call	debug_impl
	cmpq	$0, soup_connection(%rip)
	je	.L137
	movl	$.LC43, %esi
	xorl	%eax, %eax
	movl	$.LC2, %edi
	call	debug_impl
	movq	soup_connection(%rip), %rdi
	call	soup_session_abort
	movq	request_timer(%rip), %rdi
	call	g_timer_destroy
	movq	soup_connection(%rip), %rdi
	movq	$0, request_timer(%rip)
	call	g_object_unref
	movl	$.LC44, %esi
	movl	$.LC2, %edi
	xorl	%eax, %eax
	call	debug_impl
.L137:
	leaq	36(%rsp), %rbp
	xorl	%edx, %edx
	movl	$8, %esi
	movl	$.LC62, %edi
	xorl	%eax, %eax
	call	soup_session_async_new_with_options
	movq	%rax, soup_connection(%rip)
	call	conf_get
	movl	$.LC63, %esi
	movq	%rbp, %rdx
	movq	%rax, %rdi
	movq	%rax, %rbx
	call	conf_get_bool
	movl	36(%rsp), %esi
	testl	%esi, %esi
	jne	.L144
.L142:
	addq	$40, %rsp
	popq	%rbx
	popq	%rbp
	ret
	.p2align 4,,10
	.p2align 3
.L144:
	leaq	24(%rsp), %rdx
	movl	$.LC64, %esi
	movq	%rbx, %rdi
	movq	$0, 24(%rsp)
	call	conf_get_string
	leaq	32(%rsp), %rdx
	movq	%rbx, %rdi
	movl	$.LC65, %esi
	call	conf_get_int
	movq	24(%rsp), %rdi
	testq	%rdi, %rdi
	je	.L142
	cmpb	$0, (%rdi)
	jne	.L139
.L143:
	call	g_free
	addq	$40, %rsp
	popq	%rbx
	popq	%rbp
	.p2align 4,,2
	.p2align 3
	ret
	.p2align 4,,10
	.p2align 3
.L139:
	movq	%rbp, %rdx
	movl	$.LC66, %esi
	movq	%rbx, %rdi
	movl	$0, 36(%rsp)
	call	conf_get_bool
	movl	36(%rsp), %ecx
	testl	%ecx, %ecx
	je	.L145
	leaq	16(%rsp), %rdx
	movl	$.LC68, %esi
	movq	%rbx, %rdi
	call	conf_get_string
	leaq	8(%rsp), %rdx
	movl	$.LC69, %esi
	movq	%rbx, %rdi
	call	conf_get_string
	movq	24(%rsp), %rcx
	movq	8(%rsp), %rdx
	movl	$.LC70, %edi
	movq	16(%rsp), %rsi
	movl	32(%rsp), %r8d
	xorl	%eax, %eax
	call	g_strdup_printf
	movq	16(%rsp), %rdi
	movq	%rax, %rbp
	call	g_free
	movq	8(%rsp), %rdi
	call	g_free
.L141:
	movq	%rbp, %rdx
	movl	$.LC71, %esi
	movl	$.LC2, %edi
	xorl	%eax, %eax
	call	debug_impl
	movq	%rbp, %rdi
	call	soup_uri_new
	movq	soup_connection(%rip), %rdi
	movq	%rax, %rbx
	movl	$80, %esi
	call	g_type_check_instance_cast
	xorl	%ecx, %ecx
	movq	%rax, %rdi
	movq	%rbx, %rdx
	movl	$.LC72, %esi
	xorl	%eax, %eax
	call	g_object_set
	movq	%rbx, %rdi
	call	soup_uri_free
	movq	24(%rsp), %rdi
	call	g_free
	movq	%rbp, %rdi
	jmp	.L143
	.p2align 4,,10
	.p2align 3
.L145:
	movl	32(%rsp), %edx
	movq	24(%rsp), %rsi
	movl	$.LC67, %edi
	xorl	%eax, %eax
	call	g_strdup_printf
	movq	%rax, %rbp
	jmp	.L141
.LFE45:
	.size	network_new, .-network_new
	.section	.rodata.str1.1
.LC73:
	.string	"Logout"
	.text
	.p2align 4,,15
.globl network_logout
	.type	network_logout, @function
network_logout:
.LFB49:
	subq	$8, %rsp
.LCFI39:
	call	network_new
	movq	global_username(%rip), %rdi
	testq	%rdi, %rdi
	je	.L147
	call	g_free
.L147:
	movq	global_password(%rip), %rdi
	testq	%rdi, %rdi
	je	.L148
	call	g_free
.L148:
	movq	current_timeline(%rip), %rdi
	testq	%rdi, %rdi
	je	.L149
	call	g_free
	movq	$0, current_timeline(%rip)
.L149:
	movl	$.LC73, %esi
	movl	$.LC2, %edi
	xorl	%eax, %eax
	addq	$8, %rsp
	jmp	debug_impl
.LFE49:
	.size	network_logout, .-network_logout
	.p2align 4,,15
.globl network_get_image
	.type	network_get_image, @function
network_get_image:
.LFB61:
	movq	%rbp, -40(%rsp)
.LCFI40:
	movq	%r12, -32(%rsp)
.LCFI41:
	movq	%rdi, %r12
	movq	%r13, -24(%rsp)
.LCFI42:
	movq	%r14, -16(%rsp)
.LCFI43:
	movq	%r15, -8(%rsp)
.LCFI44:
	movq	%rbx, -48(%rsp)
.LCFI45:
	subq	$56, %rsp
.LCFI46:
	movl	64(%rsp), %eax
	movq	72(%rsp), %r15
	movq	80(%rsp), %r14
	movq	88(%rsp), %r13
	movl	%eax, 4(%rsp)
	call	images_get_filename
	movl	$17, %esi
	movq	%rax, %rdi
	movq	%rax, %rbp
	call	g_file_test
	testl	%eax, %eax
	jne	.L154
	movl	$40, %edi
	call	g_malloc0
	movq	%rax, %rbx
	movq	%rbp, (%rax)
	movq	%r13, 32(%rax)
	movq	%r14, 24(%rax)
	movq	%r15, 16(%rax)
	movq	%r12, %rdx
	movl	4(%rsp), %eax
	movl	$.LC9, %esi
	movl	$.LC2, %edi
	movl	%eax, 8(%rbx)
	xorl	%eax, %eax
	call	debug_impl
	movq	%r12, %rsi
	movl	$.LC10, %edi
	call	soup_message_new
	movq	soup_connection(%rip), %rdi
	movq	%rbx, %rcx
	movq	16(%rsp), %rbp
	movq	8(%rsp), %rbx
	movq	24(%rsp), %r12
	movq	%rax, %rsi
	movq	32(%rsp), %r13
	movq	40(%rsp), %r14
	movl	$network_cb_on_image, %edx
	movq	48(%rsp), %r15
	addq	$56, %rsp
	jmp	soup_session_queue_message
	.p2align 4,,10
	.p2align 3
.L154:
	movl	4(%rsp), %eax
	movq	%r15, 72(%rsp)
	movq	%rbp, %rdi
	movq	%r14, 80(%rsp)
	movq	%r13, 88(%rsp)
	movq	8(%rsp), %rbx
	movq	16(%rsp), %rbp
	movl	%eax, 64(%rsp)
	movq	24(%rsp), %r12
	movq	32(%rsp), %r13
	movq	40(%rsp), %r14
	movq	48(%rsp), %r15
	addq	$56, %rsp
	jmp	app_set_image
.LFE61:
	.size	network_get_image, .-network_get_image
	.p2align 4,,15
.globl network_get_timeline
	.type	network_get_timeline, @function
network_get_timeline:
.LFB53:
	movq	%rbp, -8(%rsp)
.LCFI47:
	movq	%rbx, -16(%rsp)
.LCFI48:
	movq	%rdi, %rbp
	subq	$24, %rsp
.LCFI49:
	movl	processing(%rip), %edi
	testl	%edi, %edi
	je	.L158
	movq	8(%rsp), %rbx
	movq	16(%rsp), %rbp
	addq	$24, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L158:
	call	parser_reset_lastid
	movl	$5, %edx
	movl	$.LC8, %esi
	xorl	%edi, %edi
	call	dcgettext
	movq	%rax, %rdi
	call	app_set_statusbar_msg
	movq	%rbp, %rdi
	movl	$1, processing(%rip)
	call	g_strdup
	movq	%rbp, %rdx
	movq	%rax, %rbx
	movl	$.LC9, %esi
	movl	$.LC2, %edi
	xorl	%eax, %eax
	call	debug_impl
	movq	%rbp, %rsi
	movl	$.LC10, %edi
	call	soup_message_new
	movq	soup_connection(%rip), %rdi
	movq	%rbx, %rcx
	movq	16(%rsp), %rbp
	movq	8(%rsp), %rbx
	movq	%rax, %rsi
	movl	$network_cb_on_timeline, %edx
	addq	$24, %rsp
	jmp	soup_session_queue_message
.LFE53:
	.size	network_get_timeline, .-network_get_timeline
	.section	.rodata.str1.1
.LC74:
	.string	"Reloading timeline..."
.LC75:
	.string	"Auto reloading. Timeout: %i"
	.text
	.p2align 4,,15
	.type	network_timeout, @function
network_timeout:
.LFB77:
	cmpq	$0, current_timeline(%rip)
	pushq	%rbx
.LCFI50:
	je	.L160
	movl	processing(%rip), %r8d
	testl	%r8d, %r8d
	jne	.L160
	movl	$5, %edx
	movl	$.LC74, %esi
	xorl	%edi, %edi
	call	dcgettext
	movq	%rax, %rdi
	call	app_set_statusbar_msg
	movl	timeout_id(%rip), %edx
	movl	$.LC75, %esi
	movl	$.LC2, %edi
	xorl	%eax, %eax
	call	debug_impl
	movq	current_timeline(%rip), %rbx
	movl	$.LC9, %esi
	movl	$.LC2, %edi
	xorl	%eax, %eax
	movl	$1, processing(%rip)
	movq	%rbx, %rdx
	call	debug_impl
	movq	%rbx, %rsi
	movl	$.LC10, %edi
	call	soup_message_new
	movq	soup_connection(%rip), %rdi
	movq	%rax, %rsi
	xorl	%ecx, %ecx
	movl	$network_cb_on_timeline, %edx
	call	soup_session_queue_message
	movl	$0, timeout_id(%rip)
.L160:
	xorl	%eax, %eax
	popq	%rbx
	ret
.LFE77:
	.size	network_timeout, .-network_timeout
	.section	.rodata.str1.1
.LC76:
	.string	"Downloading Image: %s\nGet: %s"
	.text
	.p2align 4,,15
.globl network_download_avatar
	.type	network_download_avatar, @function
network_download_avatar:
.LFB59:
	pushq	%rbp
.LCFI51:
	pushq	%rbx
.LCFI52:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI53:
	call	images_get_filename
	movl	$17, %esi
	movq	%rax, %rdi
	movq	%rax, %rbp
	call	g_file_test
	testl	%eax, %eax
	movl	$1, %edx
	je	.L168
.L164:
	addq	$8, %rsp
	movl	%edx, %eax
	popq	%rbx
	popq	%rbp
	ret
	.p2align 4,,10
	.p2align 3
.L168:
	movq	%rbx, %rcx
	xorl	%eax, %eax
	movq	%rbp, %rdx
	movl	$.LC76, %esi
	movl	$.LC2, %edi
	call	debug_impl
	movq	%rbx, %rdx
	xorl	%eax, %eax
	movl	$.LC9, %esi
	movl	$.LC2, %edi
	call	debug_impl
	movq	%rbx, %rsi
	movl	$.LC10, %edi
	call	soup_message_new
	movq	soup_connection(%rip), %rdi
	movq	%rax, %rbx
	movq	%rax, %rsi
	call	soup_session_send_message
	movq	%rbx, %rdi
	call	network_check_http
	testl	%eax, %eax
	movl	$0, %eax
	movl	$.LC33, %esi
	cmove	%rax, %rbx
	movl	$.LC2, %edi
	movl	32(%rbx), %edx
	call	debug_impl
	movq	%rbx, %rdi
	call	network_check_http
	xorl	%edx, %edx
	testl	%eax, %eax
	je	.L164
	movq	64(%rbx), %rax
	xorl	%ecx, %ecx
	movq	%rbp, %rdi
	movq	8(%rax), %rdx
	movq	(%rax), %rsi
	call	g_file_set_contents
	xorl	%edx, %edx
	testl	%eax, %eax
	setne	%dl
	addq	$8, %rsp
	popq	%rbx
	movl	%edx, %eax
	popq	%rbp
	ret
.LFE59:
	.size	network_download_avatar, .-network_download_avatar
	.section	.rodata.str1.1
.LC77:
	.string	"greettweetknow"
.LC78:
	.string	"source=%s&status=%s"
	.section	.rodata.str1.8
	.align 8
.LC79:
	.string	"source=%s&in_reply_to_status_id=%lu&status=%s"
	.align 8
.LC80:
	.string	"https://twitter.com/statuses/update.xml"
	.align 8
.LC81:
	.string	"application/x-www-form-urlencoded"
	.text
	.p2align 4,,15
.globl network_post_status
	.type	network_post_status, @function
network_post_status:
.LFB50:
	pushq	%rbp
.LCFI54:
	pushq	%rbx
.LCFI55:
	subq	$8, %rsp
.LCFI56:
	movq	in_reply_to_status_id(%rip), %rdx
	testq	%rdx, %rdx
	je	.L174
	movq	%rdi, %rcx
	movl	$.LC77, %esi
	movl	$.LC79, %edi
	xorl	%eax, %eax
	call	g_strdup_printf
	movq	%rax, %rbp
.L171:
	movl	$.LC80, %edx
	xorl	%eax, %eax
	movl	$.LC1, %esi
	movl	$.LC2, %edi
	call	debug_impl
	movl	$.LC80, %esi
	movl	$.LC3, %edi
	call	soup_message_new
	movq	56(%rax), %rdi
	movq	%rax, %rbx
	movl	$.LC4, %edx
	movl	$.LC5, %esi
	call	soup_message_headers_append
	movq	56(%rbx), %rdi
	movl	$.LC6, %edx
	movl	$.LC7, %esi
	call	soup_message_headers_append
	testq	%rbp, %rbp
	je	.L172
	movq	%rbp, %rdi
	call	strlen
	movq	%rbp, %rcx
	movq	%rax, %r8
	movl	$1, %edx
	movl	$.LC81, %esi
	movq	%rbx, %rdi
	call	soup_message_set_request
.L172:
	movq	%rbx, %rsi
	movq	soup_connection(%rip), %rdi
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	xorl	%ecx, %ecx
	movl	$network_cb_on_post, %edx
	jmp	soup_session_queue_message
	.p2align 4,,10
	.p2align 3
.L174:
	movq	%rdi, %rdx
	movl	$.LC77, %esi
	movl	$.LC78, %edi
	xorl	%eax, %eax
	call	g_strdup_printf
	movq	%rax, %rbp
	jmp	.L171
.LFE50:
	.size	network_post_status, .-network_post_status
	.section	.rodata.str1.1
.LC82:
	.string	"source=%s&user=%s&text=%s"
	.section	.rodata.str1.8
	.align 8
.LC83:
	.string	"https://twitter.com/direct_messages/new.xml"
	.text
	.p2align 4,,15
.globl network_send_message
	.type	network_send_message, @function
network_send_message:
.LFB51:
	pushq	%rbp
.LCFI57:
	movq	%rsi, %rcx
	movq	%rdi, %rdx
	movl	$.LC77, %esi
	movl	$.LC82, %edi
	xorl	%eax, %eax
	pushq	%rbx
.LCFI58:
	subq	$8, %rsp
.LCFI59:
	call	g_strdup_printf
	movl	$.LC83, %edx
	movq	%rax, %rbp
	movl	$.LC1, %esi
	xorl	%eax, %eax
	movl	$.LC2, %edi
	call	debug_impl
	movl	$.LC83, %esi
	movl	$.LC3, %edi
	call	soup_message_new
	movq	56(%rax), %rdi
	movq	%rax, %rbx
	movl	$.LC4, %edx
	movl	$.LC5, %esi
	call	soup_message_headers_append
	movq	56(%rbx), %rdi
	movl	$.LC6, %edx
	movl	$.LC7, %esi
	call	soup_message_headers_append
	testq	%rbp, %rbp
	je	.L176
	movq	%rbp, %rdi
	call	strlen
	movq	%rbp, %rcx
	movq	%rax, %r8
	movl	$1, %edx
	movl	$.LC81, %esi
	movq	%rbx, %rdi
	call	soup_message_set_request
.L176:
	movq	%rbx, %rsi
	movq	soup_connection(%rip), %rdi
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	xorl	%ecx, %ecx
	movl	$network_cb_on_message, %edx
	jmp	soup_session_queue_message
.LFE51:
	.size	network_send_message, .-network_send_message
	.section	.rodata.str1.8
	.align 8
.LC84:
	.string	"/apps/greet-tweet-know/auth/user_id"
	.align 8
.LC85:
	.string	"https://twitter.com/statuses/user_timeline/%s.xml"
	.text
	.p2align 4,,15
.globl network_get_user
	.type	network_get_user, @function
network_get_user:
.LFB55:
	movq	%rbx, -16(%rsp)
.LCFI60:
	movq	%rbp, -8(%rsp)
.LCFI61:
	subq	$40, %rsp
.LCFI62:
	testq	%rdi, %rdi
	je	.L186
	call	g_strdup
	movq	%rax, 16(%rsp)
	movq	%rax, %rdi
.L180:
	testq	%rdi, %rdi
	je	.L185
	cmpb	$0, (%rdi)
	jne	.L187
.L182:
	call	g_free
.L185:
	movq	24(%rsp), %rbx
	movq	32(%rsp), %rbp
	addq	$40, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L187:
	movq	%rdi, %rsi
	xorl	%eax, %eax
	movl	$.LC85, %edi
	call	g_strdup_printf
	movl	processing(%rip), %r9d
	movq	%rax, %rbp
	testl	%r9d, %r9d
	je	.L188
.L183:
	testq	%rbp, %rbp
	je	.L184
	movq	%rbp, %rdi
	call	g_free
.L184:
	movq	16(%rsp), %rdi
	testq	%rdi, %rdi
	jne	.L182
	jmp	.L185
	.p2align 4,,10
	.p2align 3
.L188:
	.p2align 4,,6
	.p2align 3
	call	parser_reset_lastid
	movl	$5, %edx
	movl	$.LC8, %esi
	xorl	%edi, %edi
	call	dcgettext
	movq	%rax, %rdi
	call	app_set_statusbar_msg
	movq	%rbp, %rdi
	movl	$1, processing(%rip)
	call	g_strdup
	movq	%rbp, %rdx
	movq	%rax, %rbx
	movl	$.LC9, %esi
	xorl	%eax, %eax
	movl	$.LC2, %edi
	call	debug_impl
	movq	%rbp, %rsi
	movl	$.LC10, %edi
	call	soup_message_new
	movq	soup_connection(%rip), %rdi
	movq	%rax, %rsi
	movq	%rbx, %rcx
	movl	$network_cb_on_timeline, %edx
	call	soup_session_queue_message
	jmp	.L183
	.p2align 4,,10
	.p2align 3
.L186:
	call	conf_get
	leaq	16(%rsp), %rdx
	movq	%rax, %rdi
	movl	$.LC84, %esi
	call	conf_get_string
	movq	16(%rsp), %rdi
	jmp	.L180
.LFE55:
	.size	network_get_user, .-network_get_user
	.section	.rodata.str1.8
	.align 8
.LC86:
	.string	"https://twitter.com/friendships/create/%s.xml"
	.text
	.p2align 4,,15
.globl network_follow_user
	.type	network_follow_user, @function
network_follow_user:
.LFB62:
	movq	%rbx, -16(%rsp)
.LCFI63:
	movq	%r12, -8(%rsp)
.LCFI64:
	subq	$24, %rsp
.LCFI65:
	testq	%rdi, %rdi
	je	.L191
	cmpb	$0, (%rdi)
	jne	.L192
.L191:
	movq	8(%rsp), %rbx
	movq	16(%rsp), %r12
	addq	$24, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L192:
	movq	%rdi, %rsi
	xorl	%eax, %eax
	movl	$.LC86, %edi
	call	g_strdup_printf
	movl	$.LC1, %esi
	movq	%rax, %r12
	movq	%rax, %rdx
	movl	$.LC2, %edi
	xorl	%eax, %eax
	call	debug_impl
	movq	%r12, %rsi
	movl	$.LC3, %edi
	call	soup_message_new
	movq	56(%rax), %rdi
	movq	%rax, %rbx
	movl	$.LC4, %edx
	movl	$.LC5, %esi
	call	soup_message_headers_append
	movq	56(%rbx), %rdi
	movl	$.LC6, %edx
	movl	$.LC7, %esi
	call	soup_message_headers_append
	movq	soup_connection(%rip), %rdi
	movq	%rbx, %rsi
	xorl	%ecx, %ecx
	movl	$network_cb_on_add, %edx
	call	soup_session_queue_message
	movq	%r12, %rdi
	movq	8(%rsp), %rbx
	movq	16(%rsp), %r12
	addq	$24, %rsp
	jmp	g_free
.LFE62:
	.size	network_follow_user, .-network_follow_user
	.section	.rodata.str1.8
	.align 8
.LC87:
	.string	"https://twitter.com/blocks/create/%s.xml"
	.text
	.p2align 4,,15
.globl network_block_user
	.type	network_block_user, @function
network_block_user:
.LFB64:
	movq	%rbx, -16(%rsp)
.LCFI66:
	movq	%r12, -8(%rsp)
.LCFI67:
	subq	$24, %rsp
.LCFI68:
	testq	%rdi, %rdi
	je	.L195
	cmpb	$0, (%rdi)
	jne	.L196
.L195:
	movq	8(%rsp), %rbx
	movq	16(%rsp), %r12
	addq	$24, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L196:
	movq	%rdi, %rsi
	xorl	%eax, %eax
	movl	$.LC87, %edi
	call	g_strdup_printf
	movl	$.LC1, %esi
	movq	%rax, %r12
	movq	%rax, %rdx
	movl	$.LC2, %edi
	xorl	%eax, %eax
	call	debug_impl
	movq	%r12, %rsi
	movl	$.LC3, %edi
	call	soup_message_new
	movq	56(%rax), %rdi
	movq	%rax, %rbx
	movl	$.LC4, %edx
	movl	$.LC5, %esi
	call	soup_message_headers_append
	movq	56(%rbx), %rdi
	movl	$.LC6, %edx
	movl	$.LC7, %esi
	call	soup_message_headers_append
	movq	soup_connection(%rip), %rdi
	movq	%rbx, %rsi
	xorl	%ecx, %ecx
	movl	$network_cb_on_del, %edx
	call	soup_session_queue_message
	movq	%r12, %rdi
	movq	8(%rsp), %rbx
	movq	16(%rsp), %r12
	addq	$24, %rsp
	jmp	g_free
.LFE64:
	.size	network_block_user, .-network_block_user
.globl getting_followers
	.bss
	.align 4
	.type	getting_followers, @object
	.size	getting_followers, 4
getting_followers:
	.zero	4
	.local	soup_connection
	.comm	soup_connection,8,8
	.local	request_timer
	.comm	request_timer,8,8
	.local	current_timeline
	.comm	current_timeline,8,8
	.local	processing
	.comm	processing,4,4
	.local	all_users
	.comm	all_users,8,8
	.local	global_username
	.comm	global_username,8,8
	.local	global_password
	.comm	global_password,8,8
	.local	timeout_id
	.comm	timeout_id,4,4
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC13:
	.long	0
	.long	1078067200
	.align 8
.LC14:
	.long	0
	.long	1077411840
	.align 8
.LC15:
	.long	0
	.long	1076363264
	.align 8
.LC16:
	.long	0
	.long	1075314688
	.align 8
.LC17:
	.long	0
	.long	1072693248
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
	.long	.LFB63
	.long	.LFE63-.LFB63
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI2-.LFB63
	.byte	0xe
	.uleb128 0x20
	.byte	0x8c
	.uleb128 0x2
	.byte	0x83
	.uleb128 0x3
	.align 8
.LEFDE1:
.LSFDE3:
	.long	.LEFDE3-.LASFDE3
.LASFDE3:
	.long	.LASFDE3-.Lframe1
	.long	.LFB52
	.long	.LFE52-.LFB52
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI3-.LFB52
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
	.long	.LFB67
	.long	.LFE67-.LFB67
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI4-.LFB67
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
	.uleb128 0x40
	.byte	0x83
	.uleb128 0x3
	.align 8
.LEFDE5:
.LSFDE7:
	.long	.LEFDE7-.LASFDE7
.LASFDE7:
	.long	.LASFDE7-.Lframe1
	.long	.LFB75
	.long	.LFE75-.LFB75
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI7-.LFB75
	.byte	0xe
	.uleb128 0x10
	.byte	0x86
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI8-.LCFI7
	.byte	0xe
	.uleb128 0x18
	.byte	0x83
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI9-.LCFI8
	.byte	0xe
	.uleb128 0x20
	.align 8
.LEFDE7:
.LSFDE9:
	.long	.LEFDE9-.LASFDE9
.LASFDE9:
	.long	.LASFDE9-.Lframe1
	.long	.LFB71
	.long	.LFE71-.LFB71
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI10-.LFB71
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE9:
.LSFDE11:
	.long	.LEFDE11-.LASFDE11
.LASFDE11:
	.long	.LASFDE11-.Lframe1
	.long	.LFB70
	.long	.LFE70-.LFB70
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI11-.LFB70
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
	.long	.LFB74
	.long	.LFE74-.LFB74
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI12-.LFB74
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
	.long	.LFB73
	.long	.LFE73-.LFB73
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI13-.LFB73
	.byte	0xe
	.uleb128 0x10
	.byte	0x86
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI14-.LCFI13
	.byte	0xe
	.uleb128 0x18
	.byte	0x83
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI15-.LCFI14
	.byte	0xe
	.uleb128 0x40
	.align 8
.LEFDE15:
.LSFDE17:
	.long	.LEFDE17-.LASFDE17
.LASFDE17:
	.long	.LASFDE17-.Lframe1
	.long	.LFB54
	.long	.LFE54-.LFB54
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI18-.LFB54
	.byte	0xe
	.uleb128 0x20
	.byte	0x8c
	.uleb128 0x2
	.byte	0x83
	.uleb128 0x3
	.align 8
.LEFDE17:
.LSFDE19:
	.long	.LEFDE19-.LASFDE19
.LASFDE19:
	.long	.LASFDE19-.Lframe1
	.long	.LFB72
	.long	.LFE72-.LFB72
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI19-.LFB72
	.byte	0xe
	.uleb128 0x10
	.byte	0x86
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI20-.LCFI19
	.byte	0xe
	.uleb128 0x18
	.byte	0x83
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI21-.LCFI20
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE19:
.LSFDE21:
	.long	.LEFDE21-.LASFDE21
.LASFDE21:
	.long	.LASFDE21-.Lframe1
	.long	.LFB69
	.long	.LFE69-.LFB69
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI22-.LFB69
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE21:
.LSFDE23:
	.long	.LEFDE23-.LASFDE23
.LASFDE23:
	.long	.LASFDE23-.Lframe1
	.long	.LFB68
	.long	.LFE68-.LFB68
	.uleb128 0x0
	.align 8
.LEFDE23:
.LSFDE25:
	.long	.LEFDE25-.LASFDE25
.LASFDE25:
	.long	.LASFDE25-.Lframe1
	.long	.LFB47
	.long	.LFE47-.LFB47
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI23-.LFB47
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE25:
.LSFDE27:
	.long	.LEFDE27-.LASFDE27
.LASFDE27:
	.long	.LASFDE27-.Lframe1
	.long	.LFB46
	.long	.LFE46-.LFB46
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI24-.LFB46
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE27:
.LSFDE29:
	.long	.LEFDE29-.LASFDE29
.LASFDE29:
	.long	.LASFDE29-.Lframe1
	.long	.LFB56
	.long	.LFE56-.LFB56
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI25-.LFB56
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI26-.LCFI25
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI27-.LCFI26
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI28-.LCFI27
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.long	.LCFI29-.LCFI28
	.byte	0xe
	.uleb128 0x30
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x8d
	.uleb128 0x2
	.align 8
.LEFDE29:
.LSFDE31:
	.long	.LEFDE31-.LASFDE31
.LASFDE31:
	.long	.LASFDE31-.Lframe1
	.long	.LFB57
	.long	.LFE57-.LFB57
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI30-.LFB57
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI31-.LCFI30
	.byte	0xe
	.uleb128 0x18
	.byte	0x83
	.uleb128 0x3
	.byte	0x86
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI32-.LCFI31
	.byte	0xe
	.uleb128 0x40
	.align 8
.LEFDE31:
.LSFDE33:
	.long	.LEFDE33-.LASFDE33
.LASFDE33:
	.long	.LASFDE33-.Lframe1
	.long	.LFB48
	.long	.LFE48-.LFB48
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI33-.LFB48
	.byte	0xe
	.uleb128 0x10
	.byte	0x86
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI34-.LCFI33
	.byte	0xe
	.uleb128 0x18
	.byte	0x83
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI35-.LCFI34
	.byte	0xe
	.uleb128 0x20
	.align 8
.LEFDE33:
.LSFDE35:
	.long	.LEFDE35-.LASFDE35
.LASFDE35:
	.long	.LASFDE35-.Lframe1
	.long	.LFB45
	.long	.LFE45-.LFB45
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI36-.LFB45
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI37-.LCFI36
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI38-.LCFI37
	.byte	0xe
	.uleb128 0x40
	.byte	0x83
	.uleb128 0x3
	.byte	0x86
	.uleb128 0x2
	.align 8
.LEFDE35:
.LSFDE37:
	.long	.LEFDE37-.LASFDE37
.LASFDE37:
	.long	.LASFDE37-.Lframe1
	.long	.LFB49
	.long	.LFE49-.LFB49
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI39-.LFB49
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE37:
.LSFDE39:
	.long	.LEFDE39-.LASFDE39
.LASFDE39:
	.long	.LASFDE39-.Lframe1
	.long	.LFB61
	.long	.LFE61-.LFB61
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI41-.LFB61
	.byte	0x8c
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x6
	.byte	0x4
	.long	.LCFI46-.LCFI41
	.byte	0xe
	.uleb128 0x40
	.byte	0x83
	.uleb128 0x7
	.byte	0x8f
	.uleb128 0x2
	.byte	0x8e
	.uleb128 0x3
	.byte	0x8d
	.uleb128 0x4
	.align 8
.LEFDE39:
.LSFDE41:
	.long	.LEFDE41-.LASFDE41
.LASFDE41:
	.long	.LASFDE41-.Lframe1
	.long	.LFB53
	.long	.LFE53-.LFB53
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI48-.LFB53
	.byte	0x83
	.uleb128 0x3
	.byte	0x86
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI49-.LCFI48
	.byte	0xe
	.uleb128 0x20
	.align 8
.LEFDE41:
.LSFDE43:
	.long	.LEFDE43-.LASFDE43
.LASFDE43:
	.long	.LASFDE43-.Lframe1
	.long	.LFB77
	.long	.LFE77-.LFB77
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI50-.LFB77
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE43:
.LSFDE45:
	.long	.LEFDE45-.LASFDE45
.LASFDE45:
	.long	.LASFDE45-.Lframe1
	.long	.LFB59
	.long	.LFE59-.LFB59
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI51-.LFB59
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI52-.LCFI51
	.byte	0xe
	.uleb128 0x18
	.byte	0x83
	.uleb128 0x3
	.byte	0x86
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI53-.LCFI52
	.byte	0xe
	.uleb128 0x20
	.align 8
.LEFDE45:
.LSFDE47:
	.long	.LEFDE47-.LASFDE47
.LASFDE47:
	.long	.LASFDE47-.Lframe1
	.long	.LFB50
	.long	.LFE50-.LFB50
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI54-.LFB50
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI55-.LCFI54
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI56-.LCFI55
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x3
	.byte	0x86
	.uleb128 0x2
	.align 8
.LEFDE47:
.LSFDE49:
	.long	.LEFDE49-.LASFDE49
.LASFDE49:
	.long	.LASFDE49-.Lframe1
	.long	.LFB51
	.long	.LFE51-.LFB51
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI57-.LFB51
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI58-.LCFI57
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI59-.LCFI58
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x3
	.byte	0x86
	.uleb128 0x2
	.align 8
.LEFDE49:
.LSFDE51:
	.long	.LEFDE51-.LASFDE51
.LASFDE51:
	.long	.LASFDE51-.Lframe1
	.long	.LFB55
	.long	.LFE55-.LFB55
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI62-.LFB55
	.byte	0xe
	.uleb128 0x30
	.byte	0x86
	.uleb128 0x2
	.byte	0x83
	.uleb128 0x3
	.align 8
.LEFDE51:
.LSFDE53:
	.long	.LEFDE53-.LASFDE53
.LASFDE53:
	.long	.LASFDE53-.Lframe1
	.long	.LFB62
	.long	.LFE62-.LFB62
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI65-.LFB62
	.byte	0xe
	.uleb128 0x20
	.byte	0x8c
	.uleb128 0x2
	.byte	0x83
	.uleb128 0x3
	.align 8
.LEFDE53:
.LSFDE55:
	.long	.LEFDE55-.LASFDE55
.LASFDE55:
	.long	.LASFDE55-.Lframe1
	.long	.LFB64
	.long	.LFE64-.LFB64
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI68-.LFB64
	.byte	0xe
	.uleb128 0x20
	.byte	0x8c
	.uleb128 0x2
	.byte	0x83
	.uleb128 0x3
	.align 8
.LEFDE55:
	.ident	"GCC: (SUSE Linux) 4.3.2 [gcc-4_3-branch revision 141291]"
	.section	.comment.SUSE.OPTs,"MS",@progbits,1
	.ascii	"OspWg"
	.section	.note.GNU-stack,"",@progbits
