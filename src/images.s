	.file	"images.c"
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC0:
	.string	"Image error: risizing of pixmap to: %d x %d failed."
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC1:
	.string	"Images"
	.text
	.p2align 4,,15
.globl images_scale_pixbuf
	.type	images_scale_pixbuf, @function
images_scale_pixbuf:
.LFB40:
	movq	%rbx, -24(%rsp)
.LCFI0:
	movq	%rbp, -16(%rsp)
.LCFI1:
	movl	%edx, %ebx
	movq	%r12, -8(%rsp)
.LCFI2:
	subq	$24, %rsp
.LCFI3:
	cmpl	$23, %esi
	movl	%esi, %ebp
	jg	.L2
	cmpl	$23, %ebx
	movl	$24, %ebp
	jg	.L4
.L10:
	movl	$24, %ebx
.L5:
	movl	$2, %ecx
	movl	%ebx, %edx
	movl	%ebp, %esi
	call	gdk_pixbuf_scale_simple
	testq	%rax, %rax
	movq	%rax, %r12
	je	.L9
.L6:
	movq	%r12, %rax
	movq	(%rsp), %rbx
	movq	8(%rsp), %rbp
	movq	16(%rsp), %r12
	addq	$24, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L2:
	cmpl	$97, %esi
	movl	$96, %eax
	cmovge	%eax, %ebp
	cmpl	$23, %ebx
	jle	.L10
.L4:
	cmpl	$97, %ebx
	movl	$96, %eax
	cmovge	%eax, %ebx
	jmp	.L5
.L9:
	movl	%ebx, %ecx
	movl	%ebp, %edx
	movl	$.LC0, %esi
	movl	$.LC1, %edi
	xorl	%eax, %eax
	call	debug_impl
	jmp	.L6
.LFE40:
	.size	images_scale_pixbuf, .-images_scale_pixbuf
	.section	.rodata.str1.1
.LC2:
	.string	"Image error: %s: %s"
	.text
	.p2align 4,,15
.globl images_get_unscaled_pixbuf_from_filename
	.type	images_get_unscaled_pixbuf_from_filename, @function
images_get_unscaled_pixbuf_from_filename:
.LFB44:
	movq	%rbx, -16(%rsp)
.LCFI4:
	movq	%rbp, -8(%rsp)
.LCFI5:
	subq	$40, %rsp
.LCFI6:
	leaq	16(%rsp), %rsi
	movq	%rdi, %rbp
	movq	$0, 16(%rsp)
	call	gdk_pixbuf_new_from_file
	testq	%rax, %rax
	movq	%rax, %rbx
	je	.L14
.L12:
	movq	%rbx, %rax
	movq	32(%rsp), %rbp
	movq	24(%rsp), %rbx
	addq	$40, %rsp
	ret
.L14:
	movq	16(%rsp), %rax
	movl	$.LC1, %edi
	movq	%rbp, %rdx
	movl	$.LC2, %esi
	movq	8(%rax), %rcx
	xorl	%eax, %eax
	call	debug_impl
	movq	16(%rsp), %rdi
	testq	%rdi, %rdi
	je	.L12
	call	g_error_free
	.p2align 4,,3
	.p2align 3
	jmp	.L12
.LFE44:
	.size	images_get_unscaled_pixbuf_from_filename, .-images_get_unscaled_pixbuf_from_filename
	.section	.rodata.str1.1
.LC3:
	.string	"/"
.LC4:
	.string	"unknown_image"
.LC5:
	.string	"_"
.LC6:
	.string	"greet-tweet-know/avatars"
.LC7:
	.string	".gnome2"
	.text
	.p2align 4,,15
.globl images_get_filename
	.type	images_get_filename, @function
images_get_filename:
.LFB25:
	pushq	%rbp
.LCFI7:
	movl	$7, %edx
	movl	$.LC3, %esi
	pushq	%rbx
.LCFI8:
	subq	$8, %rsp
.LCFI9:
	call	g_strsplit
	movq	40(%rax), %rdi
	movq	%rax, %rbx
	testq	%rdi, %rdi
	je	.L16
	movq	48(%rax), %rdx
	testq	%rdx, %rdx
	je	.L16
	xorl	%ecx, %ecx
	xorl	%eax, %eax
	movl	$.LC5, %esi
	call	g_strconcat
	testq	%rbx, %rbx
	movq	%rax, %rbp
	je	.L18
	movq	%rbx, %rdi
	call	g_strfreev
