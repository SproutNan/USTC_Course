.text
main:
	li x5, 0xaaffffee
	lui x6, 0xabc
	bne x5, x6, TEST_BNE
	j EXIT
TEST_BNE:
	li x7, 0xffffffff
	li x8, 0x11111111
	bltu x8, x7, TEST_BLTU
	j EXIT
TEST_BLTU:
	bgeu x7, x8, TEST_BGEU
	j EXIT
TEST_BGEU:
	li x9, 0x18
	sw x5, 0(x9)
	addi x9, x9, 0x4
	sh x5, 0(x9)
	addi x9, x9, 0x4
	sb x5, 0(x9)
	li x9, 0x18
	lw x10, 0(x9)
	lh x11, 0(x9)
	lb x12, 0(x9)
	lbu x13, 0(x9)
	lhu x14, 0(x9)
	slti x15, x14, 0x777
	sltiu x16, x14, 0x777
	xor x17, x15, x16
	xori x18, x17, 0xff
	ori x19, x18, 0xaa
	or x20, x19, x18
	srli x21, x20, 0x4
	li x22, 0x4
	sll x23, x20, x22
	slt x24, x23, x22
	sltu x25, x23, x22
	srl x26, x19, x22
	sra x27, x19, x22
EXIT:
