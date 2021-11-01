	.file	"dudstdio.c"
	.machine power8
	.abiversion 2
	.section	".text"
	.section	.rodata.dud_number.str1.8,"aMS",@progbits,1
	.align 3
.LC0:
	.string	"dud_number1"
	.align 3
.LC1:
	.string	"dud_number2"
	.align 3
.LC2:
	.string	"^ base2"
	.align 3
.LC3:
	.string	"^ num2"
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

# %r31 = 765
# %r9 = 10

#	divdu %r7,%r31,%r9
	li %r7, 76

#	mulld %r8,%r7,%r9
#	subf %r8,%r8,%r31
#	mr %r31,%r7

	addi %r30,%r1,32 # output buffer on stack
#	stbx %r8,%r30,%r10 # this fails?
	li %r3, 0x24 # something arbitrary
	bl uart_hexdigit

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

# 	addi %r30,%r1,32 # output buffer on stack
# 	li %r8, 5 # value to store
# #	stbx %r8,%r30,%r10 # this fails?
# 	stb %r8,0(%r30)

# 	addis %r3,%r2,.LC0@toc@ha # string title
# 	addi %r3,%r3,.LC0@toc@l
# 	mr %r4,%r30 # buffer to print
# 	li %r5,1 # length=1
# 	bl printhex

	# divdu %r7,%r31,%r9
	# mulld %r8,%r7,%r9
	# subf %r8,%r8,%r31
	# mr %r31,%r7
	# stbx %r8,%r30,%r10
	# addi %r10,%r10,1

	# divdu %r7,%r31,%r9
	# mulld %r8,%r7,%r9
	# subf %r8,%r8,%r31
	# mr %r31,%r7
	# stbx %r8,%r30,%r10
	# addi %r10,%r10,1
.L2:
#	cmpdi %cr0,%r31,0
#	bne %cr0,.L3
	nop
	ld %r9,120(%r1)
	lwz %r8,128(%r1)
#	b .L4
.L5:
	li %r31, 0 # reset i=0
# OK loop
	divdu %r7,%r9,%r8
	mulld %r10,%r7,%r8
	subf %r10,%r10,%r9
	mr %r9,%r7
	stbx %r10,%r30,%r31
	addi %r31,%r31,1
	divdu %r7,%r9,%r8
	mulld %r10,%r7,%r8
	subf %r10,%r10,%r9
	mr %r9,%r7
	stbx %r10,%r30,%r31
	addi %r31,%r31,1
	divdu %r7,%r9,%r8
	mulld %r10,%r7,%r8
	subf %r10,%r10,%r9
	mr %r9,%r7
	stbx %r10,%r30,%r31
	addi %r31,%r31,1
.L4:
#	cmpdi %cr0,%r9,0
#	bne %cr0,.L5
	addis %r3,%r2,.LC1@toc@ha
	extsw %r5,%r31
	mr %r4,%r30
	addi %r3,%r3,.LC1@toc@l
	bl printhex
	nop
	addis %r3,%r2,.LC2@toc@ha
	addi %r4,%r1,128
	li %r5,4
	addi %r3,%r3,.LC2@toc@l
	bl printhex
	nop
	addis %r3,%r2,.LC3@toc@ha
	addi %r4,%r1,120
	li %r5,8
	addi %r3,%r3,.LC3@toc@l
	bl printhex
	nop
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
	ori %r5, %r5, 0x3000

	li %r4, 0x2b # '+'
	sync
	stbcix %r4,0,%r5

	# high nibble
	rldicl %r4, %r3, 60, 60
	addi %r4,%r4, 0x30
	sync
	stbcix %r4,0,%r5

	# low nibble
	rldicl %r4, %r3, 0, 60
	addi %r4,%r4, 0x30
	sync
	stbcix %r4,0,%r5

	li %r4, 0x2d # '-'
	sync
	stbcix %r4,0,%r5
	blr

.LFE0:
	.size	dud_number,.-dud_number
	.ident	"GCC: (Ubuntu 11.2.0-5ubuntu1) 11.2.0"
	.section	.note.GNU-stack,"",@progbits

