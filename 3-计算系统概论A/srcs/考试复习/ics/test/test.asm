.ORIG X3000
LD R0, INP
LD R1, NUM
LEA R2, NUM
POS0 AND R3, R3, #0 
ADD R3, R3, #1 
ADD R4, R3, #0
ADD R5, R3, #1 
ADD R6, R5, #0 
ADD R0, R0, #-2
BRZ POS3
BRN POS4
POS1 ADD R3, R3, R3
ADD R6, R6, R3 
AND R6, R6, R1
ADD R3, R4, #0
ADD R4, R5, #0
ADD R5, R6, #0
ADD R0, R0, #-1
BRP POS1
ADD R7, R7, #0
BRP #2
ADD R7, R6, #0
BRNZP #4
STR R6, R2, #0
BRNZP POS2
POS3 ADD R7, R7, #1
POS4 ADD R7, R7, #1
POS2 ADD R2, R2, #1
LDR R0, R2, #0
BRZP POS0
HALT
INP .FILL #144
NUM BRP NUM
A   .FILL x0014
B   .FILL x000B
C   .FILL x0010
D   .FILL x003B
E .FILL #-1
 .end