.L18:
	call	g_get_home_dir
	movq	%rbp, %rcx
	movq	%rax, %rdi
	xorl	%r8d, %r8d
	movl	$.LC6, %edx
	movl	$.LC7, %esi
	xorl	%eax, %eax
	call	g_build_filename
	movq	%rbp, %rdi
	movq	%rax, %rbx
	call	g_free
	movq	%rbx, %rax
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	ret
	.p2align 4,,10
	.p2align 3
.L16:
	addq	$8, %rsp
	movl	$.LC4, %edi
	popq	%rbx
	popq	%rbp
	jmp	g_strdup
.LFE25:
	.size	images_get_filename, .-images_get_filename
	.p2align 4,,15
.globl images_get_and_scale_pixbuf_from_filename
	.type	images_get_and_scale_pixbuf_from_filename, @function
images_get_and_scale_pixbuf_from_filename:
.LFB46:
	movq	%rbp, -24(%rsp)
.LCFI10:
	movq	%r12, -16(%rsp)
.LCFI11:
	movl	%edx, %ebp
	movq	%r13, -8(%rsp)
.LCFI12:
	movq	%rbx, -32(%rsp)
.LCFI13:
	subq	$56, %rsp
.LCFI14:
	cmpl	$23, %esi
	movq	%rdi, %r13
	movl	%esi, %r12d
	jg	.L21
	cmpl	$23, %ebp
	movl	$24, %r12d
	jg	.L23
.L30:
	movl	$24, %ebp
.L24:
	leaq	16(%rsp), %rsi
	movq	%r13, %rdi
	movq	$0, 16(%rsp)
	call	gdk_pixbuf_new_from_file
	testq	%rax, %rax
	movq	%rax, %rbx
	je	.L29
	movl	%ebp, %edx
	movq	%rax, %rdi
	movl	$2, %ecx
	movl	%r12d, %esi
	call	gdk_pixbuf_scale_simple
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	g_object_unref
.L27:
	movq	%rbp, %rax
	movq	24(%rsp), %rbx
	movq	32(%rsp), %rbp
	movq	40(%rsp), %r12
	movq	48(%rsp), %r13
	addq	$56, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L21:
	cmpl	$97, %esi
	movl	$96, %eax
	cmovge	%eax, %r12d
	cmpl	$23, %ebp
	jle	.L30
.L23:
	cmpl	$97, %ebp
	movl	$96, %eax
	cmovge	%eax, %ebp
	jmp	.L24
.L29:
	movq	16(%rsp), %rax
	movl	$.LC1, %edi
	movq	%r13, %rdx
	movl	$.LC2, %esi
	xorl	%ebp, %ebp
	movq	8(%rax), %rcx
	xorl	%eax, %eax
	call	debug_impl
	movq	16(%rsp), %rdi
	testq	%rdi, %rdi
	je	.L27
	call	g_error_free
	xorl	%ebp, %ebp
	jmp	.L27
.LFE46:
	.size	images_get_and_scale_pixbuf_from_filename, .-images_get_and_scale_pixbuf_from_filename
	.p2align 4,,15
.globl images_minimize_pixbuf
	.type	images_minimize_pixbuf, @function
images_minimize_pixbuf:
.LFB39:
	pushq	%rbx
.LCFI15:
	movl	$2, %ecx
	movl	$24, %edx
	movl	$24, %esi
	call	gdk_pixbuf_scale_simple
	testq	%rax, %rax
	movq	%rax, %rbx
	je	.L34
.L32:
	movq	%rbx, %rax
	popq	%rbx
	ret
.L34:
	movl	$24, %ecx
	movl	$24, %edx
	movl	$.LC0, %esi
	movl	$.LC1, %edi
	xorl	%eax, %eax
	call	debug_impl
	jmp	.L32
.LFE39:
	.size	images_minimize_pixbuf, .-images_minimize_pixbuf
	.p2align 4,,15
.globl images_normalize_pixbuf
	.type	images_normalize_pixbuf, @function
images_normalize_pixbuf:
.LFB38:
	pushq	%rbx
