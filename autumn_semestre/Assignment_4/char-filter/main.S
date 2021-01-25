  .text
  .global main
main:
  push {r4-r5, lr}
  ldr r4, =stdin
  ldr r5, =stdout
check_digit:
  cmp r2, #9
  bls print
input_loop:
  ldr r0, [r4]
  bl fgetc
  cmn r0, #1
  sub r2, r0, #48
  bne check_digit
  mov r0, #0
  pop {r4-r5, lr}
  bx lr
print:
  ldr r1, [r5]
  bl fputc
  b input_loop
