#include <stdint.h>
#include <stdbool.h>

#include "console.h"
#include "microwatt_soc.h"
#include "io.h"

static unsigned long freq;

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

static uint64_t mtfb(void)
{
	uint64_t ret;
	__asm__ volatile("mftb %0" : "=r" (ret));
	return ret;
}

// interruptible by uart
static void microdelay_waitchar(unsigned long usec)
{
	unsigned long until;
	unsigned long incs = freq * usec / 1000000;

	until = mtfb() + incs;
	while (mtfb() < until && !havechar());
}

void reboot(void)
{
	uint64_t ctrl;

	putchar('X');
	microdelay_waitchar(300000);
	putchar('x');
	microdelay_waitchar(300000);
	putchar('X');
	microdelay_waitchar(300000);
	ctrl = readq(SYSCON_BASE + SYS_REG_CTRL);
	writeq(ctrl | SYS_REG_CTRL_SOC_RESET, SYSCON_BASE + SYS_REG_CTRL);
}

static void hexnibble(uint8_t c)
{
	if (c > 0xf)
	{
		putchar('x');
	} else if (c >= 0xa)
	{
		putchar('f' + c-0xf);
	}
	else
	{
		putchar('0' + c);
	}
}
static void printhex64(uint64_t val)
{
	for (int i = 60; i >= 0; i-=4)
	{
		hexnibble((val>>i)&0xf);
	}
}

static long random(void)
{
	uint64_t ret;
	__asm__ volatile("darn %0,1" : "=r" (ret));
	return (long)ret;
}

static void time_init(void)
{
	freq = readq(SYSCON_BASE + SYS_REG_CLKINFO) & SYS_REG_CLKINFO_FREQ_MASK;
}

int main(void)
{
	unsigned char ucount = 0;

	time_init();
	console_init();

	puts(mw_logo);

	printhex64(random());
	putchar('\r');
	putchar('\n');
	printhex64(random());
	putchar('\r');
	putchar('\n');
	printhex64(random());
	putchar('\r');
	putchar('\n');

	while (1) {
		microdelay_waitchar(4000000);
		if (!havechar()) {
			putchar('#');
			continue;
		}

		unsigned char c = getchar();
		putchar(c);
		if (c == 13) // if CR send LF
			putchar(10);

		// rrr-reboot
		if (c == 'r') {
			ucount++;
		} else {
			ucount = 0;
		}
		if (ucount == 3) {
			reboot();
		}
	}
}
