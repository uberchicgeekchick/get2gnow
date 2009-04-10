	.file	"keyring.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"twitter.com"
.LC1:
	.string	"GNOME_KEYRING_RESULT_DENIED"
.LC2:
	.string	"GNOME_KEYRING_RESULT_OK"
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC3:
	.string	"GNOME_KEYRING_RESULT_NO_KEYRING_DAEMON"
	.align 8
.LC4:
	.string	"GNOME_KEYRING_RESULT_ALREADY_UNLOCKED"
	.align 8
.LC5:
	.string	"GNOME_KEYRING_RESULT_NO_SUCH_KEYRING"
	.align 8
.LC6:
	.string	"GNOME_KEYRING_RESULT_BAD_ARGUMENTS"
	.section	.rodata.str1.1
.LC7:
	.string	"GNOME_KEYRING_RESULT_IO_ERROR"
	.section	.rodata.str1.8
	.align 8
.LC8:
	.string	"GNOME_KEYRING_RESULT_CANCELLED"
	.align 8
.LC9:
	.string	"GNOME_KEYRING_RESULT_ALREADY_EXISTS"
	.section	.rodata.str1.1
.LC10:
	.string	"GNOME_KEYRING_RESULT_NO_MATCH"
.LC11:
	.string	""
	.section	.rodata.str1.8
	.align 8
.LC12:
	.string	"Could not set password to keyring, result:%d->'%s'"
	.section	.rodata.str1.1
.LC13:
	.string	"Keyring"
	.text
	.p2align 4,,15
.globl keyring_set_password
	.type	keyring_set_password, @function
keyring_set_password:
.LFB27:
	subq	$56, %rsp
.LCFI0:
	xorl	%edx, %edx
	xorl	%r9d, %r9d
	leaq	52(%rsp), %rax
	movq	%rsi, 16(%rsp)
	xorl	%r8d, %r8d
	movq	%rdi, %rsi
	movl	$.LC0, %ecx
	xorl	%edi, %edi
	movq	%rax, 24(%rsp)
	movl	$0, 8(%rsp)
	movq	$0, (%rsp)
	call	gnome_keyring_set_network_password_sync
	movl	%eax, %edx
	movl	$1, %eax
	testl	%edx, %edx
	je	.L3
	cmpl	$9, %edx
	movl	$.LC11, %ecx
	jbe	.L19
.L16:
	xorl	%eax, %eax
	movl	$.LC12, %esi
	movl	$.LC13, %edi
	call	debug_impl
	xorl	%eax, %eax
.L3:
	addq	$56, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L19:
	mov	%edx, %eax
	jmp	*.L15(,%rax,8)
	.section	.rodata
	.align 8
	.align 4
.L15:
	.quad	.L5
	.quad	.L6
	.quad	.L7
	.quad	.L8
	.quad	.L9
	.quad	.L10
	.quad	.L11
	.quad	.L12
	.quad	.L13
	.quad	.L14
	.text
	.p2align 4,,10
	.p2align 3
.L14:
	movl	$.LC10, %ecx
	jmp	.L16
	.p2align 4,,10
	.p2align 3
.L5:
	movl	$.LC2, %ecx
	jmp	.L16
	.p2align 4,,10
	.p2align 3
.L6:
	movl	$.LC1, %ecx
	jmp	.L16
	.p2align 4,,10
	.p2align 3
.L7:
	movl	$.LC3, %ecx
	jmp	.L16
	.p2align 4,,10
	.p2align 3
.L8:
	movl	$.LC4, %ecx
	jmp	.L16
	.p2align 4,,10
	.p2align 3
.L9:
	movl	$.LC5, %ecx
	jmp	.L16
	.p2align 4,,10
	.p2align 3
.L10:
	movl	$.LC6, %ecx
	jmp	.L16
	.p2align 4,,10
	.p2align 3
.L11:
	movl	$.LC7, %ecx
	jmp	.L16
	.p2align 4,,10
	.p2align 3
.L12:
	movl	$.LC8, %ecx
	jmp	.L16
	.p2align 4,,10
	.p2align 3
.L13:
	movl	$.LC9, %ecx
	jmp	.L16
.LFE27:
	.size	keyring_set_password, .-keyring_set_password
	.section	.rodata.str1.8
	.align 8
.LC14:
	.string	"Could not retrieve password from keyring, result:%d->'%s'"
	.align 8
