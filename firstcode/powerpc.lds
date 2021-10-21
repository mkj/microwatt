SECTIONS
{
    . = 0x50000;
	.head : {
		KEEP(*(.head))
 	}
	.text : { *(.text) }
	.data : { *(.data) }
	.bss : { *(.bss) }
}
