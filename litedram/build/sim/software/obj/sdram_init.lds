SECTIONS
{
 . = 0xff000000;
 start = .;
 .head : {
  KEEP(*(.head))
 }
 . = 0xff000000 | 0x1000;
 .text : { *(.text*) *(.sfpr) *(.rodata*) }
 .data : { *(.data*) }
 .bss : { *(.bss*) }
}
