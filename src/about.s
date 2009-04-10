	.file	"about.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"\n\n"
.LC1:
	.string	"translator-credits"
.LC2:
	.string	"Copyright \302\251 2009 Kaity G. B."
.LC3:
	.string	"A GNOME client for Twitter."
.LC4:
	.string	"comments"
.LC5:
	.string	"artists"
.LC6:
	.string	"authors"
.LC7:
	.string	"greet-tweet-know"
.LC8:
	.string	"logo-icon-name"
.LC9:
	.string	"wrap-license"
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC10:
	.string	"http://uberChicGeekChick.Com/?projects=Greet-Tweet-Know"
	.section	.rodata.str1.1
.LC11:
	.string	"website"
.LC12:
	.string	"0.0.eCa666"
.LC13:
	.string	"version"
.LC14:
	.string	"license"
.LC15:
	.string	"copyright"
	.text
	.p2align 4,,15
.globl about_dialog_new
	.type	about_dialog_new, @function
about_dialog_new:
.LFB38:
	pushq	%r14
.LCFI0:
	xorl	%edx, %edx
	movq	%rdi, %r14
	xorl	%esi, %esi
	movl	$about_dialog_activate_link_cb, %edi
	pushq	%r13
.LCFI1:
	pushq	%r12
.LCFI2:
	pushq	%rbx
.LCFI3:
	subq	$136, %rsp
.LCFI4:
	call	gtk_about_dialog_set_url_hook
	movq	license+16(%rip), %rsi
	movl	$5, %edx
	xorl	%edi, %edi
	call	dcgettext
	movq	license+8(%rip), %rsi
	movl	$5, %edx
	xorl	%edi, %edi
	movq	%rax, %r12
	call	dcgettext
	movq	license(%rip), %rsi
	movl	$5, %edx
	xorl	%edi, %edi
	movq	%rax, %rbx
	call	dcgettext
	movl	$.LC0, %r9d
	movq	%r12, %r8
	movq	%rax, %rdi
	movq	%r9, %rcx
	movq	%r9, %rsi
	movq	%rbx, %rdx
	xorl	%eax, %eax
	movq	$0, (%rsp)
	call	g_strconcat
	movl	$5, %edx
	movl	$.LC1, %esi
	xorl	%edi, %edi
	movq	%rax, %r13
	call	dcgettext
	movl	$5, %edx
	movl	$.LC2, %esi
	xorl	%edi, %edi
	movq	%rax, %r12
	call	dcgettext
	movl	$5, %edx
	movl	$.LC3, %esi
	xorl	%edi, %edi
	movq	%rax, %rbx
	call	dcgettext
	movq	%r14, %rdi
	movq	%rax, (%rsp)
	movl	$.LC4, %r9d
	movl	$artists, %r8d
	movl	$.LC5, %ecx
	movl	$authors, %edx
	movl	$.LC6, %esi
	xorl	%eax, %eax
	movq	%r12, 48(%rsp)
	movq	%r13, 32(%rsp)
	movq	%rbx, 16(%rsp)
	movq	$0, 120(%rsp)
	movq	$.LC7, 112(%rsp)
	movq	$.LC8, 104(%rsp)
	movl	$1, 96(%rsp)
	movq	$.LC9, 88(%rsp)
	movq	$.LC10, 80(%rsp)
	movq	$.LC11, 72(%rsp)
	movq	$.LC12, 64(%rsp)
	movq	$.LC13, 56(%rsp)
	movq	$.LC1, 40(%rsp)
	movq	$.LC14, 24(%rsp)
	movq	$.LC15, 8(%rsp)
	call	gtk_show_about_dialog
	addq	$136, %rsp
	movq	%r13, %rdi
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%r14
	jmp	g_free
.LFE38:
	.size	about_dialog_new, .-about_dialog_new
	.section	.rodata.str1.1
.LC16:
	.string	"Couldn't show URL: '%s'"
	.text
	.p2align 4,,15
	.type	about_dialog_activate_link_cb, @function
about_dialog_activate_link_cb:
.LFB37:
	pushq	%rbx
.LCFI5:
	movq	%rsi, %rbx
	xorl	%edx, %edx
	xorl	%esi, %esi
	movq	%rbx, %rdi
	call	g_app_info_launch_default_for_uri
	testl	%eax, %eax
	je	.L7
	popq	%rbx
	ret
	.p2align 4,,10
	.p2align 3
