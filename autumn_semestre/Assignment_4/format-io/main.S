	.data
prompt: .ascii "%d%d\0"
format: .ascii "%d\n\0"
	.text
	.global main
main:
	push {lr}
	sub sp, #8
	ldr r0, =prompt
	mov r1, sp
	mov r2, sp
	add r2, #4
	bl scanf
	ldr r1, [sp]
	ldr r2, [sp, #4]
	add r1, r1, r2
	ldr r0, =format
	bl printf
	mov r0, #0
	add sp, #8
	pop {lr}
	bx lr
