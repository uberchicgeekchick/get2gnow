	.file	"spell.c"
	.text
	.p2align 4,,15
.globl spell_free_suggestions
	.type	spell_free_suggestions, @function
spell_free_suggestions:
.LFB55:
	pushq	%rbx
.LCFI0:
	xorl	%edx, %edx
	movq	%rdi, %rbx
	movl	$g_free, %esi
	call	g_list_foreach
	movq	%rbx, %rdi
	popq	%rbx
	jmp	g_list_free
.LFE55:
	.size	spell_free_suggestions, .-spell_free_suggestions
	.p2align 4,,15
.globl spell_free_language_codes
	.type	spell_free_language_codes, @function
spell_free_language_codes:
.LFB51:
	pushq	%rbx
.LCFI1:
	xorl	%edx, %edx
	movq	%rdi, %rbx
	movl	$g_free, %esi
	call	g_list_foreach
	movq	%rbx, %rdi
	popq	%rbx
	jmp	g_list_free
.LFE51:
	.size	spell_free_language_codes, .-spell_free_language_codes
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"SPELL_DISABLED"
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC1:
	.string	"SPELL_DISABLE env variable defined"
	.section	.rodata.str1.1
.LC2:
	.string	"Spell"
.LC3:
	.string	"Support enabled"
	.text
	.p2align 4,,15
.globl spell_supported
	.type	spell_supported, @function
spell_supported:
.LFB54:
	subq	$8, %rsp
.LCFI2:
	movl	$.LC0, %edi
	call	g_getenv
	testq	%rax, %rax
	je	.L6
	xorl	%eax, %eax
	movl	$.LC1, %esi
	movl	$.LC2, %edi
	call	debug_impl
	xorl	%eax, %eax
	addq	$8, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L6:
	xorl	%eax, %eax
	movl	$.LC3, %esi
	movl	$.LC2, %edi
	call	debug_impl
	movl	$1, %eax
	addq	$8, %rsp
	ret
.LFE54:
	.size	spell_supported, .-spell_supported
	.section	.rodata.str1.8
	.align 8
.LC4:
	.string	"/apps/greet-tweet-know/ui/spell_checker_languages"
	.section	.rodata.str1.1
.LC5:
	.string	"No languages to setup"
.LC6:
	.string	","
.LC7:
	.string	"Setting up language:'%s'"
	.text
	.p2align 4,,15
	.type	spell_setup_languages, @function
spell_setup_languages:
.LFB47:
	movq	%rbx, -32(%rsp)
.LCFI3:
	movq	%rbp, -24(%rsp)
.LCFI4:
	movq	%r12, -16(%rsp)
.LCFI5:
	movq	%r13, -8(%rsp)
.LCFI6:
	subq	$56, %rsp
.LCFI7:
	movl	conf_notify_inited(%rip), %eax
	testl	%eax, %eax
	je	.L19
	cmpq	$0, languages(%rip)
	je	.L12
.L20:
	movl	$.LC5, %esi
	movl	$.LC2, %edi
	xorl	%eax, %eax
	call	debug_impl
.L17:
	movq	24(%rsp), %rbx
	movq	32(%rsp), %rbp
	movq	40(%rsp), %r12
	movq	48(%rsp), %r13
	addq	$56, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L19:
	call	conf_get
	xorl	%ecx, %ecx
	movq	%rax, %rdi
	movl	$spell_notify_languages_cb, %edx
	movl	$.LC4, %esi
	call	conf_notify_add
	cmpq	$0, languages(%rip)
	movl	$1, conf_notify_inited(%rip)
	jne	.L20
.L12:
	call	conf_get
	leaq	16(%rsp), %rdx
	movq	%rax, %rdi
	movl	$.LC4, %esi
	call	conf_get_string
	testl	%eax, %eax
	je	.L17
	movq	16(%rsp), %rdi
	testq	%rdi, %rdi
	je	.L17
	movl	$-1, %edx
	movl	$.LC6, %esi
	call	g_strsplit
	testq	%rax, %rax
	movq	%rax, %r13
	jne	.L21
.L14:
	movq	16(%rsp), %rdi
	call	g_free
	jmp	.L17
	.p2align 4,,10
	.p2align 3
