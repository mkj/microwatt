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

void flush_cpu_icache(void)
{
	__asm__ volatile ("icbi 0,0; isync" : : : "memory");
}

static void(*dram_init_main)(void) = (void*)DRAM_INIT_BASE;
static void(*dram_main)(void) = (void*)DRAM_BASE;

extern void* _start;
extern void* boot_entry;

static bool fl_read(void *dst, uint32_t offset, uint32_t size)
{
	uint8_t *d = dst;

#ifdef FLASH_LOADER_USE_MAP
	memcpy(d, (void *)(unsigned long)(SPI_FLASH_BASE + offset), size);
#else
	if (size < 1)
		return false;
	fl_cs_on();
	writeb(SPI_CMD_QUAD_FREAD, SPI_FCTRL_BASE + SPI_REG_DATA);
	writeb(offset >> 16, SPI_FCTRL_BASE + SPI_REG_DATA);
	writeb(offset >>  8, SPI_FCTRL_BASE + SPI_REG_DATA);
	writeb(offset, SPI_FCTRL_BASE + SPI_REG_DATA);
	writeb(0x00, SPI_FCTRL_BASE + SPI_REG_DATA);
	while(size--)
		*(d++) = readb(SPI_FCTRL_BASE + SPI_REG_DATA_QUAD);
	fl_cs_off();
#endif

	return true;
}

static unsigned long boot_flash(unsigned int offset)
{
	Elf64_Ehdr ehdr;
	Elf64_Phdr ph;
	unsigned int i, poff, size, off;
	void *addr;

	printf("Trying flash...\n");
	if (!fl_read(&ehdr, offset, sizeof(ehdr)))
		return -1ul;
	if (!IS_ELF(ehdr) || ehdr.e_ident[EI_CLASS] != ELFCLASS64) {
		printf("Doesn't look like an elf64\n");
		goto dump;
	}
	if (ehdr.e_ident[EI_DATA] != ELFDATA2LSB ||
	    ehdr.e_machine != EM_PPC64) {
		printf("Not a ppc64le binary\n");
		goto dump;
	}

	poff = offset + ehdr.e_phoff;
	for (i = 0; i < ehdr.e_phnum; i++) {
		if (!fl_read(&ph, poff, sizeof(ph)))
			goto dump;
		if (ph.p_type != PT_LOAD)
			continue;

		/* XXX Add bound checking ! */
		size = ph.p_filesz;
		addr = (void *)ph.p_vaddr;
		off  = offset + ph.p_offset;
		printf("Copy segment %d (0x%x bytes) to %p\n", i, size, addr);
		fl_read(addr, off, size);
		poff += ehdr.e_phentsize;
		for (int i = 0; i < 1024; i++) {
			printf("%02x ", ((uint8_t*)addr)[i]);
			if (i % 32 == 31) {
				puts("\n");
			}
		}
	}

	printf("\nBooting from DRAM at %x\n", (unsigned int)ehdr.e_entry);
	for (int i = 0; i < 128; i++) {
		printf("%02x ", ((uint8_t*)ehdr.e_entry)[i]);
		if (i % 32 == 31) {
			puts("\n");
		}
	}
	printf("\ndram:\n");
	for (int i = 0; i < 128; i++) {
		printf("%02x ", ((uint8_t*)DRAM_BASE + ehdr.e_entry)[i]);
		if (i % 32 == 31) {
			puts("\n");
		}
	}
	printf("\nnow!\n");
	flush_cpu_icache();
	return ehdr.e_entry;
dump:
	printf("HDR: %02x %02x %02x %02x %02x %02x %02x %02x\n",
	       ehdr.e_ident[0], ehdr.e_ident[1], ehdr.e_ident[2], ehdr.e_ident[3],
	       ehdr.e_ident[4], ehdr.e_ident[5], ehdr.e_ident[6], ehdr.e_ident[7]);
	return -1ul;
}
int main(void)
{
	uint8_t *dst = (void*)DRAM_BASE;
	unsigned char ucount = 0;
	bool high_nibble = true;
	uint8_t current = 0;

	console_init();
	puts("\nfirstcode\n");
	time_init();
	printhex64(freq);
	puts(" freq\n");
	microdelay(2000000);
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

		if (c == 13) {
			// if CR send LF
			putchar(13);
			putchar(10);
		}

		if (c == '!') {
			printhex64((uint64_t)dst - DRAM_BASE);
			puts(" bytes, starting\n");
			flush_cpu_icache();
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
