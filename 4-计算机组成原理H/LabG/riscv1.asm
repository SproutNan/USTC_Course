	.file	"code.cpp"
	.option nopic
	.attribute arch, "rv32i2p0_m2p0_a2p0_c2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	1
	.globl	_Z10Initializei
	.type	_Z10Initializei, @function
_Z10Initializei:
.LFB0:
	.cfi_startproc
	lui	a4,%hi(_ZL2mt)
	lui	a3,%hi(_ZL2mt+4)
	li	a2,1812434944
	sw	a0,%lo(_ZL2mt)(a4)
	mv	a5,a0
	addi	a3,a3,%lo(_ZL2mt+4)
	li	a4,1
	addi	a2,a2,-1691
	li	a1,624
.L3:
	srai	a0,a5,30
	xor	a0,a0,a5
	mul	a0,a0,a2
	addi	a3,a3,4
	add	a5,a0,a4
	sw	a5,-4(a3)
	addi	a4,a4,1
	bne	a4,a1,.L3
	lui	a5,%hi(_ZL5index)
	sh	a4,%lo(_ZL5index)(a5)
	ret
	.cfi_endproc
.LFE0:
	.size	_Z10Initializei, .-_Z10Initializei
	.align	1
	.globl	_Z10ExtractU32v
	.type	_Z10ExtractU32v, @function
_Z10ExtractU32v:
.LFB2:
	.cfi_startproc
	lui	t5,%hi(_ZL5index)
	lh	a7,%lo(_ZL5index)(t5)
	li	a5,623
	bgtu	a7,a5,.L6
	addi	a3,a7,1
	mv	a5,a7
	slli	a3,a3,16
	lui	a7,%hi(_ZL2mt)
	srai	a3,a3,16
	addi	a7,a7,%lo(_ZL2mt)
.L7:
	slli	a5,a5,2
	add	a7,a7,a5
	lw	a5,0(a7)
	li	a4,-1658040320
	addi	a4,a4,1664
	srai	a0,a5,11
	xor	a0,a0,a5
	slli	a5,a0,7
	and	a5,a5,a4
	xor	a0,a5,a0
	slli	a5,a0,15
	li	a4,-272236544
	and	a5,a5,a4
	xor	a5,a5,a0
	srai	a0,a5,18
	sh	a3,%lo(_ZL5index)(t5)
	xor	a0,a0,a5
	ret
.L6:
	lui	a1,%hi(_ZL2mt)
	li	t1,-2147483648
	li	t4,-1727483904
	addi	a7,a1,%lo(_ZL2mt)
	li	a2,0
	addi	a1,a1,%lo(_ZL2mt)
	li	a6,624
	xori	t3,t1,-1
	addi	t4,t4,223
.L9:
	mv	a4,a2
	addi	a2,a2,1
	remu	a3,a2,a6
	lw	a5,0(a1)
	addi	a4,a4,397
	and	a0,t1,a5
	slli	a3,a3,2
	add	a3,a7,a3
	lw	a5,0(a3)
	remu	a4,a4,a6
	and	a5,a5,t3
	or	a5,a5,a0
	andi	a3,a5,1
	srai	a5,a5,1
	beq	a3,zero,.L8
	xor	a5,a5,t4
.L8:
	slli	a4,a4,2
	add	a4,a7,a4
	lw	a4,0(a4)
	addi	a1,a1,4
	xor	a5,a4,a5
	sw	a5,-4(a1)
	bne	a2,a6,.L9
	li	a3,1
	li	a5,0
	j	.L7
	.cfi_endproc
.LFE2:
	.size	_Z10ExtractU32v, .-_Z10ExtractU32v
	.section	.text.startup,"ax",@progbits
	.align	1
	.globl	main
	.type	main, @function
main:
.LFB3:
	.cfi_startproc
	li	a0,20111360
	addi	sp,sp,-16
	.cfi_def_cfa_offset 16
	addi	a0,a0,326
	sw	ra,12(sp)
	.cfi_offset 1, -4
	call	_Z10Initializei
	lw	ra,12(sp)
	.cfi_restore 1
	addi	sp,sp,16
	.cfi_def_cfa_offset 0
	tail	_Z10ExtractU32v
	.cfi_endproc
.LFE3:
	.size	main, .-main
	.bss
	.align	2
	.type	_ZL2mt, @object
	.size	_ZL2mt, 2496
_ZL2mt:
	.zero	2496
	.section	.sbss,"aw",@nobits
	.align	1
	.type	_ZL5index, @object
	.size	_ZL5index, 2
_ZL5index:
	.zero	2
	.ident	"GCC: () 10.2.0"
