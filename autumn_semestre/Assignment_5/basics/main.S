.intel_syntax noprefix

.text
.globl summ
.globl everyday795

summ:
    endbr64
    push rbx
    mov ebx, DWORD PTR N[RIP] 
    mov r9, QWORD PTR A[RIP]
    mov rsi, QWORD PTR B[RIP]
    mov rdi, QWORD PTR R[RIP]
    mov eax, ebx
count:
    sub eax, 1
    mov edx, DWORD PTR [r9 + rax * 4]
    add edx, DWORD PTR [rsi + rax * 4]
    mov DWORD PTR [rdi + rax * 4], edx
    cmp eax, 0
    jnz count
    jz summ_exit
summ_exit:
    pop rbx
    ret

everyday795:
    endbr64
    push rbx
    push rbp 
    mov rbx, rdi
    mov rcx, rsi
    mov eax, 0
    lea rdi, QWORD PTR format_input[RIP]
    lea rsi, QWORD PTR input_num[RIP]
    push rcx
    call scanf
    pop rcx
    mov rsi, QWORD PTR input_num[RIP]
    imul rsi, rbx
    add rsi, rcx
    mov eax, 0
    lea rdi, QWORD PTR format_output[RIP]
    call printf
    pop rbp
    pop rbx
    ret

.data
format_input: .string "%d"
format_output: .string "%d\n"
input_num: .long 0