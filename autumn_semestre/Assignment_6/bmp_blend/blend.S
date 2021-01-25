	.file	"blender.cpp"
	.intel_syntax noprefix
	.text
	.p2align 4
	.globl	_Z10AlphaBlendPhS_S_
	.type	_Z10AlphaBlendPhS_S_, @function
_Z10AlphaBlendPhS_S_:
.LFB0:
	.cfi_startproc
	endbr64
	movzx	eax, BYTE PTR [rsi]
	pxor	xmm4, xmm4
	pxor	xmm0, xmm0
	pxor	xmm2, xmm2
	movsd	xmm3, QWORD PTR .LC0[rip]
	movsd	xmm1, QWORD PTR .LC1[rip]
	cvtsi2sd	xmm4, eax
	movzx	eax, BYTE PTR [rdi]
	movapd	xmm5, xmm1
	pxor	xmm1, xmm1
	cvtsi2sd	xmm0, eax
	movzx	eax, BYTE PTR 1[rdi]
	divsd	xmm4, xmm3
	cvtsi2sd	xmm2, eax
	movzx	eax, BYTE PTR 1[rsi]
	cvtsi2sd	xmm1, eax
	subsd	xmm5, xmm4
	mulsd	xmm1, xmm4
	mulsd	xmm0, xmm5
	mulsd	xmm2, xmm5
	divsd	xmm0, xmm3
	addsd	xmm2, xmm1
	pxor	xmm1, xmm1
	addsd	xmm0, xmm4
	divsd	xmm2, xmm0
	cvttsd2si	eax, xmm2
	pxor	xmm2, xmm2
	mov	BYTE PTR 1[rdx], al
	movzx	eax, BYTE PTR 2[rdi]
	cvtsi2sd	xmm2, eax
	movzx	eax, BYTE PTR 2[rsi]
	cvtsi2sd	xmm1, eax
	mulsd	xmm2, xmm5
	mulsd	xmm1, xmm4
	addsd	xmm2, xmm1
	pxor	xmm1, xmm1
	divsd	xmm2, xmm0
	cvttsd2si	eax, xmm2
	pxor	xmm2, xmm2
	mov	BYTE PTR 2[rdx], al
	movzx	eax, BYTE PTR 3[rdi]
	cvtsi2sd	xmm1, eax
	movzx	eax, BYTE PTR 3[rsi]
	cvtsi2sd	xmm2, eax
	mulsd	xmm1, xmm5
	mulsd	xmm4, xmm2
	addsd	xmm1, xmm4
	divsd	xmm1, xmm0
	mulsd	xmm0, xmm3
	cvttsd2si	eax, xmm1
	mov	BYTE PTR 3[rdx], al
	cvttsd2si	eax, xmm0
	mov	BYTE PTR [rdx], al
	ret
	.cfi_endproc
.LFE0:
	.size	_Z10AlphaBlendPhS_S_, .-_Z10AlphaBlendPhS_S_
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC0:
	.long	0
	.long	1081073664
	.align 8
.LC1:
	.long	0
	.long	1072693248
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
