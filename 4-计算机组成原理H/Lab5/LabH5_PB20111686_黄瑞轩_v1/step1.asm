.text
	addi a1, a1, 0x432
	addi a2, a2, 0x18
	addi a3, a3, 0x430
	ori a4, a4, 0xab
	xori a5, a5, 0x55
	andi a6, a6, 0x0ff
	sub a1, a1, a2
	xor a7, a3, a4
	nop
	sw a5, 0(a2)
	nop
	nop
	nop
	nop
	lw s2, 0(a2)
	srai a5, a5, 3
