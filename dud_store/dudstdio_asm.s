	.file	"dudstdio.c"
	.machine power8
	.abiversion 2
	.section	.text.dud_number,"ax",@progbits
	.align 2
	.globl dud_number
	.type	dud_number, @function
dud_number:
.LFB0:
	.cfi_startproc
.LCF0:
0:	addis 2,12,.TOC.-.LCF0@ha
	addi 2,2,.TOC.-.LCF0@l
	.localentry	dud_number,.-dud_number
	mflr %r0
	std %r30,-16(%r1)
	std %r31,-8(%r1)
	std %r0,16(%r1)
	stdu %r1,-160(%r1)
	.cfi_def_cfa_offset 160
	.cfi_offset 65, 16
	.cfi_offset 30, -16
	.cfi_offset 31, -8
	mr %r31,%r3
	rldicl %r9,%r4,0,32
	stw %r4,128(%r1)
	std %r3,120(%r1)
#	b .L2
.L3:
# failing loop

	# addi %r30,%r1,32 # output buffer on stack
	# lis %r30, 0x0500 # or an arbitrary main address 0x0500000
	lis %r30, 0x4500 # or an arbitrary DRAM address 0x4500000

	# # edge of 512MB dram
	# lis %r30, 0x6000
	# addi %r30, %r30, -256

	li %r3, 0x24 # something arbitrary to test printing
	bl uart_hexdigit

	li %r22, 0
.storeloop:
	addi %r22, %r22, -1
	mr %r3, %r22
	bl uart_hexdigit
	stbx %r22,%r22,%r30
	lbzx %r3,%r22,%r30
	bl uart_hexdigit

	addi %r22, %r22, 1
	mr %r3, %r22
	bl uart_hexdigit
	stbx %r22,%r22,%r30
	lbzx %r3,%r22,%r30
	bl uart_hexdigit

	#addi %r22, %r22, 64 ; every second is blank
	addi %r22, %r22, 16
	cmpdi %cr0, %r22, 1000 # end
	bng .storeloop

	li %r8, 0x45  # value1 to store
	stb %r8,0(%r30)

	lbz %r10,0(%r30)

	li %r8, 0x89  # value2 to store
	stb %r8,0(%r30)
	lbz %r11,0(%r30)

	# print value1
	mr %r3, %r10
	bl uart_hexdigit
	# print value2
	mr %r3, %r11
	bl uart_hexdigit

	addi %r1,%r1,160
	.cfi_def_cfa_offset 0
	b _restgpr0_30
	.cfi_restore 31
	.cfi_restore 30
	.cfi_restore 65
	.long 0
	.byte 0,0,0,1,128,2,0,0
	.cfi_endproc

uart_hexdigit:
	# uart1 TX 0xc0003000
	# change to 0xc0002000 for uart0
	lis %r5, 0xc000
	# ori %r5, %r5, 0x3000
	ori %r5, %r5, 0x2000

	li %r4, 0x2b # '+'
	stbcix %r4,0,%r5

	# high nibble
	rldicl %r4, %r3, 60, 60
	cmpdi %cr0,%r4,0xa
	blt 8
	addi %r4,%r4, 'a'-'0'-0xa # a-f digit
	addi %r4,%r4, 0x30
	stbcix %r4,0,%r5

	# low nibble
	rldicl %r4, %r3, 0, 60
	cmpdi %cr0,%r4,0xa
	blt 8
	addi %r4,%r4, 'a'-'0'-0xa # a-f digit
	addi %r4,%r4, 0x30
	stbcix %r4,0,%r5

	li %r4, 0x2d # '-'
	stbcix %r4,0,%r5

	# wait for TX empty
	lbz %r3, 0x14(%r5)
	andi. %r3, %r3, 0x20
	beq -8

	blr

####

delay:
.iter:
	cmpdi %cr0, %r3, 0
	addi %r3, %r3, -1
	bgt .iter
	blr

.LFE0:
	.size	dud_number,.-dud_number
	.ident	"GCC: (Ubuntu 11.2.0-5ubuntu1) 11.2.0"
	.section	.note.GNU-stack,"",@progbits

