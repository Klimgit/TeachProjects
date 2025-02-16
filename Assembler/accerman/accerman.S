.intel_syntax noprefix

.data
result: .quad 0

.text
.global ackermann

ackermann:
    push rbp
    mov rbp, rsp
    sub rsp, 16
    mov qword ptr [rbp-8], rdi
    mov qword ptr [rbp-16], rsi


    cmp rdi, 0
    jne check_n
    add rsi, 1
    mov rax, rsi
    leave
    ret

check_n:
    cmp rsi, 0
    jne recursive_case
    mov rdi, qword ptr [rbp-8]
    sub rdi, 1
    mov rsi, 1
    call ackermann
    leave
    ret

recursive_case:
    mov rdi, qword ptr [rbp-8]
    mov rsi, qword ptr [rbp-16]
    sub rsi, 1
    call ackermann

    mov rsi, rax
    mov rdi, qword ptr [rbp-8]
    sub rdi, 1
    call ackermann

    leave
    ret