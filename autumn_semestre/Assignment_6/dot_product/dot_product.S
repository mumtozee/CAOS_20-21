  .intel_syntax noprefix
  .text
  .globl dot_product

dot_product:
  endbr64
  push rbx
  mov rbx, rdi
  sar rbx, 2 // rbx >>= 2
  sal rbx, 2 // rbx <<= 2
  mov rcx, rdi
  sub rcx, rbx // rcx = rdi % 4
  mov rdi, rbx // rdi now is divisible by 4
  sal rdi, 2
  sal rcx, 2
  xor rax, rax // i = 0
  vxorps xmm0, xmm0, xmm0
  main_loop:
    cmp rax, rdi
    jge process_remainder
    vmovups xmm1, XMMWORD PTR [rsi + rax]
    vmovups xmm2, XMMWORD PTR [rdx + rax]
    vdpps xmm3, xmm1, xmm2, 0xF1 // standard dot product
    vaddss xmm0, xmm0, xmm3
    add rax, 16
    jmp main_loop

  process_remainder:
    mov rbx, rax
    add rdi, rcx
    second_loop:
      cmp rbx, rdi
      jge exit
      vmovss xmm1, DWORD PTR [rsi + rbx]
      vmovss xmm2, DWORD PTR [rdx + rbx]
      vmulss xmm1, xmm1, xmm2
      vaddss xmm0, xmm1, xmm0
      add rbx, 4
      jmp second_loop
exit:
  pop rbx
  ret