.LCFI16:
	movl	$2, %ecx
	movl	$48, %edx
	movl	$48, %esi
	call	gdk_pixbuf_scale_simple
	testq	%rax, %rax
	movq	%rax, %rbx
	je	.L38
.L36:
	movq	%rbx, %rax
	popq	%rbx
	ret
.L38:
	movl	$48, %ecx
	movl	$48, %edx
	movl	$.LC0, %esi
	movl	$.LC1, %edi
	xorl	%eax, %eax
	call	debug_impl
	jmp	.L36
.LFE38:
	.size	images_normalize_pixbuf, .-images_normalize_pixbuf
	.p2align 4,,15
.globl images_expand_pixbuf
	.type	images_expand_pixbuf, @function
images_expand_pixbuf:
.LFB37:
	pushq	%rbx
.LCFI17:
	movl	$2, %ecx
	movl	$72, %edx
	movl	$72, %esi
	call	gdk_pixbuf_scale_simple
	testq	%rax, %rax
	movq	%rax, %rbx
	je	.L42
.L40:
	movq	%rbx, %rax
	popq	%rbx
	ret
.L42:
	movl	$72, %ecx
	movl	$72, %edx
	movl	$.LC0, %esi
	movl	$.LC1, %edi
	xorl	%eax, %eax
	call	debug_impl
	jmp	.L40
.LFE37:
	.size	images_expand_pixbuf, .-images_expand_pixbuf
	.p2align 4,,15
.globl images_maximize_pixbuf
	.type	images_maximize_pixbuf, @function
images_maximize_pixbuf:
.LFB36:
	pushq	%rbx
.LCFI18:
	movl	$2, %ecx
	movl	$96, %edx
	movl	$96, %esi
	call	gdk_pixbuf_scale_simple
	testq	%rax, %rax
	movq	%rax, %rbx
	je	.L46
.L44:
	movq	%rbx, %rax
	popq	%rbx
	ret
.L46:
	movl	$96, %ecx
	movl	$96, %edx
	movl	$.LC0, %esi
	movl	$.LC1, %edi
	xorl	%eax, %eax
	call	debug_impl
	jmp	.L44
.LFE36:
	.size	images_maximize_pixbuf, .-images_maximize_pixbuf
	.p2align 4,,15
.globl images_get_default_pixbuf_from_filename
	.type	images_get_default_pixbuf_from_filename, @function
images_get_default_pixbuf_from_filename:
.LFB34:
	movq	%rbx, -16(%rsp)
.LCFI19:
	movq	%rbp, -8(%rsp)
.LCFI20:
	subq	$40, %rsp
.LCFI21:
	leaq	16(%rsp), %rsi
	movq	%rdi, %rbp
	movq	$0, 16(%rsp)
	call	gdk_pixbuf_new_from_file
	testq	%rax, %rax
	movq	%rax, %rbx
	je	.L52
	movq	%rax, %rdi
	movl	$2, %ecx
	movl	$48, %edx
	movl	$48, %esi
	call	gdk_pixbuf_scale_simple
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	g_object_unref
.L50:
	movq	%rbp, %rax
	movq	24(%rsp), %rbx
	movq	32(%rsp), %rbp
	addq	$40, %rsp
	ret
.L52:
	movq	16(%rsp), %rax
	movq	%rbp, %rdx
	movl	$.LC1, %edi
	movl	$.LC2, %esi
	xorl	%ebp, %ebp
	movq	8(%rax), %rcx
	xorl	%eax, %eax
	call	debug_impl
	movq	16(%rsp), %rdi
	testq	%rdi, %rdi
	je	.L50
	call	g_error_free
	xorl	%ebp, %ebp
	jmp	.L50
.LFE34:
	.size	images_get_default_pixbuf_from_filename, .-images_get_default_pixbuf_from_filename
	.p2align 4,,15
.globl images_get_expanded_pixbuf_from_filename
	.type	images_get_expanded_pixbuf_from_filename, @function
images_get_expanded_pixbuf_from_filename:
.LFB33:
	movq	%rbx, -16(%rsp)
.LCFI22:
	movq	%rbp, -8(%rsp)
.LCFI23:
	subq	$40, %rsp
