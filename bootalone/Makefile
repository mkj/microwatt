#### Directories

OBJ = .
SRC_DIR=.
GENINC_DIR=orangecrab

PROGRAM = bootalone
OBJECTS = $(OBJ)/head.o $(OBJ)/main.o $(OBJ)/console.o

#### Compiler

ARCH = $(shell uname -m)
ifneq ("$(ARCH)", "ppc64")
ifneq ("$(ARCH)", "ppc64le")
        CROSS_COMPILE = powerpc64le-linux-gnu-
        endif
        endif

CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld
OBJCOPY = $(CROSS_COMPILE)objcopy
STRIP = $(CROSS_COMPILE)strip

#### Flags

CPPFLAGS = -nostdinc -D__USE_LIBC $(EXTRA_CFLAGS)

# These includes must be first ...
CPPFLAGS += -I$(GENINC_DIR) -I$(SRC_DIR)/include -I$(SRC_DIR)/../include -I$(SRC_DIR)/libc/include 

CPPFLAGS += -DMEMTEST_BUS_DEBUG -DMEMTEST_DATA_DEBUG -DMEMTEST_ADDR_DEBUG
CPPFLAGS += -DMEMTEST_DATA_RETRIES=3 -DMEMTEST_DEBUG_MAX_ERRORS=20

CPPFLAGS += -isystem $(shell $(CC) -print-file-name=include)
CFLAGS = -Os -g -Wall -std=c99 -m64 -mabi=elfv2 -msoft-float -mno-string -mno-multiple -mno-vsx -mno-altivec -mlittle-endian -fno-stack-protector -mstrict-align -ffreestanding -fdata-sections -ffunction-sections -fno-delete-null-pointer-checks 
CFLAGS += -Werror
ASFLAGS = $(CPPFLAGS) $(CFLAGS)
LDFLAGS = -static -nostdlib -T $(OBJ)/$(PROGRAM).lds --gc-sections

#### Pretty print

ifeq ($(V),1)
define Q
  $(2)
endef
else
define Q
  @echo "	[$1] " $(shell basename $3)
  @$(2)
endef
endif

#### Rules. This is a bit crappy, I'm sure we can do better with the
####        handling of the various path, but this will have to do
####        until I can be bothered getting my head around the finer
####        points of Makefiles

all: objdir $(OBJ)/$(PROGRAM).hex

$(OBJ)/console.o: $(SRC_DIR)/../lib/console.c
	$(call Q,CC, $(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@, $@)
$(OBJ)/%.o : $(SRC_DIR)/%.c bootalone.h
	$(call Q,CC, $(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@, $@)
$(OBJ)/%.o : $(SRC_DIR)/%.S bootalone.h
	$(call Q,AS, $(CC) $(ASFLAGS) -c $< -o $@, $@)
$(OBJ)/%.o : $(SRC_DIR)/libc/src/%.c
	$(call Q,CC, $(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@, $@)
$(OBJ)/%.lds : $(SRC_DIR)/%.lds.S bootalone.h
	$(call Q,CC, $(CC) $(CPPFLAGS) -P -E $< -o $@, $@)
$(OBJ)/%.elf.strip : $(SRC_DIR)/%.elf
	$(call Q,STRIP, $(STRIP) $< -o $@, $@)

LIBC_SRC := $(wildcard $(SRC_DIR)/libc/src/*.c)
LIBC_OBJ := $(patsubst $(SRC_DIR)/libc/src/%.c, $(OBJ)/%.o,$(LIBC_SRC))
$(OBJ)/libc.o: $(LIBC_OBJ)
	$(call Q,LD, $(LD) -r -o $@ $^, $@)

$(OBJ)/$(PROGRAM).elf: $(OBJECTS) $(OBJ)/libc.o $(OBJ)/$(PROGRAM).lds
	$(call Q,LD, $(LD) $(LDFLAGS) -o $@ $(OBJECTS) $(OBJ)/libc.o, $@)

$(OBJ)/$(PROGRAM).bin: $(OBJ)/$(PROGRAM).elf
	$(call Q,OC, $(OBJCOPY) -O binary -S $^ $@, $@)

$(OBJ)/$(PROGRAM).hex: $(OBJ)/$(PROGRAM).bin
	$(call Q,HX, $(SRC_DIR)/bin2hex.py $^ > $@, $@)

objdir:
	@mkdir -p $(OBJ)

apollo: $(PROGRAM).elf
	apollo flash-info
	apollo flash $^ 0x400000
