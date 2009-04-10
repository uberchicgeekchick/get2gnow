	.file	"friends-manager.c"
	.text
	.p2align 4,,15
	.type	friends_manager_free, @function
friends_manager_free:
.LFB46:
	movq	friends_manager(%rip), %rax
	pushq	%rbx
.LCFI0:
	testq	%rax, %rax
	je	.L9
	movq	24(%rax), %rbx
	testq	%rbx, %rbx
	je	.L3
	call	gtk_widget_get_type
	movq	%rbx, %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_widget_destroy
	movq	friends_manager(%rip), %rax
	movq	$0, 24(%rax)
.L3:
	movq	32(%rax), %rbx
	testq	%rbx, %rbx
	je	.L4
	call	gtk_widget_get_type
	movq	%rbx, %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_widget_destroy
	movq	friends_manager(%rip), %rax
	movq	$0, 32(%rax)
.L4:
	movq	40(%rax), %rbx
	testq	%rbx, %rbx
	je	.L5
	call	gtk_widget_get_type
	movq	%rbx, %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_widget_destroy
	movq	friends_manager(%rip), %rax
	movq	$0, 40(%rax)
.L5:
	movq	48(%rax), %rbx
	testq	%rbx, %rbx
	je	.L6
	call	gtk_widget_get_type
	movq	%rbx, %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_widget_destroy
	movq	friends_manager(%rip), %rax
	movq	$0, 48(%rax)
.L6:
	movq	56(%rax), %rbx
	testq	%rbx, %rbx
	je	.L7
	call	gtk_widget_get_type
	movq	%rbx, %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_widget_destroy
	movq	friends_manager(%rip), %rax
	movq	$0, 56(%rax)
.L7:
	movq	(%rax), %rbx
	testq	%rbx, %rbx
	je	.L8
	call	gtk_widget_get_type
	movq	%rbx, %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_widget_destroy
	movq	friends_manager(%rip), %rax
	movq	$0, (%rax)
.L8:
	movq	friends_manager(%rip), %rdi
	call	g_free
	movq	$0, friends_manager(%rip)
.L9:
	popq	%rbx
	ret
.LFE46:
	.size	friends_manager_free, .-friends_manager_free
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"Destroying friends manager"
.LC1:
	.string	"Friends-Manager"
	.text
	.p2align 4,,15
	.type	friends_manager_destroy, @function
friends_manager_destroy:
.LFB44:
	subq	$8, %rsp
.LCFI1:
	movl	$.LC0, %esi
	movl	$.LC1, %edi
	xorl	%eax, %eax
	call	debug_impl
	addq	$8, %rsp
	jmp	friends_manager_free
.LFE44:
	.size	friends_manager_destroy, .-friends_manager_destroy
	.p2align 4,,15
	.type	friends_manager_friend_selected, @function
friends_manager_friend_selected:
.LFB52:
	movq	%rbx, -32(%rsp)
.LCFI2:
	movq	%rbp, -24(%rsp)
.LCFI3:
	movq	%rcx, %rbp
	movq	%r13, -8(%rsp)
.LCFI4:
	movq	%r12, -16(%rsp)
.LCFI5:
	subq	$88, %rsp
.LCFI6:
	movq	%rsi, %r13
	movq	$0, 40(%rsp)
	call	gtk_tree_model_get_type
	movq	16(%rbp), %rdi
	movq	%rax, %rsi
	movq	%rax, %rbx
	call	g_type_check_instance_cast
	movq	%r13, %rdx
	movq	%rax, %rdi
	movq	%rsp, %rsi
	call	gtk_tree_model_get_iter
	movq	16(%rbp), %rdi
	movq	%rbx, %rsi
	call	g_type_check_instance_cast
	leaq	40(%rsp), %rcx
	movq	%rax, %rdi
	xorl	%edx, %edx
	xorl	%eax, %eax
	movl	$-1, %r8d
	movq	%rsp, %rsi
	call	gtk_tree_model_get
	cmpq	$0, 40(%rsp)
	je	.L15
	call	gtk_window_get_type
	movq	(%rbp), %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	movq	40(%rsp), %rdi
	movq	%rax, %rsi
	call	view_profile
	movq	40(%rsp), %rdi
	call	g_free
.L15:
	movq	56(%rsp), %rbx
	movq	64(%rsp), %rbp
	movq	72(%rsp), %r12
	movq	80(%rsp), %r13
	addq	$88, %rsp
	ret
