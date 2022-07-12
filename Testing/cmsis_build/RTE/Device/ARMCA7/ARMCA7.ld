

MEMORY
{
  ROM (rx)   : ORIGIN = 0x80000000, LENGTH = 0x00200000
  L_TTB (rw) : ORIGIN = 0x80500000, LENGTH = 0x00005000 
  RAM (rwx)  : ORIGIN = 0x80200000, LENGTH = 0x00300000
}

ENTRY(Reset_Handler)

SECTIONS
{
    .text :
    {

        Image$$VECTORS$$Base = .;
        * (RESET)
        KEEP(*(.isr_vector))
        Image$$VECTORS$$Limit = .;

        *(SVC_TABLE)
        *(.text*)

        KEEP(*(.init))
        KEEP(*(.fini))

        /* .ctors */
        *crtbegin.o(.ctors)
        *crtbegin?.o(.ctors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
        *(SORT(.ctors.*))
        *(.ctors)

        /* .dtors */
        *crtbegin.o(.dtors)
        *crtbegin?.o(.dtors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
        *(SORT(.dtors.*))
        *(.dtors)

        Image$$RO_DATA$$Base = .;
        *(.rodata*)
        Image$$RO_DATA$$Limit = .;

        KEEP(*(.eh_frame*))
    } > ROM

    .ARM.extab : 
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > ROM

    __exidx_start = .;
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > ROM
    __exidx_end = .;


    .copy.table :
    {
        . = ALIGN(4);
        __copy_table_start__ = .;
        LONG (__etext)
        LONG (__data_start__)
        LONG (__data_end__ - __data_start__)
        __copy_table_end__ = .;
    } > ROM

    .zero.table :
    {
        . = ALIGN(4);
        __zero_table_start__ = .;
        LONG (__bss_start__)
        LONG (__bss_end__ - __bss_start__)
        __zero_table_end__ = .;
    } > ROM

    __etext = .;
        
    .ttb :
    {
        Image$$TTB$$ZI$$Base = .;
        . += 0x00005000;
        Image$$TTB$$ZI$$Limit = .;
    } > L_TTB

    .data : AT (__etext)
    {
        Image$$RW_DATA$$Base = .;
        __data_start__ = .;
        *(vtable)
        *(.data*)
        Image$$RW_DATA$$Limit = .;

        . = ALIGN(4);
        /* preinit data */
        PROVIDE (__preinit_array_start = .);
        KEEP(*(.preinit_array))
        PROVIDE (__preinit_array_end = .);

        . = ALIGN(4);
        /* init data */
        PROVIDE (__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE (__init_array_end = .);


        . = ALIGN(4);
        /* finit data */
        PROVIDE (__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array))
        PROVIDE (__fini_array_end = .);

        . = ALIGN(4);
        /* All data end */
        __data_end__ = .;

    } > RAM

    
    .bss ALIGN(0x400):
    {
        Image$$ZI_DATA$$Base = .;
        __bss_start__ = .;
        *(.bss*)
        *(COMMON)
        __bss_end__ = .;
        Image$$ZI_DATA$$Limit = .;
        __end__ = .;
        end = __end__;
    } > RAM

    .heap (NOLOAD):
    {
        . = ALIGN(8);
        Image$$HEAP$$ZI$$Base = .;
        . += 0x00100000;
        Image$$HEAP$$ZI$$Limit = .;
        __HeapLimit = .;
    } > RAM  

    .stack (NOLOAD):
    {
        . = ORIGIN(RAM) + LENGTH(RAM) - 0x00002000 - 0x00000100 - 0x00000100 - 0x00000100 - 0x00000100 - 0x00000100;
        . = ALIGN(8);
        
        __StackTop = .;
        Image$$SYS_STACK$$ZI$$Base = .;
        . += 0x00002000;
        Image$$SYS_STACK$$ZI$$Limit = .;
        __stack = .;

        Image$$FIQ_STACK$$ZI$$Base = .;
        . += 0x00000100;
        Image$$FIQ_STACK$$ZI$$Limit = .;

        Image$$IRQ_STACK$$ZI$$Base = .;
        . += 0x00000100;
        Image$$IRQ_STACK$$ZI$$Limit = .;

        Image$$SVC_STACK$$ZI$$Base = .;
        . += 0x00000100;
        Image$$SVC_STACK$$ZI$$Limit = .;

        Image$$ABT_STACK$$ZI$$Base = .;
        . += 0x00000100;
        Image$$ABT_STACK$$ZI$$Limit = .;

        Image$$UND_STACK$$ZI$$Base = .;
        . += 0x00000100;
        Image$$UND_STACK$$ZI$$Limit = .;
        
    } > RAM
}
