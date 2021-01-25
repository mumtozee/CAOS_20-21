	.text
	.global	summ
summ:
	mov r3, #0
	push {ip}
	while_start:
		cmp r3, r1
		beq while_end
		ldr ip, [r2, r3, lsl #2]
		add r0, r0, ip
		add r3, r3, #1
		b while_start
	while_end:
		pop {ip}
		bx lr