.LFE52:
	.size	friends_manager_friend_selected, .-friends_manager_friend_selected
	.p2align 4,,15
	.type	friends_manager_view_profile, @function
friends_manager_view_profile:
.LFB50:
	pushq	%rbp
.LCFI7:
	movq	%rsi, %rbp
	pushq	%rbx
.LCFI8:
	subq	$56, %rsp
.LCFI9:
	movq	8(%rsi), %rdi
	movq	$0, 40(%rsp)
	call	gtk_tree_view_get_selection
	movq	%rsp, %rdx
	xorl	%esi, %esi
	movq	%rax, %rdi
	call	gtk_tree_selection_get_selected
	call	gtk_tree_model_get_type
	movq	16(%rbp), %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	leaq	40(%rsp), %rcx
	movq	%rax, %rdi
	xorl	%edx, %edx
	xorl	%eax, %eax
	movl	$-1, %r8d
	movq	%rsp, %rsi
	call	gtk_tree_model_get
	cmpq	$0, 40(%rsp)
	je	.L18
	call	gtk_window_get_type
	movq	(%rbp), %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	movq	40(%rsp), %rdi
	movq	%rax, %rsi
	call	view_profile
	movq	40(%rsp), %rdi
	call	g_free
.L18:
	addq	$56, %rsp
	popq	%rbx
	popq	%rbp
	ret
.LFE50:
	.size	friends_manager_view_profile, .-friends_manager_view_profile
	.p2align 4,,15
	.type	friends_manager_view_timeline, @function
friends_manager_view_timeline:
.LFB51:
	pushq	%r12
.LCFI10:
	pushq	%rbx
.LCFI11:
	movq	%rsi, %rbx
	subq	$56, %rsp
.LCFI12:
	movq	8(%rsi), %rdi
	call	gtk_tree_view_get_selection
	movq	%rsp, %rdx
	xorl	%esi, %esi
	movq	%rax, %rdi
	call	gtk_tree_selection_get_selected
	call	gtk_tree_model_get_type
	movq	16(%rbx), %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	leaq	40(%rsp), %rcx
	movq	%rax, %rdi
	xorl	%edx, %edx
	xorl	%eax, %eax
	movl	$-1, %r8d
	movq	%rsp, %rsi
	call	gtk_tree_model_get
	movq	40(%rsp), %rdi
	testq	%rdi, %rdi
	je	.L21
	call	network_get_user
	movq	40(%rsp), %rdi
	call	g_free
.L21:
	addq	$56, %rsp
	popq	%rbx
	popq	%r12
	ret
.LFE51:
	.size	friends_manager_view_timeline, .-friends_manager_view_timeline
	.section	.rodata.str1.1
.LC2:
	.string	"Yes"
.LC3:
	.string	"No"
	.text
	.p2align 4,,15
	.type	friends_manager_unfollow, @function
friends_manager_unfollow:
.LFB48:
	pushq	%rbp
.LCFI13:
	pushq	%rbx
.LCFI14:
	movq	%rsi, %rbx
	subq	$72, %rsp
.LCFI15:
	movq	8(%rsi), %rdi
	leaq	16(%rsp), %rbp
	movq	$0, 56(%rsp)
	movq	$0, 48(%rsp)
	call	gtk_tree_view_get_selection
	xorl	%esi, %esi
	movq	%rax, %rdi
	movq	%rbp, %rdx
	call	gtk_tree_selection_get_selected
	testl	%eax, %eax
	jne	.L28
.L27:
	addq	$72, %rsp
	popq	%rbx
	popq	%rbp
	ret
	.p2align 4,,10
	.p2align 3
.L28:
	movq	16(%rbx), %rdi
	leaq	48(%rsp), %rcx
	leaq	56(%rsp), %r9
	xorl	%r8d, %r8d
	xorl	%eax, %eax
	movl	$2, %edx
	movq	%rbp, %rsi
	movl	$-1, (%rsp)
	call	gtk_tree_model_get
	cmpq	$0, 56(%rsp)
	je	.L29
	movq	48(%rsp), %rdi
	movl	$.LC2, %esi
	call	g_str_equal
	testl	%eax, %eax
	jne	.L30
	call	gtk_list_store_get_type
	movq	16(%rbx), %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	movq	%rbp, %rsi
	movq	%rax, %rdi
	call	gtk_list_store_remove
	movq	24(%rbx), %rdi
	xorl	%esi, %esi
	call	gtk_widget_set_sensitive
	movq	32(%rbx), %rdi
	xorl	%esi, %esi
	call	gtk_widget_set_sensitive
	movq	40(%rbx), %rdi
	xorl	%esi, %esi
	call	gtk_widget_set_sensitive
	movq	48(%rbx), %rdi
	xorl	%esi, %esi
	call	gtk_widget_set_sensitive
	movq	56(%rbx), %rdi
	xorl	%esi, %esi
	call	gtk_widget_set_sensitive
.L26:
	movq	56(%rsp), %rdi
	call	network_unfollow_user
	movq	48(%rsp), %rdi
	call	g_free
	movq	56(%rsp), %rdi
	call	g_free
	addq	$72, %rsp
	popq	%rbx
	popq	%rbp
	ret
	.p2align 4,,10
	.p2align 3
.L30:
	call	gtk_list_store_get_type
	movq	16(%rbx), %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	movl	$-1, %r8d
	movq	%rax, %rdi
	movl	$.LC3, %ecx
	movl	$2, %edx
	movq	%rbp, %rsi
	xorl	%eax, %eax
	call	gtk_list_store_set
	jmp	.L26
	.p2align 4,,10
	.p2align 3
.L29:
	movq	48(%rsp), %rdi
	call	g_free
	.p2align 4,,5
	.p2align 3
	jmp	.L27
.LFE48:
	.size	friends_manager_unfollow, .-friends_manager_unfollow
	.p2align 4,,15
	.type	friends_manager_follow, @function
friends_manager_follow:
.LFB47:
	pushq	%rbp
.LCFI16:
	pushq	%rbx
.LCFI17:
	movq	%rsi, %rbx
	subq	$72, %rsp
.LCFI18:
	movq	8(%rsi), %rdi
	leaq	16(%rsp), %rbp
	call	gtk_tree_view_get_selection
	xorl	%esi, %esi
	movq	%rax, %rdi
	movq	%rbp, %rdx
	call	gtk_tree_selection_get_selected
	testl	%eax, %eax
	jne	.L35
.L34:
	addq	$72, %rsp
	popq	%rbx
	popq	%rbp
	ret
	.p2align 4,,10
	.p2align 3
.L35:
	movq	16(%rbx), %rdi
	leaq	56(%rsp), %rcx
	leaq	48(%rsp), %r9
	xorl	%r8d, %r8d
	xorl	%eax, %eax
	movl	$2, %edx
	movq	%rbp, %rsi
	movl	$-1, (%rsp)
	call	gtk_tree_model_get
	movq	56(%rsp), %rdi
	movl	$.LC3, %esi
	call	g_str_equal
	testl	%eax, %eax
	jne	.L36
.L33:
	movq	48(%rsp), %rdi
	testq	%rdi, %rdi
	je	.L34
	call	network_follow_user
	movq	56(%rsp), %rdi
	call	g_free
	movq	48(%rsp), %rdi
	call	g_free
	addq	$72, %rsp
	popq	%rbx
	popq	%rbp
	ret
	.p2align 4,,10
	.p2align 3
.L36:
	call	gtk_list_store_get_type
	movq	16(%rbx), %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	movl	$-1, %r8d
	movq	%rax, %rdi
	movl	$.LC2, %ecx
	movl	$2, %edx
	movq	%rbp, %rsi
	xorl	%eax, %eax
	call	gtk_list_store_set
	jmp	.L33
.LFE47:
	.size	friends_manager_follow, .-friends_manager_follow
	.p2align 4,,15
	.type	friends_manager_response, @function
friends_manager_response:
.LFB45:
	jmp	gtk_widget_hide
.LFE45:
	.size	friends_manager_response, .-friends_manager_response
	.section	.rodata.str1.1
.LC4:
	.string	"Initialising friends manager"
.LC5:
	.string	"user_follow"
.LC6:
	.string	"friends_and_followers"
.LC7:
	.string	"friends_manager"
.LC8:
	.string	"friends-manager.ui"
.LC9:
	.string	"view_timeline"
.LC10:
	.string	"view_profile"
.LC11:
	.string	"user_block"
.LC12:
	.string	"user_unfollow"
.LC13:
	.string	"destroy"
.LC14:
	.string	"cursor-changed"
.LC15:
	.string	"row-activated"
.LC16:
	.string	"clicked"
.LC17:
	.string	"response"
.LC18:
	.string	"close"
.LC19:
	.string	"destroy-event"
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC20:
	.string	"Please wait while friends and followers are loaded.  This may take several minutes..."
	.section	.rodata.str1.1
.LC21:
	.string	""
	.text
	.p2align 4,,15
.globl friends_manager_show
	.type	friends_manager_show, @function
friends_manager_show:
.LFB56:
	pushq	%r15
.LCFI19:
	pushq	%r14
.LCFI20:
	pushq	%r13
.LCFI21:
	pushq	%r12
.LCFI22:
	pushq	%rbp
.LCFI23:
	movq	%rdi, %rbp
	pushq	%rbx
.LCFI24:
	subq	$296, %rsp
.LCFI25:
	movq	friends_manager(%rip), %rax
	testq	%rax, %rax
	je	.L40
	movq	(%rax), %rbx
	testq	%rbx, %rbx
	je	.L40
	call	gtk_window_get_type
	movq	%rbx, %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_window_present
.L53:
	addq	$296, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	ret
	.p2align 4,,10
	.p2align 3
.L40:
	xorl	%eax, %eax
	movl	$.LC4, %esi
	movl	$.LC1, %edi
	call	debug_impl
	cmpq	$0, friends_manager(%rip)
	je	.L42
	call	friends_manager_free
