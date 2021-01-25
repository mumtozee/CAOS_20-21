	.file	"main.c"
	.intel_syntax noprefix
	.text
	.p2align 4
	.globl	my_sin
	.type	my_sin, @function
my_sin:
.LFB0:
	.cfi_startproc
	endbr64
	ucomisd	xmm0, QWORD PTR .LC1[rip]
	movapd	xmm2, xmm0
	jp	.L6
	je	.L8
.L6:
	movapd	xmm7, xmm2
	movsd	xmm3, QWORD PTR .LC0[rip]
	movapd	xmm0, xmm2
	movq	xmm6, QWORD PTR .LC2[rip]
	mulsd	xmm7, xmm2
	mov	edx, 3
	movapd	xmm4, xmm3
	.p2align 4,,10
	.p2align 3
.L7:
	lea	eax, -1[rdx]
	pxor	xmm1, xmm1
	xorpd	xmm4, xmm6
	imul	eax, edx
	mulsd	xmm2, xmm7
	movapd	xmm5, xmm0
	add	edx, 2
	cvtsi2sd	xmm1, eax
	mulsd	xmm3, xmm1
	movapd	xmm1, xmm2
	divsd	xmm1, xmm3
	mulsd	xmm1, xmm4
	addsd	xmm0, xmm1
	ucomisd	xmm0, xmm5
	jp	.L7
	jne	.L7
	ret
	.p2align 4,,10
	.p2align 3
.L8:
	ret
	.cfi_endproc
.LFE0:
	.size	my_sin, .-my_sin
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC0:
	.long	0
	.long	1072693248
	.align 8
.LC1:
	.long	0
	.long	0
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC2:
	.long	0
	.long	-2147483648
	.long	0
	.long	0
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