.LCFI24:
	leaq	16(%rsp), %rsi
	movq	%rdi, %rbp
	movq	$0, 16(%rsp)
	call	gdk_pixbuf_new_from_file
	testq	%rax, %rax
	movq	%rax, %rbx
	je	.L58
	movq	%rax, %rdi
	movl	$2, %ecx
	movl	$72, %edx
	movl	$72, %esi
	call	gdk_pixbuf_scale_simple
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	g_object_unref
.L56:
	movq	%rbp, %rax
	movq	24(%rsp), %rbx
	movq	32(%rsp), %rbp
	addq	$40, %rsp
	ret
.L58:
	movq	16(%rsp), %rax
	movq	%rbp, %rdx
	movl	$.LC1, %edi
	movl	$.LC2, %esi
	xorl	%ebp, %ebp
	movq	8(%rax), %rcx
	xorl	%eax, %eax
	call	debug_impl
	movq	16(%rsp), %rdi
	testq	%rdi, %rdi
	je	.L56
	call	g_error_free
	xorl	%ebp, %ebp
	jmp	.L56
.LFE33:
	.size	images_get_expanded_pixbuf_from_filename, .-images_get_expanded_pixbuf_from_filename
	.p2align 4,,15
.globl images_get_maximized_pixbuf_from_filename
	.type	images_get_maximized_pixbuf_from_filename, @function
images_get_maximized_pixbuf_from_filename:
.LFB32:
	movq	%rbx, -16(%rsp)
.LCFI25:
	movq	%rbp, -8(%rsp)
.LCFI26:
	subq	$40, %rsp
.LCFI27:
	leaq	16(%rsp), %rsi
	movq	%rdi, %rbp
	movq	$0, 16(%rsp)
	call	gdk_pixbuf_new_from_file
	testq	%rax, %rax
	movq	%rax, %rbx
	je	.L64
	movq	%rax, %rdi
	movl	$2, %ecx
	movl	$96, %edx
	movl	$96, %esi
	call	gdk_pixbuf_scale_simple
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	g_object_unref
.L62:
	movq	%rbp, %rax
	movq	24(%rsp), %rbx
	movq	32(%rsp), %rbp
	addq	$40, %rsp
	ret
.L64:
	movq	16(%rsp), %rax
	movq	%rbp, %rdx
	movl	$.LC1, %edi
	movl	$.LC2, %esi
	xorl	%ebp, %ebp
	movq	8(%rax), %rcx
	xorl	%eax, %eax
	call	debug_impl
	movq	16(%rsp), %rdi
	testq	%rdi, %rdi
	je	.L62
	call	g_error_free
	xorl	%ebp, %ebp
	jmp	.L62
.LFE32:
	.size	images_get_maximized_pixbuf_from_filename, .-images_get_maximized_pixbuf_from_filename
	.p2align 4,,15
.globl images_get_scaled_image_from_filename
	.type	images_get_scaled_image_from_filename, @function
images_get_scaled_image_from_filename:
.LFB31:
	movq	%rbp, -24(%rsp)
.LCFI28:
	movq	%r12, -16(%rsp)
.LCFI29:
	movl	%edx, %ebp
	movq	%r13, -8(%rsp)
.LCFI30:
	movq	%rbx, -32(%rsp)
.LCFI31:
	subq	$56, %rsp
.LCFI32:
	cmpl	$23, %esi
	movq	%rdi, %r13
	movl	%esi, %r12d
	jg	.L66
	cmpl	$23, %ebp
	movl	$24, %r12d
	jg	.L68
.L75:
	movl	$24, %ebp
.L69:
	leaq	16(%rsp), %rsi
	movq	%r13, %rdi
	movq	$0, 16(%rsp)
	call	gdk_pixbuf_new_from_file
	testq	%rax, %rax
	movq	%rax, %rbx
	je	.L74
	movl	%ebp, %edx
	movq	%rax, %rdi
	movl	$2, %ecx
	movl	%r12d, %esi
	call	gdk_pixbuf_scale_simple
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	g_object_unref
.L72:
	call	gtk_image_get_type
	movq	%rbp, %rdi
	movq	%rax, %rbx
	call	gtk_image_new_from_pixbuf
	movq	%rbx, %rsi
	movq	%rax, %rdi
	call	g_type_check_instance_cast
	movq	%rbp, %rdi
	movq	%rax, %rbx
	call	g_object_unref
	movq	%rbx, %rax
	movq	32(%rsp), %rbp
	movq	24(%rsp), %rbx
	movq	40(%rsp), %r12
	movq	48(%rsp), %r13
	addq	$56, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L66:
	cmpl	$97, %esi
	movl	$96, %eax
	cmovge	%eax, %r12d
	cmpl	$23, %ebp
	jle	.L75
.L68:
	cmpl	$97, %ebp
	movl	$96, %eax
	cmovge	%eax, %ebp
	jmp	.L69
.L74:
	movq	16(%rsp), %rax
	movl	$.LC1, %edi
	movq	%r13, %rdx
	movl	$.LC2, %esi
	movq	8(%rax), %rcx
	xorl	%eax, %eax
	call	debug_impl
	movq	16(%rsp), %rdi
	testq	%rdi, %rdi
	jne	.L76
	xorl	%ebp, %ebp
	jmp	.L72
.L76:
	call	g_error_free
	xorl	%ebp, %ebp
	.p2align 4,,4
	.p2align 3
	jmp	.L72
.LFE31:
	.size	images_get_scaled_image_from_filename, .-images_get_scaled_image_from_filename
	.p2align 4,,15
.globl images_get_minimized_image_from_filename
	.type	images_get_minimized_image_from_filename, @function
images_get_minimized_image_from_filename:
.LFB29:
	movl	$24, %edx
	movl	$24, %esi
	jmp	images_get_scaled_image_from_filename
.LFE29:
	.size	images_get_minimized_image_from_filename, .-images_get_minimized_image_from_filename
	.p2align 4,,15
.globl images_get_default_image_from_filename
	.type	images_get_default_image_from_filename, @function
images_get_default_image_from_filename:
.LFB28:
	movl	$48, %edx
	movl	$48, %esi
	jmp	images_get_scaled_image_from_filename
.LFE28:
	.size	images_get_default_image_from_filename, .-images_get_default_image_from_filename
	.p2align 4,,15
.globl images_get_maximized_image_from_filename
	.type	images_get_maximized_image_from_filename, @function
images_get_maximized_image_from_filename:
.LFB27:
	movl	$96, %edx
	movl	$96, %esi
	jmp	images_get_scaled_image_from_filename
.LFE27:
	.size	images_get_maximized_image_from_filename, .-images_get_maximized_image_from_filename
	.p2align 4,,15
.globl images_get_expanded_image_from_filename
	.type	images_get_expanded_image_from_filename, @function
images_get_expanded_image_from_filename:
.LFB26:
	movl	$72, %edx
	movl	$72, %esi
	jmp	images_get_scaled_image_from_filename
.LFE26:
	.size	images_get_expanded_image_from_filename, .-images_get_expanded_image_from_filename
	.p2align 4,,15
.globl images_get_pixbuf_from_filename
	.type	images_get_pixbuf_from_filename, @function
images_get_pixbuf_from_filename:
.LFB41:
	movq	%rbx, -16(%rsp)
.LCFI33:
	movq	%rbp, -8(%rsp)
.LCFI34:
	subq	$40, %rsp
.LCFI35:
	leaq	16(%rsp), %rsi
	movq	%rdi, %rbp
	movq	$0, 16(%rsp)
	call	gdk_pixbuf_new_from_file
	testq	%rax, %rax
	movq	%rax, %rbx
	je	.L90
	movq	%rax, %rdi
	movl	$2, %ecx
	movl	$48, %edx
	movl	$48, %esi
	call	gdk_pixbuf_scale_simple
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	g_object_unref
.L88:
	movq	%rbp, %rax
	movq	24(%rsp), %rbx
	movq	32(%rsp), %rbp
	addq	$40, %rsp
	ret
.L90:
	movq	16(%rsp), %rax
	movq	%rbp, %rdx
	movl	$.LC1, %edi
	movl	$.LC2, %esi
	xorl	%ebp, %ebp
	movq	8(%rax), %rcx
	xorl	%eax, %eax
	call	debug_impl
	movq	16(%rsp), %rdi
	testq	%rdi, %rdi
	je	.L88
	call	g_error_free
	xorl	%ebp, %ebp
	jmp	.L88
.LFE41:
	.size	images_get_pixbuf_from_filename, .-images_get_pixbuf_from_filename
	.p2align 4,,15