.L21:
	movq	(%rax), %rdx
	testq	%rdx, %rdx
	je	.L15
	leaq	8(%rax), %rbp
	movq	%rax, %r12
	.p2align 4,,10
	.p2align 3
.L16:
	movl	$.LC7, %esi
	xorl	%eax, %eax
	movl	$.LC2, %edi
	call	debug_impl
	movl	$16, %edi
	call	g_slice_alloc0
	movq	%rax, %rbx
	call	enchant_broker_init
	movq	(%r12), %rsi
	movq	%rax, %rdi
	movq	%rax, (%rbx)
	movq	%rbp, %r12
	call	enchant_broker_request_dict
	movq	languages(%rip), %rdi
	movq	%rbx, %rsi
	movq	%rax, 8(%rbx)
	call	g_list_append
	movq	(%rbp), %rdx
	addq	$8, %rbp
	movq	%rax, languages(%rip)
	testq	%rdx, %rdx
	jne	.L16
.L15:
	movq	%r13, %rdi
	call	g_strfreev
	jmp	.L14
.LFE47:
	.size	spell_setup_languages, .-spell_setup_languages
	.section	.rodata.str1.1
.LC8:
	.string	"word != NULL"
	.text
	.p2align 4,,15
.globl spell_get_suggestions
	.type	spell_get_suggestions, @function
spell_get_suggestions:
.LFB53:
	pushq	%r15
.LCFI8:
	movq	%rdi, %r15
	pushq	%r14
.LCFI9:
	pushq	%r13
.LCFI10:
	pushq	%r12
.LCFI11:
	pushq	%rbp
.LCFI12:
	pushq	%rbx
.LCFI13:
	subq	$24, %rsp
.LCFI14:
	testq	%rdi, %rdi
	je	.L23
	call	spell_setup_languages
	movq	%r15, %rdi
	xorl	%r12d, %r12d
	call	strlen
	movq	languages(%rip), %r13
	testq	%r13, %r13
	je	.L25
	cltq
	xorl	%r12d, %r12d
	movq	%rax, (%rsp)
	.p2align 4,,10
	.p2align 3
.L29:
	movq	(%r13), %r14
	movq	(%rsp), %rdx
	leaq	16(%rsp), %rcx
	movq	%r15, %rsi
	movq	8(%r14), %rdi
	call	enchant_dict_suggest
	cmpq	$0, 16(%rsp)
	movq	%rax, %rbp
	je	.L26
	xorl	%ebx, %ebx
	.p2align 4,,10
	.p2align 3
.L27:
	movq	(%rbp,%rbx,8), %rdi
	addq	$1, %rbx
	call	g_strdup
	movq	%r12, %rdi
	movq	%rax, %rsi
	call	g_list_append
	cmpq	%rbx, 16(%rsp)
	movq	%rax, %r12
	ja	.L27
.L26:
	testq	%rbp, %rbp
	je	.L28
	movq	8(%r14), %rdi
	movq	%rbp, %rsi
	call	enchant_dict_free_string_list
.L28:
	movq	8(%r13), %r13
	testq	%r13, %r13
	jne	.L29
.L25:
	addq	$24, %rsp
	movq	%r12, %rax
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	ret
.L23:
	movl	$.LC8, %edx
	movl	$__PRETTY_FUNCTION__.10623, %esi
	xorl	%edi, %edi
	call	g_return_if_fail_warning
	xorl	%r12d, %r12d
	jmp	.L25
.LFE53:
	.size	spell_get_suggestions, .-spell_get_suggestions
	.section	.rodata.str1.8
	.align 8
.LC9:
	.string	"Resetting languages due to config change"
	.text
	.p2align 4,,15
	.type	spell_notify_languages_cb, @function
spell_notify_languages_cb:
.LFB46:
	pushq	%rbp
.LCFI15:
	xorl	%eax, %eax
	movl	$.LC9, %esi
	movl	$.LC2, %edi
	pushq	%rbx
.LCFI16:
	subq	$8, %rsp
.LCFI17:
	call	debug_impl
	movq	languages(%rip), %rbp
	testq	%rbp, %rbp
	je	.L34
	.p2align 4,,10
	.p2align 3
