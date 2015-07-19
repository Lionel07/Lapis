interrupt_vector_table:
    b . @ Reset
    b . 
    b . @ SWI instruction
    b . 
    b .
    b .
    b .
    b .

.comm stack, 0x10000
_start:
    .globl _start
    ldr sp, =stack+0x10000
    bl early_kernel_main
1: 
    b 1b
