# A program to implement sort by Sprout.
# Using bubble sort.

.data
base: .word 0x100 # Base address of array
end: .word 0x134 # End of array
one: .word 0x1
AR0: .word 0x800003a2 
display_ready_addr: 	.word 0x7f08
display_register_addr:	.word 0x7f0C
string:	.asciz "Hello World\n"

.text
main:
    and a0, a0, zero      # relative offset
    lw a1, end            # load end address
    lw a2, base           # load array 
    lw a4, AR0            # load init content of array
    lw s6, one            # load 1
saveLoop:
    add a3, a0, a2        # a3 is the next address to save content
    sw a4, 0(a3)          # save a4 to mem[a3]
    addi a0, a0, 4        # offset increment
    addi a4, a4, -0xdd    # set new content
    blt a3, a1, saveLoop  # if not end, branch to loop
    
    lw s2, one            # save ended, load re as one
    addi a1, a1, -4       # ignore the last element
whileRe:
    beq s2, zero, setup
    and s2, s2, zero      # set re as zero
    and a0, a0, zero      # set offset as zero
innerloop:
    add a3, a0, a2 
    lw a6, 0(a3)
    lw a7, 4(a3) 
    bge a6, a7, endloop
    
    mv s3, a7             # swap
    mv a7, a6
    mv a6, s3
    sw a6, 0(a3)
    sw a7, 4(a3)
    lw s2, one
endloop:
    addi a0, a0, 4
    blt a3, a1, innerloop
    beq a3, a1, whileRe
setup:
    # s0 <- display ready bit addr
    # s1 <- display register addr
    # s2 <- pointer to string
    li s11, 0xa
    lw a1, end            # load end address
    addi a1, a1, 0x4
    lw a2, base           # load array 
    lw s0, display_ready_addr
    lw s1, display_register_addr
	
wait_for_ready1:
    lw t0, 0(s0)
    beq t0, zero, wait_for_ready1
	
    beq a1, a2, halt
    lb t0, 3(a2)
    srai t0, t0, 4
    andi t0, t0, 0xf
    blt t0, s11, not_letter1
    addi t0, t0, 55
    j save1
not_letter1:
    addi t0, t0, 48
save1:
    sw t0, 0(s1)
wait_for_ready1_2:
    lw t0, 0(s0)
    beq t0, zero, wait_for_ready1_2
	
    lb t0, 3(a2)
    andi t0, t0, 0xf
    blt t0, s11, not_letter1_2
    addi t0, t0, 55
    j save1_2
not_letter1_2:
    addi t0, t0, 48
save1_2:
    sw t0, 0(s1)

wait_for_ready2:
    lw t0, 0(s0)
    beq t0, zero, wait_for_ready2
    
    lb t0, 2(a2)
    srai t0, t0, 4
    andi t0, t0, 0xf
    blt t0, s11, not_letter2
    addi t0, t0, 55
    j save2
not_letter2:
    addi t0, t0, 48
save2:
    sw t0, 0(s1)
wait_for_ready2_2:
    lw t0, 0(s0)
    beq t0, zero, wait_for_ready2_2
	
    lb t0, 2(a2)
    andi t0, t0, 0xf
    blt t0, s11, not_letter2_2
    addi t0, t0, 55
    j save2_2
not_letter2_2:
    addi t0, t0, 48
save2_2:
    sw t0, 0(s1)
    
wait_for_ready3:
    lw t0, 0(s0)
    beq t0, zero, wait_for_ready3
    
    lb t0, 1(a2)
    srai t0, t0, 4
    andi t0, t0, 0xf
    blt t0, s11, not_letter3
    addi t0, t0, 55
    j save3
not_letter3:
    addi t0, t0, 48
save3:
    sw t0, 0(s1)
wait_for_ready3_2:
    lw t0, 0(s0)
    beq t0, zero, wait_for_ready3_2
	
    lb t0, 1(a2)
    andi t0, t0, 0xf
    blt t0, s11, not_letter3_2
    addi t0, t0, 55
    j save3_2
not_letter3_2:
    addi t0, t0, 48
save3_2:
    sw t0, 0(s1)
    
wait_for_ready4:
    lw t0, 0(s0)
    beq t0, zero, wait_for_ready4
    
    lb t0, 0(a2)
    srai t0, t0, 4
    andi t0, t0, 0xf
    blt t0, s11, not_letter4
    addi t0, t0, 55
    j save4
not_letter4:
    addi t0, t0, 48
save4:
    sw t0, 0(s1)
wait_for_ready4_2:
    lw t0, 0(s0)
    beq t0, zero, wait_for_ready4_2
	
    lb t0, 0(a2)
    andi t0, t0, 0xf
    blt t0, s11, not_letter4_2
    addi t0, t0, 55
    j save4_2
not_letter4_2:
    addi t0, t0, 48
save4_2:
    sw t0, 0(s1)

wait_for_ready:
    lw t0, 0(s0)
    beq t0, zero, wait_for_ready
	
    li t0, 10
    sw t0, 0(s1)
    
    # Increment pointer
    addi a2, a2, 4
    j wait_for_ready1
	
#end_of_string:
	# Terminate
	#li	a7, 10
	#ecall
halt: