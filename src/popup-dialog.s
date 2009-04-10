	.file	"popup-dialog.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"frienduser_entry"
.LC1:
	.string	"popup_friend_dialog"
.LC2:
	.string	"popup-dialog.ui"
.LC3:
	.string	"destroy"
.LC4:
	.string	"response"
.LC5:
	.string	"Unfollow a friend:"
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC6:
	.string	"Block a user from reading your tweets and sending you messages:"
	.section	.rodata.str1.1
.LC7:
	.string	"Follow a new friend:"
	.text
	.p2align 4,,15
	.type	popup_dialog_show, @function
popup_dialog_show:
.LFB39:
	pushq	%r12
.LCFI0:
	movq	%rdi, %r12
	pushq	%rbp
.LCFI1:
	movl	%esi, %ebp
	pushq	%rbx
.LCFI2:
	subq	$32, %rsp
.LCFI3:
	movq	popup.43391(%rip), %rbx
	testq	%rbx, %rbx
	je	.L2
	cmpl	%esi, 16(%rbx)
	je	.L11
	movq	(%rbx), %rdi
	call	gtk_widget_destroy
	movq	8(%rbx), %rdi
	call	gtk_widget_destroy
	movq	%rbx, %rdi
	call	g_free
.L2:
	movl	$24, %edi
	call	g_malloc0
	leaq	8(%rax), %r8
	xorl	%r9d, %r9d
	movq	%rax, %rdx
	movq	%rax, popup.43391(%rip)
	movl	$.LC0, %ecx
	movl	$.LC1, %esi
	movl	$.LC2, %edi
	xorl	%eax, %eax
	call	gtkbuilder_get_file
	movq	popup.43391(%rip), %rsi
	movl	$.LC1, %r9d
	movq	%rax, %rdi
	movq	%rax, %rbx
	movl	$popup_destroy_cb, %r8d
	xorl	%eax, %eax
	movl	$.LC3, %ecx
	movq	%r9, %rdx
	movq	$0, 16(%rsp)
	movq	$popup_response_cb, 8(%rsp)
	movq	$.LC4, (%rsp)
	call	gtkbuilder_connect
	movq	%rbx, %rdi
	call	g_object_unref
	cmpl	$1, %ebp
	je	.L5
	cmpl	$2, %ebp
	.p2align 4,,2
	.p2align 3
	je	.L12
	movl	$.LC7, %edi
	call	g_strdup
	movq	%rax, %rbp