.globl images_get_minimized_pixbuf_from_filename
	.type	images_get_minimized_pixbuf_from_filename, @function
images_get_minimized_pixbuf_from_filename:
.LFB35:
	movq	%rbx, -16(%rsp)
.LCFI36:
	movq	%rbp, -8(%rsp)
.LCFI37:
	subq	$40, %rsp
.LCFI38:
	leaq	16(%rsp), %rsi
	movq	%rdi, %rbp
	movq	$0, 16(%rsp)
	call	gdk_pixbuf_new_from_file
	testq	%rax, %rax
	movq	%rax, %rbx
	je	.L96
	movq	%rax, %rdi
	movl	$2, %ecx
	movl	$24, %edx
	movl	$24, %esi
	call	gdk_pixbuf_scale_simple
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	g_object_unref
.L94:
	movq	%rbp, %rax
	movq	24(%rsp), %rbx
	movq	32(%rsp), %rbp
	addq	$40, %rsp
	ret
.L96:
	movq	16(%rsp), %rax
	movq	%rbp, %rdx
	movl	$.LC1, %edi
	movl	$.LC2, %esi
	xorl	%ebp, %ebp
	movq	8(%rax), %rcx
	xorl	%eax, %eax
	call	debug_impl
	movq	16(%rsp), %rdi
	testq	%rdi, %rdi
	je	.L94
	call	g_error_free
	xorl	%ebp, %ebp
	jmp	.L94
.LFE35:
	.size	images_get_minimized_pixbuf_from_filename, .-images_get_minimized_pixbuf_from_filename
	.p2align 4,,15
.globl images_get_scaled_pixbuf_from_filename
	.type	images_get_scaled_pixbuf_from_filename, @function
images_get_scaled_pixbuf_from_filename:
.LFB45:
	movq	%rbp, -24(%rsp)
.LCFI39:
	movq	%r12, -16(%rsp)
.LCFI40:
	movl	%edx, %ebp
	movq	%r13, -8(%rsp)
.LCFI41:
	movq	%rbx, -32(%rsp)
.LCFI42:
	subq	$56, %rsp
.LCFI43:
	cmpl	$23, %esi
	movq	%rdi, %r13
	movl	%esi, %r12d
	jg	.L98
	cmpl	$23, %ebp
	movl	$24, %r12d
	jg	.L100
.L107:
	movl	$24, %ebp
.L101:
	leaq	16(%rsp), %rsi
	movq	%r13, %rdi
	movq	$0, 16(%rsp)
	call	gdk_pixbuf_new_from_file
	testq	%rax, %rax
	movq	%rax, %rbx
	je	.L106
	movl	%ebp, %edx
	movq	%rax, %rdi
	movl	$2, %ecx
	movl	%r12d, %esi
	call	gdk_pixbuf_scale_simple
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	g_object_unref
.L104:
	movq	%rbp, %rax
	movq	24(%rsp), %rbx
	movq	32(%rsp), %rbp
	movq	40(%rsp), %r12
	movq	48(%rsp), %r13
	addq	$56, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L98:
	cmpl	$97, %esi
	movl	$96, %eax
	cmovge	%eax, %r12d
	cmpl	$23, %ebp
	jle	.L107
.L100:
	cmpl	$97, %ebp
	movl	$96, %eax
	cmovge	%eax, %ebp
	jmp	.L101
.L106:
	movq	16(%rsp), %rax
	movl	$.LC1, %edi
	movq	%r13, %rdx
	movl	$.LC2, %esi
	xorl	%ebp, %ebp
	movq	8(%rax), %rcx
	xorl	%eax, %eax
	call	debug_impl
	movq	16(%rsp), %rdi
	testq	%rdi, %rdi
	je	.L104
	call	g_error_free
	xorl	%ebp, %ebp
	jmp	.L104
.LFE45:
	.size	images_get_scaled_pixbuf_from_filename, .-images_get_scaled_pixbuf_from_filename
	.p2align 4,,15
.globl images_get_image_from_filename
	.type	images_get_image_from_filename, @function
images_get_image_from_filename:
.LFB30:
	pushq	%rbp
.LCFI44:
	movq	%rdi, %rbp
	pushq	%rbx
.LCFI45:
	subq	$24, %rsp
.LCFI46:
	leaq	16(%rsp), %rsi
	movq	$0, 16(%rsp)
	call	gdk_pixbuf_new_from_file
	testq	%rax, %rax
	movq	%rax, %rbx
	je	.L113
	movq	%rax, %rdi
	movl	$2, %ecx
	movl	$48, %edx
	movl	$48, %esi
	call	gdk_pixbuf_scale_simple
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	g_object_unref
.L111:
	call	gtk_image_get_type
	movq	%rbp, %rdi
	movq	%rax, %rbx
	call	gtk_image_new_from_pixbuf
	movq	%rbx, %rsi
	movq	%rax, %rdi
	call	g_type_check_instance_cast
	movq	%rbp, %rdi
	movq	%rax, %rbx
	call	g_object_unref
	movq	%rbx, %rax
	addq	$24, %rsp
	popq	%rbx
	popq	%rbp
	ret
.L113:
	movq	16(%rsp), %rax
	movl	$.LC1, %edi
	movq	%rbp, %rdx
	movl	$.LC2, %esi
	movq	8(%rax), %rcx
	xorl	%eax, %eax
	call	debug_impl
	movq	16(%rsp), %rdi
	testq	%rdi, %rdi
	jne	.L114
	xorl	%ebp, %ebp
	jmp	.L111
.L114:
	call	g_error_free
	xorl	%ebp, %ebp
	.p2align 4,,4
	.p2align 3
	jmp	.L111
.LFE30:
	.size	images_get_image_from_filename, .-images_get_image_from_filename
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
	.long	.LFB40
	.long	.LFE40-.LFB40
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI1-.LFB40
	.byte	0x86
	.uleb128 0x3
	.byte	0x83
	.uleb128 0x4
	.byte	0x4
	.long	.LCFI3-.LCFI1
	.byte	0xe
	.uleb128 0x20
	.byte	0x8c
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
	.long	.LCFI6-.LFB44
	.byte	0xe
	.uleb128 0x30
	.byte	0x86
	.uleb128 0x2
	.byte	0x83
	.uleb128 0x3
	.align 8
.LEFDE3:
.LSFDE5:
	.long	.LEFDE5-.LASFDE5
.LASFDE5:
	.long	.LASFDE5-.Lframe1
	.long	.LFB25
	.long	.LFE25-.LFB25
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI7-.LFB25
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI8-.LCFI7
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI9-.LCFI8
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x3
	.byte	0x86
	.uleb128 0x2
	.align 8
.LEFDE5:
.LSFDE7:
	.long	.LEFDE7-.LASFDE7
.LASFDE7:
	.long	.LASFDE7-.Lframe1
	.long	.LFB46
	.long	.LFE46-.LFB46
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI11-.LFB46
	.byte	0x8c
	.uleb128 0x3
	.byte	0x86
	.uleb128 0x4
	.byte	0x4
	.long	.LCFI14-.LCFI11
	.byte	0xe
	.uleb128 0x40
	.byte	0x83
	.uleb128 0x5
	.byte	0x8d
	.uleb128 0x2
	.align 8
.LEFDE7:
.LSFDE9:
	.long	.LEFDE9-.LASFDE9
.LASFDE9:
	.long	.LASFDE9-.Lframe1
	.long	.LFB39
	.long	.LFE39-.LFB39
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI15-.LFB39
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
	.long	.LFB38
	.long	.LFE38-.LFB38
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI16-.LFB38
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
	.long	.LFB37
	.long	.LFE37-.LFB37
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI17-.LFB37
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
	.long	.LCFI18-.LFB36
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
	.long	.LFB34
	.long	.LFE34-.LFB34
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI21-.LFB34
	.byte	0xe
	.uleb128 0x30
	.byte	0x86
	.uleb128 0x2
	.byte	0x83
	.uleb128 0x3
	.align 8
.LEFDE17:
.LSFDE19:
	.long	.LEFDE19-.LASFDE19
.LASFDE19:
	.long	.LASFDE19-.Lframe1
	.long	.LFB33
	.long	.LFE33-.LFB33
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI24-.LFB33
	.byte	0xe
	.uleb128 0x30
	.byte	0x86
	.uleb128 0x2
	.byte	0x83
	.uleb128 0x3
	.align 8
.LEFDE19:
.LSFDE21:
	.long	.LEFDE21-.LASFDE21
