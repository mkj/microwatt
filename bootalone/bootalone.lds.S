#include "microwatt_soc.h"

SECTIONS
{
	. = 0x50000;
	start = .;
	.head : {
		KEEP(*(.head))
	}
	. = 0x50000 | 0x1000;

	.text : { *(.text*) *(.sfpr) *(.rodata*) }
	.data : { *(.data*) }
	.bss : { *(.bss*) }
}
