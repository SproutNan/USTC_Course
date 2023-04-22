# A program to implement sort by Sprout.
# Using bubble sort.

.data
base: .word 0x100 # Base
end: .word 0x134
one: .word 0x1
AR0: .word 0x800003a2

.text
main:
    and a0, a0, zero # 相对偏移地址
    lw a1, end # 数组的终结地址
    lw a2, base # 数组的偏移基地址
    lw a4, AR0 # 数组的内容
    lw s6, one #恒为1的寄存器
saveLoop:
    add a3, a0, a2 # a3是下一个要保存数据的地址
    sw a4, 0(a3) # 把a4内容存到mem[a3]
    addi a0, a0, 4
    addi a4, a4, -0xdd
    blt a3, a1, saveLoop
    lw s2, one #re位置
    addi a1, a1, -4 # 终结地址减4
whileRe:
    beq s2, zero, halt
    and s2, s2, zero #re置为0
    and a0, a0, zero #把偏移位置重新改成0
innerloop:
    add a3, a0, a2 #数据指针
    lw a6, 0(a3)
    lw a7, 4(a3) # 获得指针指向的位置数据
    bge a6, a7, endloop
    
    and s3, s3, zero
    add s3, s3, a7
    and a7, a7, zero
    add a7, a7, a6
    and a6, a6, zero
    add a6, a6, s3
    
    sw a6, 0(a3)
    sw a7, 4(a3)
    lw s2, one
endloop:
    addi a0, a0, 4 #后移
    blt a3, a1, innerloop
    beq a3, a1, whileRe
halt:
    add x0, x0, x0
      

      
      
      
      
      
      
    