.L42:
	movl	$64, %edi
	call	g_malloc0
	leaq	56(%rax), %rdx
	leaq	8(%rax), %r8
	movl	$.LC5, %r9d
	movl	$.LC6, %ecx
	movl	$.LC7, %esi
	movq	%rax, friends_manager(%rip)
	movq	%rdx, 64(%rsp)
	leaq	48(%rax), %rdx
	movl	$.LC8, %edi
	movq	$0, 72(%rsp)
	movq	$.LC9, 56(%rsp)
	movq	%rdx, 48(%rsp)
	leaq	40(%rax), %rdx
	movq	$.LC10, 40(%rsp)
	movq	$.LC11, 24(%rsp)
	movq	$.LC12, 8(%rsp)
	movq	%rdx, 32(%rsp)
	leaq	32(%rax), %rdx
	movq	%rdx, 16(%rsp)
	leaq	24(%rax), %rdx
	movq	%rdx, (%rsp)
	movq	%rax, %rdx
	xorl	%eax, %eax
	call	gtkbuilder_get_file
	movq	friends_manager(%rip), %rbx
	movq	%rax, %r12
	movq	8(%rbx), %rdi
	call	gtk_tree_view_get_model
	movq	friends_manager(%rip), %rsi
	movl	$.LC7, %r9d
	movq	%rax, 16(%rbx)
	movq	%r9, %rdx
	movl	$friends_manager_destroy, %r8d
	movl	$.LC13, %ecx
	xorl	%eax, %eax
	movq	%r12, %rdi
	movq	$0, 232(%rsp)
	movq	$friends_manager_list_clicked, 224(%rsp)
	movq	$.LC14, 216(%rsp)
	movq	$.LC6, 208(%rsp)
	movq	$friends_manager_friend_selected, 200(%rsp)
	movq	$.LC15, 192(%rsp)
	movq	$.LC6, 184(%rsp)
	movq	$friends_manager_view_timeline, 176(%rsp)
	movq	$.LC16, 168(%rsp)
	movq	$.LC9, 160(%rsp)
	movq	$friends_manager_view_profile, 152(%rsp)
	movq	$.LC16, 144(%rsp)
	movq	$.LC10, 136(%rsp)
	movq	$friends_manager_block, 128(%rsp)
	movq	$.LC16, 120(%rsp)
	movq	$.LC11, 112(%rsp)
	movq	$friends_manager_unfollow, 104(%rsp)
	movq	$.LC16, 96(%rsp)
	movq	$.LC12, 88(%rsp)
	movq	$friends_manager_follow, 80(%rsp)
	movq	$.LC16, 72(%rsp)
	movq	$.LC5, 64(%rsp)
	movq	$friends_manager_response, 56(%rsp)
	movq	$.LC17, 48(%rsp)
	movq	$.LC7, 40(%rsp)
	movq	$friends_manager_response, 32(%rsp)
	movq	$.LC18, 24(%rsp)
	movq	$.LC7, 16(%rsp)
	movq	$friends_manager_destroy, 8(%rsp)
	movq	$.LC19, (%rsp)
	call	gtkbuilder_connect
	movq	%r12, %rdi
	call	g_object_unref
	movq	friends_manager(%rip), %rax
	movl	$80, %esi
	movq	(%rax), %rdi
	call	g_type_check_instance_cast
	movl	$friends_manager, %esi
	movq	%rax, %rdi
	call	g_object_add_weak_pointer
	call	gtk_window_get_type
	movq	%rax, %rsi
	movq	friends_manager(%rip), %rax
	movq	(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%rbp, %rsi
	movq	%rax, %rdi
	call	gtk_window_set_transient_for
	movq	friends_manager(%rip), %rax
	movq	(%rax), %rdi
	call	gtk_widget_show
	movl	$5, %edx
	movl	$.LC20, %esi
	xorl	%edi, %edi
	call	dcgettext
	movq	%rax, %rdi
	call	app_set_statusbar_msg
	movl	$150, %edi
	call	gdk_cursor_new
	movq	%rax, %rbx
	call	gtk_widget_get_type
	movq	%rax, %rsi
	movq	%rax, 240(%rsp)
	movq	friends_manager(%rip), %rax
	movq	(%rax), %rdi
	call	g_type_check_instance_cast
	movq	80(%rax), %rdi
	movq	%rbx, %rsi
	call	gdk_window_set_cursor
	movq	friends_manager(%rip), %rax
	xorl	%esi, %esi
	movq	(%rax), %rdi
	call	gtk_widget_set_sensitive
	movl	$1, %edi
	call	user_get_friends_and_followers
	testq	%rax, %rax
	movq	%rax, %rbx
	je	.L43
	call	gtk_list_store_get_type
	movq	%rax, 248(%rsp)
	jmp	.L51
	.p2align 4,,10
	.p2align 3
.L56:
	movq	16(%r13), %rbx
.L55:
	movq	friends_manager(%rip), %rax
	movl	88(%rbp), %r12d
	xorl	%r15d, %r15d
	movq	248(%rsp), %rsi
	movl	$.LC3, %r14d
	movq	%rbx, %r13
	movq	16(%rax), %rdi
	testl	%r12d, %r12d
	sete	%r15b
	call	g_type_check_instance_cast
	leaq	256(%rsp), %rsi
	movq	%rax, %rdi
	call	gtk_list_store_append
	testl	%r12d, %r12d
	movl	$.LC2, %eax
	cmovne	%rax, %r14
	testl	%r15d, %r15d
	movl	$.LC3, %r15d
	cmovne	%rax, %r15
.L50:
	movq	friends_manager(%rip), %rax
	movq	248(%rsp), %rsi
	movq	16(%rbp), %rbx
	movq	8(%rbp), %r12
	movq	16(%rax), %rdi
	call	g_type_check_instance_cast
	leaq	256(%rsp), %rsi
	movq	%rax, %rdi
	movq	%rbx, %r9
	xorl	%edx, %edx
	xorl	%eax, %eax
	movl	$1, %r8d
	movq	%r12, %rcx
	movl	$-1, 48(%rsp)
	movq	%rbp, 40(%rsp)
	movl	$4, 32(%rsp)
	movq	%r14, 24(%rsp)
	movl	$3, 16(%rsp)
	movq	%r15, 8(%rsp)
	movl	$2, (%rsp)
	call	gtk_list_store_set
	movq	8(%r13), %rbx
	testq	%rbx, %rbx
	je	.L43
.L51:
	movq	8(%rbx), %r13
	movq	(%rbx), %rbp
	testq	%r13, %r13
	je	.L55
	movq	(%r13), %rax
	movq	8(%rbp), %rdi
	movq	8(%rax), %rsi
	call	g_str_equal
	testl	%eax, %eax
	je	.L56
	movq	friends_manager(%rip), %rax
	movq	248(%rsp), %rsi
	movl	$.LC2, %r15d
	movq	%r15, %r14
	movq	16(%rax), %rdi
	call	g_type_check_instance_cast
	leaq	256(%rsp), %rsi
	movq	%rax, %rdi
	call	gtk_list_store_append
	jmp	.L50
	.p2align 4,,10
	.p2align 3
.L43:
	movq	friends_manager(%rip), %rax
	movq	240(%rsp), %rsi
	movq	(%rax), %rdi
	call	g_type_check_instance_cast
	movq	80(%rax), %rdi
	xorl	%esi, %esi
	call	gdk_window_set_cursor
	movq	friends_manager(%rip), %rax
	movl	$1, %esi
	movq	(%rax), %rdi
	call	gtk_widget_set_sensitive
	movq	friends_manager(%rip), %rbx
	xorl	%esi, %esi
	movq	24(%rbx), %rdi
	call	gtk_widget_set_sensitive
	movq	32(%rbx), %rdi
	xorl	%esi, %esi
	call	gtk_widget_set_sensitive
	movq	40(%rbx), %rdi
	xorl	%esi, %esi
	call	gtk_widget_set_sensitive
	movq	48(%rbx), %rdi
	xorl	%esi, %esi
	call	gtk_widget_set_sensitive
	movq	56(%rbx), %rdi
	xorl	%esi, %esi
	call	gtk_widget_set_sensitive
	movl	$.LC21, %edi
	call	app_set_statusbar_msg
	jmp	.L53
.LFE56:
	.size	friends_manager_show, .-friends_manager_show
	.p2align 4,,15
	.type	friends_manager_list_clicked, @function
friends_manager_list_clicked:
.LFB54:
	pushq	%rbx
.LCFI26:
	movq	24(%rsi), %rdi
	movq	%rsi, %rbx
	movl	$1, %esi
	call	gtk_widget_set_sensitive
	movq	32(%rbx), %rdi
	movl	$1, %esi
	call	gtk_widget_set_sensitive
	movq	40(%rbx), %rdi
	movl	$1, %esi
	call	gtk_widget_set_sensitive
	movq	48(%rbx), %rdi
	movl	$1, %esi
	call	gtk_widget_set_sensitive
	movq	56(%rbx), %rdi
	movl	$1, %esi
	popq	%rbx
	jmp	gtk_widget_set_sensitive
.LFE54:
	.size	friends_manager_list_clicked, .-friends_manager_list_clicked
	.p2align 4,,15
	.type	friends_manager_block, @function
friends_manager_block:
.LFB49:
	pushq	%rbp
.LCFI27:
	pushq	%rbx
.LCFI28:
	movq	%rsi, %rbx
	subq	$72, %rsp
.LCFI29:
	movq	8(%rsi), %rdi
	leaq	16(%rsp), %rbp
	movq	$0, 56(%rsp)
	movq	$0, 48(%rsp)
	call	gtk_tree_view_get_selection
	xorl	%esi, %esi
	movq	%rax, %rdi
	movq	%rbp, %rdx
	call	gtk_tree_selection_get_selected
	testl	%eax, %eax
	jne	.L64
	addq	$72, %rsp
	popq	%rbx
	popq	%rbp
	ret
	.p2align 4,,10
	.p2align 3
.L64:
	movq	16(%rbx), %rdi
	leaq	48(%rsp), %rcx
	leaq	56(%rsp), %r9
	xorl	%r8d, %r8d
	xorl	%eax, %eax
	movl	$2, %edx
	movq	%rbp, %rsi
	movl	$-1, (%rsp)
	call	gtk_tree_model_get
	cmpq	$0, 56(%rsp)
	je	.L63
	call	gtk_list_store_get_type
	movq	16(%rbx), %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	movq	%rbp, %rsi
	movq	%rax, %rdi
	call	gtk_list_store_remove
	movq	24(%rbx), %rdi
	xorl	%esi, %esi
	call	gtk_widget_set_sensitive
	movq	32(%rbx), %rdi
	xorl	%esi, %esi
	call	gtk_widget_set_sensitive
	movq	40(%rbx), %rdi
	xorl	%esi, %esi
	call	gtk_widget_set_sensitive
	movq	48(%rbx), %rdi
	xorl	%esi, %esi
	call	gtk_widget_set_sensitive
	movq	56(%rbx), %rdi
	xorl	%esi, %esi
	call	gtk_widget_set_sensitive
	movq	56(%rsp), %rdi
	call	network_block_user
	movq	56(%rsp), %rdi
	call	g_free
.L63:
	movq	48(%rsp), %rdi
	call	g_free
	addq	$72, %rsp
	popq	%rbx
	popq	%rbp
	ret
.LFE49:
	.size	friends_manager_block, .-friends_manager_block
	.local	friends_manager
	.comm	friends_manager,8,8
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
	.long	.LFB46
	.long	.LFE46-.LFB46
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI0-.LFB46
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE1:
.LSFDE3:
	.long	.LEFDE3-.LASFDE3
.LASFDE3:
	.long	.LASFDE3-.Lframe1
	.long	.LFB44
	.long	.LFE44-.LFB44
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI1-.LFB44
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
	.long	.LCFI3-.LFB52
	.byte	0x86
	.uleb128 0x4
	.byte	0x83
	.uleb128 0x5
	.byte	0x4
	.long	.LCFI6-.LCFI3
	.byte	0xe
	.uleb128 0x60
	.byte	0x8c
	.uleb128 0x3
	.byte	0x8d
	.uleb128 0x2
	.align 8
.LEFDE5:
.LSFDE7:
	.long	.LEFDE7-.LASFDE7
.LASFDE7:
	.long	.LASFDE7-.Lframe1
	.long	.LFB50
	.long	.LFE50-.LFB50
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI7-.LFB50
	.byte	0xe
	.uleb128 0x10
	.byte	0x86
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI8-.LCFI7
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI9-.LCFI8
	.byte	0xe
	.uleb128 0x50
	.byte	0x83
	.uleb128 0x3
	.align 8
.LEFDE7:
.LSFDE9:
	.long	.LEFDE9-.LASFDE9
.LASFDE9:
	.long	.LASFDE9-.Lframe1
	.long	.LFB51
	.long	.LFE51-.LFB51
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI10-.LFB51
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI11-.LCFI10
	.byte	0xe
	.uleb128 0x18
	.byte	0x83
	.uleb128 0x3
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI12-.LCFI11
	.byte	0xe
	.uleb128 0x50
	.align 8
.LEFDE9:
.LSFDE11:
	.long	.LEFDE11-.LASFDE11
.LASFDE11:
	.long	.LASFDE11-.Lframe1
	.long	.LFB48
	.long	.LFE48-.LFB48
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI13-.LFB48
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI14-.LCFI13
	.byte	0xe
	.uleb128 0x18
	.byte	0x83
	.uleb128 0x3
	.byte	0x86
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI15-.LCFI14
	.byte	0xe
	.uleb128 0x60
	.align 8
.LEFDE11:
.LSFDE13:
	.long	.LEFDE13-.LASFDE13
.LASFDE13:
	.long	.LASFDE13-.Lframe1
	.long	.LFB47
	.long	.LFE47-.LFB47
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI16-.LFB47
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI17-.LCFI16
	.byte	0xe
	.uleb128 0x18
	.byte	0x83
	.uleb128 0x3
	.byte	0x86
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI18-.LCFI17
	.byte	0xe
	.uleb128 0x60
	.align 8
.LEFDE13:
.LSFDE15:
	.long	.LEFDE15-.LASFDE15
.LASFDE15:
	.long	.LASFDE15-.Lframe1
	.long	.LFB45
	.long	.LFE45-.LFB45
	.uleb128 0x0
	.align 8
.LEFDE15:
.LSFDE17:
	.long	.LEFDE17-.LASFDE17
.LASFDE17:
	.long	.LASFDE17-.Lframe1
	.long	.LFB56
	.long	.LFE56-.LFB56
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI19-.LFB56
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI20-.LCFI19
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI21-.LCFI20
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI22-.LCFI21
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.long	.LCFI23-.LCFI22
	.byte	0xe
	.uleb128 0x30
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
	.byte	0x4
	.long	.LCFI24-.LCFI23
	.byte	0xe
	.uleb128 0x38
	.byte	0x4
	.long	.LCFI25-.LCFI24
	.byte	0xe
	.uleb128 0x160
	.byte	0x83
	.uleb128 0x7
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
	.long	.LCFI26-.LFB54
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE19:
.LSFDE21:
	.long	.LEFDE21-.LASFDE21
.LASFDE21:
	.long	.LASFDE21-.Lframe1
	.long	.LFB49
	.long	.LFE49-.LFB49
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI27-.LFB49
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI28-.LCFI27
	.byte	0xe
	.uleb128 0x18
	.byte	0x83
	.uleb128 0x3
	.byte	0x86
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI29-.LCFI28
	.byte	0xe
	.uleb128 0x60
	.align 8
.LEFDE21:
	.ident	"GCC: (SUSE Linux) 4.3.2 [gcc-4_3-branch revision 141291]"
	.section	.comment.SUSE.OPTs,"MS",@progbits,1
	.ascii	"OspWg"
	.section	.note.GNU-stack,"",@progbits