.L37:
	movq	(%rbp), %rbx
	movq	8(%rbx), %rsi
	movq	(%rbx), %rdi
	call	enchant_broker_free_dict
	movq	(%rbx), %rdi
	call	enchant_broker_free
	movq	%rbx, %rsi
	movl	$16, %edi
	call	g_slice_free1
	movq	8(%rbp), %rbp
	testq	%rbp, %rbp
	jne	.L37
.L34:
	movq	languages(%rip), %rdi
	call	g_list_free
	movq	$0, languages(%rip)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	ret
.LFE46:
	.size	spell_notify_languages_cb, .-spell_notify_languages_cb
	.section	.rodata.str1.1
.LC10:
	.string	"No languages to check against"
	.section	.rodata.str1.8
	.align 8
.LC11:
	.string	"Not spell checking word:'%s', it is all digits"
	.text
	.p2align 4,,15
.globl spell_check
	.type	spell_check, @function
spell_check:
.LFB52:
	pushq	%r13
.LCFI18:
	pushq	%r12
.LCFI19:
	movq	%rdi, %r12
	pushq	%rbp
.LCFI20:
	pushq	%rbx
.LCFI21:
	subq	$8, %rsp
.LCFI22:
	testq	%rdi, %rdi
	je	.L40
	call	spell_setup_languages
	movq	languages(%rip), %rbp
	testq	%rbp, %rbp
	je	.L41
	cmpb	$0, (%r12)
	je	.L43
	movq	g_utf8_skip(%rip), %r13
	movq	%r12, %rbx
.L45:
	movq	%rbx, %rdi
	call	g_utf8_get_char
	movl	%eax, %edi
	call	g_unichar_isdigit
	movl	%eax, %edx
	movzbl	(%rbx), %eax
	movsbq	(%r13,%rax),%rax
	addq	%rax, %rbx
	cmpb	$0, (%rbx)
	jne	.L54
	testl	%edx, %edx
	je	.L47
.L43:
	movq	%r12, %rdx
	xorl	%eax, %eax
	movl	$.LC11, %esi
	movl	$.LC2, %edi
	call	debug_impl
	addq	$8, %rsp
	movl	$1, %eax
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
	.p2align 4,,10
	.p2align 3
.L54:
	testl	%edx, %edx
	jne	.L45
.L47:
	movq	%r12, %rdi
	call	strlen
	movslq	%eax,%rbx
	.p2align 4,,10
	.p2align 3
.L49:
	movq	(%rbp), %rax
	movq	%rbx, %rdx
	movq	%r12, %rsi
	movq	8(%rax), %rdi
	call	enchant_dict_check
	testl	%eax, %eax
	je	.L55
	movq	8(%rbp), %rbp
	testq	%rbp, %rbp
	jne	.L49
	xorl	%eax, %eax
.L44:
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
	.p2align 4,,10
	.p2align 3
.L55:
	addq	$8, %rsp
	movb	$1, %al
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
	.p2align 4,,10
	.p2align 3
.L40:
	movl	$.LC8, %edx
	movl	$__PRETTY_FUNCTION__.10587, %esi
	xorl	%edi, %edi
	call	g_return_if_fail_warning
	addq	$8, %rsp
	xorl	%eax, %eax
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L41:
	xorl	%eax, %eax
	movl	$.LC10, %esi
	movl	$.LC2, %edi
	call	debug_impl
	movl	$1, %eax
	jmp	.L44
.LFE52:
	.size	spell_check, .-spell_check
	.p2align 4,,15
.globl spell_get_language_codes
	.type	spell_get_language_codes, @function
spell_get_language_codes:
.LFB50:
	pushq	%rbx
.LCFI23:
	subq	$16, %rsp
.LCFI24:
	movq	$0, 8(%rsp)
	call	enchant_broker_init
	leaq	8(%rsp), %rdx
	movq	%rax, %rbx
	movq	%rax, %rdi
	movl	$enumerate_dicts, %esi
	call	enchant_broker_list_dicts
	movq	%rbx, %rdi
	call	enchant_broker_free
	movq	8(%rsp), %rax
	addq	$16, %rsp
	popq	%rbx
	ret
