OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)

ENTRY(Entry)

MEMORY
{
    SRAM       (rwx) : ORIGIN = 0x80000000, LENGTH = 128K
    DDR_unused (rwx) : ORIGIN = 0xC0000000, LENGTH = 0x01080000
    DDR        (rwx) : ORIGIN = 0xC1080000, LENGTH = (64M - 0x01080000)
}

_stack = ORIGIN(DDR) + LENGTH(DDR) - 4;

SECTIONS
{
    .startcode :
    {
        . = ALIGN(4);
        *init.o      (.text)
    } > DDR

    .text :
    {
        . = ALIGN(4);
        *(.text)
    } > DDR

    .data :
    {
        . = ALIGN(4);
        *(.data)
    } > DDR

    .bss :
    {
        . = ALIGN(4);
        _bss_start = .;
        *(.bss)
        . = ALIGN(4);
        _bss_end = .;
    } > DDR
}