.LASFDE21:
	.long	.LASFDE21-.Lframe1
	.long	.LFB32
	.long	.LFE32-.LFB32
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI27-.LFB32
	.byte	0xe
	.uleb128 0x30
	.byte	0x86
	.uleb128 0x2
	.byte	0x83
	.uleb128 0x3
	.align 8
.LEFDE21:
.LSFDE23:
	.long	.LEFDE23-.LASFDE23
.LASFDE23:
	.long	.LASFDE23-.Lframe1
	.long	.LFB31
	.long	.LFE31-.LFB31
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI29-.LFB31
	.byte	0x8c
	.uleb128 0x3
	.byte	0x86
	.uleb128 0x4
	.byte	0x4
	.long	.LCFI32-.LCFI29
	.byte	0xe
	.uleb128 0x40
	.byte	0x83
	.uleb128 0x5
	.byte	0x8d
	.uleb128 0x2
	.align 8
.LEFDE23:
.LSFDE25:
	.long	.LEFDE25-.LASFDE25
.LASFDE25:
	.long	.LASFDE25-.Lframe1
	.long	.LFB29
	.long	.LFE29-.LFB29
	.uleb128 0x0
	.align 8
.LEFDE25:
.LSFDE27:
	.long	.LEFDE27-.LASFDE27
.LASFDE27:
	.long	.LASFDE27-.Lframe1
	.long	.LFB28
	.long	.LFE28-.LFB28
	.uleb128 0x0
	.align 8
.LEFDE27:
.LSFDE29:
	.long	.LEFDE29-.LASFDE29
.LASFDE29:
	.long	.LASFDE29-.Lframe1
	.long	.LFB27
	.long	.LFE27-.LFB27
	.uleb128 0x0
	.align 8
.LEFDE29:
.LSFDE31:
	.long	.LEFDE31-.LASFDE31
.LASFDE31:
	.long	.LASFDE31-.Lframe1
	.long	.LFB26
	.long	.LFE26-.LFB26
	.uleb128 0x0
	.align 8
.LEFDE31:
.LSFDE33:
	.long	.LEFDE33-.LASFDE33
.LASFDE33:
	.long	.LASFDE33-.Lframe1
	.long	.LFB41
	.long	.LFE41-.LFB41
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI35-.LFB41
	.byte	0xe
	.uleb128 0x30
	.byte	0x86
	.uleb128 0x2
	.byte	0x83
	.uleb128 0x3
	.align 8
.LEFDE33:
.LSFDE35:
	.long	.LEFDE35-.LASFDE35
.LASFDE35:
	.long	.LASFDE35-.Lframe1
	.long	.LFB35
	.long	.LFE35-.LFB35
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI38-.LFB35
	.byte	0xe
	.uleb128 0x30
	.byte	0x86
	.uleb128 0x2
	.byte	0x83
	.uleb128 0x3
	.align 8
.LEFDE35:
.LSFDE37:
	.long	.LEFDE37-.LASFDE37
.LASFDE37:
	.long	.LASFDE37-.Lframe1
	.long	.LFB45
	.long	.LFE45-.LFB45
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI40-.LFB45
	.byte	0x8c
	.uleb128 0x3
	.byte	0x86
	.uleb128 0x4
	.byte	0x4
	.long	.LCFI43-.LCFI40
	.byte	0xe
	.uleb128 0x40
	.byte	0x83
	.uleb128 0x5
	.byte	0x8d
	.uleb128 0x2
	.align 8
.LEFDE37:
.LSFDE39:
	.long	.LEFDE39-.LASFDE39
.LASFDE39:
	.long	.LASFDE39-.Lframe1
	.long	.LFB30
	.long	.LFE30-.LFB30
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI44-.LFB30
	.byte	0xe
	.uleb128 0x10
	.byte	0x86
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI45-.LCFI44
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI46-.LCFI45
	.byte	0xe
	.uleb128 0x30
	.byte	0x83
	.uleb128 0x3
	.align 8
.LEFDE39:
	.ident	"GCC: (SUSE Linux) 4.3.2 [gcc-4_3-branch revision 141291]"
	.section	.comment.SUSE.OPTs,"MS",@progbits,1
	.ascii	"OspWg"
	.section	.note.GNU-stack,"",@progbits