.LFE50:
	.size	spell_get_language_codes, .-spell_get_language_codes
	.p2align 4,,15
	.type	enumerate_dicts, @function
enumerate_dicts:
.LFB49:
	pushq	%rbp
.LCFI25:
	movq	%r8, %rbp
	pushq	%rbx
.LCFI26:
	subq	$8, %rsp
.LCFI27:
	call	g_strdup
	movl	$95, %esi
	movq	%rax, %rdi
	movq	%rax, %rbx
	call	strchr
	testq	%rax, %rax
	je	.L59
	movb	$0, (%rax)
.L59:
	movq	(%rbp), %rdi
	movl	$strcmp, %edx
	movq	%rbx, %rsi
	call	g_list_find_custom
	testq	%rax, %rax
	je	.L60
	addq	$8, %rsp
	movq	%rbx, %rdi
	popq	%rbx
	popq	%rbp
	jmp	g_free
	.p2align 4,,10
	.p2align 3
.L60:
	movq	%rbx, %rdi
	call	g_strdup
	movq	(%rbp), %rdi
	movq	%rax, %rsi
	call	g_list_append
	movq	%rax, (%rbp)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	ret
.LFE49:
	.size	enumerate_dicts, .-enumerate_dicts
	.section	.rodata.str1.1
.LC12:
	.string	"/usr/share/locale"
.LC13:
	.string	"iso_639"
.LC14:
	.string	"UTF-8"
	.section	.rodata.str1.8
	.align 8
.LC15:
	.string	"/usr/share/xml/iso-codes/iso_639.xml"
	.section	.rodata.str1.1
.LC16:
	.string	"Failed to parse '%s': %s"
.LC17:
	.string	"Failed to load '%s': %s"
.LC18:
	.string	"code != NULL"
	.text
	.p2align 4,,15
.globl spell_get_language_name
	.type	spell_get_language_name, @function
spell_get_language_name:
.LFB48:
	movq	%rbx, -24(%rsp)
.LCFI28:
	movq	%rbp, -16(%rsp)
.LCFI29:
	movq	%rdi, %rbx
	movq	%r12, -8(%rsp)
.LCFI30:
	subq	$88, %rsp
.LCFI31:
	testq	%rdi, %rdi
	je	.L63
	cmpq	$0, iso_code_names(%rip)
	je	.L70
.L64:
	movq	iso_code_names(%rip), %rdi
	movq	%rbx, %rsi
	call	g_hash_table_lookup
	movq	%rax, %rcx
	xorl	%eax, %eax
	testq	%rcx, %rcx
	je	.L68
	movl	$5, %edx
	movq	%rcx, %rsi
	movl	$.LC13, %edi
	call	dcgettext
.L68:
	movq	64(%rsp), %rbx
	movq	72(%rsp), %rbp
	movq	80(%rsp), %r12
	addq	$88, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L63:
	movl	$.LC18, %edx
	movl	$__PRETTY_FUNCTION__.10541, %esi
	xorl	%edi, %edi
	call	g_return_if_fail_warning
	xorl	%eax, %eax
	jmp	.L68
	.p2align 4,,10
	.p2align 3
.L70:
	movl	$g_free, %ecx
	movl	$g_str_equal, %esi
	movl	$g_str_hash, %edi
	movq	%rcx, %rdx
	movq	$0, 56(%rsp)
	leaq	56(%rsp), %r12
	call	g_hash_table_new_full
	movl	$.LC12, %esi
	movl	$.LC13, %edi
	movq	%rax, iso_code_names(%rip)
	call	bindtextdomain
	movl	$.LC14, %esi
	movl	$.LC13, %edi
	call	bind_textdomain_codeset
	leaq	40(%rsp), %rdx
	leaq	48(%rsp), %rsi
	movq	%r12, %rcx
	movl	$.LC15, %edi
	call	g_file_get_contents
	testl	%eax, %eax
	je	.L65
	xorl	%ecx, %ecx
	xorl	%edx, %edx
	xorl	%esi, %esi
	movq	%rsp, %rdi
	movq	$spell_iso_codes_parse_start_tag, (%rsp)
	movq	$0, 8(%rsp)
	movq	$0, 16(%rsp)
	movq	$0, 24(%rsp)
	movq	$0, 32(%rsp)
	call	g_markup_parse_context_new
	movq	40(%rsp), %rdx
	movq	48(%rsp), %rsi
	movq	%r12, %rcx
	movq	%rax, %rdi
	movq	%rax, %rbp
	call	g_markup_parse_context_parse
	testl	%eax, %eax
	je	.L71
