ARCH = $(shell uname -m)
ifneq ("$(ARCH)", "ppc64")
ifneq ("$(ARCH)", "ppc64le")
	CROSS_COMPILE ?= powerpc64le-linux-gnu-
endif
endif

CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld
AS = $(CROSS_COMPILE)as
OBJCOPY = $(CROSS_COMPILE)objcopy

CFLAGS = -Os -g -Wno-unused -Wall -std=c99 -msoft-float -mno-string -mno-multiple -mno-vsx -mno-altivec -mlittle-endian -fno-stack-protector -mstrict-align -ffreestanding -fdata-sections -ffunction-sections -I../include
CFLAGS += -Werror -Wextra
ASFLAGS = 
LDFLAGS = -T powerpc.lds --gc-sections

all: dud_store.hex

console.o: ../lib/console.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

dud_store.elf: dud_store.o head.o console.o dudstdio_asm.o 
	$(LD) $(LDFLAGS) -o $@ $^

dud_store.bin: dud_store.elf
	$(OBJCOPY) -O binary $^ $@

dud_store.hex: dud_store.bin
	../scripts/bin2hex.py $^ > $@

clean:
	@rm -f *.o dud_store.elf dud_store.bin dud_store.hex
distclean: clean
	rm -f *~


apollo: dud_store.elf
	apollo flash-info
	apollo flash $^ 0x400000

.PHONY: apollo
