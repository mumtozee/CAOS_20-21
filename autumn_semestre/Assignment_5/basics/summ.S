.intel_syntax noprefix
.text
.globl summ
.globl everyday795

summ:
  push rbx
  endbr64
  mov ebx, DWORD PTR N[rip]
  mov rsi, QWORD PTR A[rip]
  mov rdi, QWORD PTR B[rip]
  mov rcx, QWORD PTR R[rip]
  mov rax, 0
  while_loop:
    cmp rbx, rax
    jz while_end
    mov rdx, QWORD PTR [rsi+rax*4]
    add rdx, QWORD PTR [rdi+rax*4]
    mov QWORD PTR [rcx+rax*4], rdx
    add rax, 1
    jmp while_loop
  while_end:
    pop rbx
    ret

everyday795:
  endbr64
  push r15
  push rbx
  push rbp
  mov rbx, rdi
  mov r15, rsi
  xor eax, eax
  lea rdi, QWORD PTR input_format[rip]
  lea rsi, QWORD PTR input_num[rip]
  call scanf
  mov rsi, QWORD PTR input_num[rip]
  imul rsi, rbx
  add rsi, r15
  xor eax, eax
  lea rdi, QWORD PTR output_format[rip]
  call printf
  pop rbp
  pop rbx
  pop r15
  ret

.data
  input_format: .string "%d"
  output_format: .string "%d\n"
  input_num: .long 0