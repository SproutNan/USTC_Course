# A program to implement sort by Sprout.
# Using bubble sort.

.data
base: .word 0x100 # Base
end: .word 0x134
one: .word 0x1
AR0: .word 0x800003a2

.text
main:
    and a0, a0, zero # ���ƫ�Ƶ�ַ
    lw a1, end # ������ս��ַ
    lw a2, base # �����ƫ�ƻ���ַ
    lw a4, AR0 # ���������
    lw s6, one #��Ϊ1�ļĴ���
saveLoop:
    add a3, a0, a2 # a3����һ��Ҫ�������ݵĵ�ַ
    sw a4, 0(a3) # ��a4���ݴ浽mem[a3]
    addi a0, a0, 4
    addi a4, a4, -0xdd
    blt a3, a1, saveLoop
    lw s2, one #reλ��
    addi a1, a1, -4 # �ս��ַ��4
whileRe:
    beq s2, zero, halt
    and s2, s2, zero #re��Ϊ0
    and a0, a0, zero #��ƫ��λ�����¸ĳ�0
innerloop:
    add a3, a0, a2 #����ָ��
    lw a6, 0(a3)
    lw a7, 4(a3) # ���ָ��ָ���λ������
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
    addi a0, a0, 4 #����
    blt a3, a1, innerloop
    beq a3, a1, whileRe
halt:
    add x0, x0, x0
      

      
      
      
      
      
      
    