.L7:
	movq	%rbx, %rcx
	movl	$.LC16, %edx
	movl	$16, %esi
	popq	%rbx
	xorl	%edi, %edi
	jmp	g_log
.LFE37:
	.size	about_dialog_activate_link_cb, .-about_dialog_activate_link_cb
.globl authors
	.section	.rodata.str1.8
	.align 8
.LC17:
	.string	"Kaity G. B. <uberChick@uberChicGeekChick.Com>"
	.data
	.align 16
	.type	authors, @object
	.size	authors, 16
authors:
	.quad	.LC17
	.quad	0
.globl artists
	.align 16
	.type	artists, @object
	.size	artists, 16
artists:
	.quad	.LC17
	.quad	0
.globl license
	.section	.rodata.str1.8
	.align 8
.LC18:
	.ascii	"Unless explicitly acquired and licensed from Licensor under "
	.ascii	"anotherlicense, the contents of this "
	.string	"file are subject to the Reciprocal PublicLicense (\"RPL\") Version 1.5, or subsequent versions as allowed by the RPL,and You may not copy or use this file in either source code or executableform, except in compliance with the terms and conditions of the RPL."
	.align 8
.LC19:
	.ascii	"All software distributed under the RPL is provided strictly "
	.ascii	"on an\"AS IS\" basis, WITHOUT WARRANTY OF ANY KIND, EITHER E"
	.ascii	"XPRESS OR IMPLIED, ANDLICENS"
	.string	"OR HEREBY DISCLAIMS ALL SUCH WARRANTIES, INCLUDING WITHOUTLIMITATION, ANY WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULARPURPOSE, QUIET ENJOYMENT, OR NON-INFRINGEMENT. See the RPL for specificlanguage governing rights and limitations under the RPL."
	.align 8
.LC20:
	.string	"The User-Visible Attribution Notice below, when provided, must appear in eachuser-visible display as defined in Section 6.4 (d):"
	.align 8
.LC21:
	.ascii	"Initial art work including: design, logic, programming, and "
	.ascii	"graphics areCopyright (C) 2009 Kaity G. B. an"
	.string	"d released under the RPL where sapplicable.All materials not covered under the terms of the RPL are all stillCopyright (C) 2009 Kaity G. B. and released under the terms of theCreative Commons Non-Comercial, Attribution, Share-A-Like version 3.0 US license."
	.align 8
.LC22:
	.ascii	"Any & all data stored by this Software created, generated an"
	.ascii	"d/or uploaded by any Userand any data gathered by the Softwa"
	.ascii	"re that connects back to the User.  All data storedby this S"
	.ascii	"oftware is Copyright (C) of the User the data is connected t"
	.ascii	"o.Users may lisences their data under the terms of an OSI ap"
	.ascii	"proved or Creative Commonslicense.  Users must be allowed to"
	.ascii	" select their choice of license for each pie"
	.string	"ce of dataon an individual bases and cannot be blanketly applied to all of the Users.  The User mayselect a default license for their data.  All of the Software's data pertaining to eachUser must be fully accessible, exportable, and deletable to that User."
	.data
	.align 32
	.type	license, @object
	.size	license, 48
license:
	.quad	.LC18
	.quad	.LC19
	.quad	.LC20
	.quad	.LC21
	.quad	.LC22
	.quad	0
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
	.long	.LFB38
	.long	.LFE38-.LFB38
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI0-.LFB38
	.byte	0xe
	.uleb128 0x10
	.byte	0x8e
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI1-.LCFI0
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI2-.LCFI1
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI3-.LCFI2
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.long	.LCFI4-.LCFI3
	.byte	0xe
	.uleb128 0xb0
	.byte	0x83
	.uleb128 0x5
	.byte	0x8c
	.uleb128 0x4
	.byte	0x8d
	.uleb128 0x3
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
	.long	.LCFI5-.LFB37
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE3:
	.ident	"GCC: (SUSE Linux) 4.3.2 [gcc-4_3-branch revision 141291]"
	.section	.comment.SUSE.OPTs,"MS",@progbits,1
	.ascii	"OspWg"
	.section	.note.GNU-stack,"",@progbits
