#include <stdint.h>
#include <stdbool.h>

#include "liteuart_console.h"

static char mw_logo[] =

"\n"
"   .oOOo.     \n"
" .\"      \". \n"
" ;  .mw.  ;   Microwatt, it works.\n"
"  . '  ' .    \n"
"   \\ || /    \n"
"    ;..;      \n"
"    ;..;      \n"
"    `ww'   \n";

int main(void)
{
	usb_console_init();

	usb_puts(mw_logo);

	while (1) {
		unsigned char c = usb_getchar();
		usb_putchar(c);
		if (c == 13) // if CR send LF
			usb_putchar(10);
	}
}
