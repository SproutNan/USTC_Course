.text
	li a5, 0x512
	li a6, 0x18
	addi a1, x0, 0x411
	addi a2, x0, 0x411
	nop
	nop
	nop
	nop
	beq a1, a2, not_exit
	j exit
not_exit:
	bne a1, x0, exit
	and a1, a5, a6
	or a5, a1, a2
	add a7, a2, a2
exit:
	sw a5, 0(a6)
	lw a4, 0(a6)
	