.L66:
	movq	%rbp, %rdi
	call	g_markup_parse_context_free
	movq	48(%rsp), %rdi
	call	g_free
	jmp	.L64
	.p2align 4,,10
	.p2align 3
.L65:
	movq	56(%rsp), %rax
	xorl	%edi, %edi
	movl	$.LC15, %ecx
	movl	$.LC17, %edx
	movl	$16, %esi
	movq	8(%rax), %r8
	xorl	%eax, %eax
	call	g_log
	movq	56(%rsp), %rdi
	testq	%rdi, %rdi
	je	.L64
	call	g_error_free
	.p2align 4,,3
	.p2align 3
	jmp	.L64
	.p2align 4,,10
	.p2align 3
.L71:
	movq	56(%rsp), %rax
	xorl	%edi, %edi
	movl	$.LC15, %ecx
	movl	$.LC16, %edx
	movl	$16, %esi
	movq	8(%rax), %r8
	xorl	%eax, %eax
	call	g_log
	movq	56(%rsp), %rdi
	testq	%rdi, %rdi
	je	.L66
	call	g_error_free
	.p2align 4,,3
	.p2align 3
	jmp	.L66
.LFE48:
	.size	spell_get_language_name, .-spell_get_language_name
	.section	.rodata.str1.1
.LC19:
	.string	"iso_639_entry"
.LC20:
	.string	"iso_639_1_code"
.LC21:
	.string	"iso_639_2B_code"
.LC22:
	.string	"iso_639_2T_code"
.LC23:
	.string	"name"
	.text
	.p2align 4,,15
	.type	spell_iso_codes_parse_start_tag, @function
spell_iso_codes_parse_start_tag:
.LFB44:
	pushq	%r15
.LCFI32:
	movq	%rsi, %rdi
	movl	$.LC19, %esi
	pushq	%r14
.LCFI33:
	movq	%rcx, %r14
	pushq	%r13
.LCFI34:
	pushq	%r12
.LCFI35:
	pushq	%rbp
.LCFI36:
	pushq	%rbx
.LCFI37:
	subq	$40, %rsp
.LCFI38:
	movq	%rdx, 8(%rsp)
	call	g_str_equal
	testl	%eax, %eax
	je	.L83
	cmpq	$0, 8(%rsp)
	je	.L83
	testq	%r14, %r14
	.p2align 4,,4
	.p2align 3
	je	.L83
	movq	8(%rsp), %rax
	movq	(%rax), %rdi
	testq	%rdi, %rdi
	je	.L83
	cmpq	$0, (%r14)
	je	.L83
	movq	$0, 24(%rsp)
	xorl	%r15d, %r15d
	movq	$0, 32(%rsp)
	movq	$0, 16(%rsp)
	xorl	%ebx, %ebx
	jmp	.L80
	.p2align 4,,10
	.p2align 3
.L84:
	movq	(%r14,%rbx), %rbp
	movq	24(%rsp), %r12
	movq	%r15, %r13
	cmpb	$0, (%rbp)
	jne	.L76
.L75:
	movq	24(%rsp), %r12
	movq	32(%rsp), %rbp
	movq	%r15, %r13
.L76:
	movq	8(%rsp), %rax
	movq	8(%rax,%rbx), %rdi
	testq	%rdi, %rdi
	je	.L79
	movq	8(%r14,%rbx), %rax
	addq	$8, %rbx
	testq	%rax, %rax
	je	.L79
	movq	%r12, 24(%rsp)
	movq	%r13, %r15
	movq	%rbp, 32(%rsp)
