ENTRY (_start)
SECTIONS
{
    . = 0;
    kernel_start = .;
    .text : { *(.text*) *(.rodata*) }
    .data : { *(.data*) }
    .bss : { *(.bss*) *(COMMON*) }
    kernel_end = .;
}