.LC15:
	.string	"Found %d matching passwords in keyring, using first available"
	.text
	.p2align 4,,15
.globl keyring_get_password
	.type	keyring_get_password, @function
keyring_get_password:
.LFB26:
	pushq	%rbx
.LCFI1:
	xorl	%r9d, %r9d
	xorl	%r8d, %r8d
	xorl	%ecx, %ecx
	movl	$.LC0, %edx
	movq	%rsi, %rbx
	subq	$32, %rsp
.LCFI2:
	xorl	%esi, %esi
	leaq	24(%rsp), %rax
	movl	$0, (%rsp)
	movq	%rax, 8(%rsp)
	call	gnome_keyring_find_network_password_sync
	testl	%eax, %eax
	movl	%eax, %edx
	je	.L21
	cmpl	$9, %eax
	movl	$.LC11, %ecx
	jbe	.L38
.L34:
	xorl	%eax, %eax
	movl	$.LC14, %esi
	movl	$.LC13, %edi
	call	debug_impl
	xorl	%eax, %eax
	addq	$32, %rsp
	popq	%rbx
	ret
	.p2align 4,,10
	.p2align 3
.L38:
	mov	%eax, %eax
	jmp	*.L33(,%rax,8)
	.section	.rodata
	.align 8
	.align 4
.L33:
	.quad	.L23
	.quad	.L24
	.quad	.L25
	.quad	.L26
	.quad	.L27
	.quad	.L28
	.quad	.L29
	.quad	.L30
	.quad	.L31
	.quad	.L32
	.text
	.p2align 4,,10
	.p2align 3
.L21:
	movq	24(%rsp), %rdi
	call	g_list_length
	cmpl	$1, %eax
	jbe	.L36
	movq	24(%rsp), %rdi
	call	g_list_length
	movl	$.LC15, %esi
	movl	%eax, %edx
	movl	$.LC13, %edi
	xorl	%eax, %eax
	call	debug_impl
.L36:
	movq	24(%rsp), %rax
	movq	(%rax), %rax
	movq	72(%rax), %rdi
	call	g_strdup
	movq	24(%rsp), %rdi
	movq	%rax, (%rbx)
	xorl	%edx, %edx
	movl	$g_free, %esi
	call	g_list_foreach
	movq	24(%rsp), %rdi
	call	g_list_free
	movl	$1, %eax
	addq	$32, %rsp
	popq	%rbx
	ret
	.p2align 4,,10
	.p2align 3
.L32:
	movl	$.LC10, %ecx
	jmp	.L34
	.p2align 4,,10
	.p2align 3
.L31:
	movl	$.LC9, %ecx
	jmp	.L34
	.p2align 4,,10
	.p2align 3
.L30:
	movl	$.LC8, %ecx
	jmp	.L34
	.p2align 4,,10
	.p2align 3
.L29:
	movl	$.LC7, %ecx
	jmp	.L34
	.p2align 4,,10
	.p2align 3
.L28:
	movl	$.LC6, %ecx
	jmp	.L34
	.p2align 4,,10
	.p2align 3
.L27:
	movl	$.LC5, %ecx
	jmp	.L34
	.p2align 4,,10
	.p2align 3
.L26:
	movl	$.LC4, %ecx
	jmp	.L34
	.p2align 4,,10
	.p2align 3
.L25:
	movl	$.LC3, %ecx
	jmp	.L34
	.p2align 4,,10
	.p2align 3
.L24:
	movl	$.LC1, %ecx
	jmp	.L34
	.p2align 4,,10
	.p2align 3
.L23:
	movl	$.LC2, %ecx
	jmp	.L34
.LFE26:
	.size	keyring_get_password, .-keyring_get_password
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
	.long	.LFB27
	.long	.LFE27-.LFB27
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI0-.LFB27
	.byte	0xe
	.uleb128 0x40
	.align 8
.LEFDE1:
.LSFDE3:
	.long	.LEFDE3-.LASFDE3
.LASFDE3:
	.long	.LASFDE3-.Lframe1
	.long	.LFB26
	.long	.LFE26-.LFB26
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI1-.LFB26
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI2-.LCFI1
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE3:
	.ident	"GCC: (SUSE Linux) 4.3.2 [gcc-4_3-branch revision 141291]"
	.section	.comment.SUSE.OPTs,"MS",@progbits,1
	.ascii	"OspWg"
	.section	.note.GNU-stack,"",@progbits
