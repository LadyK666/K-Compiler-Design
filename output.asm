# MIPS Assembly Code
# Generated from optimized TAC
# Target: MARS/QTSPIM simulator

.data
    newline: .asciiz "\n"

.text
.globl main

main:
    # Prologue: allocate stack frame
    addiu $sp, $sp, -12   # allocate 12 bytes

    li $v0, 5         # syscall: read_int
    syscall
    move $s0, $v0      # a = input
    li $t9, 10       # load constant
    add $t0, $s0, $t9   # t0 = a + 10
    move $s1, $t0       # b = t0
    move $a0, $s1      # prepare to print b
    li $v0, 1         # syscall: print_int
    syscall
    li $v0, 11        # syscall: print_char
    li $a0, 10        # newline
    syscall

    # Epilogue: restore stack and exit
    addiu $sp, $sp, 12    # deallocate stack
    li $v0, 10            # syscall: exit
    syscall
