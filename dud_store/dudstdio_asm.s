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
	mflr 0
	std 30,-16(1)
	std 31,-8(1)
	std 0,16(1)
	stdu 1,-160(1)
	.cfi_def_cfa_offset 160
	.cfi_offset 65, 16
	.cfi_offset 30, -16
	.cfi_offset 31, -8
	li 10,0
	mr 31,3
	rldicl 9,4,0,32
	stw 4,128(1)
	std 3,120(1)
	addi 30,1,32
.L2:
	cmpdi 0,31,0
	bne 0,.L3
	addis 3,2,.LC0@toc@ha
	extsw 5,10
	mr 4,30
	addi 3,3,.LC0@toc@l
	bl printhex
	nop
	ld 9,120(1)
	lwz 8,128(1)
.L4:
	cmpdi 0,9,0
	bne 0,.L5
	addis 3,2,.LC1@toc@ha
	extsw 5,31
	mr 4,30
	addi 3,3,.LC1@toc@l
	bl printhex
	nop
	addis 3,2,.LC2@toc@ha
	addi 4,1,128
	li 5,4
	addi 3,3,.LC2@toc@l
	bl printhex
	nop
	addis 3,2,.LC3@toc@ha
	addi 4,1,120
	li 5,8
	addi 3,3,.LC3@toc@l
	bl printhex
	nop
	addi 1,1,160
	.cfi_remember_state
	.cfi_def_cfa_offset 0
	b _restgpr0_30
	.cfi_restore 31
	.cfi_restore 30
	.cfi_restore 65
.L3:
	.cfi_restore_state
	divdu 7,31,9
	mulld 8,7,9
	subf 8,8,31
	mr 31,7
	stbx 8,30,10
	addi 10,10,1
	b .L2
.L5:
	divdu 7,9,8
	mulld 10,7,8
	subf 10,10,9
	mr 9,7
	stbx 10,30,31
	addi 31,31,1
	b .L4
	.long 0
	.byte 0,0,0,1,128,2,0,0
	.cfi_endproc
.LFE0:
	.size	dud_number,.-dud_number
	.ident	"GCC: (Ubuntu 11.2.0-5ubuntu1) 11.2.0"
	.section	.note.GNU-stack,"",@progbits
