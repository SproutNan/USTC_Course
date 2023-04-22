.text
	addi a1, a1, 0x432
	addi a2, a1, 0x18
	addi a3, a2, 0x430
	ori a4, a3, 0xab
	xori a5, a4, 0x55
	andi a6, a5, 0x0ff
	nop
	xor a7, a1, a6
	li a2, 0x18
	sw a5, 0(a2)
	nop
	nop
	nop
	lw s2, 0(a2)
	add s2, s2, s2
	nop
	add a1, a1, a2
	add a1, a1, a3
	add a1, a1, a4