.L80:
	movl	$.LC20, %esi
	call	g_str_equal
	testl	%eax, %eax
	jne	.L84
	movq	8(%rsp), %rax
	movl	$.LC21, %esi
	movq	(%rax,%rbx), %rdi
	call	g_str_equal
	testl	%eax, %eax
	je	.L77
	movq	(%r14,%rbx), %r12
	movq	%r15, %r13
	movq	32(%rsp), %rbp
	cmpb	$0, (%r12)
	jne	.L76
	jmp	.L75
	.p2align 4,,10
	.p2align 3
.L79:
	cmpq	$0, 16(%rsp)
	je	.L83
	testq	%rbp, %rbp
	.p2align 4,,6
	.p2align 3
	je	.L81
	movq	16(%rsp), %rdi
	call	g_strdup
	movq	%rbp, %rdi
	movq	%rax, %rbx
	call	g_strdup
	movq	iso_code_names(%rip), %rdi
	movq	%rax, %rsi
	movq	%rbx, %rdx
	call	g_hash_table_insert
.L81:
	testq	%r12, %r12
	je	.L82
	movq	16(%rsp), %rdi
	call	g_strdup
	movq	%r12, %rdi
	movq	%rax, %rbx
	call	g_strdup
	movq	iso_code_names(%rip), %rdi
	movq	%rax, %rsi
	movq	%rbx, %rdx
	call	g_hash_table_insert
.L82:
	testq	%r13, %r13
	jne	.L85
	.p2align 4,,10
	.p2align 3
.L83:
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
.L77:
	movq	8(%rsp), %rax
	movl	$.LC22, %esi
	movq	(%rax,%rbx), %rdi
	call	g_str_equal
	testl	%eax, %eax
	je	.L78
	movq	(%r14,%rbx), %r13
	movq	24(%rsp), %r12
	movq	32(%rsp), %rbp
	cmpb	$0, (%r13)
	jne	.L76
	jmp	.L75
	.p2align 4,,10
	.p2align 3
.L85:
	movq	16(%rsp), %rdi
	call	g_strdup
	movq	%r13, %rdi
	movq	%rax, %rbx
	call	g_strdup
	movq	%rbx, %rdx
	movq	iso_code_names(%rip), %rdi
	addq	$40, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	movq	%rax, %rsi
	jmp	g_hash_table_insert
	.p2align 4,,10
	.p2align 3
.L78:
	movq	8(%rsp), %rax
	movl	$.LC23, %esi
	movq	(%rax,%rbx), %rdi
	call	g_str_equal
	testl	%eax, %eax
	je	.L75
	movq	(%r14,%rbx), %rax
	movq	24(%rsp), %r12
	movq	%r15, %r13
	movq	32(%rsp), %rbp
	movq	%rax, 16(%rsp)
	jmp	.L76
.LFE44:
	.size	spell_iso_codes_parse_start_tag, .-spell_iso_codes_parse_start_tag
	.local	languages
	.comm	languages,8,8
	.section	.rodata
	.align 16
	.type	__PRETTY_FUNCTION__.10623, @object
	.size	__PRETTY_FUNCTION__.10623, 22
__PRETTY_FUNCTION__.10623:
	.string	"spell_get_suggestions"
	.local	conf_notify_inited
	.comm	conf_notify_inited,4,4
	.type	__PRETTY_FUNCTION__.10587, @object
	.size	__PRETTY_FUNCTION__.10587, 12
__PRETTY_FUNCTION__.10587:
	.string	"spell_check"
	.local	iso_code_names
	.comm	iso_code_names,8,8
	.align 16
	.type	__PRETTY_FUNCTION__.10541, @object
	.size	__PRETTY_FUNCTION__.10541, 24
__PRETTY_FUNCTION__.10541:
	.string	"spell_get_language_name"
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
	.long	.LFB55
	.long	.LFE55-.LFB55
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI0-.LFB55
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
	.long	.LFB51
	.long	.LFE51-.LFB51
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI1-.LFB51
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
	.long	.LFB54
	.long	.LFE54-.LFB54
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI2-.LFB54
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE5:
.LSFDE7:
	.long	.LEFDE7-.LASFDE7
