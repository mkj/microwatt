ARCH = $(shell uname -m)
ifneq ("$(ARCH)", "ppc64")
ifneq ("$(ARCH)", "ppc64le")
	CROSS_COMPILE ?= powerpc64le-linux-gnu-
endif
endif

CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld
OBJCOPY = $(CROSS_COMPILE)objcopy

CFLAGS = -Os -g -Wall -std=c99 -msoft-float -mno-string -mno-multiple -mno-vsx -mno-altivec -mlittle-endian -fno-stack-protector -mstrict-align -ffreestanding -fdata-sections -ffunction-sections -I../include
CFLAGS += -Werror -Wextra
ASFLAGS = $(CFLAGS)
LDFLAGS = -T powerpc.lds

all: firstcode.hex

console.o: ../lib/console.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

firstcode.elf: firstcode.o head.o console.o
	$(LD) $(LDFLAGS) -o $@ $^

firstcode.bin: firstcode.elf
	$(OBJCOPY) -O binary $^ $@

firstcode.hex: firstcode.bin
	../scripts/bin2hex.py $^ > $@

clean:
	@rm -f *.o firstcode.elf firstcode.bin firstcode.hex
distclean: clean
	rm -f *~

