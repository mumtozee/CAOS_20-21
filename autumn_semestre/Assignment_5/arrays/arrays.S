.intel_syntax noprefix
.text
.globl summ

summ:
  endbr64
  push rbx
  mov eax, 0
  while_loop:
    cmp edi, eax
    jz while_end
    mov ebx, DWORD PTR [rsi+rax*4]
    add ebx, DWORD PTR [rdx+rax*4]
    mov DWORD PTR [rcx+rax*4], ebx
    add rax, 1
    jmp while_loop
  while_end:
    pop rbx
    ret
