	.file	"main.c"
	.intel_syntax noprefix
	.text
	.p2align 4
	.globl	_start
	.type	_start, @function
_start:
.LFB6:
	.cfi_startproc
	endbr64
	xor	esi, esi
	mov	edi, 60
	xor	eax, eax
	jmp	syscall@PLT
	.cfi_endproc
.LFE6:
	.size	_start, .-_start
	.ident	"GCC: (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
