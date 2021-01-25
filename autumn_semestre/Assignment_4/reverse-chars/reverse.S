  .text
  .global main
main:
  push {r4-r8, lr}
  ldr r4, =stdin
  ldr r5, =stdout
  mov r6, #128 ;@ max_length
  mov r7, #0 ;@ curr_length
  mov r0, r6
  bl malloc
  mov r8, r0 ;@ allocated array
  input_loop:
    ldr r0, [r4]
    bl fgetc
    mov r2, r0
    cmp r2, #-1
    beq print
    str r2, [r8, r7]
    add r7, #1
    cmp r7, r6
    bge resize
    b input_loop

resize:
  mov r3, #2
  mul r6, r3
  mov r1, r6
  mov r0, r8
  bl realloc
  mov r8, r0
  b input_loop

print:
  mov r4, r7 ;@ loop counter r4 = cur_length
  sub r4, #1
  print_loop:
    cmp r4, #-1
    beq exit
    ldr r0, [r8, r4] // r0 = *(r8 + r4)
    ldr r1, [r5] // r1 = *r5
    bl fputc
    sub r4, #1
    b print_loop

exit:
  mov r0, r8
  bl free
  mov r0, #0
  pop {r4-r8, lr}
  bx lr