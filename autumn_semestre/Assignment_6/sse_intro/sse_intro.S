  .intel_syntax noprefix
  .text
  .globl calc
  .globl vector_sum

calc:
  endbr64
  addsd	xmm0, xmm1
  pxor xmm1, xmm1
  cvtsi2sd xmm1, edi
  addsd xmm1, xmm2
  divsd	xmm0, xmm1
  ret

vector_sum:
  endbr64
  sal rdi, 2
  mov rax, 0
  while_loop:
    cmp rax, rdi
    jz while_end
    movaps xmm0, XMMWORD PTR [rdx + rax]
    addps xmm0, XMMWORD PTR [rsi + rax]
    movaps XMMWORD PTR [rcx + rax], xmm0
    add rax, 16
    jmp while_loop
  while_end:
    ret