  .text
  .global R
  .comm R, 4, 4
  .global calculate
calculate:
  push {r4, r5}
  ldr r0, =R
  ldr r1, =A
  ldr r1, [r1]
  ldr r2, =B
  ldr r2, [r2]
  ldr r3, =C
  ldr r3, [r3]
  ldr r4, =D
  ldr r4, [r4]
  mul r5, r3, r4
  mla r5, r1, r2, r5
  str r5, [r0]
  pop {r4, r5}
  bx lr
