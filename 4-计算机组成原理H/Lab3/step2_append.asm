# A program to implement sort by Sprout.
# Using bubble sort.

.data
base: .word 0x100 # Base address of array
one: .word 0x1
keyboard_ready_addr:	.word 0x7f00
keyboard_register_addr:	.word 0x7f04
display_ready_addr: 	.word 0x7f08
display_register_addr:	.word 0x7f0C

.text
main:
    lw s0, keyboard_ready_addr
    lw s1, keyboard_register_addr
    li s11, 0xa
    li s10, 58
    
    and a0, a0, zero      # relative offset
    lw a2, base           # load array 
    lw s6, one            # load 1
getNum1:
    lw t0, 0(s0)
    beq t0, zero, getNum1
    
    lw t0, 0(s1)
    blt t0, s10, not_letterK1
    addi t0, t0, -87
    j store_1
not_letterK1:
    addi t0, t0, -48
store_1:
    add a0, a0, t0
    slli a0, a0, 4

getNum2:
    lw t0, 0(s0)
    beq t0, zero, getNum2
    
    lw t0, 0(s1)
    blt t0, s10, not_letterK2
    addi t0, t0, -87
    j store_2
not_letterK2:
    addi t0, t0, -48
store_2:
    add a0, a0, t0
    slli a0, a0, 4

getNum3:
    lw t0, 0(s0)
    beq t0, zero, getNum3
    
    lw t0, 0(s1)
    blt t0, s10, not_letterK3
    addi t0, t0, -87
    j store_3
not_letterK3:
    addi t0, t0, -48
store_3:
    add a0, a0, t0
    
    slli a0, a0, 2
    addi a0, a0, -4
    add a1, a0, a2 # a1 is the end of array
    mv s9, a1
    and a0, a0, zero
        
saveLoop:
    add a3, a0, a2        # a3 is the next address to save content
    li a4, 0
    
getARRNum1:
    lw t0, 0(s0)
    beq t0, zero, getARRNum1
    
    lw t0, 0(s1)
    blt t0, s10, not_letterR1
    addi t0, t0, -87
    j store_R1
not_letterR1:
    addi t0, t0, -48
store_R1:
    add a4, a4, t0
    slli a4, a4, 4
    
getARRNum2:
    lw t0, 0(s0)
    beq t0, zero, getARRNum2
    
    lw t0, 0(s1)
    blt t0, s10, not_letterR2
    addi t0, t0, -87
    j store_R2
not_letterR2:
    addi t0, t0, -48
store_R2:
    add a4, a4, t0
    slli a4, a4, 4

getARRNum3:
    lw t0, 0(s0)
    beq t0, zero, getARRNum3
    
    lw t0, 0(s1)
    blt t0, s10, not_letterR3
    addi t0, t0, -87
    j store_R3
not_letterR3:
    addi t0, t0, -48
store_R3:
    add a4, a4, t0
    slli a4, a4, 4

getARRNum4:
    lw t0, 0(s0)
    beq t0, zero, getARRNum4
    
    lw t0, 0(s1)
    blt t0, s10, not_letterR4
    addi t0, t0, -87
    j store_R4
not_letterR4:
    addi t0, t0, -48
store_R4:
    add a4, a4, t0
    slli a4, a4, 4

getARRNum5:
    lw t0, 0(s0)
    beq t0, zero, getARRNum5
    
    lw t0, 0(s1)
    blt t0, s10, not_letterR5
    addi t0, t0, -87
    j store_R5
not_letterR5:
    addi t0, t0, -48
store_R5:
    add a4, a4, t0
    slli a4, a4, 4

getARRNum6:
    lw t0, 0(s0)
    beq t0, zero, getARRNum6
    
    lw t0, 0(s1)
    blt t0, s10, not_letterR6
    addi t0, t0, -87
    j store_R6
not_letterR6:
    addi t0, t0, -48
store_R6:
    add a4, a4, t0
    slli a4, a4, 4

getARRNum7:
    lw t0, 0(s0)
    beq t0, zero, getARRNum7
    
    lw t0, 0(s1)
    blt t0, s10, not_letterR7
    addi t0, t0, -87
    j store_R7
not_letterR7:
    addi t0, t0, -48
store_R7:
    add a4, a4, t0
    slli a4, a4, 4

getARRNum8:
    lw t0, 0(s0)
    beq t0, zero, getARRNum8
    
    lw t0, 0(s1)
    blt t0, s10, not_letterR8
    addi t0, t0, -87
    j store_R8
not_letterR8:
    addi t0, t0, -48
store_R8:
    add a4, a4, t0
    
    sw a4, 0(a3)          # save a4 to mem[a3]
    addi a0, a0, 4        # offset increment
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
    mv a1, s9            # load end address
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