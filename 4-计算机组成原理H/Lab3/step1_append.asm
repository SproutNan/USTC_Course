# test instructions
.data
NUM1: .word 0xffffeeee
NUM2: .word 0x45677654
ADDR: .word 0x00000018
RIGHT: .word 0x1
ZERO: .word 0x0
.text
main:
	lw s11, RIGHT
	lw a0, NUM1
	lw a1, NUM2
	lw a5, ADDR
	li s1, 0x0
	# auto test
	lw s10, ZERO
	beq s10, s1, ADDI_FINI
	auipc s11, 0
	j END
ADDI_FINI:
	li s2, 0x1
	blt s2, s1, main
	blt s1, s2, TEST
START:  add a2, a0, a1
	# auto test
	li s10, 0x45676542
	beq s10, a2, ADD_FINI
	auipc s11, 0
	j END
ADD_FINI:
	addi a3, a0, 0x464
	sub a4, a0, a1
	# auto test
	li s10, 0xba98789a
	beq s10, a4, SUB_FINI
	auipc s11, 0
	j END
SUB_FINI:
	sw a5, 0(a5)
	auipc a6, 0x0
	auipc a7, 0x12
TEST:   addi s1, s1, 0x1
	beq s1, s2, START
	# test jal & jalr
        li t6, 1
	li t5, 0
JALBG:  beq t6, zero, EXIT
	addi t6, t6, -1
	addi t5, t5, 1
	jal JALBG
EXIT:	auipc s10, 0x0
        addi s10, s10, 8
        addi s10, s10, 8
        addi s10, s10, 8
	jalr s10
	j END
FUNC:   add s8, a2, a3
	ret
END:
