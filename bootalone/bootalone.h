/* Only preprocessor directives in this file, it's used by various syntaxes */
#pragma once

// offset 9MB, allows
// 5MB for first elf,
// 7MB for bootalone-loaded elf on 16MB orangecrab
#define BOOTALONE_FLASH_OFFSET 0x900000

// #define BOOTALONE_BASE 0x50000 // works

// last megabyte on 128MB orangecrab, 127MB. (85F 0.2.1 is actually 512MB)
// #define BOOTALONE_BASE 0x7f00000 // bad

// XXX
// #define BOOTALONE_BASE 0xa9d000 // Bad
// #define BOOTALONE_BASE 0xa9c000 // a bit corrupt but middling
// #define BOOTALONE_BASE 0xa9b000 // same bit corrupt
// #define BOOTALONE_BASE 0xa9a000 // same bit corrupt
// #define BOOTALONE_BASE 0xa9c800  // same bit corrupt
#define BOOTALONE_BASE 0xa9cc00 // bad
// #define BOOTALONE_BASE 0xa9ca00 // bad
// #define BOOTALONE_BASE 0xa9c900 // bad
// #define BOOTALONE_BASE 0xa9c880 // bad
// #define BOOTALONE_BASE 0xa9c840 // bad
// #define BOOTALONE_BASE 0xa9c820 // alright, a few dropped bytes
// #define BOOTALONE_BASE 0xa9c830 // bad
// #define BOOTALONE_BASE 0xa9c828 // alright, few dropped
// #define BOOTALONE_BASE 0xa9c82c // bad
// #define BOOTALONE_BASE 0xa9c82a // doesn't boot at all: now!
// #define BOOTALONE_BASE 0xa9c829 // doesn't boot at all: now!
// #define BOOTALONE_BASE 0xa9c828 
