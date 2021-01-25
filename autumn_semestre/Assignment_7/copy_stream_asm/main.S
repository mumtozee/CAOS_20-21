 #include <sys/syscall.h>
 
  .intel_syntax noprefix
  .text
  .globl _start

_start:
  endbr64
  lea rsi, BYTE PTR input_char[rip]
  mov rdx, 1
  while_loop:
    mov rax, SYS_read
    mov rdi, 0
    syscall
    cmp rax, 0
    jz exit_loop
    mov rax, SYS_write
    mov rdi, 1
    syscall
    jmp while_loop
  exit_loop:
    mov rax, SYS_exit_group
    syscall

  .data
input_char: .byte 0