.L7:
	call	gtk_window_get_type
	movq	%rax, %rbx
	movq	%rax, %rsi
	movq	popup.43391(%rip), %rax
	movq	(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%rbp, %rsi
	movq	%rax, %rdi
	call	gtk_window_set_title
	movq	%rbp, %rdi
	call	g_free
	movq	popup.43391(%rip), %rax
	movl	$80, %esi
	movq	(%rax), %rdi
	call	g_type_check_instance_cast
	movl	$popup.43391, %esi
	movq	%rax, %rdi
	call	g_object_add_weak_pointer
	movq	popup.43391(%rip), %rax
	movq	%rbx, %rsi
	movq	(%rax), %rdi
	call	g_type_check_instance_cast
	movq	%r12, %rsi
	movq	%rax, %rdi
	call	gtk_window_set_transient_for
	movq	popup.43391(%rip), %rax
	movq	(%rax), %rdi
	addq	$32, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	jmp	gtk_widget_show
	.p2align 4,,10
	.p2align 3
.L12:
	movl	$.LC6, %edi
	call	g_strdup
	movq	%rax, %rbp
	jmp	.L7
	.p2align 4,,10
	.p2align 3
.L11:
	call	gtk_window_get_type
	movq	(%rbx), %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	addq	$32, %rsp
	movq	%rax, %rdi
	popq	%rbx
	popq	%rbp
	popq	%r12
	jmp	gtk_window_present
	.p2align 4,,10
	.p2align 3
.L5:
	movl	$.LC5, %edi
	call	g_strdup
	movq	%rax, %rbp
	jmp	.L7
.LFE39:
	.size	popup_dialog_show, .-popup_dialog_show
	.p2align 4,,15
.globl popup_friend_block
	.type	popup_friend_block, @function
popup_friend_block:
.LFB38:
	pushq	%rbx
.LCFI4:
	movq	%rdi, %rbx
	call	gtk_window_get_type
	movq	%rbx, %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	popq	%rbx
	movq	%rax, %rdi
	movl	$2, %esi
	jmp	popup_dialog_show
.LFE38:
	.size	popup_friend_block, .-popup_friend_block
	.p2align 4,,15
.globl popup_friend_unfollow
	.type	popup_friend_unfollow, @function
popup_friend_unfollow:
.LFB37:
	pushq	%rbx
.LCFI5:
	movq	%rdi, %rbx
	call	gtk_window_get_type
	movq	%rbx, %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	popq	%rbx
	movq	%rax, %rdi
	movl	$1, %esi
	jmp	popup_dialog_show
.LFE37:
	.size	popup_friend_unfollow, .-popup_friend_unfollow
	.p2align 4,,15
.globl popup_friend_follow
	.type	popup_friend_follow, @function
popup_friend_follow:
.LFB36:
	pushq	%rbx
.LCFI6:
	movq	%rdi, %rbx
	call	gtk_window_get_type
	movq	%rbx, %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	popq	%rbx
	movq	%rax, %rdi
	xorl	%esi, %esi
	jmp	popup_dialog_show
.LFE36:
	.size	popup_friend_follow, .-popup_friend_follow
	.p2align 4,,15
	.type	popup_response_cb, @function
popup_response_cb:
.LFB33:
	cmpl	$-5, %esi
	pushq	%rbx
.LCFI7:
	movq	%rdx, %rbx
	jne	.L26
	movl	16(%rdx), %eax
	cmpl	$1, %eax
	je	.L23
	jb	.L22
	cmpl	$2, %eax
	.p2align 4,,5
	.p2align 3
	jne	.L26
	.p2align 4,,9
	.p2align 3
	call	gtk_entry_get_type
	movq	8(%rbx), %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_entry_get_text
	popq	%rbx
	movq	%rax, %rdi
	jmp	network_block_user
	.p2align 4,,10
	.p2align 3
.L26:
	popq	%rbx
	jmp	gtk_widget_destroy
	.p2align 4,,10
	.p2align 3
.L22:
	call	gtk_entry_get_type
	movq	8(%rbx), %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_entry_get_text
	popq	%rbx
	movq	%rax, %rdi
	jmp	network_follow_user
	.p2align 4,,10
	.p2align 3
.L23:
	call	gtk_entry_get_type
	movq	8(%rbx), %rdi
	movq	%rax, %rsi
	call	g_type_check_instance_cast
	movq	%rax, %rdi
	call	gtk_entry_get_text
	popq	%rbx
	movq	%rax, %rdi
	jmp	network_unfollow_user
.LFE33:
	.size	popup_response_cb, .-popup_response_cb
	.p2align 4,,15
	.type	popup_destroy_cb, @function
popup_destroy_cb:
.LFB34:
	movq	(%rsi), %rdi
	pushq	%rbx
.LCFI8:
	movq	%rsi, %rbx
	call	gtk_widget_destroy
	movq	8(%rbx), %rdi
	call	gtk_widget_destroy
	movq	%rbx, %rdi
	popq	%rbx
	jmp	g_free
.LFE34:
	.size	popup_destroy_cb, .-popup_destroy_cb
	.local	popup.43391
	.comm	popup.43391,8,8
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
	.long	.LFB39
	.long	.LFE39-.LFB39
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI0-.LFB39
	.byte	0xe
	.uleb128 0x10
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI1-.LCFI0
	.byte	0xe
	.uleb128 0x18
	.byte	0x86
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI2-.LCFI1
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI3-.LCFI2
	.byte	0xe
	.uleb128 0x40
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
	.long	.LCFI5-.LFB37
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
	.long	.LFB36
	.long	.LFE36-.LFB36
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI6-.LFB36
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
	.long	.LFB33
	.long	.LFE33-.LFB33
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI7-.LFB33
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
	.long	.LFB34
	.long	.LFE34-.LFB34
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI8-.LFB34
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE11:
	.ident	"GCC: (SUSE Linux) 4.3.2 [gcc-4_3-branch revision 141291]"
	.section	.comment.SUSE.OPTs,"MS",@progbits,1
	.ascii	"OspWg"
	.section	.note.GNU-stack,"",@progbits
