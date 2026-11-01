// Entry point for the virt machine of QEMU. QEMU jumps here at EL1, with the MMU off and without a stack
    .section .text.boot, "ax"
    .global _start
_start:
    // Allow FP and SIMD instructions at EL1 (CPACR_EL1.FPEN). LLVM uses the SIMD registers e.g. for copying memory
    mov x0, #(3 << 20)
    msr cpacr_el1, x0
    isb
    // Switch to the stack, that the linker script reserves, and call the Spice code. QEMU zeroes the .bss section already
    ldr x0, =__stack_top
    mov sp, x0
    bl kmain
1:  wfe
    b 1b