.LASFDE7:
	.long	.LASFDE7-.Lframe1
	.long	.LFB47
	.long	.LFE47-.LFB47
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI7-.LFB47
	.byte	0xe
	.uleb128 0x40
	.byte	0x8d
	.uleb128 0x2
	.byte	0x8c
	.uleb128 0x3
	.byte	0x86
	.uleb128 0x4
	.byte	0x83
	.uleb128 0x5
	.align 8
.LEFDE7:
.LSFDE9:
	.long	.LEFDE9-.LASFDE9
.LASFDE9:
	.long	.LASFDE9-.Lframe1
	.long	.LFB53
	.long	.LFE53-.LFB53
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI8-.LFB53
	.byte	0xe
	.uleb128 0x10
	.byte	0x8f
	.uleb128 0x2
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
	.byte	0x4
	.long	.LCFI12-.LCFI11
	.byte	0xe
	.uleb128 0x30
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
	.byte	0x86
	.uleb128 0x6
	.byte	0x8c
	.uleb128 0x5
	.byte	0x8d
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x3
	.align 8
.LEFDE9:
.LSFDE11:
	.long	.LEFDE11-.LASFDE11
.LASFDE11:
	.long	.LASFDE11-.Lframe1
	.long	.LFB46
	.long	.LFE46-.LFB46
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI15-.LFB46
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI16-.LCFI15
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI17-.LCFI16
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x3
	.byte	0x86
	.uleb128 0x2
	.align 8
.LEFDE11:
.LSFDE13:
	.long	.LEFDE13-.LASFDE13
.LASFDE13:
	.long	.LASFDE13-.Lframe1
	.long	.LFB52
	.long	.LFE52-.LFB52
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI18-.LFB52
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI19-.LCFI18
	.byte	0xe
	.uleb128 0x18
	.byte	0x8c
	.uleb128 0x3
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI20-.LCFI19
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI21-.LCFI20
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.long	.LCFI22-.LCFI21
	.byte	0xe
	.uleb128 0x30
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.align 8
.LEFDE13:
.LSFDE15:
	.long	.LEFDE15-.LASFDE15
.LASFDE15:
	.long	.LASFDE15-.Lframe1
	.long	.LFB50
	.long	.LFE50-.LFB50
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI23-.LFB50
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI24-.LCFI23
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE15:
.LSFDE17:
	.long	.LEFDE17-.LASFDE17
.LASFDE17:
	.long	.LASFDE17-.Lframe1
	.long	.LFB49
	.long	.LFE49-.LFB49
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI25-.LFB49
	.byte	0xe
	.uleb128 0x10
	.byte	0x86
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI26-.LCFI25
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI27-.LCFI26
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x3
	.align 8
.LEFDE17:
.LSFDE19:
	.long	.LEFDE19-.LASFDE19
.LASFDE19:
	.long	.LASFDE19-.Lframe1
	.long	.LFB48
	.long	.LFE48-.LFB48
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI29-.LFB48
	.byte	0x86
	.uleb128 0x3
	.byte	0x83
	.uleb128 0x4
	.byte	0x4
	.long	.LCFI31-.LCFI29
	.byte	0xe
	.uleb128 0x60
	.byte	0x8c
	.uleb128 0x2
	.align 8
.LEFDE19:
.LSFDE21:
	.long	.LEFDE21-.LASFDE21
.LASFDE21:
	.long	.LASFDE21-.Lframe1
	.long	.LFB44
	.long	.LFE44-.LFB44
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI32-.LFB44
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI33-.LCFI32
	.byte	0xe
	.uleb128 0x18
	.byte	0x8e
	.uleb128 0x3
	.byte	0x8f
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI34-.LCFI33
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI35-.LCFI34
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.long	.LCFI36-.LCFI35
	.byte	0xe
	.uleb128 0x30
	.byte	0x4
	.long	.LCFI37-.LCFI36
	.byte	0xe
	.uleb128 0x38
	.byte	0x4
	.long	.LCFI38-.LCFI37
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
	.align 8
.LEFDE21:
	.ident	"GCC: (SUSE Linux) 4.3.2 [gcc-4_3-branch revision 141291]"
	.section	.comment.SUSE.OPTs,"MS",@progbits,1
	.ascii	"OspWg"
	.section	.note.GNU-stack,"",@progbits
