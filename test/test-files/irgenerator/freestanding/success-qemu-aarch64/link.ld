/* Memory layout for the virt machine of QEMU. Its RAM starts at 0x40000000, where QEMU places the device tree */
ENTRY(_start)

SECTIONS
{
    . = 0x40080000;
    .text : { KEEP(*(.text.boot)) *(.text .text.*) }
    .rodata : { *(.rodata .rodata.*) }
    .data : { *(.data .data.*) }
    .bss : { *(.bss .bss.* COMMON) }
    /* 64 KiB stack */
    . = ALIGN(16);
    . += 0x10000;
    __stack_top = .;
}
