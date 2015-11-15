	.file	"switch_prediction.c"
	.section	.rodata
.LC0:
	.string	"1"
.LC1:
	.string	"101"
.LC2:
	.string	"10"
.LC3:
	.string	"default"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	$101, -4(%rbp)
	movl	-4(%rbp), %eax
	cmpl	$10, %eax
	je	.L4
	cmpl	$101, %eax
	je	.L5
	cmpl	$1, %eax
	jne	.L8
.L3:
	movl	$.LC0, %edi
	call	puts
	jmp	.L6
.L5:
	movl	$.LC1, %edi
	call	puts
	jmp	.L6
.L4:
	movl	$.LC2, %edi
	call	puts
	jmp	.L6
.L8:
	movl	$.LC3, %edi
	call	puts
	nop
.L6:
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Debian 4.7.2-5) 4.7.2"
	.section	.note.GNU-stack,"",@progbits
