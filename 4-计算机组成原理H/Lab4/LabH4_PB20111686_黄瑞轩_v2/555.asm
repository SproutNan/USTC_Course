# A program to implement sort by Sprout.
# Using bubble sort.

.text
main:
    li s1, 0x18
    li t0, 0xffffffff
    sw t0, 0(s1)
    li t0, 0x00000000
    sw t0, 0(s1)
    li t0, 0xaaaaaaaa
    sw t0, 0(s1)
