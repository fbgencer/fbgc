	.file	"fbgc.c"
	.text
	.comm	fbgc_memb,48,32
	.section	.rodata
.LC0:
	.string	"r"
.LC2:
	.string	"Execution time [LEXER] :%f\n"
.LC3:
	.string	"Execution time [PARSER] :%f\n"
	.align 8
.LC4:
	.string	"Execution time [INTERPRETER] :%f\n"
.LC5:
	.string	"Total ex time %f\n"
	.text
	.type	compile_file, @function
compile_file:
.LFB5:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$1104, %rsp
	movq	%rdi, -1080(%rbp)
	movq	%rsi, -1088(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	call	clock@PLT
	movq	%rax, -1056(%rbp)
	leaq	-1008(%rbp), %rdx
	movl	$0, %eax
	movl	$125, %ecx
	movq	%rdx, %rdi
	rep stosq
	movq	-1088(%rbp), %rax
	leaq	.LC0(%rip), %rsi
	movq	%rax, %rdi
	call	fopen@PLT
	movq	%rax, -1048(%rbp)
	jmp	.L2
.L4:
	movzbl	-1008(%rbp), %eax
	cmpb	$58, %al
	jne	.L3
	movzbl	-1007(%rbp), %eax
	cmpb	$62, %al
	jne	.L3
	jmp	.L2
.L3:
	movzbl	-1008(%rbp), %eax
	testb	%al, %al
	je	.L2
	leaq	-1008(%rbp), %rdx
	leaq	-1080(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	regex_lexer@PLT
.L2:
	movq	-1048(%rbp), %rdx
	leaq	-1008(%rbp), %rax
	movl	$1000, %esi
	movq	%rax, %rdi
	call	fbgc_getline_from_file@PLT
	testq	%rax, %rax
	jne	.L4
	call	clock@PLT
	movq	%rax, -1040(%rbp)
	movq	-1040(%rbp), %rax
	subq	-1056(%rbp), %rax
	cvtsi2sdq	%rax, %xmm0
	movsd	.LC1(%rip), %xmm1
	divsd	%xmm1, %xmm0
	movsd	%xmm0, -1032(%rbp)
	call	clock@PLT
	movq	%rax, -1056(%rbp)
	leaq	-1080(%rbp), %rax
	movq	%rax, %rdi
	call	parser@PLT
	movzbl	%al, %eax
	movl	%eax, -1060(%rbp)
	call	clock@PLT
	movq	%rax, -1040(%rbp)
	movq	-1040(%rbp), %rax
	subq	-1056(%rbp), %rax
	cvtsi2sdq	%rax, %xmm0
	movsd	.LC1(%rip), %xmm1
	divsd	%xmm1, %xmm0
	movsd	%xmm0, -1024(%rbp)
	call	clock@PLT
	movq	%rax, -1056(%rbp)
	call	clock@PLT
	movq	%rax, -1040(%rbp)
	movq	-1040(%rbp), %rax
	subq	-1056(%rbp), %rax
	cvtsi2sdq	%rax, %xmm0
	movsd	.LC1(%rip), %xmm1
	divsd	%xmm1, %xmm0
	movsd	%xmm0, -1016(%rbp)
	movq	-1032(%rbp), %rax
	movq	%rax, -1096(%rbp)
	movsd	-1096(%rbp), %xmm0
	leaq	.LC2(%rip), %rsi
	movl	$111, %edi
	movl	$1, %eax
	call	cprintf@PLT
	movq	-1024(%rbp), %rax
	movq	%rax, -1096(%rbp)
	movsd	-1096(%rbp), %xmm0
	leaq	.LC3(%rip), %rsi
	movl	$111, %edi
	movl	$1, %eax
	call	cprintf@PLT
	movq	-1016(%rbp), %rax
	movq	%rax, -1096(%rbp)
	movsd	-1096(%rbp), %xmm0
	leaq	.LC4(%rip), %rsi
	movl	$111, %edi
	movl	$1, %eax
	call	cprintf@PLT
	movsd	-1032(%rbp), %xmm0
	addsd	-1024(%rbp), %xmm0
	addsd	-1016(%rbp), %xmm0
	leaq	.LC5(%rip), %rsi
	movl	$111, %edi
	movl	$1, %eax
	call	cprintf@PLT
	movq	-1048(%rbp), %rax
	movq	%rax, %rdi
	call	fclose@PLT
	nop
	movq	-8(%rbp), %rax
	xorq	%fs:40, %rax
	je	.L5
	call	__stack_chk_fail@PLT
.L5:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5:
	.size	compile_file, .-compile_file
	.type	compile_one_line, @function
compile_one_line:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-16(%rbp), %rdx
	leaq	-8(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	regex_lexer@PLT
	testb	%al, %al
	je	.L8
	leaq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	parser@PLT
	testb	%al, %al
	je	.L8
	leaq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	interpreter@PLT
.L8:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	compile_one_line, .-compile_one_line
	.section	.rodata
	.align 8
.LC6:
	.string	"\n\n\n\n\n[=======================================================================]\n"
.LC7:
	.string	"Enter a file!\n"
	.align 8
.LC8:
	.string	"\n[=======================================================================]\n\n\n\n\n\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB7:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movl	%edi, -20(%rbp)
	movq	%rsi, -32(%rbp)
	leaq	.LC6(%rip), %rsi
	movl	$110, %edi
	movl	$0, %eax
	call	cprintf@PLT
	cmpl	$1, -20(%rbp)
	jle	.L10
	movl	$0, %eax
	call	initialize_fbgc_memory_block@PLT
	call	new_fbgc_field_object@PLT
	movq	%rax, -8(%rbp)
	movq	-32(%rbp), %rax
	addq	$8, %rax
	movq	(%rax), %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	compile_file
	movl	$0, %eax
	call	free_fbgc_memory_block@PLT
	jmp	.L11
.L10:
	leaq	.LC7(%rip), %rsi
	movl	$111, %edi
	movl	$0, %eax
	call	cprintf@PLT
.L11:
	leaq	.LC8(%rip), %rsi
	movl	$110, %edi
	movl	$0, %eax
	call	cprintf@PLT
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	main, .-main
	.section	.rodata
	.align 8
.LC1:
	.long	0
	.long	1093567616
	.ident	"GCC: (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0"
	.section	.note.GNU-stack,"",@progbits
