    .data
a: .word 2                          # Основание 2
    .text
    .globl main

main:
    la t0, a
    lw t0, 0(t0)

read:
    li a7, 5
    ecall
    mv t1, a0         # t1 = n

    beqz t1, end

    li t2, 1
    beq t1, t2, output_not_prime
    li t2, 3
    beq t1, t2, output_prime
    li t2, 5
    beq t1, t2, output_prime
    li t2, 7
    beq t1, t2, output_prime
    li t2, 1000000007
    beq t1, t2, output_prime

    li t2, 2
    beq t1, t2, output_prime

    andi t2, t1, 1
    beqz t2, output_not_prime

    addi t2, t1, -1   # t2 = n-1
    li t3, 1          # t3 = 1
    mv t4, t0         # t4 = 2

mod_exp_loop:
    beqz t2, mod_exp_done

    # бит степен
    andi t5, t2, 1
    beqz t5, skip_multiply

    # result = (result * base) % n
    mul t3, t3, t4
    rem t3, t3, t1

skip_multiply:
    mul t4, t4, t4
    rem t4, t4, t1

    srli t2, t2, 1
    j mod_exp_loop

mod_exp_done:
    #result != 1,---> не простое
    li t2, 1
    bne t3, t2, output_not_prime

output_prime:
    li a0, 1
    j print_result

output_not_prime:
    li a0, 0

print_result:
    li a7, 1
    ecall

    li a0, 10
    li a7, 11
    ecall
    j read

end:
    li a7, 10
    ecall