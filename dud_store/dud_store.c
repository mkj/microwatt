#include <stdint.h>
#include <stdbool.h>

#include "console.h"
#include "dudstdio.h"


// void deep_printf(void) {
// 	// waste some stack
// 	volatile char somebuf[5000];
// 	somebuf[0] = 0x0;
// 	dud_number(327, 10);
// 	puts((void*)somebuf);
// 	// dud_printf("%s %d ret %d eof\n", __func__, 327, 0);
// }

int main(void)
{
	console_init();

	// puts(mw_logo);
	// puts("hello\n");
	// deep_printf();
	dud_number(765, 10);

	while (1) {
		// unsigned char c = getchar();
		// putchar(c);
		// if (c == 13) // if CR send LF
		// 	putchar(10);
	}
}
