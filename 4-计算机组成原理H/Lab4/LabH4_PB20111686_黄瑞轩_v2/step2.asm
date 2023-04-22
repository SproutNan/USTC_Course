# A program to implement sort by Sprout.
# Using bubble sort.

.text
main:
    and a0, a0, zero      # relative offset
    li a1, 0x134            # load end address
    li a2, 0x100           # load array 
    li a4, 0x800003a2            # load init content of array
    li s6, 0x1            # load 1
saveLoop:
    add a3, a0, a2        # a3 is the next address to save content
    sw a4, 0(a3)          # save a4 to mem[a3]
    addi a0, a0, 4        # offset increment
    addi a4, a4, -0xdd    # set new content
    blt a3, a1, saveLoop  # if not end, branch to loop
    
    li s2, 0x1            # save ended, load re as one
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
    li s2, 0x1
endloop:
    addi a0, a0, 4
    blt a3, a1, innerloop
    beq a3, a1, whileRe
setup:
    # s0 <- display ready bit addr
    # s1 <- display register addr
    # s2 <- pointer to string
    li a1, 0x134            # load end address
    addi a1, a1, 0x4
    li a2, 0x100           # load array 
    li s0, 0x3c8
    li s1, 0x3cc
	
wait_for_ready1:
    lw t0, 0(s0)
    beq t0, zero, wait_for_ready1
	
    beq a1, a2, halt
    lb t0, 3(a2)
    srai t0, t0, 4
    andi t0, t0, 0xf
    sw t0, 0(s1)
wait_for_ready1_2:
    lw t0, 0(s0)
    beq t0, zero, wait_for_ready1_2
	
    lb t0, 3(a2)
    andi t0, t0, 0xf
    sw t0, 0(s1)

wait_for_ready2:
    lw t0, 0(s0)
    beq t0, zero, wait_for_ready2
    
    lb t0, 2(a2)
    srai t0, t0, 4
    andi t0, t0, 0xf
    sw t0, 0(s1)
wait_for_ready2_2:
    lw t0, 0(s0)
    beq t0, zero, wait_for_ready2_2
	
    lb t0, 2(a2)
    andi t0, t0, 0xf
    sw t0, 0(s1)
    
wait_for_ready3:
    lw t0, 0(s0)
    beq t0, zero, wait_for_ready3
    
    lb t0, 1(a2)
    srai t0, t0, 4
    andi t0, t0, 0xf
    sw t0, 0(s1)
wait_for_ready3_2:
    lw t0, 0(s0)
    beq t0, zero, wait_for_ready3_2
	
    lb t0, 1(a2)
    andi t0, t0, 0xf
    sw t0, 0(s1)
    
wait_for_ready4:
    lw t0, 0(s0)
    beq t0, zero, wait_for_ready4
    
    lb t0, 0(a2)
    srai t0, t0, 4
    andi t0, t0, 0xf
    sw t0, 0(s1)
wait_for_ready4_2:
    lw t0, 0(s0)
    beq t0, zero, wait_for_ready4_2
	
    lb t0, 0(a2)
    andi t0, t0, 0xf
    sw t0, 0(s1)

wait_for_ready:
    lw t0, 0(s0)
    beq t0, zero, wait_for_ready
	
    li t0, 10
    sw t0, 0(s1)
    
    # Increment pointer
    addi a2, a2, 4
    j wait_for_ready1
	
halt:
