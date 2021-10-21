/* Loads hex bytes from serial into dram */

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
" ;  .mw.  ;   Microwatt, firstcode\n"
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

// interruptible by uart
static void microdelay(unsigned long usec)
{
	unsigned long until;
	unsigned long incs = freq * usec / 1000000;

	until = mtfb() + incs;
	while (mtfb() < until);
}


// static void reboot(void)
// {
// 	uint64_t ctrl;

// 	putchar('X');
// 	microdelay_waitchar(300000);
// 	putchar('x');
// 	microdelay_waitchar(300000);
// 	putchar('X');
// 	microdelay_waitchar(300000);
// 	ctrl = readq(SYSCON_BASE + SYS_REG_CTRL);
// 	writeq(ctrl | SYS_REG_CTRL_CORE_RESET, SYSCON_BASE + SYS_REG_CTRL);
// }

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

// static long random(void)
// {
// 	uint64_t ret;
// 	__asm__ volatile("darn %0,1" : "=r" (ret));
// 	return (long)ret;
// }

static void time_init(void)
{
	freq = readq(SYSCON_BASE + SYS_REG_CLKINFO) & SYS_REG_CLKINFO_FREQ_MASK;
}

static int tolower(char c) {
	if (c >= 'A' && c <= 'Z') {
		c = c | 32;
	}
	return c;
}

static void(*dram_init_main)(void) = (void*)DRAM_INIT_BASE;
static void(*dram_main)(void) = (void*)DRAM_BASE;

extern void* _start;
extern void* boot_entry;
int main(void)
{
	uint8_t *dst = (void*)DRAM_BASE;
	unsigned char ucount = 0;
	bool high_nibble = true;
	uint8_t current = 0;

	puts("\nfirstcode\n");
	time_init();
	printhex64(freq);
	puts(" freq\n");
	microdelay(2000000);
	// console_init();
	microdelay(2000000);

	puts(mw_logo);
	microdelay(2000000);
	puts("send hex then ! to start\n");

	while (1) {
		microdelay_waitchar(4000000);
		if (!havechar()) {
			putchar('#');
			continue;
		}

		uint8_t c = getchar();
		c = tolower(c);

		if (c == 'r') {
			// rrr-reboot
			ucount++;
		} else {
			ucount = 0;
		}
		if (ucount == 3) {
			dram_init_main();
		}

		if (c == 13) // if CR send LF
			putchar(10);

		if (c == '!') {
			printhex64((uint64_t)dst - DRAM_BASE);
			puts(" bytes, starting\n");
			microdelay(500000);
			dram_main();
		} else if ((c >= 'a' && c <= 'f')) {
			c = c - 'a';
		} else if (c >= '0' && c <= '9') {
			c = c - '0';
		} else {
			continue;
		}

		if (high_nibble) {
			current = c << 4;
			high_nibble = false;
		} else {
			current |= c;
			*dst = current;
			dst++;
			high_nibble = true;
			putchar('.');
		}
	}
	return 0;
}
