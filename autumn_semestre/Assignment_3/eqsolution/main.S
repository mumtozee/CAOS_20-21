// int solve (int A, int B, int C, int D);
  .text
  .global solve
solve:
  push {r4, r5}
  mov r4, #0
  pickup_loop:
    cmp r4, #255
    beq pickup_exit
    mov r5, #0
    mla r5, r0, r4, r1
    mla r5, r4, r5, r2
    mla r5, r4, r5, r3
    cmp r5, #0
    beq pickup_exit
    add r4, r4, #1
    b pickup_loop
  pickup_exit:
    mov r0, r4
    pop {r4, r5}
    bx lr
