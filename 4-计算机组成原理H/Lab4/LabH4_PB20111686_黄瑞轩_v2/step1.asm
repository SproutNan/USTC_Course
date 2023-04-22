# test instructions
.text
main:
	li a0, 0xffffeeee
	li a1, 0x45677654
	li a5, 0x00000018
	li s1, 0x0
	li s2, 0x1
	blt s2, s1, main
	blt s1, s2, TEST
START:  add a2, a0, a1
	addi a3, a0, 0x464
	sub a4, a0, a1
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
