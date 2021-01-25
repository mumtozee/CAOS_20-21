  .intel_syntax noprefix
  .text
  .globl my_sin

my_sin:
  endbr64
  push rbx
  mov rbx, 1
  movsd xmm1, xmm0  // answer
  cvtsi2sd xmm3, rbx  // sign
  mov rbx, 0
  cvtsi2sd xmm2, rbx  // old_answer
  movsd xmm4, xmm0 // part
  mov rbx, 1
  cvtsi2sd xmm5, rbx  // fac
  movsd xmm6, xmm0 // xn
  mov rax, 3  // i = 3
  mov rbx, 1
  mulsd xmm0, xmm0
  for_loop:
    comisd xmm1, xmm2 // result != old_result
    jz for_exit
    imul rbx, -1
    cvtsi2sd xmm3, rbx  // sign *= -1
    mov rsi, rax
    sub rsi, 1
    imul rsi, rax
    cvtsi2sd xmm7, rsi
    mulsd xmm5, xmm7 // fac *= i * (i - 1)
    mulsd xmm6, xmm0 // xn *= x * x
    movsd xmm7, xmm6
    divsd xmm7, xmm5
    movsd xmm4, xmm7 // part = xn / fac
    mulsd xmm4, xmm3 // part *= sign
    movsd xmm2, xmm1 // old_answer = answer
    addsd xmm1, xmm4 // answer += part
    add rax, 2
    jmp for_loop
  for_exit:
    movsd xmm0, xmm1
    pop rbx
    ret