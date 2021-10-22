#include "microwatt_soc.h"
#include "bootalone.h"

SECTIONS
{
	. = BOOTALONE_BASE;
	start = .;
	.head : {
		KEEP(*(.head))
	}
	. = BOOTALONE_BASE + 0x2000;

	.text : { *(.text*) *(.sfpr) *(.rodata*) }
	.data : { *(.data*) }
	.bss : { *(.bss*) }
}
