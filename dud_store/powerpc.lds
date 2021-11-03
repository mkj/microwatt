SECTIONS
{
	. = 0x00000000;
	.head : {
		KEEP(*(.head))
 	}
	. = 0x00000000 + 0x1000;
	.text : { *(.text) }
	. = 0x00000000 + 6000;
	.data : { *(.data) }
	.bss : { *(.bss) }
}
